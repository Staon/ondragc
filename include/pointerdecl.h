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
template<class Object> class TypeRootPtr : public RootPtr {
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
        Object* object_);

    /**
     * @brief Copy ctor
     */
    TypeRootPtr(
        const TypeRootPtr<Object>& ptr_);

    /**
     * @brief Swap two pointers
     */
    void swap(
        TypeRootPtr<Object>& ptr_);

    /**
     * @brief Assignment
     */
    TypeRootPtr<Object>& operator = (
        const TypeRootPtr<Object>& ptr_);

    /**
     * @brief Assignment
     *
     * @param ptr_ A GC object. The ownership is taken.
     */
    TypeRootPtr<Object> & operator = (
        Object* ptr_);

    /**
     * @brief Get the object
     */
    Object& operator *() const;

    /**
     * @brief Get the object
     */
    Object* operator -> () const;

    /**
     * @brief Get the object
     */
    Object* objectAddr() const;
};

/**
 * @brief Typed member pointer
 */
template<class Object> class TypeMemberPtr : public MemberPtr {
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
        Object* object_);

    /**
     * @brief Copy ctor
     */
    TypeMemberPtr(
        const TypeMemberPtr<Object>& ptr_);

    /**
     * @brief Swap pointers
     */
    void swap(
        TypeMemberPtr<Object>& ptr_);

    /**
     * @brief Assignment
     */
    TypeMemberPtr<Object>& operator = (
        const TypeMemberPtr<Object>& ptr_);

    /**
     * @brief Get the GC object
     */
    Object& operator *() const;

    /**
     * @brief Get the GC object
     */
    Object* operator -> () const;

    /**
     * @brief Get the GC object
     */
    Object* objectAddr() const;
};

} /* -- namespace OndraGC */

#endif /* OndraGC_POINTERDECL_H_ */
