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

#ifndef OndraGCASSERT_H_
#define OndraGCASSERT_H_

namespace OndraGC {

/**
 * @brief Assertion
 *
 * This function terminates the process when the @a flag_ is false. Before
 * exiting a diagnostic message is written.
 *
 * @param flag_ Checked assertion flag
 * @param expression_ String message, usually the assertion expression.
 * @param file_ Name of the source file
 * @param line_ Line number
 */
void oassert(
    bool flag_,
    const char* expression_,
    const char* file_,
    int line_);

} /* -- namespace OndraGC */

#define OASSERT_1(expression_) \
  ::OndraGC::oassert(expression_, #expression_, __FILE__, __LINE__)

#define OASSERT_NEVER() \
  ::OndraGC::oassert(false, "Oops, this code should not be reached.", __FILE__, __LINE__)

#endif /* OndraGCASSERT_H_ */
