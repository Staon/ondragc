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

#ifndef OndraGC_PROTOOBJECT_H_
#define OndraGC_PROTOOBJECT_H_

#include <cstddef>
#include <iosfwd>

namespace OndraGC {

class Manager;

/**
 * @brief Proto-object
 *
 * The proto-object keeps data needed by the garbage collector: a pointer
 * to the collector, color flags of the object, pointers to be linked
 * in the list of objects and the root counter (number of root references).
 *
 * The proto-object fiddles with dynamic allocation too. The object cannot
 * be de-allocated during destruction (see the bug #3561), because it can
 * be referenced by a member or a root pointer. Thus the delete operator
 * does nothing. The destructor method only sets a flag that the object
 * is already destroyed. The collector checks the flag and when it's set,
 * the destructor method is not called and only memory is cleaned.
 */
class ProtoObject {
  private:
    Manager* manager;   /*!< GC manager which owns this object */
    unsigned int flags;        /*!< object flags */
    ProtoObject* prev;  /*!< previous object in a list */
    ProtoObject* next;  /*!< next object in a list */
    unsigned int root_count;   /*!< counter of the root references */
    void* memblock;            /**< pointer to the beginning of the memory block */

    enum {
        COLOR_MASK = (1 << 2) - 1,
        WEAK_BIT = 1 << 2,
        DESTROYED_BIT = 1 << 3,
    };

    friend class Manager;
    friend class ManagerImpl;
    friend class ManagerHead;
    friend class Referent;
    friend class BasePtr;
    friend class RootPtr;
    friend class MemberPtr;

    /* -- avoid copying */
    ProtoObject(
        const ProtoObject&);
    ProtoObject& operator = (
        const ProtoObject&);

    /* -- lists of objects */
    void insertBefore(
        ProtoObject* pivot_);
    void remove();

    virtual void gcScan(
        Manager* manager_) = 0;

    /* -- disallow array allocation */
    void* operator new[](
        std::size_t size_);

  public:
    /**
     * @brief Ctor
     */
    ProtoObject();

  protected:
    /**
     * @brief Dtor
     */
    virtual ~ProtoObject();

  private:
    /**
     * @brief Finalize the object
     */
    virtual void gcFinalize() = 0;

  public:
    /**
     * @brief Get the garbage collector which keeps lifetime of this object
     */
    Manager* gcManager() const;

    /**
     * @brief Return true, when the object is a zombie
     *
     * An object is a zombie, when it's detected by the GC as unreachable and
     * it's appended into the list for finalization or for destruction.
     */
    bool gcIsZombie() const;

    /**
     * @brief Overloaded new operator
     *
     * @param size_ Size of the allocated block
     * @param gc_ Garbage collector
     * @param offset_ Byte offset of the GC object in the full class
     */
    void* operator new(
        std::size_t size_,
        Manager* gc_,
        std::size_t offset_);

    /* -- note: the delete operator is not defined, because Watcom compiler
     *    cannot pair new operator with arguments with the delete operator
     *    with the same arguments. No one delete operator is invoked in
     *    this case. */

    /* -- test interface - never use it */
    virtual std::ostream& test_gcTestMark(
        std::ostream& os_) const = 0;
};

} /* -- namespace OndraGC */

#endif /* OndraGC_PROTOOBJECT_H_ */
