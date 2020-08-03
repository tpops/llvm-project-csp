// RUN: env TACO=${TACO}
// RUN: %clang -I%TACO/include -L%TACO/build/lib -ltaco -std=c++11 -fplugin=%llvmshlibdir/TacoTokensSyntax%pluginext -emit-llvm -S %s -o - | FileCheck %s
// REQUIRES: plugins, examples

#include <string>

[[clang::syntax(taco)]] void mat_vec_mul(taco_tensor_t *a,taco_tensor_t *b
		,taco_tensor_t *c , std::string format = "") {
   a(i) = b(i,j) * c(i)
}

//CHECK: define dso_local void @_Z13__mat_vec_mulP13taco_tensor_tS0_S0_NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE(%struct.taco_tensor_t* %a, %struct.taco_tensor_t* %b, %struct.taco_tensor_t* %c, %"class.std::__cxx11::basic_string"* %format) #0 !dbg !847 {
//CHECK-NEXT: entry:
//CHECK-NEXT:   %a.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %b.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %c.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   store %struct.taco_tensor_t* %a, %struct.taco_tensor_t** %a.addr, align 8
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata %struct.taco_tensor_t** %a.addr, metadata !873, metadata !DIExpression()), !dbg !874
//CHECK-NEXT:   store %struct.taco_tensor_t* %b, %struct.taco_tensor_t** %b.addr, align 8
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata %struct.taco_tensor_t** %b.addr, metadata !875, metadata !DIExpression()), !dbg !876
//CHECK-NEXT:   store %struct.taco_tensor_t* %c, %struct.taco_tensor_t** %c.addr, align 8
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata %struct.taco_tensor_t** %c.addr, metadata !877, metadata !DIExpression()), !dbg !878
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata %"class.std::__cxx11::basic_string"* %format, metadata !879, metadata !DIExpression()), !dbg !880
//CHECK-NEXT:   ret void, !dbg !881
//CHECK-NEXT: }
//CHECK-NEXT: ; Function Attrs: nounwind readnone speculatable willreturn
//CHECK-NEXT: declare void @llvm.dbg.declare(metadata, metadata, metadata) #1
//CHECK-NEXT: ; Function Attrs: noinline nounwind optnone uwtable
//CHECK-NEXT: define dso_local i32 @_Z13__taco_assm_0P13taco_tensor_tS0_S0_(%struct.taco_tensor_t* %a, %struct.taco_tensor_t* %b, %struct.taco_tensor_t* %c) #0 !dbg !883 {
//CHECK-NEXT: entry:
//CHECK-NEXT:   %a.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %b.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %c.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %a1_dimension = alloca i32, align 4
//CHECK-NEXT:   %a_vals = alloca double*, align 8
//CHECK-NEXT:   store %struct.taco_tensor_t* %a, %struct.taco_tensor_t** %a.addr, align 8
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata %struct.taco_tensor_t** %a.addr, metadata !886, metadata !DIExpression()), !dbg !887
//CHECK-NEXT:   store %struct.taco_tensor_t* %b, %struct.taco_tensor_t** %b.addr, align 8
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata %struct.taco_tensor_t** %b.addr, metadata !888, metadata !DIExpression()), !dbg !889
//CHECK-NEXT:   store %struct.taco_tensor_t* %c, %struct.taco_tensor_t** %c.addr, align 8
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata %struct.taco_tensor_t** %c.addr, metadata !890, metadata !DIExpression()), !dbg !891
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata i32* %a1_dimension, metadata !892, metadata !DIExpression()), !dbg !893
//CHECK-NEXT:   %0 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %a.addr, align 8, !dbg !894
//CHECK-NEXT:   %dimensions = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %0, i32 0, i32 1, !dbg !895
//CHECK-NEXT:   %1 = load i32*, i32** %dimensions, align 8, !dbg !895
//CHECK-NEXT:   %arrayidx = getelementptr inbounds i32, i32* %1, i64 0, !dbg !894
//CHECK-NEXT:   %2 = load i32, i32* %arrayidx, align 4, !dbg !894
//CHECK-NEXT:   store i32 %2, i32* %a1_dimension, align 4, !dbg !893
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata double** %a_vals, metadata !896, metadata !DIExpression()), !dbg !898
//CHECK-NEXT:   %3 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %a.addr, align 8, !dbg !899
//CHECK-NEXT:   %vals = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %3, i32 0, i32 6, !dbg !900
//CHECK-NEXT:   %4 = load i8*, i8** %vals, align 8, !dbg !900
//CHECK-NEXT:   %5 = bitcast i8* %4 to double*, !dbg !901
//CHECK-NEXT:   store double* %5, double** %a_vals, align 8, !dbg !898
//CHECK-NEXT:   %6 = load i32, i32* %a1_dimension, align 4, !dbg !902
//CHECK-NEXT:   %conv = sext i32 %6 to i64, !dbg !902
//CHECK-NEXT:   %mul = mul i64 8, %conv, !dbg !903
//CHECK-NEXT:   %call = call noalias i8* @malloc(i64 %mul) #3, !dbg !904
//CHECK-NEXT:   %7 = bitcast i8* %call to double*, !dbg !905
//CHECK-NEXT:   store double* %7, double** %a_vals, align 8, !dbg !906
//CHECK-NEXT:   %8 = load double*, double** %a_vals, align 8, !dbg !907
//CHECK-NEXT:   %9 = bitcast double* %8 to i8*, !dbg !908
//CHECK-NEXT:   %10 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %a.addr, align 8, !dbg !909
//CHECK-NEXT:   %vals1 = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %10, i32 0, i32 6, !dbg !910
//CHECK-NEXT:   store i8* %9, i8** %vals1, align 8, !dbg !911
//CHECK-NEXT:   ret i32 0, !dbg !912
//CHECK-NEXT: }






