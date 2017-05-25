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

#include "history.h"

#include "../ondragc/gcassert.h"

namespace OndraGC {

History::History(
    unsigned int size_) :
  sum(0),
  history_size(size_),
  curr(0) {
  OGCASSERT(size_ > 0);

  samples = new int[size_];
  for(int i = 0; i < history_size; ++ i)
    samples[i] = 0;
}

History::~History() {
  delete [] samples; samples = 0;
}

void History::addSample(
    int sample_) {
  /* -- subtract the sample which is removed from the history */
  sum -= samples[curr];
  /* -- add new sample */
  samples[curr] = sample_;
  sum += sample_;
  /* -- move pointer in the circular buffer of the history */
  ++curr;
  if(curr >= history_size)
    curr = 0;
}

int History::getAverage() const {
  return sum / history_size;
}

} /* -- namespace OndraGC */
