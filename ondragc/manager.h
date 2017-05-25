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

#ifndef OndraGC_MANAGER_H_
#define OndraGC_MANAGER_H_

#include <cstddef>
#include <iosfwd>

namespace OndraGC {

struct ManagerImpl;
class Object;
class ProtoObject;

/**
 * @brief The garbage collector
 */
class Manager {
  private:
    ManagerImpl* pimpl;

    /* -- avoid copying */
    Manager(
        const Manager&);
    Manager& operator = (
        const Manager&);

  public:
    /**
     * @brief Ctor
     */
    Manager();

    /**
     * @brief Ctor
     *
     * @param quantum_length_ Percentual length of one GC quantum. Value 100
     *     means exactly work to clean according to count of allocated objects.
     *     Value more than 100 forces the GC to do more work.
     * @param wait_length_ Percentual length of interval between two GC cycles.
     *     Value less than or equal 100 means no interval. Value greater than
     *     defines wait interval based on count of currently allocated
     *     objects.
     * @param history_size_ Number of samples to compute average of allocations
     *     to compute size of a GC quantum.
     */
    Manager(
        unsigned int quantum_length_,
        unsigned int wait_length_,
        unsigned int history_size_);

    /**
     * @brief Dtor
     */
    ~Manager();

    /* -- object initialization */
    void initObject(
        ProtoObject* object_,
        void* block_,
        std::size_t size_,
        std::size_t offset_);

    /* -- coloring of objects */
    void makeWhite(
        ProtoObject* object_);
    void makeGrey(
        ProtoObject* object_);
    void makeBlack(
        ProtoObject* object_);

    /* -- object manipulations */
    void writeBarrier(
        Object* parent_,
        ProtoObject* object_);
    void makeRoot(
        ProtoObject* object_);
    void freeRoot(
        ProtoObject* object_);
    void scavenge(
        ProtoObject* object_);

    /**
     * @brief Do one GC quantum
     */
    void doGCQuantum();

    /**
     * @brief Do full GC cycle
     *
     * This method processes full mark and sweep cycle.
     */
    void doGCCycle();

    /**
     * @brief Force clean
     *
     * This method runs the cycle three times to force cleaning of
     * all unreachable objects.
     */
    void forceClean();

    /**
     * @brief Force start new GC cycle.
     * You have to call doGCQuantum to do the work
     */
    void forceCycle();

    /**
     * @brief Set length of the quantum
     *
     * @param quantum_ The length
     */
    void setQuantumLength(
        unsigned int quantum_);

    /**
     * @brief Set length of waiting for new GC cycle
     *
     * @param length_ The length
     */
    void setWaitLength(
        unsigned int length_);

    /**
     * @brief Test if the object is a zombie
     *
     * @param object_ The tested object
     * @return True when the object is a zombie (an object prepared for
     *         destruction)
     */
    bool isZombie(
        const ProtoObject* object_) const;

    /**
     * @warning This method is designated only for testing
     *          purposes. Never use it!
     */
    std::ostream& test_testMark(
        std::ostream& os_) const;
};

} /* -- namespace OndraGC */

#endif /* OndraGC_MANAGER_H_ */
