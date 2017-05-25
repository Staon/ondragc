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

#ifndef OndraGC_TCOLLECTOR_H_
#define OndraGC_TCOLLECTOR_H_

//#define TEST_STORE_REGRESSION
#include <otest/otest.h>

#include <md5/hashstream.h>
#include <datstr/dstring.h>
#include <ostream>

#include <ctor.h>
#include <manager.h>
#include "testobject.h"

namespace OndraGC {

TEST_SUITE(GarbageCollector) {
  TEST_CASE(Collecting) {
    TEST_INIT_STATE() {
      ::MD5::HashStream hos_;   /* -- must be before the manager */
      Manager gc_manager_;
      std::ostream* reporter_(&hos_);

      TestObjectRootPtr root_(newGC(TestObject, &gc_manager_)(
          dstring("Root"), reporter_));
      TestObject* child1_(newGC(TestObject, &gc_manager_)("Child 1", reporter_));
      TestObject* child2_(newGC(TestObject, &gc_manager_)("Child 2", reporter_));
      root_ -> appendChild(child1_);
      root_ -> appendChild(child2_);
      TestObject* child3_(newGC(TestObject, &gc_manager_)("Child 3", reporter_));
      TestObject* child4_(newGC(TestObject, &gc_manager_)("Child 4", reporter_));
      child1_ -> appendChild(child3_);
      child1_ -> appendChild(child4_);
      child1_ -> appendChild(child2_);
      child3_ -> appendChild(child1_);
      TestObject* child5_(newGC(TestObject, &gc_manager_)("Child 5", reporter_));
      TestObject* child6_(newGC(TestObject, &gc_manager_)("Child 6", reporter_));
      TestObject* child7_(newGC(TestObject, &gc_manager_)("Child 7", reporter_));
      child3_ -> appendChild(child5_);
      child5_ -> appendChild(child6_);
      child5_ -> appendChild(child7_);
      TestObjectRootPtr root2(child5_);

      gc_manager_.doGCCycle();
      TEST_REGRESS("first cycle", gc_manager_);

      child1_ -> removeChild(child4_);
      gc_manager_.doGCCycle();
      TEST_REGRESS("second cycle", gc_manager_);

      root_ = 0;
      gc_manager_.doGCCycle();
      TEST_REGRESS("third cycle", gc_manager_);

      root2 = 0;
      gc_manager_.doGCCycle();
      TEST_REGRESS("fourth cycle", gc_manager_);

      TEST_REGRESS_D("reporter hash", hos_.getHash());
    }
  };

  TEST_CASE(BigTest) {
    ::MD5::HashStream hos;   /* -- must be before the manager */
    Manager gc_manager;
    std::ostream* reporter;

    BigTest() :
      hos(),
      gc_manager(),
      reporter(& hos) {

    }

    TestObject* makeTree(int number_) {
      dstrostream dos_;

      /* -- make root */
      dos_ << "Object " << number_;
      TestObject* root_(newGC(TestObject, &gc_manager)(dos_.str(), reporter));

      /* -- make children */
      for(int i_(0); i_ < 10; ++i_) {
        dos_ << "Object " << number_ << "/" << i_;
        root_ -> appendChild(newGC(TestObject, &gc_manager)(dos_.str(), reporter));
      }

      return root_;
    }

    TEST_INIT_STATE() {
      dstrostream dos_;
      for(int j_(0); j_ < 100; ++j_) {
        dos_ << "Root " << j_;
        TestObjectRootPtr root_(
            newGC(TestObject, &gc_manager)(dos_.str(), reporter));
        for(int i_(0); i_ < 10; i_ += 2) {
          root_ -> appendChild(makeTree(i_));
          root_ -> appendChild(makeTree(i_ + 1));
          gc_manager.doGCQuantum();
        }
        dos_ << "Cycle " << j_;
        TEST_REGRESS(dos_.str(), gc_manager);
      }
      gc_manager.doGCCycle();
      gc_manager.doGCCycle();
      TEST_REGRESS("cleaned", gc_manager);
    }
  };

  TEST_CASE(WeakReferencesRoot) {
    TEST_INIT_STATE() {
      ::MD5::HashStream hos_;   /* -- must be before the manager */
      Manager gc_manager_;
      std::ostream* reporter_(&hos_);

      TestObjectRootPtr root1_(
          newGC(TestObject, &gc_manager_)("Root", reporter_));
      TestObjectRootPtr root2_(root1_);
      TestObjectRootPtr root3_(root1_);
      gc_manager_.doGCCycle();
      TEST_REGRESS("all strong", gc_manager_);
      TEST_ASSERT(root1_.isValid());
      TEST_ASSERT(!root1_.isWeak());
      TEST_ASSERT(root2_.isValid());
      TEST_ASSERT(!root2_.isWeak());
      TEST_ASSERT(root3_.isValid());
      TEST_ASSERT(!root3_.isWeak());

      root2_.makeWeak();
      gc_manager_.doGCCycle();
      TEST_REGRESS("one weak", gc_manager_);
      TEST_ASSERT(root1_.isValid());
      TEST_ASSERT(!root1_.isWeak());
      TEST_ASSERT(root2_.isValid());
      TEST_ASSERT(root2_.isWeak());
      TEST_ASSERT(root3_.isValid());
      TEST_ASSERT(!root3_.isWeak());

      /* -- make weak once more again (nothing should happen) */
      root2_.makeWeak();
      gc_manager_.doGCCycle();
      TEST_REGRESS("one weak again", gc_manager_);
      TEST_ASSERT(root1_.isValid());
      TEST_ASSERT(!root1_.isWeak());
      TEST_ASSERT(root2_.isValid());
      TEST_ASSERT(root2_.isWeak());
      TEST_ASSERT(root3_.isValid());
      TEST_ASSERT(!root3_.isWeak());

      /* -- make all weak */
      root1_.makeWeak();
      root3_.makeWeak();
      gc_manager_.doGCCycle();
      TEST_REGRESS("all weak", gc_manager_);
      TEST_ASSERT(!root1_.isValid());
      TEST_ASSERT(root1_.isWeak());
      TEST_ASSERT(!root2_.isValid());
      TEST_ASSERT(root2_.isWeak());
      TEST_ASSERT(!root3_.isValid());
      TEST_ASSERT(root3_.isWeak());

      /* -- make weak on invalid referent (nothing should happen) */
      root1_.makeWeak();
      root3_.makeWeak();
      gc_manager_.doGCCycle();
      TEST_REGRESS("all weak again", gc_manager_);
      TEST_ASSERT(!root1_.isValid());
      TEST_ASSERT(root1_.isWeak());
      TEST_ASSERT(!root2_.isValid());
      TEST_ASSERT(root2_.isWeak());
      TEST_ASSERT(!root3_.isValid());
      TEST_ASSERT(root3_.isWeak());

      /* -- destroy the referent */
      root1_ = 0; root2_ = 0; root3_ = 0;
      gc_manager_.doGCCycle();
      TEST_REGRESS("destroyed referent", gc_manager_);
      TEST_ASSERT(!root1_.isValid());
      TEST_ASSERT(!root1_.isWeak());
      TEST_ASSERT(!root2_.isValid());
      TEST_ASSERT(!root2_.isWeak());
      TEST_ASSERT(!root3_.isValid());
      TEST_ASSERT(!root3_.isWeak());

      /* -- make weak on invalid reference (nothing should happen) */
      root1_.makeWeak();
      gc_manager_.doGCCycle();
      TEST_REGRESS("destroyed referent again", gc_manager_);
      TEST_ASSERT(!root1_.isValid());
      TEST_ASSERT(!root1_.isWeak());
      TEST_ASSERT(!root2_.isValid());
      TEST_ASSERT(!root2_.isWeak());
      TEST_ASSERT(!root3_.isValid());
      TEST_ASSERT(!root3_.isWeak());

      /* -- make strong on invalid reference (nothing should happen) */
      root1_.makeStrong();
      gc_manager_.doGCCycle();
      TEST_REGRESS("strong on invalid", gc_manager_);
      TEST_ASSERT(!root1_.isValid());
      TEST_ASSERT(!root1_.isWeak());
      TEST_ASSERT(!root2_.isValid());
      TEST_ASSERT(!root2_.isWeak());
      TEST_ASSERT(!root3_.isValid());
      TEST_ASSERT(!root3_.isWeak());

      /* -- make strong */
      root1_ = newGC(TestObject, &gc_manager_)("Root", reporter_);
      root2_ = root1_;
      root3_ = root1_;
      root1_.makeWeak();
      root2_.makeWeak();
      root3_.makeWeak();
      root2_.makeStrong();
      gc_manager_.doGCCycle();
      TEST_REGRESS("make strong", gc_manager_);
      TEST_ASSERT(root1_.isValid());
      TEST_ASSERT(root1_.isWeak());
      TEST_ASSERT(root2_.isValid());
      TEST_ASSERT(!root2_.isWeak());
      TEST_ASSERT(root3_.isValid());
      TEST_ASSERT(root3_.isWeak());

      /* -- make strong on strong (nothing should happen) */
      root2_.makeStrong();
      gc_manager_.doGCCycle();
      TEST_REGRESS("make strong again", gc_manager_);
      TEST_ASSERT(root1_.isValid());
      TEST_ASSERT(root1_.isWeak());
      TEST_ASSERT(root2_.isValid());
      TEST_ASSERT(!root2_.isWeak());
      TEST_ASSERT(root3_.isValid());
      TEST_ASSERT(root3_.isWeak());

      root2_ = 0;
      gc_manager_.doGCCycle();
      TEST_REGRESS("removed strong", gc_manager_);
      TEST_ASSERT(!root1_.isValid());
      TEST_ASSERT(root1_.isWeak());
      TEST_ASSERT(!root2_.isValid());
      TEST_ASSERT(!root2_.isWeak());
      TEST_ASSERT(!root3_.isValid());
      TEST_ASSERT(root3_.isWeak());

      TEST_REGRESS_D("reporter hash", hos_.getHash());
    }
  };

  TEST_CASE(WeakReferencesMember) {
    TEST_INIT_STATE() {
      ::MD5::HashStream hos_;   /* -- must be before the manager */
      Manager gc_manager_;
      std::ostream* reporter_(&hos_);

      TestObjectRootPtr root_(
          newGC(TestObject, &gc_manager_)("Root", reporter_));

      TestObjectMemberPtr mem1_(TestObjectMemberPtr(
          root_.objectAddr(),
          newGC(TestObject, &gc_manager_)("Child 1", reporter_)));
      TestObjectMemberPtr mem2_(mem1_);
      TestObjectMemberPtr mem3_(mem1_);
      gc_manager_.doGCCycle();
      TEST_REGRESS("all strong", gc_manager_);
      TEST_ASSERT(mem1_.isValid());
      TEST_ASSERT(!mem1_.isWeak());
      TEST_ASSERT(mem2_.isValid());
      TEST_ASSERT(!mem2_.isWeak());
      TEST_ASSERT(mem3_.isValid());
      TEST_ASSERT(!mem3_.isWeak());

      mem2_.makeWeak();
      gc_manager_.doGCCycle();
      TEST_REGRESS("one weak", gc_manager_);
      TEST_ASSERT(mem1_.isValid());
      TEST_ASSERT(!mem1_.isWeak());
      TEST_ASSERT(mem2_.isValid());
      TEST_ASSERT(mem2_.isWeak());
      TEST_ASSERT(mem3_.isValid());
      TEST_ASSERT(!mem3_.isWeak());

      /* -- make weak once more again (nothing should happen) */
      mem2_.makeWeak();
      gc_manager_.doGCCycle();
      TEST_REGRESS("one weak again", gc_manager_);
      TEST_ASSERT(mem1_.isValid());
      TEST_ASSERT(!mem1_.isWeak());
      TEST_ASSERT(mem2_.isValid());
      TEST_ASSERT(mem2_.isWeak());
      TEST_ASSERT(mem3_.isValid());
      TEST_ASSERT(!mem3_.isWeak());

      mem1_.makeWeak();
      mem3_.makeWeak();
      gc_manager_.doGCCycle();
      TEST_REGRESS("all weak", gc_manager_);
      TEST_ASSERT(!mem1_.isValid());
      TEST_ASSERT(mem1_.isWeak());
      TEST_ASSERT(!mem2_.isValid());
      TEST_ASSERT(mem2_.isWeak());
      TEST_ASSERT(!mem3_.isValid());
      TEST_ASSERT(mem3_.isWeak());

      /* -- make weak on invalid referent (nothing should happen) */
      mem1_.makeWeak();
      mem3_.makeWeak();
      gc_manager_.doGCCycle();
      TEST_REGRESS("all weak again", gc_manager_);
      TEST_ASSERT(!mem1_.isValid());
      TEST_ASSERT(mem1_.isWeak());
      TEST_ASSERT(!mem2_.isValid());
      TEST_ASSERT(mem2_.isWeak());
      TEST_ASSERT(!mem3_.isValid());
      TEST_ASSERT(mem3_.isWeak());

      /* -- destroy the referent */
      mem1_ = TestObjectMemberPtr(root_.objectAddr(), 0);
      mem2_ = TestObjectMemberPtr(root_.objectAddr(), 0);
      mem3_ = TestObjectMemberPtr(root_.objectAddr(), 0);
      gc_manager_.doGCCycle();
      TEST_REGRESS("destroyed referent", gc_manager_);
      TEST_ASSERT(!mem1_.isValid());
      TEST_ASSERT(!mem1_.isWeak());
      TEST_ASSERT(!mem2_.isValid());
      TEST_ASSERT(!mem2_.isWeak());
      TEST_ASSERT(!mem3_.isValid());
      TEST_ASSERT(!mem3_.isWeak());

      /* -- make weak on invalid reference (nothing should happen) */
      mem1_.makeWeak();
      gc_manager_.doGCCycle();
      TEST_REGRESS("destroyed referent again", gc_manager_);
      TEST_ASSERT(!mem1_.isValid());
      TEST_ASSERT(!mem1_.isWeak());
      TEST_ASSERT(!mem2_.isValid());
      TEST_ASSERT(!mem2_.isWeak());
      TEST_ASSERT(!mem3_.isValid());
      TEST_ASSERT(!mem3_.isWeak());

      /* -- make strong on invalid reference (nothing should happen) */
      mem1_.makeStrong();
      gc_manager_.doGCCycle();
      TEST_REGRESS("strong on invalid", gc_manager_);
      TEST_ASSERT(!mem1_.isValid());
      TEST_ASSERT(!mem1_.isWeak());
      TEST_ASSERT(!mem2_.isValid());
      TEST_ASSERT(!mem2_.isWeak());
      TEST_ASSERT(!mem3_.isValid());
      TEST_ASSERT(!mem3_.isWeak());

      mem1_ = TestObjectMemberPtr(
          root_.objectAddr(),
          newGC(TestObject, &gc_manager_)("Child 2", reporter_));
      mem2_ = mem1_;
      mem3_ = mem1_;
      mem1_.makeWeak();
      mem2_.makeWeak();
      mem3_.makeWeak();
      mem2_.makeStrong();
      gc_manager_.doGCCycle();
      TEST_REGRESS("make strong", gc_manager_);
      TEST_ASSERT(mem1_.isValid());
      TEST_ASSERT(mem1_.isWeak());
      TEST_ASSERT(mem2_.isValid());
      TEST_ASSERT(!mem2_.isWeak());
      TEST_ASSERT(mem3_.isValid());
      TEST_ASSERT(mem3_.isWeak());

      /* -- make strong on strong (nothing should happen) */
      mem2_.makeStrong();
      gc_manager_.doGCCycle();
      TEST_REGRESS("make strong again", gc_manager_);
      TEST_ASSERT(mem1_.isValid());
      TEST_ASSERT(mem1_.isWeak());
      TEST_ASSERT(mem2_.isValid());
      TEST_ASSERT(!mem2_.isWeak());
      TEST_ASSERT(mem3_.isValid());
      TEST_ASSERT(mem3_.isWeak());

      mem2_ = TestObjectMemberPtr(root_.objectAddr(), 0);
      gc_manager_.doGCCycle();
      TEST_REGRESS("removed strong", gc_manager_);
      TEST_ASSERT(!mem1_.isValid());
      TEST_ASSERT(mem1_.isWeak());
      TEST_ASSERT(!mem2_.isValid());
      TEST_ASSERT(!mem2_.isWeak());
      TEST_ASSERT(!mem3_.isValid());
      TEST_ASSERT(mem3_.isWeak());

      TEST_REGRESS_D("reporter hash", hos_.getHash());
    }
  };

  TEST_CASE(InvalidationOfWeakReferences) {
    TEST_INIT_STATE() {
      ::MD5::HashStream hos_;   /* -- must be before the manager */
      Manager gc_manager_;
      std::ostream* reporter_(&hos_);

      TestObjectRootPtr root_(
          newGC(TestObject, &gc_manager_)("Root", reporter_));
      TestObjectMemberPtr mem1_(TestObjectMemberPtr(
          root_.objectAddr(),
          newGC(TestObject, &gc_manager_)("Child 1", reporter_)));
      TestObjectMemberPtr mem2_(mem1_);
      TestObjectMemberPtr mem3_(mem1_);
      gc_manager_.doGCCycle();
      TEST_REGRESS("all strong", gc_manager_);
      TEST_ASSERT(root_.isValid());
      TEST_ASSERT(!root_.isWeak());
      TEST_ASSERT(mem1_.isValid());
      TEST_ASSERT(!mem1_.isWeak());
      TEST_ASSERT(mem2_.isValid());
      TEST_ASSERT(!mem2_.isWeak());
      TEST_ASSERT(mem3_.isValid());
      TEST_ASSERT(!mem3_.isWeak());

      /* -- invalidate on weak */
      root_.makeWeak();
      mem1_.makeWeak();
      mem2_.makeWeak();
      mem3_.makeWeak();
      TEST_REGRESS("all weak", gc_manager_);
      TEST_ASSERT(root_.isValid());
      TEST_ASSERT(root_.isWeak());
      TEST_ASSERT(mem1_.isValid());
      TEST_ASSERT(mem1_.isWeak());
      TEST_ASSERT(mem2_.isValid());
      TEST_ASSERT(mem2_.isWeak());
      TEST_ASSERT(mem3_.isValid());
      TEST_ASSERT(mem3_.isWeak());

      mem1_.invalidateWeakReferences();
      TEST_REGRESS("invalidate the child", gc_manager_);
      TEST_ASSERT(root_.isValid());
      TEST_ASSERT(root_.isWeak());
      TEST_ASSERT(!mem1_.isValid());
      TEST_ASSERT(mem1_.isWeak());
      TEST_ASSERT(!mem2_.isValid());
      TEST_ASSERT(mem2_.isWeak());
      TEST_ASSERT(!mem3_.isValid());
      TEST_ASSERT(mem3_.isWeak());

      /* -- invalidate once more (nothing should happen) */
      mem1_.invalidateWeakReferences();
      TEST_REGRESS("invalidate the child again", gc_manager_);
      TEST_ASSERT(root_.isValid());
      TEST_ASSERT(root_.isWeak());
      TEST_ASSERT(!mem1_.isValid());
      TEST_ASSERT(mem1_.isWeak());
      TEST_ASSERT(!mem2_.isValid());
      TEST_ASSERT(mem2_.isWeak());
      TEST_ASSERT(!mem3_.isValid());
      TEST_ASSERT(mem3_.isWeak());

      gc_manager_.doGCCycle();
      TEST_REGRESS("clear", gc_manager_);

      /* -- invalidate on strong reference */
      root_ = newGC(TestObject, &gc_manager_)("Root 2", reporter_);
      TestObjectMemberPtr mem4_(TestObjectMemberPtr(
          root_.objectAddr(),
          newGC(TestObject, &gc_manager_)("Child 2", reporter_)));
      TestObjectMemberPtr mem5_(mem4_);
      TestObjectMemberPtr mem6_(mem4_);

      root_.makeWeak();
      mem5_.makeWeak();
      mem6_.makeWeak();
      TEST_REGRESS("all weak 2", gc_manager_);
      TEST_ASSERT(root_.isValid());
      TEST_ASSERT(root_.isWeak());
      TEST_ASSERT(mem4_.isValid());
      TEST_ASSERT(!mem4_.isWeak());
      TEST_ASSERT(mem5_.isValid());
      TEST_ASSERT(mem5_.isWeak());
      TEST_ASSERT(mem6_.isValid());
      TEST_ASSERT(mem6_.isWeak());

      mem4_.invalidateWeakReferences();
      TEST_REGRESS("invalidate the child 2", gc_manager_);
      TEST_ASSERT(root_.isValid());
      TEST_ASSERT(root_.isWeak());
      TEST_ASSERT(mem4_.isValid());
      TEST_ASSERT(!mem4_.isWeak());
      TEST_ASSERT(!mem5_.isValid());
      TEST_ASSERT(mem5_.isWeak());
      TEST_ASSERT(!mem6_.isValid());
      TEST_ASSERT(mem6_.isWeak());

      /* -- invalidate once more (nothing should happen) */
      mem4_.invalidateWeakReferences();
      TEST_REGRESS("invalidate the child 2 again", gc_manager_);
      TEST_ASSERT(root_.isValid());
      TEST_ASSERT(root_.isWeak());
      TEST_ASSERT(mem4_.isValid());
      TEST_ASSERT(!mem4_.isWeak());
      TEST_ASSERT(!mem5_.isValid());
      TEST_ASSERT(mem5_.isWeak());
      TEST_ASSERT(!mem6_.isValid());
      TEST_ASSERT(mem6_.isWeak());

      gc_manager_.doGCCycle();
      TEST_REGRESS("clear 2", gc_manager_);

      /* -- invalidate on invalid reference (nothing should happen) */
      TestObjectRootPtr root2_;
      root2_.invalidateWeakReferences();
      gc_manager_.doGCCycle();
      TEST_REGRESS("invalidate on invalid", gc_manager_);

      TEST_REGRESS_D("reporter hash", hos_.getHash());
    }
  };

  TEST_CASE(SwapOfContent) {
    TEST_INIT_STATE() {
      ::MD5::HashStream hos_;   /* -- must be before the manager */
      Manager gc_manager_;
      std::ostream* reporter_(&hos_);

      TestObjectRootPtr root1_(
          newGC(TestObject, &gc_manager_)("Root 1", reporter_));
      root1_ -> appendChild(
          newGC(TestObject, &gc_manager_)("Child 1", reporter_));
      TestObjectRootPtr root2_(
          newGC(TestObject, &gc_manager_)("Root 2", reporter_));
      root2_ -> appendChild(
          newGC(TestObject, &gc_manager_)("Child 2", reporter_));
      TEST_REGRESS("prepared", gc_manager_);

      root1_ -> swapFirstItems(root2_.objectAddr());
      TEST_REGRESS("swapped", gc_manager_);

      root1_.clear();
      gc_manager_.forceClean();
      TEST_REGRESS("cleaned", gc_manager_);
    }
  };
};

} /* -- namespace OndraGC */

#endif /* OndraGC_TCOLLECTOR_H_ */
