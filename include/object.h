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

#ifndef OndraGC_OBJECT_H_
#define OndraGC_OBJECT_H_

#include <iosfwd>
#include <ondragc/protoobject.h>

namespace OndraGC {

class Manager;
class Referent;
class MemberPtr;

/**
 * @brief Generic parent of all GC object outside of the library
 */
class Object : public ProtoObject {
  private:
    Referent* referent;   /**< the referent of weak references */
    MemberPtr* ch_begin;  /**< beginning of the list of children */
    MemberPtr* ch_end;    /**< end of the list of children */

    friend class Manager;
    friend class ManagerImpl;
    friend class ManagerHead;
    friend class Referent;
    friend class BasePtr;
    friend class RootPtr;
    friend class MemberPtr;

    /* -- avoid copying */
    Object(
        const Object&);
    Object& operator = (
        const Object&);

    /* -- children */
    void appendChild(
        MemberPtr* child_);
    void removeChild(
        MemberPtr* child_);
    virtual void gcScan(
        Manager* manager_);

    bool checkChildValidity(
        MemberPtr* child_) const;

  public:
    /**
     * @brief Ctor
     */
    Object();

  protected:
    /**
     * @brief Dtor
     */
    virtual ~Object() = 0;

  protected:
    /**
     * @brief Finalize the object
     */
    virtual void gcFinalize();

  public:
    /**
     * @warning This method is designated only for testing
     *          purposes. Never use it!
     */
    virtual std::ostream& test_gcTestMark(
        std::ostream& os_) const;
};

} /* -- namespace OndraGC */

#endif /* OndraGC_OBJECT_H_ */
