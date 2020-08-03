// RUN: %clang -I$TACO/include -L$TACO/build/lib -ltaco -std=c++11 -fplugin=%llvmshlibdir/TacoTokensSyntax%pluginext -emit-llvm -S %s -o - | FileCheck %s
// REQUIRES: plugins, examples

typedef struct csr_d {
  double *vals; // values
  int *cols;    // column index
  int *rptr;    // row pointer
  int nnz;      // number of non zeros
  int *order;   // storage mapping order 0,1 for row first
  int nr;       // number of rows
  
  
  // function pointer to be overloaded 
  // for converting csr to taco. if not
  // overloaded default is used.
  taco_tensor_t* (* csr_d2taco) (struct csr_d *);

  // function pointer for 
  // converting back
  // to csr
  void (* taco2csr_d) (taco_tensor_t*,struct csr_d* );

} csr_d;


[[clang::syntax(taco)]] void mat_vec_mul(taco_tensor_t *a,csr_d *b
		,taco_tensor_t *c,char* format="-f=b:ds:0,1") {
   a(i) = b(i,j) * c(j)
}

// FORGOTTEN FUNCTION BY TACO PLUGIN

//CHECK: define dso_local void @_Z13__mat_vec_mulP13taco_tensor_tP5csr_dS0_Pc(%struct.taco_tensor_t* %a, %struct.csr_d* %b, %struct.taco_tensor_t* %c, i8* %format) #0 {
//CHECK-NEXT: entry:
//CHECK-NEXT:   %a.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %b.addr = alloca %struct.csr_d*, align 8
//CHECK-NEXT:   %c.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %format.addr = alloca i8*, align 8
//CHECK-NEXT:   store %struct.taco_tensor_t* %a, %struct.taco_tensor_t** %a.addr, align 8
//CHECK-NEXT:   store %struct.csr_d* %b, %struct.csr_d** %b.addr, align 8
//CHECK-NEXT:   store %struct.taco_tensor_t* %c, %struct.taco_tensor_t** %c.addr, align 8
//CHECK-NEXT:   store i8* %format, i8** %format.addr, align 8
//CHECK-NEXT:   ret void
//CHECK-NEXT: }




// GENERATED ASSEMBLY CODE

//CHECK: define dso_local i32 @_Z13__taco_assm_0P13taco_tensor_tS0_S0_(%struct.taco_tensor_t* %a, %struct.taco_tensor_t* %b, %struct.taco_tensor_t* %c) #0 {
//CHECK-NEXT: entry:
//CHECK-NEXT:   %a.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %b.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %c.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %a1_dimension = alloca i32, align 4
//CHECK-NEXT:   %a_vals = alloca double*, align 8
//CHECK-NEXT:   store %struct.taco_tensor_t* %a, %struct.taco_tensor_t** %a.addr, align 8
//CHECK-NEXT:   store %struct.taco_tensor_t* %b, %struct.taco_tensor_t** %b.addr, align 8
//CHECK-NEXT:   store %struct.taco_tensor_t* %c, %struct.taco_tensor_t** %c.addr, align 8
//CHECK-NEXT:   %0 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %a.addr, align 8
//CHECK-NEXT:   %dimensions = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %0, i32 0, i32 1
//CHECK-NEXT:   %1 = load i32*, i32** %dimensions, align 8
//CHECK-NEXT:   %arrayidx = getelementptr inbounds i32, i32* %1, i64 0
//CHECK-NEXT:   %2 = load i32, i32* %arrayidx, align 4
//CHECK-NEXT:   store i32 %2, i32* %a1_dimension, align 4
//CHECK-NEXT:   %3 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %a.addr, align 8
//CHECK-NEXT:   %vals = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %3, i32 0, i32 6
//CHECK-NEXT:   %4 = load i8*, i8** %vals, align 8
//CHECK-NEXT:   %5 = bitcast i8* %4 to double*
//CHECK-NEXT:   store double* %5, double** %a_vals, align 8
//CHECK-NEXT:   %6 = load i32, i32* %a1_dimension, align 4
//CHECK-NEXT:   %conv = sext i32 %6 to i64
//CHECK-NEXT:   %mul = mul i64 8, %conv
//CHECK-NEXT:   %call = call noalias i8* @malloc(i64 %mul) #3
//CHECK-NEXT:   %7 = bitcast i8* %call to double*
//CHECK-NEXT:   store double* %7, double** %a_vals, align 8
//CHECK-NEXT:   %8 = load double*, double** %a_vals, align 8
//CHECK-NEXT:   %9 = bitcast double* %8 to i8*
//CHECK-NEXT:   %10 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %a.addr, align 8
//CHECK-NEXT:   %vals1 = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %10, i32 0, i32 6
//CHECK-NEXT:   store i8* %9, i8** %vals1, align 8
//CHECK-NEXT:   ret i32 0
//CHECK-NEXT: }



