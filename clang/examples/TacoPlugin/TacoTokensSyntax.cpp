//===- TacoTokensSyntax.cpp ----------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//
// Plugin enables the generation of taco functions and direct insertion
// of the code into the source code.
//
//===----------------------------------------------------------------------===//

#include "clang/Parse/Parser.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/RandomNumberGenerator.h"
#include "llvm/Support/raw_ostream.h"
#include <cstdio>
#include <exception>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <map>
using namespace clang;

namespace {

struct VarAnalysis{
  bool RequiresConversion = false;
  std::string StructTypeName;
  std::string Format;
  bool IsOutput =false;
  bool IsNull = true;
};
class TacoTokensHandler : public SyntaxHandler {
private:
  std::map<std::string,VarAnalysis> TensorMap;
  uint32_t Unique; // variable to ensure unique
                   // naming during code generation
  void replaceAll(std::string &Original, const std::string &ToReplace,
                  const std::string &Replacement) {

    size_t pos = Original.find(ToReplace);
    while (pos != std::string::npos) {
      Original.replace(pos, ToReplace.length(), Replacement);
      pos = Original.find(ToReplace);
    }
  }
 /**Function determines if a field exists in some struct
  * This function is usefull to determine if conversion
  * routines are present in the structures passed as a 
  * parameter to the plugin.
  */
 FieldDecl* FieldExistsInRecord (RecordDecl * Decl, std::string Field){
    auto it = Decl->field_begin();
    auto fend =Decl->field_end();
    while(it != fend){
       if((*it)->getName() == Field){
          return *it;
       }
       it++;	    
    }
    return NULL;
 }

public:
  TacoTokensHandler() : SyntaxHandler("taco"), Unique(0) {}

