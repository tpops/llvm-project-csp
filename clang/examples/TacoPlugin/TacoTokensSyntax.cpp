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
  std::string Format; // format of the tensor: not currently in use
  bool IsOutput =false; // indicates the type of tensor
  bool IsNull = true; // check if tensor is null
  bool IsVisited = false; //indicates if code has been generated for Tensor
  bool IsTemplate = false; // indicates that Variable is a template.
};
class TacoTokensHandler : public SyntaxHandler {
private:
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
    std::map<std::string,VarAnalysis> TensorMap;
    auto DeclCharRange = Lexer::getAsCharRange(
        D.getSourceRange(), PP.getSourceManager(), PP.getLangOpts());
    auto DeclText = Lexer::getSourceText(DeclCharRange, PP.getSourceManager(),
                                         PP.getLangOpts());
    std::string TemplateList= "";
    auto TemplateParameterList = D.getTemplateParameterLists();
    if (TemplateParameterList.size()!=0){
      TemplateList+=" template< ";
      auto it = TemplateParameterList.begin();
      auto end = TemplateParameterList.end();
      
      //while(it!=end){
        
     // }
    }
    llvm::errs() << "Template List: " <<
	    TemplateParameterList.size() << "\n";
    std::string Expr = "";
    for (auto &Tok : Toks) {
      Expr += PP.getSpelling(Tok);
    }
    std::string Format= "";
    unsigned NumParams = D.getFunctionTypeInfo().NumParams;
    DeclaratorChunk::ParamInfo *Params = D.getFunctionTypeInfo().Params;
    for (unsigned i = 0; i < NumParams; i++) {
      // Check if the parameter is of struct pointer
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
	   // Get the text of the default.
	   auto DefaultRange = Lexer::getAsCharRange(
                           VarDecl->getDefaultArgRange(), 
			   PP.getSourceManager(), 
			   PP.getLangOpts());
           Format = Lexer::getSourceText(
			   DefaultRange, 
			   PP.getSourceManager(),
                           PP.getLangOpts()).str();
           replaceAll(Format,"\"","");
	   continue;
	} 
	llvm::errs() <<Expr << " VAR NAME: " <<VarName << "\n";
       	 // Check if this parameter is a template. 
        if (VarDecl->getOriginalType().getTypePtr()->isDependentType()){
	   llvm::errs()<< "Is templated \n";
	   TensorMap[VarName] = {};
	   TensorMap[VarName].RequiresConversion = true;
	   TensorMap[VarName].IsTemplate = true;
	   continue;
	}


	
	// Get parameter variable declaration data type
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
					 RT->getDecl()->getName().str() +"2taco");
		  auto ConvRoutineFromTaco =
			 FieldExistsInRecord(RT->getDecl(),
					  "taco2"+RT->getDecl()->getName().str());
		  if (!ConvRoutineToTaco){
	             llvm::errs() << "taco: Error in Plugin Function: " 
			     << DeclText << "\n.";
	             llvm::errs()<<"taco: non taco_tensor_t: "<<
			    RT->getDecl()->getName().str()<<
			    " should have a function ptr "<<
		            "routine taco_tensor_t* (*"<<
			    RT->getDecl()->getName().str() <<"2taco) ("
			    <<RT->getDecl()->getName().str()<<"*)\n\n";
		  }
                  if (!ConvRoutineFromTaco){
	             llvm::errs() << "taco: Error in Plugin Function: " 
			     << DeclText << "\n.";
   		     llvm::errs()<<"taco: non taco_tensor_t: "<<
			    RT->getDecl()->getName().str()<<
			    " should have a function ptr "<<
		            "routine taco_tensor_t* (*taco2"<<
			    RT->getDecl()->getName().str() <<") ("
			    <<RT->getDecl()->getName().str()<<"*)\n\n";
		  }	

		  assert(ConvRoutineToTaco);
		  assert(ConvRoutineFromTaco);

		  // Do implicit conversion.
		  TensorMap[VarName].StructTypeName= 
			  RT->getDecl()->getName().str();
	          TensorMap[VarName].RequiresConversion = true;
	      }
           }
         }else{
	    // Throw an error as this is required to be 
	    // a struct type.
	 }

      }
    }

    // create temporary file
    auto TempFilePath = "taco_" + std::to_string(Unique) + ".inc";

    std::string Cmd = "taco \"" + Expr +
                      "\" "+Format+" -print-compute -print-assembly"+
		      "  -print-nocolor  > " +
                      TempFilePath;
    int Res = system(Cmd.c_str());
    if (Res != 0){
       std::remove(TempFilePath.c_str());
       llvm::errs() << "taco: Error in Plugin Function: " 
	       << DeclText << "\n. Check if taco is installed \n";
       assert( 0);
    }
    // read the file generated by taco.
    std::ifstream IS(TempFilePath, std::ifstream::binary);
    
    assert(IS && "Taco could not create temp file");
    std::string TacoCode;
    llvm::raw_string_ostream TacoStream(TacoCode);
//    IS.seekg(0, IS.end);
//    int Length = IS.tellg();
//    IS.seekg(0, IS.beg);
//
//    char *buffer = new char[Length];
//    IS.read(buffer, Length);
//    IS.close();
    std::string line;
    if (IS.is_open()){
       while ( getline (IS,line) ){
          TacoStream << line << '\n';
       }
       IS.close();
    }
    TacoStream.str();
    
    
    std::remove(TempFilePath.c_str());
    

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
	llvm::errs() << ExprName << " \n";
	assert(SearchTensor != TensorMap.end() && 
			"taco: Tensor parameters do not match expression");
	
	// Check if code has already been generated for the 
	// tensor. This is to prevent duplications.
	if (SearchTensor->second.IsVisited){
	   continue;
	}
        SearchTensor->second.IsVisited = true;

	//conversion routines
	if (SearchTensor->second.RequiresConversion){
            //calls the taco conversion routine function ptr
	   if (SearchTensor->second.IsTemplate){
	      
	      TacoPreConversionRoutines +="taco_tensor_t * __taco_"+
		   ExprName + " = "+ExprName+"->__2taco ();\n";
	      
	   }else{
	      TacoPreConversionRoutines +="taco_tensor_t * __taco_"+
		   ExprName + " = "+ExprName+"->"+
		   SearchTensor->second.StructTypeName+"2taco ("+ExprName+
		   ");\n";
           
	   }

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
              if (SearchTensor->second.IsTemplate){
	         // Member function is called for templates
		 // to revert back to original object.
		 TacoPostConversionRoutines+=ExprName+
			 "->taco2__(__taco_"+ExprName+");\n";

	      }else{
		 // Non template types are assumed to use function
		 // pointers. 
	         TacoPostConversionRoutines+=ExprName+"->taco2"+
		      SearchTensor->second.StructTypeName+ 
		      "(__taco_"+ExprName+","+ExprName+");\n";
	      }
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
    
    // write taco definitions
    OS << TacoCode << " \n";
    // rewrite original function
    OS << DeclText << "\n";
    OS << "\n{\n";
    OS << TacoPreConversionRoutines << "\n";
       // write assemble function call
    OS << AssembleFunctionCall << " \n";
    // write compute function call.
    OS << ComputeFunctionCall << "\n";

    OS << TacoPostConversionRoutines << "\n";
    
    OS << TacoVariableCleanup << "\n";


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
    
    
    
    
    
    // Write taco clean up routine for temp variables.

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
  }
};

} // namespace

static SyntaxHandlerRegistry::Add<TacoTokensHandler> X("taco",
                                                       "generates taco code");
