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

#include <pointer.h>

#include <algorithm>

#include <ctor.h>
#include <manager.h>
#include <oassert.h>
#include <object.h>
#include <referent.h>

namespace OndraGC {

/*
==========================================================================
                        Base pointer class
==========================================================================
 */
BasePtr::BasePtr() :
  object(0) {

}

BasePtr::BasePtr(
    Object* object_) :
  object(object_) {

}

BasePtr::BasePtr(
    const BasePtr& ptr_) :
  object(ptr_.object) {

}

BasePtr::~BasePtr() {

}

void BasePtr::swap(
    BasePtr& ptr_) {
  std::swap(object, ptr_.object);
}

BasePtr& BasePtr::operator = (
    const BasePtr& ptr_) {
  BasePtr tmp_(ptr_);
  swap(tmp_);
  return *this;
}

bool BasePtr::makeWeak() {
  OASSERT_1(object != 0);
  if((object -> flags & Object::WEAK_BIT) == 0) {
    Object* o_(static_cast<Object*>(object));
    if(o_ -> referent == 0) {  /* -- the referent doesn't exist yet */
      Referent* referent_(
          new (
              o_ -> manager,
              ::OndraGC::computeGCObjectOffset<Referent>())
          Referent);
      referent_ -> ref_object = o_;
      o_ -> referent = referent_;
    }
    object = o_ -> referent;
    return true;
  }
  else
    return false;
}

bool BasePtr::makeStrong() {
  OASSERT_1(object != 0);
  if((object -> flags & Object::WEAK_BIT) != 0) {
    object = static_cast<Referent*>(object) -> ref_object;
    return true;
  }
  else
    return false;
}

bool BasePtr::isValid() const {
  return object != 0
      && ((object -> flags & Object::WEAK_BIT) == 0
          || static_cast<Referent *>(object) -> ref_object != 0);
}

bool BasePtr::isWeak() const {
  return object != 0 && (object -> flags & Object::WEAK_BIT) != 0;
}

Object * BasePtr::getObject() const {
  if(object && (object -> flags & Object::WEAK_BIT))
    return static_cast<Referent *>(object) -> ref_object;
  else
    return static_cast<Object *>(object);
}

void BasePtr::invalidateWeakReferences() {
  Object* object_(getObject());
  if(object_ && object_ -> referent) {
    object_ -> referent -> ref_object = 0;
    object_ -> referent = 0;
  }
}

/*
 * Implementation of comparison operators.
 */
#define ONDRA_GC_BASEPTR_OPERATOR_( OP__ ) \
    bool operator OP__ ( \
        const BasePtr& ptr1_, \
        const BasePtr& ptr2_) { \
      return ptr1_.object OP__ ptr2_.object; \
    } \
    bool operator OP__ ( \
        const Object* ptr1_, \
        const BasePtr& ptr2_) { \
      return ptr1_ OP__ ptr2_.object; \
    } \
    bool operator OP__ ( \
        const BasePtr& ptr1_, \
        const Object* ptr2_) { \
      return ptr1_.object OP__ ptr2_; \
    }

ONDRA_GC_BASEPTR_OPERATOR_(==)
ONDRA_GC_BASEPTR_OPERATOR_(!=)
ONDRA_GC_BASEPTR_OPERATOR_(<)
ONDRA_GC_BASEPTR_OPERATOR_(<=)
ONDRA_GC_BASEPTR_OPERATOR_(>)
ONDRA_GC_BASEPTR_OPERATOR_(>=)

#undef ONDRA_GC_BASEPTR_OPERATOR_

/*
==========================================================================
                            Root pointer
==========================================================================
 */
RootPtr::RootPtr() :
  BasePtr() {

}

RootPtr::RootPtr(
    Object* object_) :
  BasePtr(object_) {
  if(object != 0)
    object -> manager -> makeRoot(object);
}

RootPtr::RootPtr(
    const RootPtr& ptr_) :
  BasePtr(ptr_) {
  if(object)
    object -> manager -> makeRoot(object);
}

RootPtr::~RootPtr() {
  if(object) {
    object -> manager -> freeRoot(object);
    object = 0;
  }
}

void RootPtr::swap(
    RootPtr& ptr_) {
  BasePtr::swap(ptr_);
}

RootPtr& RootPtr::operator = (
    const RootPtr& ptr_) {
  RootPtr tmp_(ptr_);
  swap(tmp_);
  return *this;
}

RootPtr& RootPtr::operator = (
    Object* ptr_) {
  RootPtr tmp_(ptr_);
  swap(tmp_);
  return *this;
}

void RootPtr::makeWeak() {
  if(object) {
    object -> manager -> freeRoot(object);
    BasePtr::makeWeak();
    object -> manager -> makeRoot(object);
  }
}

void RootPtr::makeStrong() {
  if(object) {
    object -> manager -> freeRoot(object);
    BasePtr::makeStrong();
    object -> manager -> makeRoot(object);
  }
}

void RootPtr::clear() {
  if(object) {
    object -> manager -> freeRoot(object);
    object = 0;
  }
}

/*
==========================================================================
                            Member pointer
==========================================================================
 */
MemberPtr::ParentMark MemberPtr::PARENT;

MemberPtr::MemberPtr() :
  parent(0),
  prev(0),
  next(0) {

}

MemberPtr::MemberPtr(
    Object* parent_,
    const MemberPtr::ParentMark &) :
  parent(0),
  prev(0),
  next(0) {
  OASSERT_1(parent_ != 0);
  parent_ -> appendChild(this);
}

MemberPtr::MemberPtr(
    Object* parent_,
    Object* object_) :
  BasePtr(object_),
  parent(0),
  prev(0),
  next(0) {
  OASSERT_1(parent_ != 0);
  parent_ -> appendChild(this);
  if(object_)
    object_ -> manager -> writeBarrier(parent_, object_);
}

MemberPtr::MemberPtr(
    const MemberPtr& ptr_) :
  BasePtr(ptr_),
  parent(0),
  prev(0),
  next(0) {
  if(ptr_.parent != 0)
    ptr_.parent -> appendChild(this);
  Object* object_(getObject());
  if(object_)
    object_ -> manager -> writeBarrier(parent, object_);
}

MemberPtr::~MemberPtr() {
  if(parent)
    parent -> removeChild(this);
  parent = 0;
  object = 0;
}

void MemberPtr::swap(
    MemberPtr& ptr_) {
  OASSERT_1(parent == ptr_.parent || parent == 0 || ptr_.parent == 0);

  /* -- swap parents */
  if(parent != ptr_.parent) {
    Object* p1_(parent);
    Object* p2_(ptr_.parent);
    if(p1_) {
      p1_ -> removeChild(this);
      p1_ -> appendChild(& ptr_);
    }
    if(p2_) {
      p2_ -> removeChild(& ptr_);
      p2_ -> appendChild(this);
    }
  }

  /* -- swap objects */
  BasePtr::swap(ptr_);

  /* -- note: there is no need of the barrier because I swap objects in
   *    the same parent. */
}

MemberPtr& MemberPtr::operator = (
    const MemberPtr& ptr_) {
  MemberPtr tmp_(ptr_);
  swap(tmp_);
  return *this;
}

void MemberPtr::swapContent(
    MemberPtr& ptr_) {
  /* -- swap objects */
  BasePtr::swap(ptr_);

  /* -- apply write barriers */
  if(object != 0)
    object -> manager -> writeBarrier(parent, object);
  if(ptr_.object != 0)
    ptr_.object -> manager -> writeBarrier(ptr_.parent, ptr_.object);
}

void MemberPtr::makeWeak() {
  if(object && BasePtr::makeWeak())
    object -> manager -> writeBarrier(parent, object);
}

void MemberPtr::makeStrong() {
  if(object && BasePtr::makeStrong())
    object -> manager -> writeBarrier(parent, object);
}

void MemberPtr::clear() {
  if(parent)
    parent -> removeChild(this);
  parent = 0;
  object = 0;
}

} /* -- namespace OndraGC */
