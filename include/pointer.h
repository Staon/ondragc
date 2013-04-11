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

#ifndef OndraGC_POINTER_H_
#define OndraGC_POINTER_H_

namespace OndraGC {

class Object;
class ProtoObject;

/**
 * @brief Base pointer class
 */
class BasePtr {
  protected:
    ProtoObject* object;

  protected:
    /**
     * @brief Default ctor - invalid pointer
     */
    BasePtr();

    /**
     * @brief Ctor
     *
     * @param object_ An object. The ownership is taken.
     */
    explicit BasePtr(
        Object* object_);

    /**
     * @brief Copy ctor
     */
    BasePtr(
        const BasePtr& ptr_);

    /**
     * @brief Dtor
     */
    ~BasePtr();

    /**
     * @brief Swap contents
     */
    void swap(
        BasePtr& ptr_);

    /**
     * @brief Assignment
     */
    BasePtr& operator = (
        const BasePtr& ptr_);

    /**
     * @brief Make this reference weak (if it's not yet)
     */
    bool makeWeak();

    /**
     * @brief Make this reference strong (if it's not yet)
     */
    bool makeStrong();

  public:
    /**
     * @brief Check validity of the pointer
     *
     * The pointer is valid when points directly an object or if it points
     * a valid weak reference.
     */
    bool isValid() const;

    /**
     * @brief Check if the pointer is a weak reference
     */
    bool isWeak() const;

    /**
     * @brief Get pointer object
     *
     * @return The object or null pointer when the object is not valid.
     */
    Object* getObject() const;

    /**
     * @brief Invalidate all weak reference which points this object
     *     (including myself)
     */
    void invalidateWeakReferences();

    friend bool operator == (
        const BasePtr& ptr1_,
        const BasePtr& ptr2_);
    friend bool operator == (
        const Object* ptr1_,
        const BasePtr& ptr2_);
    friend bool operator == (
        const BasePtr& ptr1_,
        const Object* ptr2_);
    friend bool operator != (
        const BasePtr& ptr1_,
        const BasePtr& ptr2_);
    friend bool operator != (
        const Object* ptr1_,
        const BasePtr& ptr2_);
    friend bool operator != (
        const BasePtr& ptr1_,
        const Object* ptr2_);
};

bool operator == (
    const BasePtr& ptr1_,
    const BasePtr& ptr2_);
bool operator == (
    const Object* ptr1_,
    const BasePtr& ptr2_);
bool operator == (
    const BasePtr& ptr1_,
    const Object* ptr2_);
bool operator != (
    const BasePtr& ptr1_,
    const BasePtr& ptr2_);
bool operator != (
    const Object* ptr1_,
    const BasePtr& ptr2_);
bool operator != (
    const BasePtr& ptr1_,
    const Object* ptr2_);

/**
 * @brief Root pointer
 *
 * This smart pointer keeps an object as a root object
 */
class RootPtr : public BasePtr {
  protected:
    /**
     * @brief Default ctor - invalid pointer
     */
    RootPtr();

    /**
     * @brief Ctor
     *
     * @param object_ An object. The ownership is taken.
     */
    explicit RootPtr(
        Object* object_);

    /**
     * @brief Copy ctor
     */
    RootPtr(
        const RootPtr& ptr_);

    /**
     * @brief Dtor
     */
    ~RootPtr();

    /**
     * @brief Swap contents
     */
    void swap(
        RootPtr& ptr_);

    /**
     * @brief Assignment
     */
    RootPtr& operator = (
        const RootPtr& ptr_);

    /**
     * @brief Assignment
     */
    RootPtr& operator = (
        Object* ptr_);

  public:
    /**
     * @brief Make the pointer weak (if it's not yet)
     */
    void makeWeak();

    /**
     * @brief Make the pointer strong (if it's not yet)
     */
    void makeStrong();

    /**
     * @brief Clear (invalidate) the pointer
     */
    void clear();
};

/**
 * @brief Member pointer
 *
 * This smart pointer keeps a member of another object
 */
class MemberPtr : public BasePtr {
  private:
    Object* parent;
    MemberPtr* prev;
    MemberPtr* next;
    friend class Object;

    struct ParentMark { };

  public:
    static ParentMark PARENT;

  protected:
    /**
     * @brief Ctor - invalid pointer
     */
    MemberPtr();

    /**
     * @brief Ctor
     *
     * Initialize the pointer as a member pointer of an parent, but with
     * a null child pointer
     *
     * @param parent_ The parent object
     * @param mark_ Dummy mark to select correct constructor. Use
     *     the OndraGCMemberPtr::PARENT
     */
    MemberPtr(
        Object* parent_,
        const ParentMark& mark_);

    /**
     * @brief Ctor
     *
     * @param parent_ Parent object
     * @param object_ The object. The ownership is taken.
     */
    MemberPtr(
        Object* parent_,
        Object* object_);

    /**
     * @brief Copy ctor
     */
    MemberPtr(
        const MemberPtr& ptr_);

    /**
     * @brief Dtor
     */
    ~MemberPtr();

    /**
     * @brief Swap contents
     */
    void swap(
        MemberPtr& ptr_);

    /**
     * @brief Assignment
     */
    MemberPtr& operator = (
        const MemberPtr& ptr_);

  public:
    /**
     * @brief Make the pointer weak (if it's not yet)
     */
    void makeWeak();

    /**
     * @brief Make the pointer strong (if it's not yet)
     */
    void makeStrong();

    /**
     * @brief Clear (invalidate) the pointer
     */
    void clear();
};

} /* -- namespace OndraGC */

#endif /* OndraGC_POINTER_H_ */
