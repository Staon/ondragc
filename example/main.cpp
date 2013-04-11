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

#include <iostream>
#include <string>
#include <vector>

#include <ctor.h>
#include <manager.h>
#include <object.h>
#include <pointerdecl.h>
#include <pointerimpl.h>

namespace OndraGC {

class Example;


typedef TypeRootPtr<Example> ExampleRootPtr;
typedef TypeMemberPtr<Example> ExampleMemberPtr;

/**
 * @brief Example GC object
 */
class Example : public Object {
  private:
    std::string name;

    typedef std::vector<ExampleMemberPtr> Children;
    Children children;

    /* -- avoid copying */
    Example(
        const Example&);
    Example& operator = (
        const Example&);

  protected:
    /**
     * @brief Dtor
     */
    virtual ~Example();

  public:
    /**
     * @brief Ctor
     */
    Example(
        const std::string& name_);

    /**
     * @brief Append a child
     */
    void appendChild(
        Example* child_);

    /**
     * @brief Remove all children
     */
    void resetChildren();
};

template class TypeRootPtr<Example>;
template class TypeMemberPtr<Example>;

Example::Example(
    const std::string& name_) :
  name(name_) {
  std::cout << "Constructed " << name << std::endl;
}

Example::~Example() {
  std::cout << "Destructed " << name << std::endl;
}

void Example::appendChild(
    Example* child_) {
  children.push_back(ExampleMemberPtr(this, child_));
}

void Example::resetChildren() {
  children.clear();
}

} /* -- namespace OndraGC */

int main(
    int argc_,
    char *argv_[]) {
  using namespace ::OndraGC;

  Manager gc_;
  ExampleRootPtr root_(newGC(Example, &gc_)("root"));
  root_ -> appendChild(newGC(Example, &gc_)("child1"));
  Example* child2_(newGC(Example, &gc_)("child2"));
  root_ -> appendChild(child2_);
  child2_ -> appendChild(root_.objectAddr());
  child2_ -> appendChild(newGC(Example, &gc_)("child3"));
  ExampleRootPtr root2_(newGC(Example, &gc_)("root2"));
  root2_ -> appendChild(child2_);

  std::cout << "nothing should be cleaned" << std::endl;
  gc_.forceClean();

  std::cout << "nothing again" << std::endl;
  root_.clear();
  gc_.forceClean();

  std::cout << "clean root and child1 and child3" << std::endl;
  child2_ -> resetChildren();
  gc_.forceClean();

  std::cout << "clean root2 and child2" << std::endl;
  root2_.clear();
  gc_.forceClean();

  return 0;
}
