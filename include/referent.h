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

#ifndef OndraGC_REFERENT_H_
#define OndraGC_REFERENT_H_

#include <iosfwd>
#include <ondragc/protoobject.h>

namespace OndraGC {

class BasePtr;
class ManagerImpl;
class Object;

/**
 * @brief Referent of weak references
 */
class Referent : public ProtoObject {
  private:
    Object* ref_object;

    friend class BasePtr;
    friend class ManagerImpl;
    friend class Object;

    /* -- avoid copying */
    Referent(
        const Referent&);
    Referent& operator = (
        const Referent&);

    virtual void gcScan(
        Manager* manager_);
    virtual void gcFinalize();
    virtual std::ostream& test_gcTestMark(
        std::ostream& os_) const;

  public:
    /**
     * @brief Ctor
     */
    Referent();

    /**
     * @brief Dtor
     */
    virtual ~Referent();
};

} /* -- namespace OndraGC */

#endif /* OndraGC_REFERENT_H_ */
