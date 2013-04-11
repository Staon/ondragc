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

#ifndef OndraGC_OBJECTPTR_H_
#define OndraGC_OBJECTPTR_H_

#include <ondragc/pointerdecl.h>

namespace OndraGC {

class Object;
typedef TypeRootPtr<Object> ObjectRootPtr;
typedef TypeMemberPtr<Object> ObjectMemberPtr;

} /* -- namespace OndraGC */

#endif /* OndraGC_OBJECTPTR_H_ */