//CHECK: define dso_local void @_Z11mat_vec_mulP13taco_tensor_tS0_S0_NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE(%struct.taco_tensor_t* %a, %struct.taco_tensor_t* %b, %struct.taco_tensor_t* %c, %"class.std::__cxx11::basic_string"* %format) #0 !dbg !1001 {
//CHECK-NEXT: entry:
//CHECK-NEXT:   %a.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %b.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %c.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   store %struct.taco_tensor_t* %a, %struct.taco_tensor_t** %a.addr, align 8
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata %struct.taco_tensor_t** %a.addr, metadata !1002, metadata !DIExpression()), !dbg !1003
//CHECK-NEXT:   store %struct.taco_tensor_t* %b, %struct.taco_tensor_t** %b.addr, align 8
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata %struct.taco_tensor_t** %b.addr, metadata !1004, metadata !DIExpression()), !dbg !1005
//CHECK-NEXT:   store %struct.taco_tensor_t* %c, %struct.taco_tensor_t** %c.addr, align 8
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata %struct.taco_tensor_t** %c.addr, metadata !1006, metadata !DIExpression()), !dbg !1007
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata %"class.std::__cxx11::basic_string"* %format, metadata !1008, metadata !DIExpression()), !dbg !1009
//CHECK-NEXT:   %0 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %a.addr, align 8, !dbg !1010
//CHECK-NEXT:   %1 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %b.addr, align 8, !dbg !1011
//CHECK-NEXT:   %2 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %c.addr, align 8, !dbg !1012
//CHECK-NEXT:   %call = call i32 @_Z13__taco_assm_0P13taco_tensor_tS0_S0_(%struct.taco_tensor_t* %0, %struct.taco_tensor_t* %1, %struct.taco_tensor_t* %2), !dbg !1013
//CHECK-NEXT:   %3 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %a.addr, align 8, !dbg !1014
//CHECK-NEXT:   %4 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %b.addr, align 8, !dbg !1015
//CHECK-NEXT:   %5 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %c.addr, align 8, !dbg !1016
//CHECK-NEXT:   %call1 = call i32 @_Z15__taco_comput_0P13taco_tensor_tS0_S0_(%struct.taco_tensor_t* %3, %struct.taco_tensor_t* %4, %struct.taco_tensor_t* %5), !dbg !1017
//CHECK-NEXT:   ret void, !dbg !1018
//CHECK-NEXT: }