// GENERATED COMPUTE CODE BY TACO COMPILER

//CHECK: define dso_local i32 @_Z15__taco_comput_0P13taco_tensor_tS0_S0_(%struct.taco_tensor_t* %a, %struct.taco_tensor_t* %b, %struct.taco_tensor_t* %c) #0 {
//CHECK-NEXT: entry:
//CHECK-NEXT:   %a.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %b.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %c.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %a1_dimension = alloca i32, align 4
//CHECK-NEXT:   %a_vals = alloca double*, align 8
//CHECK-NEXT:   %b1_dimension = alloca i32, align 4
//CHECK-NEXT:   %b2_pos = alloca i32*, align 8
//CHECK-NEXT:   %b2_crd = alloca i32*, align 8
//CHECK-NEXT:   %b_vals = alloca double*, align 8
//CHECK-NEXT:   %c1_dimension = alloca i32, align 4
//CHECK-NEXT:   %c_vals = alloca double*, align 8
//CHECK-NEXT:   %pa = alloca i32, align 4
//CHECK-NEXT:   %i = alloca i32, align 4
//CHECK-NEXT:   %jb = alloca i32, align 4
//CHECK-NEXT:   store %struct.taco_tensor_t* %a, %struct.taco_tensor_t** %a.addr, align 8
//CHECK-NEXT:   store %struct.taco_tensor_t* %b, %struct.taco_tensor_t** %b.addr, align 8
//CHECK-NEXT:   store %struct.taco_tensor_t* %c, %struct.taco_tensor_t** %c.addr, align 8
//CHECK-NEXT:   %0 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %a.addr, align 8
//CHECK-NEXT:   %dimensions = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %0, i32 0, i32 1
//CHECK-NEXT:   %1 = load i32*, i32** %dimensions, align 8
//CHECK-NEXT:   %arrayidx = getelementptr inbounds i32, i32* %1, i64 0
//CHECK-NEXT:   %2 = load i32, i32* %arrayidx, align 4
//CHECK-NEXT:   store i32 %2, i32* %a1_dimension, align 4
//CHECK-NEXT:   %3 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %a.addr, align 8
//CHECK-NEXT:   %vals = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %3, i32 0, i32 6
//CHECK-NEXT:   %4 = load i8*, i8** %vals, align 8
//CHECK-NEXT:   %5 = bitcast i8* %4 to double*
//CHECK-NEXT:   store double* %5, double** %a_vals, align 8
//CHECK-NEXT:   %6 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %b.addr, align 8
//CHECK-NEXT:   %dimensions1 = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %6, i32 0, i32 1
//CHECK-NEXT:   %7 = load i32*, i32** %dimensions1, align 8
//CHECK-NEXT:   %arrayidx2 = getelementptr inbounds i32, i32* %7, i64 0
//CHECK-NEXT:   %8 = load i32, i32* %arrayidx2, align 4
//CHECK-NEXT:   store i32 %8, i32* %b1_dimension, align 4
//CHECK-NEXT:   %9 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %b.addr, align 8
//CHECK-NEXT:   %indices = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %9, i32 0, i32 5
//CHECK-NEXT:   %10 = load i8***, i8**** %indices, align 8
//CHECK-NEXT:   %arrayidx3 = getelementptr inbounds i8**, i8*** %10, i64 1
//CHECK-NEXT:   %11 = load i8**, i8*** %arrayidx3, align 8
//CHECK-NEXT:   %arrayidx4 = getelementptr inbounds i8*, i8** %11, i64 0
//CHECK-NEXT:   %12 = load i8*, i8** %arrayidx4, align 8
//CHECK-NEXT:   %13 = bitcast i8* %12 to i32*
//CHECK-NEXT:   store i32* %13, i32** %b2_pos, align 8
//CHECK-NEXT:   %14 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %b.addr, align 8
//CHECK-NEXT:   %indices5 = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %14, i32 0, i32 5
//CHECK-NEXT:   %15 = load i8***, i8**** %indices5, align 8
//CHECK-NEXT:   %arrayidx6 = getelementptr inbounds i8**, i8*** %15, i64 1
//CHECK-NEXT:   %16 = load i8**, i8*** %arrayidx6, align 8
//CHECK-NEXT:   %arrayidx7 = getelementptr inbounds i8*, i8** %16, i64 1
//CHECK-NEXT:   %17 = load i8*, i8** %arrayidx7, align 8
//CHECK-NEXT:   %18 = bitcast i8* %17 to i32*
//CHECK-NEXT:   store i32* %18, i32** %b2_crd, align 8
//CHECK-NEXT:   %19 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %b.addr, align 8
//CHECK-NEXT:   %vals8 = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %19, i32 0, i32 6
//CHECK-NEXT:   %20 = load i8*, i8** %vals8, align 8
//CHECK-NEXT:   %21 = bitcast i8* %20 to double*
//CHECK-NEXT:   store double* %21, double** %b_vals, align 8
//CHECK-NEXT:   %22 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %c.addr, align 8
//CHECK-NEXT:   %dimensions9 = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %22, i32 0, i32 1
//CHECK-NEXT:   %23 = load i32*, i32** %dimensions9, align 8
//CHECK-NEXT:   %arrayidx10 = getelementptr inbounds i32, i32* %23, i64 0
//CHECK-NEXT:   %24 = load i32, i32* %arrayidx10, align 4
//CHECK-NEXT:   store i32 %24, i32* %c1_dimension, align 4
//CHECK-NEXT:   %25 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %c.addr, align 8
//CHECK-NEXT:   %vals11 = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %25, i32 0, i32 6
//CHECK-NEXT:   %26 = load i8*, i8** %vals11, align 8
//CHECK-NEXT:   %27 = bitcast i8* %26 to double*
//CHECK-NEXT:   store double* %27, double** %c_vals, align 8
//CHECK-NEXT:   store i32 0, i32* %pa, align 4
//CHECK-NEXT:   br label %for.cond
//CHECK-NEXT: 
//CHECK-NEXT: for.cond:                                         ; preds = %for.inc, %entry
//CHECK-NEXT:   %28 = load i32, i32* %pa, align 4
//CHECK-NEXT:   %29 = load i32, i32* %a1_dimension, align 4
//CHECK-NEXT:   %cmp = icmp slt i32 %28, %29
//CHECK-NEXT:   br i1 %cmp, label %for.body, label %for.end
//CHECK-NEXT: 
//CHECK-NEXT: for.body:                                         ; preds = %for.cond
//CHECK-NEXT:   %30 = load double*, double** %a_vals, align 8
//CHECK-NEXT:   %31 = load i32, i32* %pa, align 4
//CHECK-NEXT:   %idxprom = sext i32 %31 to i64
//CHECK-NEXT:   %arrayidx12 = getelementptr inbounds double, double* %30, i64 %idxprom
//CHECK-NEXT:   store double 0.000000e+00, double* %arrayidx12, align 8
//CHECK-NEXT:   br label %for.inc
//CHECK-NEXT: 
//CHECK-NEXT: for.inc:                                          ; preds = %for.body
//CHECK-NEXT:   %32 = load i32, i32* %pa, align 4
//CHECK-NEXT:   %inc = add nsw i32 %32, 1
//CHECK-NEXT:   store i32 %inc, i32* %pa, align 4
//CHECK-NEXT:   br label %for.cond
//CHECK-NEXT: 
//CHECK-NEXT: for.end:                                          ; preds = %for.cond
//CHECK-NEXT:   store i32 0, i32* %i, align 4
//CHECK-NEXT:   br label %for.cond13
//CHECK-NEXT: 
//CHECK-NEXT: for.cond13:                                       ; preds = %for.inc35, %for.end
//CHECK-NEXT:   %33 = load i32, i32* %i, align 4
//CHECK-NEXT:   %34 = load i32, i32* %c1_dimension, align 4
//CHECK-NEXT:   %cmp14 = icmp slt i32 %33, %34
//CHECK-NEXT:   br i1 %cmp14, label %for.body15, label %for.end37
//CHECK-NEXT: 
//CHECK-NEXT: for.body15:                                       ; preds = %for.cond13
//CHECK-NEXT:   %35 = load i32*, i32** %b2_pos, align 8
//CHECK-NEXT:   %36 = load i32, i32* %i, align 4
//CHECK-NEXT:   %idxprom16 = sext i32 %36 to i64
//CHECK-NEXT:   %arrayidx17 = getelementptr inbounds i32, i32* %35, i64 %idxprom16
//CHECK-NEXT:   %37 = load i32, i32* %arrayidx17, align 4
//CHECK-NEXT:   store i32 %37, i32* %jb, align 4
//CHECK-NEXT:   br label %for.cond18
//CHECK-NEXT: 
//CHECK-NEXT: for.cond18:                                       ; preds = %for.inc32, %for.body15
//CHECK-NEXT:   %38 = load i32, i32* %jb, align 4
//CHECK-NEXT:   %39 = load i32*, i32** %b2_pos, align 8
//CHECK-NEXT:   %40 = load i32, i32* %i, align 4
//CHECK-NEXT:   %add = add nsw i32 %40, 1
//CHECK-NEXT:   %idxprom19 = sext i32 %add to i64
//CHECK-NEXT:   %arrayidx20 = getelementptr inbounds i32, i32* %39, i64 %idxprom19
//CHECK-NEXT:   %41 = load i32, i32* %arrayidx20, align 4
//CHECK-NEXT:   %cmp21 = icmp slt i32 %38, %41
//CHECK-NEXT:   br i1 %cmp21, label %for.body22, label %for.end34
//CHECK-NEXT: 
//CHECK-NEXT: for.body22:                                       ; preds = %for.cond18
//CHECK-NEXT:   %42 = load double*, double** %a_vals, align 8
//CHECK-NEXT:   %43 = load i32, i32* %i, align 4
//CHECK-NEXT:   %idxprom23 = sext i32 %43 to i64
//CHECK-NEXT:   %arrayidx24 = getelementptr inbounds double, double* %42, i64 %idxprom23
//CHECK-NEXT:   %44 = load double, double* %arrayidx24, align 8
//CHECK-NEXT:   %45 = load double*, double** %b_vals, align 8
//CHECK-NEXT:   %46 = load i32, i32* %jb, align 4
//CHECK-NEXT:   %idxprom25 = sext i32 %46 to i64
//CHECK-NEXT:   %arrayidx26 = getelementptr inbounds double, double* %45, i64 %idxprom25
//CHECK-NEXT:   %47 = load double, double* %arrayidx26, align 8
//CHECK-NEXT:   %48 = load double*, double** %c_vals, align 8
//CHECK-NEXT:   %49 = load i32, i32* %i, align 4
//CHECK-NEXT:   %idxprom27 = sext i32 %49 to i64
//CHECK-NEXT:   %arrayidx28 = getelementptr inbounds double, double* %48, i64 %idxprom27
//CHECK-NEXT:   %50 = load double, double* %arrayidx28, align 8
//CHECK-NEXT:   %mul = fmul double %47, %50
//CHECK-NEXT:   %add29 = fadd double %44, %mul
//CHECK-NEXT:   %51 = load double*, double** %a_vals, align 8
//CHECK-NEXT:   %52 = load i32, i32* %i, align 4
//CHECK-NEXT:   %idxprom30 = sext i32 %52 to i64
//CHECK-NEXT:   %arrayidx31 = getelementptr inbounds double, double* %51, i64 %idxprom30
//CHECK-NEXT:   store double %add29, double* %arrayidx31, align 8
//CHECK-NEXT:   br label %for.inc32
//CHECK-NEXT: 
//CHECK-NEXT: for.inc32:                                        ; preds = %for.body22
//CHECK-NEXT:   %53 = load i32, i32* %jb, align 4
//CHECK-NEXT:   %inc33 = add nsw i32 %53, 1
//CHECK-NEXT:   store i32 %inc33, i32* %jb, align 4
//CHECK-NEXT:   br label %for.cond18
//CHECK-NEXT: 
//CHECK-NEXT: for.end34:                                        ; preds = %for.cond18
//CHECK-NEXT:   br label %for.inc35
//CHECK-NEXT: 
//CHECK-NEXT: for.inc35:                                        ; preds = %for.end34
//CHECK-NEXT:   %54 = load i32, i32* %i, align 4
//CHECK-NEXT:   %inc36 = add nsw i32 %54, 1
//CHECK-NEXT:   store i32 %inc36, i32* %i, align 4
//CHECK-NEXT:   br label %for.cond13
//CHECK-NEXT: 
//CHECK-NEXT: for.end37:                                        ; preds = %for.cond13
//CHECK-NEXT:   ret i32 0
//CHECK-NEXT: }


