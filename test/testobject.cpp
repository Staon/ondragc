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

#include "testobject.h"

#include <algorith>
#include <gcassert.h>
#include <ostream>

#include <pointerimpl.h>

namespace OndraGC {

template class TypeMemberPtr<TestObject>;
template class TypeRootPtr<TestObject>;

TestObject::TestObject(
    const dstring& name_,
    std::ostream* log_) :
  name(name_),
  log_stream(log_) {

  (*log_stream) << "Object '" << name << "' is constructed." << std::endl;
}

TestObject::~TestObject() {
  (*log_stream) << "Object '" << name << "' is destructed." << std::endl;
}

void TestObject::gcFinalize() {
  (*log_stream) << "Object '" << name << "' is finalized." << std::endl;
}

void TestObject::appendChild(
    TestObject* object_) {
  children.push_back(TestObjectMemberPtr(this, object_));
}

void TestObject::removeChild(
    TestObject* object_) {
  Children::iterator iter_(
      std::remove(children.begin(), children.end(), object_));
  children.erase(iter_, children.end());
}

void TestObject::swapFirstItems(
    TestObject* object_) {
  OGCASSERT(!children.empty() && object_ != 0 && !object_ -> children.empty());
  children.front().swapContent(object_ -> children.front());
}

std::ostream& TestObject::test_gcTestMark(
    std::ostream& os_) const {
  return os_ << name;
}

} /* -- namespace OndraGC */
