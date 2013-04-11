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

template<class Object>
TypeRootPtr<Object>::TypeRootPtr() {

}

template<class Object>
TypeRootPtr<Object>::TypeRootPtr(
    Object* object_) :
  RootPtr(object_) {

}

template<class Object>
TypeRootPtr<Object>::TypeRootPtr(
    const TypeRootPtr<Object>& ptr_) :
  RootPtr(ptr_) {

}

template<class Object>
void TypeRootPtr<Object>::swap(
    TypeRootPtr<Object>& ptr_) {
  RootPtr::swap(ptr_);
}

template<class Object>
TypeRootPtr<Object>& TypeRootPtr<Object>::operator = (
    const TypeRootPtr<Object>& ptr_) {
  TypeRootPtr<Object> tmp_(ptr_);
  swap(tmp_);
  return *this;
}

template<class Object>
TypeRootPtr<Object> & TypeRootPtr<Object>::operator = (
    Object* ptr_) {
  TypeRootPtr<Object> tmp_(ptr_);
  swap(tmp_);
  return *this;
}

template<class Object>
Object& TypeRootPtr<Object>::operator *() const {
  return *static_cast<Object*>(getObject());
}

template<class Object>
Object* TypeRootPtr<Object>::operator -> () const {
  return static_cast<Object*>(getObject());
}

template<class Object>
Object* TypeRootPtr<Object>::objectAddr() const {
  return static_cast<Object*>(getObject());
}

template<class Object>
TypeMemberPtr<Object>::TypeMemberPtr() {

}

template<class Object>
TypeMemberPtr<Object>::TypeMemberPtr(
    Object* parent_,
    const TypeMemberPtr<Object>::ParentMark& mark_) :
  MemberPtr(parent_, mark_) {

}

template<class Object>
TypeMemberPtr<Object>::TypeMemberPtr(
    Object* parent_,
    Object* object_) :
  MemberPtr(parent_, object_) {

}

template<class Object>
TypeMemberPtr<Object>::TypeMemberPtr(
    const TypeMemberPtr<Object>& ptr_) :
  MemberPtr(ptr_) {

}

template<class Object>
void TypeMemberPtr<Object>::swap(
    TypeMemberPtr<Object>& ptr_) {
  MemberPtr::swap(ptr_);
}

template<class Object>
TypeMemberPtr<Object>& TypeMemberPtr<Object>::operator = (
    const TypeMemberPtr<Object>& ptr_) {
  TypeMemberPtr<Object> tmp_(ptr_);
  swap(tmp_);
  return *this;
}

template<class Object>
Object& TypeMemberPtr<Object>::operator *() const {
  return *static_cast<Object*>(getObject());
}

template<class Object>
Object* TypeMemberPtr<Object>::operator -> () const {
  return static_cast<Object*>(getObject());
}

template<class Object>
Object* TypeMemberPtr<Object>::objectAddr() const {
  return static_cast<Object*>(getObject());
}

} /* -- namespace OndraGC */

#endif /* OndraGC_POINTERIMPL_H_ */
