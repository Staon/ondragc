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

#ifndef OndraGC_TESTOBJECT_H_
#define OndraGC_TESTOBJECT_H_

#include <datstr/dstring.h>
#include <iosfwd>
#include <vector>

#include "../ondragc/object.h"
#include "../ondragc/pointerdecl.h"

namespace OndraGC {

class TestObject;

typedef TypeMemberPtr<TestObject> TestObjectMemberPtr;
typedef TypeRootPtr<TestObject> TestObjectRootPtr;

/**
 * @brief Testing object
 */
class TestObject : public Object {
  private:
    dstring name;
    std::ostream* log_stream;

    typedef std::vector<TestObjectMemberPtr> Children;
    Children children;

    /* -- avoid copying */
    TestObject(
        const TestObject&);
    TestObject& operator = (
        const TestObject&);

  public:
    TestObject(
        const dstring& name_,
        std::ostream* log_);
    virtual ~TestObject();

    /* -- garbage collector */
    virtual void gcFinalize();

    /* -- public access */
    void appendChild(
        TestObject* object_);
    void removeChild(
        TestObject* object_);
    void swapFirstItems(
        TestObject* object_);

    virtual std::ostream& test_gcTestMark(
        std::ostream& os_) const;
};

} /* -- namespace OndraGC */

#endif /* OndraGC_TESTOBJECT_H_ */
