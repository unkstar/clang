// NOTE: Assertions have been autogenerated by utils/update_cc_test_checks.py
// Check that we can set bounds on references
// REQUIRES: asserts
// RUN: %cheri_purecap_cc1 -cheri-bounds=references-only -O2 -std=c++17 -emit-llvm %s -o - -mllvm -debug-only=cheri-bounds -mllvm -stats 2>%t.dbg | FileCheck %s
// RUN: FileCheck -input-file %t.dbg %s -check-prefix DBG

// DBG: Found record type 'struct Nested' -> is C-like struct type and is marked as final -> setting bounds for 'struct Nested' reference to 8
// DBG: Found scalar type -> setting bounds for 'int' reference to 4
// DBG: Found scalar type -> setting bounds for 'float' reference to 4

struct Nested final {
    int a;
    int b;
};

struct WithNested {
  float f1;
  Nested n;
  float f2;
};

void do_stuff_with_ref(int&);
void do_stuff_with_ref(float&);
void do_stuff_with_ref(Nested& nref);
void do_stuff_with_ptr(Nested* nptr);

// CHECK-LABEL: @_Z18test_subobject_refU3capR10WithNested(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[N:%.*]] = getelementptr inbounds [[STRUCT_WITHNESTED:%.*]], [[STRUCT_WITHNESTED]] addrspace(200)* [[S:%.*]], i64 0, i32 1
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast [[STRUCT_NESTED:%.*]] addrspace(200)* [[N]] to i8 addrspace(200)*
// CHECK-NEXT:    [[TMP1:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set(i8 addrspace(200)* nonnull [[TMP0]], i64 8)
// CHECK-NEXT:    [[REF_WITH_BOUNDS:%.*]] = bitcast i8 addrspace(200)* [[TMP1]] to [[STRUCT_NESTED]] addrspace(200)*
// CHECK-NEXT:    tail call void @_Z17do_stuff_with_refU3capR6Nested(%struct.Nested addrspace(200)* dereferenceable(8) [[REF_WITH_BOUNDS]]) #3
// CHECK-NEXT:    [[TMP2:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set(i8 addrspace(200)* nonnull [[TMP0]], i64 4)
// CHECK-NEXT:    [[REF_WITH_BOUNDS2:%.*]] = bitcast i8 addrspace(200)* [[TMP2]] to i32 addrspace(200)*
// CHECK-NEXT:    tail call void @_Z17do_stuff_with_refU3capRi(i32 addrspace(200)* dereferenceable(4) [[REF_WITH_BOUNDS2]]) #3
// CHECK-NEXT:    [[TMP3:%.*]] = bitcast [[STRUCT_WITHNESTED]] addrspace(200)* [[S]] to i8 addrspace(200)*
// CHECK-NEXT:    [[TMP4:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set(i8 addrspace(200)* nonnull [[TMP3]], i64 4)
// CHECK-NEXT:    [[REF_WITH_BOUNDS3:%.*]] = bitcast i8 addrspace(200)* [[TMP4]] to float addrspace(200)*
// CHECK-NEXT:    tail call void @_Z17do_stuff_with_refU3capRf(float addrspace(200)* dereferenceable(4) [[REF_WITH_BOUNDS3]]) #3
// CHECK-NEXT:    ret void
//
void test_subobject_ref(WithNested& s) {
  do_stuff_with_ref(s.n);
  do_stuff_with_ref(s.n.a);
  do_stuff_with_ref(s.f1);
}

void test_subobject_ptr(WithNested& s) {
// No bounds on the pointer here:
// CHECK-LABEL: @_Z18test_subobject_ptrU3capR10WithNested(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[N:%.*]] = getelementptr inbounds [[STRUCT_WITHNESTED:%.*]], [[STRUCT_WITHNESTED]] addrspace(200)* [[S:%.*]], i64 0, i32 1
// CHECK-NEXT:    tail call void @_Z17do_stuff_with_ptrU3capP6Nested(%struct.Nested addrspace(200)* nonnull [[N]])
// CHECK-NEXT:    ret void
  do_stuff_with_ptr(&s.n);
}

// DBG: 3 cheri-bounds     - Number of references checked for tightening bounds
// DBG: 3 cheri-bounds     - Number of references where bounds were tightend
