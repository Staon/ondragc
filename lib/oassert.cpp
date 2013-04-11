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

#include <oassert.h>

#include <iostream>

namespace OndraGC {

void oassert(
    bool flag_,
    const char* expression_,
    const char* file_,
    int line_) {
  if(!flag_) {
    std::cerr
        << "assertion '" << expression_ << "' failed (file "
        << file_ << " at line " << line_ << ")." << std::endl;
    *reinterpret_cast<int*>(0) = 0;
  }
}

} /* -- namespace OndraGC */