//CHECK: define dso_local i32 @_Z13__taco_assm_0P13taco_tensor_tS0_S0_(%struct.taco_tensor_t* %a, %struct.taco_tensor_t* %b, %struct.taco_tensor_t* %c) #0 !dbg !883 {
//CHECK-NEXT: entry:
//CHECK-NEXT:   %a.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %b.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %c.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %a1_dimension = alloca i32, align 4
//CHECK-NEXT:   %a_vals = alloca double*, align 8
//CHECK-NEXT:   store %struct.taco_tensor_t* %a, %struct.taco_tensor_t** %a.addr, align 8
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata %struct.taco_tensor_t** %a.addr, metadata !886, metadata !DIExpression()), !dbg !887
//CHECK-NEXT:   store %struct.taco_tensor_t* %b, %struct.taco_tensor_t** %b.addr, align 8
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata %struct.taco_tensor_t** %b.addr, metadata !888, metadata !DIExpression()), !dbg !889
//CHECK-NEXT:   store %struct.taco_tensor_t* %c, %struct.taco_tensor_t** %c.addr, align 8
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata %struct.taco_tensor_t** %c.addr, metadata !890, metadata !DIExpression()), !dbg !891
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata i32* %a1_dimension, metadata !892, metadata !DIExpression()), !dbg !893
//CHECK-NEXT:   %0 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %a.addr, align 8, !dbg !894
//CHECK-NEXT:   %dimensions = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %0, i32 0, i32 1, !dbg !895
//CHECK-NEXT:   %1 = load i32*, i32** %dimensions, align 8, !dbg !895
//CHECK-NEXT:   %arrayidx = getelementptr inbounds i32, i32* %1, i64 0, !dbg !894
//CHECK-NEXT:   %2 = load i32, i32* %arrayidx, align 4, !dbg !894
//CHECK-NEXT:   store i32 %2, i32* %a1_dimension, align 4, !dbg !893
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata double** %a_vals, metadata !896, metadata !DIExpression()), !dbg !898
//CHECK-NEXT:   %3 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %a.addr, align 8, !dbg !899
//CHECK-NEXT:   %vals = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %3, i32 0, i32 6, !dbg !900
//CHECK-NEXT:   %4 = load i8*, i8** %vals, align 8, !dbg !900
//CHECK-NEXT:   %5 = bitcast i8* %4 to double*, !dbg !901
//CHECK-NEXT:   store double* %5, double** %a_vals, align 8, !dbg !898
//CHECK-NEXT:   %6 = load i32, i32* %a1_dimension, align 4, !dbg !902
//CHECK-NEXT:   %conv = sext i32 %6 to i64, !dbg !902
//CHECK-NEXT:   %mul = mul i64 8, %conv, !dbg !903
//CHECK-NEXT:   %call = call noalias i8* @malloc(i64 %mul) #3, !dbg !904
//CHECK-NEXT:   %7 = bitcast i8* %call to double*, !dbg !905
//CHECK-NEXT:   store double* %7, double** %a_vals, align 8, !dbg !906
//CHECK-NEXT:   %8 = load double*, double** %a_vals, align 8, !dbg !907
//CHECK-NEXT:   %9 = bitcast double* %8 to i8*, !dbg !908
//CHECK-NEXT:   %10 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %a.addr, align 8, !dbg !909
//CHECK-NEXT:   %vals1 = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %10, i32 0, i32 6, !dbg !910
//CHECK-NEXT:   store i8* %9, i8** %vals1, align 8, !dbg !911
//CHECK-NEXT:   ret i32 0, !dbg !912
//CHECK-NEXT: }

