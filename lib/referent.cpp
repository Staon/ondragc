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

#include <referent.h>

#include <ostream>

#include <object.h>

namespace OndraGC {

Referent::Referent() :
  ProtoObject(),
  ref_object(0) {
  flags |= WEAK_BIT;
}

Referent::~Referent() {
  if(ref_object)
    ref_object -> referent = 0;
  ref_object = 0;
}

void Referent::gcScan(
    Manager* manager_) {
  /* -- nothing to do, I'm a weak reference */
}

void Referent::gcFinalize() {
  /* -- nothing to do */
}

std::ostream& Referent::test_gcTestMark(
    std::ostream& os_) const {
  os_ << '<';
  if(ref_object != 0)
    ref_object -> test_gcTestMark(os_);
  else
    os_ << "null";
  return os_ << '>';
}

} /* -- namespace OndraGC */
