#ifndef ondragcTest_TASGCTYPEPTR_H_
#define ondragcTest_TASGCTYPEPTR_H_

//#define TEST_STORE_REGRESSION
#include <otest/otest.h>

/* -- tested file */
#include <asgctypeptrimpl.h>

#include <ondragc/ctor.h>
#include <ondragc/manager.h>
#include <ondragc/object.h>
#include <testutils/invocationrecorder.h>

namespace OndraGC {

class TestIface0 {
  private:
    TestIface0(
        TestIface0 const &);
    TestIface0 & operator=(
        TestIface0 const &);

  public:
    TestIface0();
    virtual ~TestIface0() = 0;

    virtual Object* asGCObject() = 0;
    virtual void func0() = 0;
};

TestIface0::TestIface0() {
}

TestIface0::~TestIface0() {
}

class TestIface1 {
  private:
    TestIface1(
        TestIface1 const &);
    TestIface1 & operator=(
        TestIface1 const &);

  public:
    TestIface1();
    virtual ~TestIface1() = 0;

    virtual Object* asGCObject() = 0;
    virtual void func1() = 0;
};

TestIface1::TestIface1() {
}

TestIface1::~TestIface1() {
}

class TestImpl : public Object, public TestIface0, public TestIface1 {
  private:
    typedef Object Super;
    ::TestUtils::InvocationRecorder* invocation_recorder;

    TestImpl(
        TestImpl const &);
    TestImpl & operator=(
        TestImpl const &);

  public:
    TestImpl(
        ::TestUtils::InvocationRecorder* invocation_recorder_);
    virtual ~TestImpl();

    virtual Object* asGCObject();
    virtual void func0();
    virtual void func1();

    void setInvocationRecorder(
        ::TestUtils::InvocationRecorder* const invocation_recorder_);
};

TestImpl::TestImpl(
    ::TestUtils::InvocationRecorder* invocation_recorder_) :
    Super(),
    TestIface0(),
    TestIface1(),
    invocation_recorder(invocation_recorder_) {
  if(invocation_recorder != 0) {
    (*invocation_recorder) << "TestImpl::TestImpl():" << ::TestUtils::add;
  }
}

TestImpl::~TestImpl() {
  if(invocation_recorder != 0) {
    (*invocation_recorder) << "TestImpl::~TestImpl():" << ::TestUtils::add;
  }
}

Object* TestImpl::asGCObject() {
  return this;
}

void TestImpl::func0() {
  if(invocation_recorder != 0) {
    (*invocation_recorder) << "TestImpl::func0():" << ::TestUtils::add;
  }
}

void TestImpl::func1() {
  if(invocation_recorder != 0) {
    (*invocation_recorder) << "TestImpl::func1():" << ::TestUtils::add;
  }
}

void TestImpl::setInvocationRecorder(
    ::TestUtils::InvocationRecorder* const invocation_recorder_) {
  invocation_recorder = invocation_recorder_;
}

class TestDirect : public Object {
  private:
    typedef Object Super;
    ::TestUtils::InvocationRecorder* invocation_recorder;

    TestDirect(
        TestDirect const &);
    TestDirect & operator=(
        TestDirect const &);

  public:
    TestDirect(
        ::TestUtils::InvocationRecorder* invocation_recorder_);
    virtual ~TestDirect();

    virtual void func();

    void setInvocationRecorder(
        ::TestUtils::InvocationRecorder* const invocation_recorder_);
};

TestDirect::TestDirect(
    ::TestUtils::InvocationRecorder* invocation_recorder_) :
    Super(),
    invocation_recorder(invocation_recorder_) {
  if(invocation_recorder != 0) {
    (*invocation_recorder) << "TestDirect::TestDirect():" << ::TestUtils::add;
  }
}

TestDirect::~TestDirect() {
  if(invocation_recorder != 0) {
    (*invocation_recorder) << "TestDirect::~TestDirect():" << ::TestUtils::add;
  }
}

void TestDirect::func() {
  if(invocation_recorder != 0) {
    (*invocation_recorder) << "TestDirect::func():" << ::TestUtils::add;
  }
}

void TestDirect::setInvocationRecorder(
    ::TestUtils::InvocationRecorder* const invocation_recorder_) {
  invocation_recorder = invocation_recorder_;
}

template< typename Object_ >
class AsGCObjectTrait {
  private:
    static ::TestUtils::InvocationRecorder* invocation_recorder;