//CHECK: define dso_local i32 @_Z15__taco_comput_0P13taco_tensor_tS0_S0_(%struct.taco_tensor_t* %a, %struct.taco_tensor_t* %b, %struct.taco_tensor_t* %c) #0 !dbg !913 {
//CHECK-NEXT: entry:
//CHECK-NEXT:   %a.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %b.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %c.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %a1_dimension = alloca i32, align 4
//CHECK-NEXT:   %a_vals = alloca double*, align 8
//CHECK-NEXT:   %b1_dimension = alloca i32, align 4
//CHECK-NEXT:   %b2_dimension = alloca i32, align 4
//CHECK-NEXT:   %b_vals = alloca double*, align 8
//CHECK-NEXT:   %c1_dimension = alloca i32, align 4
//CHECK-NEXT:   %c_vals = alloca double*, align 8
//CHECK-NEXT:   %i = alloca i32, align 4
//CHECK-NEXT:   %a_val = alloca double, align 8
//CHECK-NEXT:   %j = alloca i32, align 4
//CHECK-NEXT:   %jb = alloca i32, align 4
//CHECK-NEXT:   store %struct.taco_tensor_t* %a, %struct.taco_tensor_t** %a.addr, align 8
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata %struct.taco_tensor_t** %a.addr, metadata !914, metadata !DIExpression()), !dbg !915
//CHECK-NEXT:   store %struct.taco_tensor_t* %b, %struct.taco_tensor_t** %b.addr, align 8
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata %struct.taco_tensor_t** %b.addr, metadata !916, metadata !DIExpression()), !dbg !917
//CHECK-NEXT:   store %struct.taco_tensor_t* %c, %struct.taco_tensor_t** %c.addr, align 8
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata %struct.taco_tensor_t** %c.addr, metadata !918, metadata !DIExpression()), !dbg !919
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata i32* %a1_dimension, metadata !920, metadata !DIExpression()), !dbg !921
//CHECK-NEXT:   %0 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %a.addr, align 8, !dbg !922
//CHECK-NEXT:   %dimensions = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %0, i32 0, i32 1, !dbg !923
//CHECK-NEXT:   %1 = load i32*, i32** %dimensions, align 8, !dbg !923
//CHECK-NEXT:   %arrayidx = getelementptr inbounds i32, i32* %1, i64 0, !dbg !922
//CHECK-NEXT:   %2 = load i32, i32* %arrayidx, align 4, !dbg !922
//CHECK-NEXT:   store i32 %2, i32* %a1_dimension, align 4, !dbg !921
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata double** %a_vals, metadata !924, metadata !DIExpression()), !dbg !925
//CHECK-NEXT:   %3 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %a.addr, align 8, !dbg !926
//CHECK-NEXT:   %vals = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %3, i32 0, i32 6, !dbg !927
//CHECK-NEXT:   %4 = load i8*, i8** %vals, align 8, !dbg !927
//CHECK-NEXT:   %5 = bitcast i8* %4 to double*, !dbg !928
//CHECK-NEXT:   store double* %5, double** %a_vals, align 8, !dbg !925
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata i32* %b1_dimension, metadata !929, metadata !DIExpression()), !dbg !930
//CHECK-NEXT:   %6 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %b.addr, align 8, !dbg !931
//CHECK-NEXT:   %dimensions1 = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %6, i32 0, i32 1, !dbg !932
//CHECK-NEXT:   %7 = load i32*, i32** %dimensions1, align 8, !dbg !932
//CHECK-NEXT:   %arrayidx2 = getelementptr inbounds i32, i32* %7, i64 0, !dbg !931
//CHECK-NEXT:   %8 = load i32, i32* %arrayidx2, align 4, !dbg !931
//CHECK-NEXT:   store i32 %8, i32* %b1_dimension, align 4, !dbg !930
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata i32* %b2_dimension, metadata !933, metadata !DIExpression()), !dbg !934
//CHECK-NEXT:   %9 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %b.addr, align 8, !dbg !935
//CHECK-NEXT:   %dimensions3 = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %9, i32 0, i32 1, !dbg !936
//CHECK-NEXT:   %10 = load i32*, i32** %dimensions3, align 8, !dbg !936
//CHECK-NEXT:   %arrayidx4 = getelementptr inbounds i32, i32* %10, i64 1, !dbg !935
//CHECK-NEXT:   %11 = load i32, i32* %arrayidx4, align 4, !dbg !935
//CHECK-NEXT:   store i32 %11, i32* %b2_dimension, align 4, !dbg !934
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata double** %b_vals, metadata !937, metadata !DIExpression()), !dbg !938
//CHECK-NEXT:   %12 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %b.addr, align 8, !dbg !939
//CHECK-NEXT:   %vals5 = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %12, i32 0, i32 6, !dbg !940
//CHECK-NEXT:   %13 = load i8*, i8** %vals5, align 8, !dbg !940
//CHECK-NEXT:   %14 = bitcast i8* %13 to double*, !dbg !941
//CHECK-NEXT:   store double* %14, double** %b_vals, align 8, !dbg !938
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata i32* %c1_dimension, metadata !942, metadata !DIExpression()), !dbg !943
//CHECK-NEXT:   %15 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %c.addr, align 8, !dbg !944
//CHECK-NEXT:   %dimensions6 = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %15, i32 0, i32 1, !dbg !945
//CHECK-NEXT:   %16 = load i32*, i32** %dimensions6, align 8, !dbg !945
//CHECK-NEXT:   %arrayidx7 = getelementptr inbounds i32, i32* %16, i64 0, !dbg !944
//CHECK-NEXT:   %17 = load i32, i32* %arrayidx7, align 4, !dbg !944
//CHECK-NEXT:   store i32 %17, i32* %c1_dimension, align 4, !dbg !943
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata double** %c_vals, metadata !946, metadata !DIExpression()), !dbg !947
//CHECK-NEXT:   %18 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %c.addr, align 8, !dbg !948
//CHECK-NEXT:   %vals8 = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %18, i32 0, i32 6, !dbg !949
//CHECK-NEXT:   %19 = load i8*, i8** %vals8, align 8, !dbg !949
//CHECK-NEXT:   %20 = bitcast i8* %19 to double*, !dbg !950
//CHECK-NEXT:   store double* %20, double** %c_vals, align 8, !dbg !947
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata i32* %i, metadata !951, metadata !DIExpression()), !dbg !953
//CHECK-NEXT:   store i32 0, i32* %i, align 4, !dbg !953
//CHECK-NEXT:   br label %for.cond, !dbg !954
//CHECK-NEXT: for.cond:                                         ; preds = %for.inc19, %entry
//CHECK-NEXT:   %21 = load i32, i32* %i, align 4, !dbg !955
//CHECK-NEXT:   %22 = load i32, i32* %c1_dimension, align 4, !dbg !957
//CHECK-NEXT:   %cmp = icmp slt i32 %21, %22, !dbg !958
//CHECK-NEXT:   br i1 %cmp, label %for.body, label %for.end21, !dbg !959
//CHECK-NEXT: for.body:                                         ; preds = %for.cond
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata double* %a_val, metadata !960, metadata !DIExpression()), !dbg !962
//CHECK-NEXT:   store double 0.000000e+00, double* %a_val, align 8, !dbg !962
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata i32* %j, metadata !963, metadata !DIExpression()), !dbg !965
//CHECK-NEXT:   store i32 0, i32* %j, align 4, !dbg !965
//CHECK-NEXT:   br label %for.cond9, !dbg !966
//CHECK-NEXT: for.cond9:                                        ; preds = %for.inc, %for.body
//CHECK-NEXT:   %23 = load i32, i32* %j, align 4, !dbg !967
//CHECK-NEXT:   %24 = load i32, i32* %b2_dimension, align 4, !dbg !969
//CHECK-NEXT:   %cmp10 = icmp slt i32 %23, %24, !dbg !970
//CHECK-NEXT:   br i1 %cmp10, label %for.body11, label %for.end, !dbg !971
//CHECK-NEXT: for.body11:                                       ; preds = %for.cond9
//CHECK-NEXT:   call void @llvm.dbg.declare(metadata i32* %jb, metadata !972, metadata !DIExpression()), !dbg !974
//CHECK-NEXT:   %25 = load i32, i32* %i, align 4, !dbg !975
//CHECK-NEXT:   %26 = load i32, i32* %b2_dimension, align 4, !dbg !976
//CHECK-NEXT:   %mul = mul nsw i32 %25, %26, !dbg !977
//CHECK-NEXT:   %27 = load i32, i32* %j, align 4, !dbg !978
//CHECK-NEXT:   %add = add nsw i32 %mul, %27, !dbg !979
//CHECK-NEXT:   store i32 %add, i32* %jb, align 4, !dbg !974
//CHECK-NEXT:   %28 = load double*, double** %b_vals, align 8, !dbg !980
//CHECK-NEXT:   %29 = load i32, i32* %jb, align 4, !dbg !981
//CHECK-NEXT:   %idxprom = sext i32 %29 to i64, !dbg !980
//CHECK-NEXT:   %arrayidx12 = getelementptr inbounds double, double* %28, i64 %idxprom, !dbg !980
//CHECK-NEXT:   %30 = load double, double* %arrayidx12, align 8, !dbg !980
//CHECK-NEXT:   %31 = load double*, double** %c_vals, align 8, !dbg !982
//CHECK-NEXT:   %32 = load i32, i32* %i, align 4, !dbg !983
//CHECK-NEXT:   %idxprom13 = sext i32 %32 to i64, !dbg !982
//CHECK-NEXT:   %arrayidx14 = getelementptr inbounds double, double* %31, i64 %idxprom13, !dbg !982
//CHECK-NEXT:   %33 = load double, double* %arrayidx14, align 8, !dbg !982
//CHECK-NEXT:   %mul15 = fmul double %30, %33, !dbg !984
//CHECK-NEXT:   %34 = load double, double* %a_val, align 8, !dbg !985
//CHECK-NEXT:   %add16 = fadd double %34, %mul15, !dbg !985
//CHECK-NEXT:   store double %add16, double* %a_val, align 8, !dbg !985
//CHECK-NEXT:   br label %for.inc, !dbg !986
//CHECK-NEXT: for.inc:                                          ; preds = %for.body11
//CHECK-NEXT:   %35 = load i32, i32* %j, align 4, !dbg !987
//CHECK-NEXT:   %inc = add nsw i32 %35, 1, !dbg !987
//CHECK-NEXT:   store i32 %inc, i32* %j, align 4, !dbg !987
//CHECK-NEXT:   br label %for.cond9, !dbg !988, !llvm.loop !989
//CHECK-NEXT: for.end:                                          ; preds = %for.cond9
//CHECK-NEXT:   %36 = load double, double* %a_val, align 8, !dbg !991
//CHECK-NEXT:   %37 = load double*, double** %a_vals, align 8, !dbg !992
//CHECK-NEXT:   %38 = load i32, i32* %i, align 4, !dbg !993
//CHECK-NEXT:   %idxprom17 = sext i32 %38 to i64, !dbg !992
//CHECK-NEXT:   %arrayidx18 = getelementptr inbounds double, double* %37, i64 %idxprom17, !dbg !992
//CHECK-NEXT:   store double %36, double* %arrayidx18, align 8, !dbg !994
//CHECK-NEXT:   br label %for.inc19, !dbg !995
//CHECK-NEXT: for.inc19:                                        ; preds = %for.end
//CHECK-NEXT:   %39 = load i32, i32* %i, align 4, !dbg !996
//CHECK-NEXT:   %inc20 = add nsw i32 %39, 1, !dbg !996
//CHECK-NEXT:   store i32 %inc20, i32* %i, align 4, !dbg !996
//CHECK-NEXT:   br label %for.cond, !dbg !997, !llvm.loop !998
//CHECK-NEXT: for.end21:                                        ; preds = %for.cond
//CHECK-NEXT:   ret i32 0, !dbg !1000
//CHECK-NEXT: }