// NEW FUNCTION GENERATED BY TACO PLUGIN

//CHECK: define dso_local void @_Z11mat_vec_mulP13taco_tensor_tP5csr_dS0_Pc(%struct.taco_tensor_t* %a, %struct.csr_d* %b, %struct.taco_tensor_t* %c, i8* %format) #2 {
//CHECK-NEXT: entry:
//CHECK-NEXT:   %a.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %b.addr = alloca %struct.csr_d*, align 8
//CHECK-NEXT:   %c.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %format.addr = alloca i8*, align 8
//CHECK-NEXT:   %__taco_b = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   store %struct.taco_tensor_t* %a, %struct.taco_tensor_t** %a.addr, align 8
//CHECK-NEXT:   store %struct.csr_d* %b, %struct.csr_d** %b.addr, align 8
//CHECK-NEXT:   store %struct.taco_tensor_t* %c, %struct.taco_tensor_t** %c.addr, align 8
//CHECK-NEXT:   store i8* %format, i8** %format.addr, align 8
//CHECK-NEXT:   %0 = load %struct.csr_d*, %struct.csr_d** %b.addr, align 8
//CHECK-NEXT:   %csr_d2taco = getelementptr inbounds %struct.csr_d, %struct.csr_d* %0, i32 0, i32 6
//CHECK-NEXT:   %1 = load %struct.taco_tensor_t* (%struct.csr_d*)*, %struct.taco_tensor_t* (%struct.csr_d*)** %csr_d2taco, align 8
//CHECK-NEXT:   %2 = load %struct.csr_d*, %struct.csr_d** %b.addr, align 8
//CHECK-NEXT:   %call = call %struct.taco_tensor_t* %1(%struct.csr_d* %2)
//CHECK-NEXT:   store %struct.taco_tensor_t* %call, %struct.taco_tensor_t** %__taco_b, align 8
//CHECK-NEXT:   %3 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %a.addr, align 8
//CHECK-NEXT:   %4 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %__taco_b, align 8
//CHECK-NEXT:   %5 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %c.addr, align 8
//CHECK-NEXT:   %call1 = call i32 @_Z13__taco_assm_0P13taco_tensor_tS0_S0_(%struct.taco_tensor_t* %3, %struct.taco_tensor_t* %4, %struct.taco_tensor_t* %5)
//CHECK-NEXT:   %6 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %a.addr, align 8
//CHECK-NEXT:   %7 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %__taco_b, align 8
//CHECK-NEXT:   %8 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %c.addr, align 8
//CHECK-NEXT:   %call2 = call i32 @_Z15__taco_comput_0P13taco_tensor_tS0_S0_(%struct.taco_tensor_t* %6, %struct.taco_tensor_t* %7, %struct.taco_tensor_t* %8)
//CHECK-NEXT:   %9 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %__taco_b, align 8
//CHECK-NEXT:   call void @_ZL19__taco_cleanup_tacoP13taco_tensor_t(%struct.taco_tensor_t* %9)
//CHECK-NEXT:   ret void
//CHECK-NEXT: }
//
//