  void GetReplacement(Preprocessor &PP, Declarator &D, CachedTokens &Toks,
                      llvm::raw_string_ostream &OS) override {

    std::string Expr = "";
    for (auto &Tok : Toks) {
      Expr += PP.getSpelling(Tok);
    }
    std::string Format= "";
    // check to see if there is need for conversion
    // to taco data structures
    unsigned NumParams = D.getFunctionTypeInfo().NumParams;
    DeclaratorChunk::ParamInfo *Params = D.getFunctionTypeInfo().Params;
    for (unsigned i = 0; i < NumParams; i++) {
      // check if the parameter is of struct pointer
      // type, not a taco_tensor_t object and perform
      // implicit conversion.
      
      if (ParmVarDecl *VarDecl = 
		      llvm::dyn_cast<ParmVarDecl>(Params[i].Param)) {
        
	std::string VarName = Params[i].Ident->getName().str();
        if (i == (NumParams-1)) {
	   assert(VarName == "format" && "taco: last argument must be format=\"\"");
	   assert(VarDecl->hasDefaultArg()&&
			   "taco: format must have default argument,"
			   "set format=\"\" if no special format is"
			   "needed.");
	   //get the text of the default 
	   auto DefaultRange = Lexer::getAsCharRange(
                           VarDecl->getDefaultArgRange(), 
			   PP.getSourceManager(), 
			   PP.getLangOpts());
           Format = Lexer::getSourceText(
			   DefaultRange, 
			   PP.getSourceManager(),
                           PP.getLangOpts()).str();
           replaceAll(Format,"\"","");
	} 
         
	// get parameter variable declaration data type
        // this is necessary to decide what kind of parameter
        // type is used so as to be able to decide on conversion
        // routines or not.
        auto ParamStructType =
            VarDecl->getOriginalType().getTypePtr()->getPointeeType();
        if (!ParamStructType.isNull()) {
           if (const RecordType *RT = ParamStructType->getAs<RecordType>()) {
              
	      TensorMap[VarName] = {};
	      if (RT->getDecl()->getName() != "taco_tensor_t"){
                  //check if the struct has conversion
		  //routines
		  auto ConvRoutineToTaco = 
			  FieldExistsInRecord(RT->getDecl(),
					  RT->getDecl()->getName().str()+"2taco");
		  auto ConvRoutineFromTaco =
			 FieldExistsInRecord(RT->getDecl(),
					  "taco2"+RT->getDecl()->getName().str());
		  assert(ConvRoutineToTaco 
		        && 
			"taco: non taco_tensor_t should have a function ptr"
		        "routine taco_tensor_t* (*<structname>2taco) (" 
		        "<structname>*)" );
		  assert(ConvRoutineFromTaco 
		        && 
			"taco: non taco_tensor_t should have a function ptr"
		        "routine void (*taco2<structname>) (" 
		        "taco_tensor_t*,<structname>*)" );

		  //do implicit conversion
		  TensorMap[VarName].StructTypeName= 
			  RT->getDecl()->getName();
	          TensorMap[VarName].RequiresConversion = true;
	      }
           }
         }
      }
    }

    // create temporary file
    auto TempFilePath = "taco_" + std::to_string(Unique) + ".inc";

    std::string Cmd = "taco \"" + Expr +
                      "\" "+Format+" -print-compute -print-assembly"+
		      "  -print-nocolor  > " +
                      TempFilePath;
    assert(system(Cmd.c_str()) == 0 && "Taco not installed");
    // read the file generated by taco.
    std::ifstream IS(TempFilePath, std::ifstream::binary);

    assert(IS && "Taco could not create temp file");
    IS.seekg(0, IS.end);
    int Length = IS.tellg();
    IS.seekg(0, IS.beg);

    char *buffer = new char[Length];
    IS.read(buffer, Length);
    IS.close();
    std::remove(TempFilePath.c_str());
    std::string TacoCode(buffer);

    // replace compute function name and assemble function name with unique name
    std::string ComputeFunctionName = "__taco_comput_" + std::to_string(Unique);
    std::string AssembleFunctionName = "__taco_assm_" + std::to_string(Unique);
    replaceAll(TacoCode, "compute", ComputeFunctionName);
    replaceAll(TacoCode, "assemble", AssembleFunctionName);

    Unique++;
    // create declarations by parsing the taco expression
    std::string ComputeFunctionCall = ComputeFunctionName + "(";
    std::string AssembleFunctionCall = AssembleFunctionName + "(";
    std::string ComputeFunctionDeclaration = "int " + ComputeFunctionName + "(";
    std::string AssembleFunctionDeclaration =
        "int " + AssembleFunctionName + "(";
   
    // TACO conversion routines if necessary
    std::string TacoPreConversionRoutines = "";
    std::string TacoPostConversionRoutines= "";
    std::string TacoVariableCleanup = ""; 
    // parsing taco expression
    int ParseCount = 0;
    bool IsFirstExpr = true;
    std::string OutputExpr;
    for (auto &Tok : Toks) {
      if (Tok.isAnyIdentifier() && ParseCount == 0) {
	std::string ExprName = PP.getSpelling(Tok);
	auto SearchTensor = TensorMap.find(ExprName);
	assert(SearchTensor != TensorMap.end() && 
			"taco: Tensor parameters do not match expression");
	
	//check if there is any need for calling 
	//conversion routines
	if (SearchTensor->second.RequiresConversion){
	   //calls the taco conversion routine function ptr
	   TacoPreConversionRoutines +="taco_tensor_t * __taco_"+
		   ExprName + " = "+ExprName+"->"+
		   SearchTensor->second.StructTypeName+"2taco ("+ExprName+
		   ");\n";
	   //clear up memory allocated for this variable
	   //this should be sent to the stream after 
	   //the computation and final conversion routines
	   //have been called
	   TacoVariableCleanup+="__taco_cleanup_taco(__taco_"+ExprName+
		   ");\n";


	   

	   //first expression is output tensor xo 
	   //call conversion routines from datastructure
	   //back to taco 
	   if (IsFirstExpr){
              TacoPostConversionRoutines+=ExprName+"->taco2"+
		      SearchTensor->second.StructTypeName+ 
		      "(__taco_"+ExprName+","+ExprName+");\n";
	   }
	   ComputeFunctionCall += ("__taco_" +ExprName + ",");
           AssembleFunctionCall += ("__taco_"+ExprName + ",");
	   
	}else{
	   ComputeFunctionCall += (ExprName + ",");
           AssembleFunctionCall += (ExprName + ",");
	}

        ComputeFunctionDeclaration += "taco_tensor_t *,";
        AssembleFunctionDeclaration += "taco_tensor_t *,";
        if(IsFirstExpr){
	   IsFirstExpr = false; 
	}	

 
      } else if (Tok.is(tok::l_paren)) {
        ParseCount++;
      } else if (Tok.is(tok::r_paren)) {
        ParseCount--;
      }
    }
    // remove the trailing ',' in the string;
    ComputeFunctionCall.pop_back();
    ComputeFunctionCall += "); ";
    AssembleFunctionCall.pop_back();
    AssembleFunctionCall += "); ";
    ComputeFunctionDeclaration.pop_back();
    ComputeFunctionDeclaration += "); ";
    AssembleFunctionDeclaration.pop_back();
    AssembleFunctionDeclaration += "); ";
    // write taco compute and assemble declaration
    OS << ComputeFunctionDeclaration << "\n";
    OS << AssembleFunctionDeclaration << "\n";
    
    // write taco clean up routine for temp variables

    OS<<"static inline void  __taco_cleanup_taco(taco_tensor_t * t){ \n";
    OS<<"  if (t){ \n";
    OS<<"     for (int i = 0; i < t->order; i++) { \n";
    OS<<"       free(t->indices[i]); \n";
    OS<<"     } \n";
    OS<<"     free(t->indices); \n";
    OS<<" \n";
    OS<<"     free(t->dimensions); \n";
    OS<<"     free(t->mode_ordering); \n";
    OS<<"     free(t->mode_types); \n";
    OS<<"     free(t); \n";
    OS<<"  } \n";
    OS<<"} \n";
    OS<<"     \n";
    // write taco definitions
    OS << TacoCode << " \n";
    // rewrite original function
    auto DeclCharRange = Lexer::getAsCharRange(
        D.getSourceRange(), PP.getSourceManager(), PP.getLangOpts());
    auto DeclText = Lexer::getSourceText(DeclCharRange, PP.getSourceManager(),
                                         PP.getLangOpts());

    OS << DeclText << "\n";
    OS << "\n{\n";
    OS << "printf(\"function started\\n\"); \n";
    OS << TacoPreConversionRoutines << "\n";
    OS << "printf(\"conversion routines done\\n\"); \n";
       // write assemble function call
    OS << AssembleFunctionCall << " \n";
    OS << "printf(\"assemble routine done\\n\"); \n";
    // write compute function call.
    OS << ComputeFunctionCall << "\n";

    OS << "printf(\"computation done\\n\"); \n";
    OS << TacoPostConversionRoutines << "\n";
    
    OS << "printf(\" post conversion routines done\\n\"); \n";
    OS << TacoVariableCleanup << "\n";

    OS << "printf(\"taco cleanup done done\\n\"); \n";
    OS << "\n}\n";
  }

