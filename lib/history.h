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

#ifndef OndraGC_HISTORY_H_
#define OndraGC_HISTORY_H_

namespace OndraGC {

/**
 * @brief History of allocations to compute length of one quantum
 */
class History {
  private:
    int* samples;
    int sum;
    unsigned int history_size;
    unsigned int curr;

    /* -- avoid copying */
    History(
        const History&);
    History& operator = (
        const History&);

  public:
    /**
     * @brief Ctor
     *
     * @param size_ Size of the history
     */
    explicit History(
        unsigned int size_);

    /**
     * @brief Dtor
     */
    ~History();

    /**
     * @brief Add new sample into the history
     *
     * @param sample_ The sample
     */
    void addSample(
        int sample_);

    /**
     * @brief Get average value of the samples in the history
     */
    int getAverage() const;
};

} /* -- namespace OndraGC */

#endif /* OndraGC_HISTORY_H_ */
