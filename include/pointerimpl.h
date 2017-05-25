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

#ifndef OndraGC_POINTERIMPL_H_
#define OndraGC_POINTERIMPL_H_

#include <ondragc/pointer.h>

namespace OndraGC {

template<class Object_>
TypeRootPtr<Object_>::TypeRootPtr() {

}

template<class Object_>
TypeRootPtr<Object_>::TypeRootPtr(
    Object_* object_) :
  RootPtr(object_) {

}

template<class Object_>
TypeRootPtr<Object_>::TypeRootPtr(
    const TypeRootPtr<Object_>& ptr_) :
  RootPtr(ptr_) {

}

template<class Object_>
void TypeRootPtr<Object_>::swap(
    TypeRootPtr<Object_>& ptr_) {
  RootPtr::swap(ptr_);
}

template<class Object_>
TypeRootPtr<Object_>& TypeRootPtr<Object_>::operator = (
    const TypeRootPtr<Object_>& ptr_) {
  TypeRootPtr<Object_> tmp_(ptr_);
  swap(tmp_);
  return *this;
}

template<class Object_>
TypeRootPtr<Object_> & TypeRootPtr<Object_>::operator = (
    Object_* ptr_) {
  TypeRootPtr<Object_> tmp_(ptr_);
  swap(tmp_);
  return *this;
}

template<class Object_>
Object_& TypeRootPtr<Object_>::operator *() const {
  return *static_cast<Object_*>(getObject());
}

template<class Object_>
Object_* TypeRootPtr<Object_>::operator -> () const {
  return static_cast<Object_*>(getObject());
}

template<class Object_>
Object_* TypeRootPtr<Object_>::objectAddr() const {
  return static_cast<Object_*>(getObject());
}

template<class Object_>
TypeMemberPtr<Object_>::TypeMemberPtr() {

}

template<class Object_>
TypeMemberPtr<Object_>::TypeMemberPtr(
    Object* parent_,
    const TypeMemberPtr<Object_>::ParentMark& mark_) :
  MemberPtr(parent_, mark_) {

}

template<class Object_>
TypeMemberPtr<Object_>::TypeMemberPtr(
    Object* parent_,
    Object_* object_) :
  MemberPtr(parent_, object_) {

}

template<class Object_>
TypeMemberPtr<Object_>::TypeMemberPtr(
    const TypeMemberPtr<Object_>& ptr_) :
  MemberPtr(ptr_) {

}

template<class Object_>
void TypeMemberPtr<Object_>::swap(
    TypeMemberPtr<Object_>& ptr_) {
  MemberPtr::swap(ptr_);
}

template<class Object_>
TypeMemberPtr<Object_>& TypeMemberPtr<Object_>::operator = (
    const TypeMemberPtr<Object_>& ptr_) {
  TypeMemberPtr<Object_> tmp_(ptr_);
  swap(tmp_);
  return *this;
}

template<class Object_>
Object_& TypeMemberPtr<Object_>::operator *() const {
  return *static_cast<Object_*>(getObject());
}

template<class Object_>
Object_* TypeMemberPtr<Object_>::operator -> () const {
  return static_cast<Object_*>(getObject());
}

template<class Object_>
Object_* TypeMemberPtr<Object_>::objectAddr() const {
  return static_cast<Object_*>(getObject());
}

} /* -- namespace OndraGC */

#endif /* OndraGC_POINTERIMPL_H_ */