  void AddToPredefines(llvm::raw_string_ostream &OS) override {
    OS << "#ifdef __cplusplus\n";
    OS << "#include <cstdlib>\n";
    OS << "#include <cstdint>\n";
    OS << "#else\n";
    OS << "#include <stdint.h>\n";
    OS << "#include <stdlib.h>\n";
    OS << "#endif\n";    
    OS<<" #define TACO_MIN(_a,_b) ((_a) < (_b) ? (_a) : (_b)) \n";
    OS<<"#define TACO_MAX(_a,_b) ((_a) > (_b) ? (_a) : (_b)) \n";
    OS<<"#define TACO_DEREF(_a) (((___context___*)(*__ctx__))->_a) \n";
   
    // generate masking for restrict
    OS << "#ifndef restrict\n";
    OS << "#define restrict __restrict\n";
    OS << "#endif\n";
 
    OS<<"typedef enum { taco_mode_dense, taco_mode_sparse } taco_mode_t; \n";
    OS<<" \n";
    OS<<"typedef struct taco_tensor_t { \n";
    OS<<"  int32_t order;           // tensor order (number of modes) \n";
    OS<<"  int32_t *dimensions;     // tensor dimensions \n";
    OS<<"  int32_t csize;           // component size \n";
    OS<<"  int32_t *mode_ordering;  // mode storage ordering \n";
    OS<<"  taco_mode_t *mode_types; // mode storage types \n";
    OS<<"  uint8_t ***indices;      // tensor index data (per mode) \n";
    OS<<"  uint8_t *vals;           // tensor values \n";
    OS<<"  int32_t vals_size;       // values array size \n";
    OS<<"} taco_tensor_t; \n";
    OS<<" \n";
  }
};

} // namespace

static SyntaxHandlerRegistry::Add<TacoTokensHandler> X("taco",
                                                       "generates taco code");
