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

#ifndef OndraGC_POINTERDECL_H_
#define OndraGC_POINTERDECL_H_

#include <ondragc/pointer.h>

namespace OndraGC {

/**
 * @brief Typed root pointer
 */
template<class Object_> class TypeRootPtr : public RootPtr {
  public:
    /**
     * @brief Ctor - null (invalid) pointer
     */
    TypeRootPtr();

    /**
     * @brief Ctor
     *
     * @param object_ Kept object. The ownership is taken.
     * @note The implicit conversion is expected
     */
    TypeRootPtr(
        Object_* object_);

    /**
     * @brief Copy ctor
     */
    TypeRootPtr(
        const TypeRootPtr<Object_>& ptr_);

    /**
     * @brief Swap two pointers
     */
    void swap(
        TypeRootPtr<Object_>& ptr_);

    /**
     * @brief Assignment
     */
    TypeRootPtr<Object_>& operator = (
        const TypeRootPtr<Object_>& ptr_);

    /**
     * @brief Assignment
     *
     * @param ptr_ A GC object. The ownership is taken.
     */
    TypeRootPtr<Object_> & operator = (
        Object_* ptr_);

    /**
     * @brief Get the object
     */
    Object_& operator *() const;

    /**
     * @brief Get the object
     */
    Object_* operator -> () const;

    /**
     * @brief Get the object
     */
    Object_* objectAddr() const;
};

/**
 * @brief Typed member pointer
 */
template<class Object_> class TypeMemberPtr : public MemberPtr {
  public:
    /**
     * @brief Ctor - null (invalid) pointer
     */
    TypeMemberPtr();

    /**
     * @brief Ctor - invalid pointer connected with a parent
     *
     * @param parent_ Parent object
     */
    TypeMemberPtr(
        Object* parent_,
        const ParentMark&);

    /**
     * @brief Ctor
     *
     * @param parent_ Parent object
     * @param object_ The kept GC object
     */
    TypeMemberPtr(
        Object* parent_,
        Object_* object_);

    /**
     * @brief Copy ctor
     */
    TypeMemberPtr(
        const TypeMemberPtr<Object_>& ptr_);

    /**
     * @brief Swap pointers
     */
    void swap(
        TypeMemberPtr<Object_>& ptr_);

    /**
     * @brief Assignment
     */
    TypeMemberPtr<Object_>& operator = (
        const TypeMemberPtr<Object_>& ptr_);

    /**
     * @brief Swap content of the pointers
     *
     * The method swaps objects which are pointed by the pointers (i.e. the
     * first member pointer now points the second object). This method can
     * be used to swap data between objects.
     */
    void swapContent(
        TypeMemberPtr<Object_>& ptr_);

    /**
     * @brief Get the GC object
     */
    Object_& operator *() const;

    /**
     * @brief Get the GC object
     */
    Object_* operator -> () const;

    /**
     * @brief Get the GC object
     */
    Object_* objectAddr() const;
};

} /* -- namespace OndraGC */

#endif /* OndraGC_POINTERDECL_H_ */
