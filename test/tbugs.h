/*
 * Copyright (C) 2013 Aveco s.r.o.
 *
 * This file is part of OndraGC.
 *
 * OndraGC is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * OndraGC is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OndraGC.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OndraGC_TBUGS_H_
#define OndraGC_TBUGS_H_

//#define TEST_STORE_REGRESSION
#include <otest/otest.h>

#include "../ondragc/ctor.h"
#include "../ondragc/manager.h"
#include "../ondragc/object.h"
#include "../ondragc/pointerdecl.h"
#include "../ondragc/pointerimpl.h"

namespace OndraGC {

namespace {

class Ba {
  private:
    int dummy;

  public:
    Ba(int c_);
    virtual ~Ba();

    virtual void func1();
    virtual void func2();
    virtual void func3();
    virtual void func4();
    virtual void func5();
    virtual void func6();
    virtual void func7();
    virtual void func8();
    virtual void func9();
};

class TotalyStrangeClass : public Object {
  private:
    int dummy;

  public:
    TotalyStrangeClass();
};

class B : public TotalyStrangeClass, public Ba {
  public:
    B();
};

class A : public Object {
  private:
    TypeMemberPtr<B> b;

  public:
    A(B* b_);
    virtual std::ostream& test_gcTestMark(
        std::ostream& os_) const;
};

Ba::Ba(int c_) :
  dummy(c_) {

}

Ba::~Ba() {

}

void Ba::func1() {

}

void Ba::func2() {

}

void Ba::func3() {

}

void Ba::func4() {

}

void Ba::func5() {

}

void Ba::func6() {

}

void Ba::func7() {

}

void Ba::func8() {

}

void Ba::func9() {

}

TotalyStrangeClass::TotalyStrangeClass() :
  Object(),
  dummy(0) {

}

A::A(B* b_) :
  Object(),
  b(this, b_) {

}

std::ostream& A::test_gcTestMark(
    std::ostream& os_) const {
  return os_ << "A";
}

B::B() :
  Ba(5),
  TotalyStrangeClass() {
  TypeRootPtr<A> a_(newGC(A, gcManager())(this));
  throw "B";
}

} /* -- namespace */


TEST_SUITE(TBugs) {
  TEST_CASE(Bug3561) {
    /* -- test of the bug #3561 */
    TEST_INIT_STATE() {
      Manager gc_;

      try {
        newGC(B, &gc_);
      }
      catch(const char*) {

      }

      TEST_REGRESS("a dead object", gc_);

      gc_.forceClean();  /* -- it caused the crash */
      TEST_REGRESS("cleaned", gc_);
    }
  };
};

} /* -- namespace OndraGC */

#endif /* OndraGC_TBUGS_H_ */