// CLEAN UP TACO GENERATED BY TACO PLUGIN


//CHECK: define internal void @_ZL19__taco_cleanup_tacoP13taco_tensor_t(%struct.taco_tensor_t* %t) #0 {
//CHECK-NEXT: entry:
//CHECK-NEXT:   %t.addr = alloca %struct.taco_tensor_t*, align 8
//CHECK-NEXT:   %i = alloca i32, align 4
//CHECK-NEXT:   store %struct.taco_tensor_t* %t, %struct.taco_tensor_t** %t.addr, align 8
//CHECK-NEXT:   %0 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %t.addr, align 8
//CHECK-NEXT:   %tobool = icmp ne %struct.taco_tensor_t* %0, null
//CHECK-NEXT:   br i1 %tobool, label %if.then, label %if.end
//CHECK-NEXT: 
//CHECK-NEXT: if.then:                                          ; preds = %entry
//CHECK-NEXT:   store i32 0, i32* %i, align 4
//CHECK-NEXT:   br label %for.cond
//CHECK-NEXT: 
//CHECK-NEXT: for.cond:                                         ; preds = %for.inc, %if.then
//CHECK-NEXT:   %1 = load i32, i32* %i, align 4
//CHECK-NEXT:   %2 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %t.addr, align 8
//CHECK-NEXT:   %order = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %2, i32 0, i32 0
//CHECK-NEXT:   %3 = load i32, i32* %order, align 8
//CHECK-NEXT:   %cmp = icmp slt i32 %1, %3
//CHECK-NEXT:   br i1 %cmp, label %for.body, label %for.end
//CHECK-NEXT: 
//CHECK-NEXT: for.body:                                         ; preds = %for.cond
//CHECK-NEXT:   %4 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %t.addr, align 8
//CHECK-NEXT:   %indices = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %4, i32 0, i32 5
//CHECK-NEXT:   %5 = load i8***, i8**** %indices, align 8
//CHECK-NEXT:   %6 = load i32, i32* %i, align 4
//CHECK-NEXT:   %idxprom = sext i32 %6 to i64
//CHECK-NEXT:   %arrayidx = getelementptr inbounds i8**, i8*** %5, i64 %idxprom
//CHECK-NEXT:   %7 = load i8**, i8*** %arrayidx, align 8
//CHECK-NEXT:   %8 = bitcast i8** %7 to i8*
//CHECK-NEXT:   call void @free(i8* %8) #3
//CHECK-NEXT:   br label %for.inc
//CHECK-NEXT: 
//CHECK-NEXT: for.inc:                                          ; preds = %for.body
//CHECK-NEXT:   %9 = load i32, i32* %i, align 4
//CHECK-NEXT:   %inc = add nsw i32 %9, 1
//CHECK-NEXT:   store i32 %inc, i32* %i, align 4
//CHECK-NEXT:   br label %for.cond
//CHECK-NEXT: 
//CHECK-NEXT: for.end:                                          ; preds = %for.cond
//CHECK-NEXT:   %10 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %t.addr, align 8
//CHECK-NEXT:   %indices1 = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %10, i32 0, i32 5
//CHECK-NEXT:   %11 = load i8***, i8**** %indices1, align 8
//CHECK-NEXT:   %12 = bitcast i8*** %11 to i8*
//CHECK-NEXT:   call void @free(i8* %12) #3
//CHECK-NEXT:   %13 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %t.addr, align 8
//CHECK-NEXT:   %dimensions = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %13, i32 0, i32 1
//CHECK-NEXT:   %14 = load i32*, i32** %dimensions, align 8
//CHECK-NEXT:   %15 = bitcast i32* %14 to i8*
//CHECK-NEXT:   call void @free(i8* %15) #3
//CHECK-NEXT:   %16 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %t.addr, align 8
//CHECK-NEXT:   %mode_ordering = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %16, i32 0, i32 3
//CHECK-NEXT:   %17 = load i32*, i32** %mode_ordering, align 8
//CHECK-NEXT:   %18 = bitcast i32* %17 to i8*
//CHECK-NEXT:   call void @free(i8* %18) #3
//CHECK-NEXT:   %19 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %t.addr, align 8
//CHECK-NEXT:   %mode_types = getelementptr inbounds %struct.taco_tensor_t, %struct.taco_tensor_t* %19, i32 0, i32 4
//CHECK-NEXT:   %20 = load i32*, i32** %mode_types, align 8
//CHECK-NEXT:   %21 = bitcast i32* %20 to i8*
//CHECK-NEXT:   call void @free(i8* %21) #3
//CHECK-NEXT:   %22 = load %struct.taco_tensor_t*, %struct.taco_tensor_t** %t.addr, align 8
//CHECK-NEXT:   %23 = bitcast %struct.taco_tensor_t* %22 to i8*
//CHECK-NEXT:   call void @free(i8* %23) #3
//CHECK-NEXT:   br label %if.end
//CHECK-NEXT: 
//CHECK-NEXT: if.end:                                           ; preds = %for.end, %entry
//CHECK-NEXT:   ret void
//CHECK-NEXT: }
