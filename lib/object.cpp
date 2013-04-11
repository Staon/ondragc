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

#include <object.h>

#include <ostream>

#include <manager.h>
#include <oassert.h>
#include <pointer.h>
#include <referent.h>

namespace OndraGC {

Object::Object() :
  ProtoObject(),
  referent(0),
  ch_begin(0),
  ch_end(0) {

}

Object::~Object() {
  /* -- detach the referent object */
  if(referent)
    referent -> ref_object = 0;
  /* -- clean list of children */
  if(ch_begin != 0) {
    MemberPtr* ptr_(ch_begin);
    while(ptr_ != 0) {
      ptr_ -> parent = 0;
      ptr_ = ptr_ -> next;
    }
  }
}

bool Object::checkChildValidity(
    MemberPtr* child_) const {
  if(child_ -> parent != this)
    return false;
  if(child_ -> prev == 0 && child_ != ch_begin)
    return false;
  if(child_ -> next == 0 && child_ != ch_end)
    return false;

  MemberPtr* ptr_(ch_begin);
  while(ptr_ != 0) {
    if(ptr_ == child_)
      return true;
    ptr_ = ptr_ -> next;
  }

  return false;
}

void Object::appendChild(
    MemberPtr* child_) {
  OASSERT_1(child_ != 0 && child_ -> parent == 0);

  child_ -> parent = this;
  if(ch_end == 0) {
    ch_begin = ch_end = child_;
    child_ -> prev = child_ -> next = 0;
  }
  else {
    ch_end -> next = child_;
    child_ -> prev = ch_end;
    child_ -> next = 0;
    ch_end = child_;
  }
}

void Object::removeChild(
    MemberPtr* child_) {
  OASSERT_1(child_ != 0);

  if(child_ -> prev == 0) {
    OASSERT_1(child_ == ch_begin);
    ch_begin = ch_begin -> next;
    if(ch_begin != 0)
      ch_begin -> prev = 0;
  }
  else
    child_ -> prev -> next = child_ -> next;

  if(child_ -> next == 0) {
    OASSERT_1(child_ == ch_end);
    ch_end = ch_end -> prev;
    if(ch_end != 0)
      ch_end -> next = 0;
  }
  else
    child_ -> next -> prev = child_ -> prev;

  child_ -> prev = child_ -> next = 0;
  child_ -> parent = 0;
}

void Object::gcScan(
    Manager* manager_) {
  OASSERT_1(manager_ != 0);

  /* -- children */
  if(ch_begin != 0) {
    MemberPtr* ptr_(ch_begin);
    while(ptr_ != 0) {
      if(ptr_ -> object)
        manager_ -> scavenge(ptr_ -> object);
      ptr_ = ptr_ -> next;
    }
  }
  /* -- referent */
  if(referent != 0)
    manager_ -> scavenge(referent);
}

void Object::gcFinalize() {
  /* -- default empty implementation */
}

std::ostream& Object::test_gcTestMark(
    std::ostream& os_) const {
  return os_ << ((void *) this);
}

} /* -- namespace OndraGC */
