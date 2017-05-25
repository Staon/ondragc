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

#include "../ondragc/protoobject.h"

#include "../ondragc/gcassert.h"
#include "../ondragc/manager.h"

namespace OndraGC {

ProtoObject::ProtoObject() {
  /* -- don't initialize members, because they are initialized in the
   *    new operator. Clear only the destroyed bit, because it's initialy
   *    set as destroyed (there is memory block, but it's not initialized
   *    yet. */
  flags &= ~static_cast<unsigned int>(DESTROYED_BIT);
}

ProtoObject::~ProtoObject() {
  /* -- Don't remove the object from the GC list. The object is not destroyed
   *    (overloaded delete operator forces it), because it can be referred
   *    by a member pointer. Thus the instance must stay in the memory. */
  flags |= DESTROYED_BIT;
}

Manager* ProtoObject::gcManager() const {
  return manager;
}

bool ProtoObject::gcIsZombie() const {
  OGCASSERT(manager != 0);
  return manager -> isZombie(this);
}

void ProtoObject::insertBefore(
    ProtoObject* pivot_) {
  OGCASSERT(pivot_ != 0 && next == prev);

  prev = pivot_ -> prev;
  next = pivot_;
  pivot_ -> prev -> next = this;
  pivot_ -> prev = this;
}

void ProtoObject::remove() {
  prev -> next = next;
  next -> prev = prev;
  next = prev = this;
}

void* ProtoObject::operator new(
    std::size_t size_,
    Manager* gc_,
    std::size_t offset_) {
  /* -- allocate memory */
  char* block_(reinterpret_cast<char*>(
      ::operator new(size_)));
  ProtoObject* object_(
      reinterpret_cast<ProtoObject*>(
          block_ + offset_));

  /* -- pre-initialize the GC object and the record */
  object_ -> memblock = block_;
  object_ -> manager = gc_;
  object_ -> flags = DESTROYED_BIT;
  object_ -> prev = object_;
  object_ -> next = object_;
  object_ -> root_count = 0;
  gc_ -> initObject(object_, block_, size_, offset_);

  return block_;
}

} /* -- namespace OndraGC */