  public:
    static Object* asGCObject(
        Object_* const object_);
    static void setInvocationRecorder(
        ::TestUtils::InvocationRecorder* const invocation_recorder_);
};

template< typename Object_ >
::TestUtils::InvocationRecorder * AsGCObjectTrait< Object_ >::invocation_recorder(0);

template< typename Object_ >
Object* AsGCObjectTrait< Object_ >::asGCObject(
    Object_* const object_) {
  if(invocation_recorder != 0) {
    (*invocation_recorder) << "AsGCObjectTrait::asGCObject("
        << (object_ != 0 ? "non-null" : "null") << "):" << ::TestUtils::add;
  }
  return (object_ != 0 ? object_ -> asGCObject() : 0);
}

template< typename Object_ >
void AsGCObjectTrait< Object_ >::setInvocationRecorder(
    ::TestUtils::InvocationRecorder* const invocation_recorder_) {
  invocation_recorder = invocation_recorder_;
}

template< typename Object_ >
class AsGCObjectTraitDirect {
  private:
    static ::TestUtils::InvocationRecorder* invocation_recorder;

  public:
    static Object* asGCObject(
        Object_* const object_);
    static void setInvocationRecorder(
        ::TestUtils::InvocationRecorder* const invocation_recorder_);
};

template< typename Object_ >
::TestUtils::InvocationRecorder * AsGCObjectTraitDirect< Object_ >::invocation_recorder(0);

template< typename Object_ >
Object* AsGCObjectTraitDirect< Object_ >::asGCObject(
    Object_* const object_) {
  if(invocation_recorder != 0) {
    (*invocation_recorder) << "AsGCObjectTraitDirect::asGCObject("
        << (object_ != 0 ? "non-null" : "null") << "):" << ::TestUtils::add;
  }
  return object_;
}

template< typename Object_ >
void AsGCObjectTraitDirect< Object_ >::setInvocationRecorder(
    ::TestUtils::InvocationRecorder* const invocation_recorder_) {
  invocation_recorder = invocation_recorder_;
}

TEST_SUITE(AsGCTypePtr) {
  TEST_CASE(Trait) {
    TEST_INIT_STATE() {
      Manager gc_;

      /* -- impl and direct trait */
      typedef AsGCObjectTrait< TestImpl > TestImplAsGCObjectTrait;
      typedef AsGCObjectTraitDirect< TestDirect > TestDirectAsGCObjectTrait;

      /* -- impl root and member ptr with trait */
      typedef AsGCTypeRootPtr< TestImpl, TestImplAsGCObjectTrait > TestImplRootPtrWithTrait;
      typedef AsGCTypeMemberPtr< TestImpl, TestImplAsGCObjectTrait > TestImplMemberPtrWithTrait;

      /* -- direct root and member ptr with trait */
      typedef AsGCTypeRootPtr< TestDirect, TestDirectAsGCObjectTrait > TestDirectRootPtrWithTrait;
      typedef AsGCTypeMemberPtr< TestDirect, TestDirectAsGCObjectTrait > TestDirectMemberPtrWithTrait;

      ::TestUtils::InvocationRecorder impl_recorder_, direct_recorder_, parent_recorder_;
      TestImplAsGCObjectTrait::setInvocationRecorder(&impl_recorder_);
      TestDirectAsGCObjectTrait::setInvocationRecorder(&direct_recorder_);

      /* -- impl null object */
      TEST_ASSERT(TestImplAsGCObjectTrait::asGCObject(0) == 0);
      TEST_REGRESS("impl_trait_null_object", impl_recorder_);
      impl_recorder_.reset();

      /* -- direct null object */
      TEST_ASSERT(TestDirectAsGCObjectTrait::asGCObject(0) == 0);
      TEST_REGRESS("direct_trait_null_object", direct_recorder_);
      direct_recorder_.reset();

      /* -- impl default root ptr */
      TestImplRootPtrWithTrait impl_root_ptr0_;
      TEST_REGRESS("impl_default_root_ptr", impl_recorder_);
      impl_recorder_.reset();

      /* -- impl default member ptr */
      TestImplMemberPtrWithTrait impl_member_ptr0_;
      TEST_REGRESS("impl_default_member_ptr", impl_recorder_);
      impl_recorder_.reset();

      /* -- direct default root ptr */
      TestDirectRootPtrWithTrait direct_root_ptr0_;
      TEST_REGRESS("direct_default_root_ptr", direct_recorder_);
      direct_recorder_.reset();

      /* -- direct default member ptr */
      TestDirectMemberPtrWithTrait direct_member_ptr0_;
      TEST_REGRESS("direct_default_member_ptr", direct_recorder_);
      direct_recorder_.reset();

      /* -- prepare parent object */
      TestDirect* const parent_(newGC(TestDirect, &gc_)(&parent_recorder_));
      TEST_ASSERT(parent_ != 0);
      parent_recorder_.reset();

      /* -- impl null object root ptr */
      TestImplRootPtrWithTrait impl_root_ptr1_(0);
      TEST_REGRESS("impl_null_object_root_ptr", impl_recorder_);
      impl_recorder_.reset();

      /* -- impl null object member ptr */
      TestImplMemberPtrWithTrait impl_member_ptr1_(parent_, 0);
      TEST_REGRESS("parent_null_object_member_ptr", parent_recorder_);
      TEST_REGRESS("impl_null_object_member_ptr", impl_recorder_);
      parent_recorder_.reset();
      impl_recorder_.reset();

      /* -- direct null object root ptr */
      TestDirectRootPtrWithTrait direct_root_ptr1_(0);
      TEST_REGRESS("direct_null_object_root_ptr", direct_recorder_);
      direct_recorder_.reset();

      /* -- direct null object member ptr */
      TestDirectMemberPtrWithTrait direct_member_ptr1_(parent_, 0);
      TEST_REGRESS("parent_null_object_member_ptr", parent_recorder_);
      TEST_REGRESS("direct_null_object_member_ptr", direct_recorder_);
      parent_recorder_.reset();
      direct_recorder_.reset();

      /* -- prepare impl object */
      TestImpl* const impl_(newGC(TestImpl, &gc_)(&impl_recorder_));
      TEST_ASSERT(impl_ != 0);
      impl_recorder_.reset();

      /* -- impl object root ptr */
      TestImplRootPtrWithTrait impl_root_ptr2_(impl_);
      TEST_REGRESS("impl_object_root_ptr", impl_recorder_);
      impl_recorder_.reset();

      /* -- impl object member ptr */
      TestImplMemberPtrWithTrait impl_member_ptr2_(parent_, impl_);
      TEST_REGRESS("parent_object_member_ptr", parent_recorder_);
      TEST_REGRESS("impl_object_member_ptr", impl_recorder_);
      parent_recorder_.reset();
      impl_recorder_.reset();

      /* -- prepare direct object */
      TestDirect* const direct_(newGC(TestDirect, &gc_)(&direct_recorder_));
      TEST_ASSERT(direct_ != 0);
      direct_recorder_.reset();

      /* -- direct object root ptr */
      TestDirectRootPtrWithTrait direct_root_ptr3_(direct_);
      TEST_REGRESS("direct_object_root_ptr", direct_recorder_);
      direct_recorder_.reset();

      /* -- direct object member ptr */
      TestDirectMemberPtrWithTrait direct_member_ptr3_(parent_, direct_);
      TEST_REGRESS("parent_object_member_ptr", parent_recorder_);
      TEST_REGRESS("direct_object_member_ptr", direct_recorder_);
      parent_recorder_.reset();
      direct_recorder_.reset();

      /* -- disconnect invocation recorders */
      direct_ -> setInvocationRecorder(0);
      impl_ -> setInvocationRecorder(0);
      parent_ -> setInvocationRecorder(0);
      TestImplAsGCObjectTrait::setInvocationRecorder(0);
      TestDirectAsGCObjectTrait::setInvocationRecorder(0);
    }
  };

  TEST_CASE(Basic) {
    TEST_INIT_STATE() {
      Manager gc_;

      typedef AsGCTypeRootPtr< TestIface0 > TestIface0RootPtr;
      typedef AsGCTypeMemberPtr< TestIface0 > TestIface0MemberPtr;
      typedef AsGCTypeRootPtr< TestImpl > TestImplRootPtr;
      typedef AsGCTypeMemberPtr< TestImpl > TestImplMemberPtr;
      typedef AsGCTypeRootPtr< TestDirect > TestDirectRootPtr;
      typedef AsGCTypeMemberPtr< TestDirect > TestDirectMemberPtr;

      /* -- as gc type root ptr */
      {
        ::TestUtils::InvocationRecorder invocation_recorder0_, invocation_recorder1_;

        TestImpl* const ptr0_(newGC(TestImpl, &gc_)(&invocation_recorder0_));
        TestImpl* const ptr1_(newGC(TestImpl, &gc_)(&invocation_recorder1_));
        TEST_ASSERT(ptr0_ != 0);
        TEST_ASSERT(ptr1_ != 0);

        // default ctor
        TestIface0RootPtr root_ptr0_;
        TEST_ASSERT(!root_ptr0_.isValid());

        // assign to self (null)
        root_ptr0_ = root_ptr0_;
        TEST_ASSERT(!root_ptr0_.isValid());

        // assign ptr
        root_ptr0_ = TestIface0RootPtr(ptr0_);
        TEST_ASSERT(root_ptr0_.isValid());

        // assign null ptr
        root_ptr0_ = TestIface0RootPtr();
        TEST_ASSERT(!root_ptr0_.isValid());

        // assign object
        root_ptr0_ = ptr0_;
        TEST_ASSERT(root_ptr0_.isValid());

        // assign null object
        root_ptr0_ = 0;
        TEST_ASSERT(!root_ptr0_.isValid());

        // assign object
        root_ptr0_ = ptr0_;
        TEST_ASSERT(root_ptr0_.isValid());

        // assign to self (not null)
        root_ptr0_ = root_ptr0_;
        TEST_ASSERT(root_ptr0_.isValid());

        // object ctor
        TestIface0RootPtr root_ptr1_(ptr1_);
        TEST_ASSERT(root_ptr1_.isValid());

        // copy ctor
        TestIface0RootPtr root_ptr2_(root_ptr0_);
        TEST_ASSERT(root_ptr0_.isValid());
        TEST_ASSERT(root_ptr2_.isValid());

        // assign null object
        root_ptr0_ = 0;
        TEST_ASSERT(!root_ptr0_.isValid());
        TEST_ASSERT(root_ptr2_.isValid());

        // swap with self (null)
        root_ptr0_.swap(root_ptr0_);
        TEST_ASSERT(!root_ptr0_.isValid());

        // swap
        root_ptr0_.swap(root_ptr2_);
        TEST_ASSERT(root_ptr0_.isValid());
        TEST_ASSERT(!root_ptr2_.isValid());

        // swap with self (not null)
        root_ptr0_.swap(root_ptr0_);
        TEST_ASSERT(root_ptr0_.isValid());

        TEST_REGRESS("ptr0#0", invocation_recorder0_);
        TEST_REGRESS("ptr1#0", invocation_recorder1_);
        invocation_recorder0_.reset();
        invocation_recorder1_.reset();

        // dereference *
        TEST_ASSERT((*root_ptr0_).asGCObject() != 0);
        (*root_ptr0_).func0();

        // dereference ->
        TEST_ASSERT(root_ptr0_ -> asGCObject() != 0);
        root_ptr0_ -> func0();

        // object address
        TEST_ASSERT(root_ptr0_.objectAddr() != 0);
        TEST_ASSERT(root_ptr0_.objectAddr() -> asGCObject() != 0);
        root_ptr0_.objectAddr() -> func0();

        TEST_ASSERT(root_ptr2_.objectAddr() == 0);

        TEST_REGRESS("ptr0#1", invocation_recorder0_);
        TEST_REGRESS("ptr1#1", invocation_recorder1_);
        invocation_recorder0_.reset();
        invocation_recorder1_.reset();

        /* -- disconnect invocation recorders */
        ptr0_ -> setInvocationRecorder(0);
        ptr1_ -> setInvocationRecorder(0);
      }

      /* -- as gc type member ptr */
      // TODO
    }
  };

  TEST_CASE(Variations) {
    TEST_INIT_STATE() {
      Manager gc_;

      typedef AsGCTypeRootPtr< TestIface0 > TestIface0RootPtr;
      typedef AsGCTypeMemberPtr< TestIface0 > TestIface0MemberPtr;
      typedef AsGCTypeRootPtr< TestImpl > TestImplRootPtr;
      typedef AsGCTypeMemberPtr< TestImpl > TestImplMemberPtr;
      typedef AsGCTypeRootPtr< TestDirect > TestDirectRootPtr;
      typedef AsGCTypeMemberPtr< TestDirect > TestDirectMemberPtr;

      {
        ::TestUtils::InvocationRecorder invocation_recorder_;
        TestImpl* const ptr_(newGC(TestImpl, &gc_)(&invocation_recorder_));
        TestIface0RootPtr const root_ptr_(ptr_);
        root_ptr_ -> func0();
        TEST_REGRESS("root_iface0_impl", invocation_recorder_);
        ptr_ -> setInvocationRecorder(0);
      }

      {
        ::TestUtils::InvocationRecorder invocation_recorder_;
        TestDirect* const root_(newGC(TestDirect, &gc_)(&invocation_recorder_));
        TestImpl* const ptr_(newGC(TestImpl, &gc_)(&invocation_recorder_));
        TestIface0MemberPtr const member_ptr_(root_, ptr_);
        member_ptr_ -> func0();
        TEST_REGRESS("member_iface0_impl", invocation_recorder_);
        root_ -> setInvocationRecorder(0);
        ptr_ -> setInvocationRecorder(0);
      }

      {
        ::TestUtils::InvocationRecorder invocation_recorder_;
        TestImpl* const ptr_(newGC(TestImpl, &gc_)(&invocation_recorder_));
        TestImplRootPtr const root_ptr_(ptr_);
        root_ptr_ -> func0();
        root_ptr_ -> func1();
        TEST_REGRESS("root_impl_impl", invocation_recorder_);
        root_ptr_ -> setInvocationRecorder(0);
      }

      {
        ::TestUtils::InvocationRecorder invocation_recorder_;
        TestDirect* const root_(newGC(TestDirect, &gc_)(&invocation_recorder_));
        TestImpl* const ptr_(newGC(TestImpl, &gc_)(&invocation_recorder_));
        TestImplMemberPtr const member_ptr_(root_, ptr_);
        member_ptr_ -> func0();
        member_ptr_ -> func1();
        TEST_REGRESS("member_impl_impl", invocation_recorder_);
        root_ -> setInvocationRecorder(0);
        member_ptr_ -> setInvocationRecorder(0);
      }

      {
        ::TestUtils::InvocationRecorder invocation_recorder_;
        TestDirect* const ptr_(newGC(TestDirect, &gc_)(&invocation_recorder_));
        TestDirectRootPtr const root_ptr_(ptr_);
        root_ptr_ -> func();
        TEST_REGRESS("root_direct_direct", invocation_recorder_);
        root_ptr_ -> setInvocationRecorder(0);
      }

      {
        ::TestUtils::InvocationRecorder invocation_recorder_;
        TestDirect* const root_(newGC(TestDirect, &gc_)(&invocation_recorder_));
        TestDirect* const ptr_(newGC(TestDirect, &gc_)(&invocation_recorder_));
        TestDirectMemberPtr const member_ptr_(root_, ptr_);
        member_ptr_ -> func();
        TEST_REGRESS("member_direct_direct", invocation_recorder_);
        root_ -> setInvocationRecorder(0);
        member_ptr_ -> setInvocationRecorder(0);
      }
    }
  };

  TEST_CASE(TAsGCObjectTrait) {
    TEST_INIT_STATE() {
      Manager gc_;

      typedef AsGCTypeRootPtr< TestImpl, AsGCObjectTrait< TestImpl > > TestImplRootPtrWithTrait;
      typedef AsGCTypeMemberPtr< TestImpl, AsGCObjectTrait< TestImpl > > TestImplMemberPtrWithTrait;
      typedef AsGCTypeRootPtr< TestDirect, AsGCObjectTraitDirect< TestDirect > > TestDirectRootPtrWithTrait;
      typedef AsGCTypeMemberPtr< TestDirect, AsGCObjectTraitDirect< TestDirect > > TestDirectMemberPtrWithTrait;

      {
        ::TestUtils::InvocationRecorder invocation_recorder_;
        AsGCObjectTrait< TestImpl >::setInvocationRecorder(&invocation_recorder_);
        TestImpl* const ptr_(newGC(TestImpl, &gc_)(&invocation_recorder_));
        TestImplRootPtrWithTrait const root_ptr_(ptr_);
        root_ptr_ -> func0();
        root_ptr_ -> func1();
        TEST_REGRESS("root_impl_impl_trait", invocation_recorder_);
        AsGCObjectTrait< TestImpl >::setInvocationRecorder(0);
        root_ptr_ -> setInvocationRecorder(0);
      }

      {
        ::TestUtils::InvocationRecorder invocation_recorder_;
        AsGCObjectTrait< TestImpl >::setInvocationRecorder(&invocation_recorder_);
        TestDirect* const root_(newGC(TestDirect, &gc_)(&invocation_recorder_));
        TestImpl* const ptr_(newGC(TestImpl, &gc_)(&invocation_recorder_));
        TestImplMemberPtrWithTrait const member_ptr_(root_, ptr_);
        member_ptr_ -> func0();
        member_ptr_ -> func1();
        TEST_REGRESS("member_impl_impl_trait", invocation_recorder_);
        AsGCObjectTrait< TestImpl >::setInvocationRecorder(0);
        root_ -> setInvocationRecorder(0);
        member_ptr_ -> setInvocationRecorder(0);
      }

      {
        ::TestUtils::InvocationRecorder invocation_recorder_;
        AsGCObjectTraitDirect< TestDirect >::setInvocationRecorder(&invocation_recorder_);
        TestDirect* const ptr_(newGC(TestDirect, &gc_)(&invocation_recorder_));
        TestDirectRootPtrWithTrait const root_ptr_(ptr_);
        root_ptr_ -> func();
        TEST_REGRESS("root_direct_direct_trait", invocation_recorder_);
        AsGCObjectTraitDirect< TestDirect >::setInvocationRecorder(0);
        root_ptr_ -> setInvocationRecorder(0);
      }

      {
        ::TestUtils::InvocationRecorder invocation_recorder_;
        AsGCObjectTraitDirect< TestDirect >::setInvocationRecorder(&invocation_recorder_);
        TestDirect* const root_(newGC(TestDirect, &gc_)(&invocation_recorder_));
        TestDirect* const ptr_(newGC(TestDirect, &gc_)(&invocation_recorder_));
        TestDirectMemberPtrWithTrait const member_ptr_(root_, ptr_);
        member_ptr_ -> func();
        TEST_REGRESS("member_direct_direct_trait", invocation_recorder_);
        AsGCObjectTraitDirect< TestDirect >::setInvocationRecorder(0);
        ptr_ -> setInvocationRecorder(0);
        root_ -> setInvocationRecorder(0);
        member_ptr_ -> setInvocationRecorder(0);
      }

      /* -- check if ptrs are still valid after gc clean */
      {
        ::TestUtils::InvocationRecorder invocation_recorder_;
        AsGCObjectTraitDirect< TestDirect >::setInvocationRecorder(&invocation_recorder_);
        TestDirect* const root_(newGC(TestDirect, &gc_)(&invocation_recorder_));
        TestDirect* const ptr_(newGC(TestDirect, &gc_)(&invocation_recorder_));
        TestDirectRootPtrWithTrait const root_ptr_(root_);
        TestDirectMemberPtrWithTrait const member_ptr_(root_, ptr_);

        /* -- clean gc */
        gc_.forceClean();
        gc_.forceClean();
        gc_.forceClean();

        /* -- check ptrs  */
        TEST_ASSERT(root_ptr_.isValid() && member_ptr_.isValid());
        member_ptr_ -> func();
        TEST_REGRESS("member_direct_direct_trait_after_gc_clean", invocation_recorder_);
        AsGCObjectTraitDirect< TestDirect >::setInvocationRecorder(0);
        root_ptr_ -> setInvocationRecorder(0);
        ptr_ -> setInvocationRecorder(0);
        root_ -> setInvocationRecorder(0);
        member_ptr_ -> setInvocationRecorder(0);
      }
    }
  };
};

} /* -- namespace OndraGC */

#endif /* ondragcTest_TASGCTYPEPTR_H_ */
