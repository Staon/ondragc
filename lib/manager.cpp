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

#include <manager.h>

#include <ostream>

#include <gcassert.h>
#include <object.h>
#include <referent.h>
#include "history.h"

namespace OndraGC {

#define IS_WHITE(object) \
  ((object -> flags & ProtoObject::COLOR_MASK) == pimpl -> white_color)
#define IS_GREY(object) \
  ((object -> flags & ProtoObject::COLOR_MASK) == pimpl -> grey_color)
#define IS_BLACK(object) \
  ((object -> flags & ProtoObject::COLOR_MASK) == pimpl -> black_color)
#define IS_ECRU(object) \
  ((object -> flags & ProtoObject::COLOR_MASK) == pimpl -> ecru_color)

#define MAKE_WHITE(object) \
  object -> flags &= ~ ProtoObject::COLOR_MASK; \
  object -> flags |= pimpl -> white_color
#define MAKE_GREY(object) \
  object -> flags &= ~ ProtoObject::COLOR_MASK; \
  object -> flags |= pimpl -> grey_color
#define MAKE_BLACK(object) \
  object -> flags &= ~ ProtoObject::COLOR_MASK; \
  object -> flags |= pimpl -> black_color
#define MAKE_ECRU(object) \
  object -> flags &= ~ ProtoObject::COLOR_MASK; \
  object -> flags |= pimpl -> ecru_color

#define IS_DESTROYED(object) \
  (object -> flags & ProtoObject::DESTROYED_BIT)

/**
 * @brief List head
 */
class ManagerHead : public ProtoObject {
  private:
    /* -- avoid copying */
    ManagerHead(
        const ManagerHead&);
    ManagerHead& operator = (
        const ManagerHead&);

  public:
    ManagerHead();
    virtual ~ManagerHead();
    virtual void gcScan(
        Manager* manager_);
    virtual void gcFinalize();

    void swap(
        ManagerHead& head_);
    bool empty() const;

    virtual std::ostream& test_gcTestMark(
        std::ostream& os_) const;
};

ManagerHead::ManagerHead() :
  ProtoObject() {
  manager = 0;
  flags = 0;
  prev = this;
  next = this;
  root_count = 0;
  memblock = 0;
}

ManagerHead::~ManagerHead() {

}

void ManagerHead::gcScan(
    Manager* manager_) {
  OGCASSERT_NEVER();
}

void ManagerHead::gcFinalize() {
  OGCASSERT_NEVER();
}

void ManagerHead::swap(
    ManagerHead& head_) {
  ProtoObject* n1_(next);
  ProtoObject* n2_(head_.next);
  remove(); head_.remove();
  if(n2_ != &head_)
    insertBefore(n2_);
  if(n1_ != this)
    head_.insertBefore(n1_);
}

bool ManagerHead::empty() const {
  return next == this;
}

std::ostream& ManagerHead::test_gcTestMark(
    std::ostream& os_) const {
  bool first_(true);
  ProtoObject* object_(next);
  while(object_ != this) {
    if(!first_)
      os_ << " ";
    else
      first_ = false;

    if(!IS_DESTROYED(object_))
      object_ -> test_gcTestMark(os_);
    else {
      os_ << "dead<>";
    }
    object_ = object_ -> next;
  }
  return os_;
}

/**
 * @brief The implementation structure
 */
struct ManagerImpl {
  Manager* owner;

  /* -- manager's cycles */
  enum Phase {
      MARK = 0,
      ROOTING,
      FINALIZE,
      SWEEP,
  };
  Phase phase;

  /* -- colors */
  unsigned int white_color;
  unsigned int grey_color;
  unsigned int black_color;
  unsigned int ecru_color;

  /* -- list of nodes */
  ManagerHead white;
  ManagerHead grey;
  ManagerHead black;
  ManagerHead ecru;
  ManagerHead disconnected;
  ManagerHead finalized;
  ManagerHead rooting;

  /* -- controlling of GC cycle */
  unsigned int quantum_length;      /*!< size of one quantum in percents */
  int wait_length;         /*!< size of waiting interval in percents */
  unsigned int allocation_counter;  /*!< counter of allocated objects */
  unsigned int black_counter;       /*!< count of black objects */
  History history;
  bool cycle_running;

  ManagerImpl(
      Manager* owner_,
      unsigned int quantum_length_,
      unsigned int wait_length_,
      unsigned int history_size_);

  void doRootingCycle(
      unsigned int& step_);
  void doMarkCycle(
      unsigned int& step_);
  void disconnectObjects();
  void doFinalizeCycle(
      unsigned int& step_);
  void doSweepCycle(
      unsigned int& step_);
  bool doGCQuantum(
      unsigned int length_);
};

ManagerImpl::ManagerImpl(
    Manager* owner_,
    unsigned int quantum_length_,
    unsigned int wait_length_,
    unsigned int history_size_) :
  owner(owner_),
  phase(ROOTING),
  white_color(0), grey_color(1), black_color(2), ecru_color(3),
  white(), grey(), black(), ecru(), rooting(),
  quantum_length(quantum_length_),
  wait_length(wait_length_ - 100),
  allocation_counter(0),
  black_counter(0),
  history(history_size_),
  cycle_running(false) {

}

void ManagerImpl::doRootingCycle(
    unsigned int& step_) {
  ProtoObject* object_(rooting.next);
  while(step_ > 0 && object_ != & rooting) {
    --step_;
    object_ -> remove();
    if(object_ -> root_count > 0)
      object_ -> insertBefore(&grey);
    else
      object_ -> insertBefore(&white);
    object_ = rooting.next;
  }
}

void ManagerImpl::doMarkCycle(
    unsigned int& step_) {
  ProtoObject* object_(grey.next);
  while(step_ > 0 && object_ != & grey) {
    --step_;
    if(!IS_DESTROYED(object_))
      object_ -> gcScan(owner);
    owner -> makeBlack(object_);
    ++black_counter;
    object_ = grey.next;
  }
}

void ManagerImpl::disconnectObjects() {
  ProtoObject* object_(white.next);
  while(object_ != &white) {
    /* -- disconnect all weak references (the object cannot be
     *    used again) */
    if(!IS_DESTROYED(object_)
       && (object_ -> flags & ProtoObject::WEAK_BIT) == 0
       && static_cast<Object *>(object_) -> referent) {
      static_cast<Object *>(object_) -> referent -> ref_object = 0;
      static_cast<Object *>(object_) -> referent = 0;
    }
    object_ -> remove();
    object_ -> insertBefore(& ecru);

    object_ = white.next;
  }
}

void ManagerImpl::doFinalizeCycle(
    unsigned int& step_) {
  ProtoObject* object_(ecru.next);
  while(step_ > 0 && object_ != & ecru) {
    --step_;

    /* -- finalize the object */
    if(!IS_DESTROYED(object_))
      object_ -> gcFinalize();
    object_ -> remove();
    object_ -> insertBefore(&finalized);

    object_ = ecru.next;
  }
}

void ManagerImpl::doSweepCycle(
    unsigned int& step_) {
  ProtoObject* object_(finalized.next);
  while(step_ > 0 && object_ != &finalized) {
    --step_;
    object_ -> remove();

    /* -- The destructor can be already called (when an exception is thrown
     *    from the constructor). If it has happened the destroyed bit was set.
     *    So, I call the destructor directly if it's needed and then manually
     *    deallocate the memory. */
    if(!IS_DESTROYED(object_))
      object_->~ProtoObject();
    ::operator delete(object_ -> memblock);

    object_ = finalized.next;
  }
}

bool ManagerImpl::doGCQuantum(
    unsigned int length_) {
  unsigned int step_(length_);
  unsigned int h_;
  switch(phase) {
    case ROOTING:
      /* -- detect roots */
      doRootingCycle(step_);
      if(step_ == 0)
        return false;
      phase = MARK;
      black_counter = 0;
      /* -- no break, I'm going to next phase */

    case MARK:
      /* -- the mark phase */
      if(grey.empty() && white.empty())
        return true;  /* -- there is no work */
      doMarkCycle(step_);
      if(step_ == 0)
        return false;  /* -- the quantum is fully used */

      /* -- the mark phase is ended, disconnect unreachable objects */
      disconnectObjects();
      /* -- prepare the black objects for rooting */
      black.swap(rooting);
      /* -- rotate colors */
      h_ = white_color;
      white_color = black_color;
      black_color = ecru_color;
      ecru_color = h_;

      phase = FINALIZE;
      /* -- no break, because I'm going to next phase */

    case FINALIZE:
      /* -- finalize the objects */
      doFinalizeCycle(step_);
      if(step_ == 0)
        return false;  /* -- the quantum is fully used */
      phase = SWEEP;
      /* -- no break */

    case SWEEP:
      doSweepCycle(step_);
      if(step_ == 0)
        return false;  /* -- used quantum */
      phase = ROOTING;
      return true;

    default:
      OGCASSERT_NEVER();
      return false;
  }
}

Manager::Manager() {
  pimpl = new ManagerImpl(this, 200, 120, 10);
}

Manager::Manager(
    unsigned int quantum_length_,
    unsigned int wait_length_,
    unsigned int history_size_) {
  pimpl = new ManagerImpl(
      this, quantum_length_, wait_length_, history_size_);
}

Manager::~Manager() {
  /* -- really clean all objects */
  forceClean();
  /* -- destroy the implementation */
  delete pimpl; pimpl = 0;
}

void Manager::initObject(
    ProtoObject* object_,
    void* block_,
    std::size_t size_,
    std::size_t offset_) {
  OGCASSERT(object_ != 0 && block_ != 0);

  MAKE_WHITE(object_);
  object_ -> insertBefore(&pimpl -> white);
  ++pimpl -> allocation_counter;
}

void Manager::makeWhite(
    ProtoObject* object_) {
  OGCASSERT(object_ != 0);

  if(!IS_WHITE(object_)) {
    object_ -> remove();
    object_ -> insertBefore(&pimpl -> white);
    MAKE_WHITE(object_);
  }
}

void Manager::makeGrey(
    ProtoObject* object_) {
  OGCASSERT(object_ != 0);

  if(!IS_GREY(object_)) {
    object_ -> remove();
    object_ -> insertBefore(&pimpl -> grey);
    MAKE_GREY(object_);
  }
}

void Manager::makeBlack(
    ProtoObject* object_) {
  OGCASSERT(object_ != 0);

  if(!IS_BLACK(object_)) {
    object_ -> remove();
    object_ -> insertBefore(&pimpl -> black);
    MAKE_BLACK(object_);
  }
}

void Manager::writeBarrier(
    Object* parent_,
    ProtoObject* object_) {
  OGCASSERT(parent_ != 0 && object_ != 0 && !IS_ECRU(object_));

  if(IS_BLACK(parent_) && IS_WHITE(object_))
    makeGrey(object_);
}

void Manager::makeRoot(
    ProtoObject* object_) {
  OGCASSERT(object_ != 0);

  ++object_ -> root_count;
  makeGrey(object_);
}

void Manager::freeRoot(
    ProtoObject* object_) {
  OGCASSERT(object_ != 0 && object_ -> root_count > 0);

  --object_ -> root_count;
}

void Manager::scavenge(
    ProtoObject* object_) {
  OGCASSERT(object_ != 0);
  if(!IS_DESTROYED(object_) && IS_WHITE(object_))
    makeGrey(object_);
}

void Manager::doGCQuantum() {
  if(pimpl -> cycle_running
     || (pimpl -> allocation_counter * 100
         >= pimpl -> black_counter * pimpl -> wait_length)) {
    /* -- update the history */
    pimpl -> history.addSample(pimpl -> allocation_counter);
    pimpl -> allocation_counter = 0;
    /* -- compute size of this quantum */
    int quantum_(pimpl -> history.getAverage());
    quantum_ *= pimpl -> quantum_length;
    quantum_ /= 25;  /* -- 100 / 4: 4 steps for one allocated object */
    quantum_ += pimpl -> black_counter / 2;
    /* -- run the quantum */
    pimpl -> cycle_running = !pimpl -> doGCQuantum(quantum_);
  }
}

void Manager::doGCCycle() {
  while(!pimpl -> doGCQuantum(1000));
  pimpl -> cycle_running = false;
}

void Manager::forceClean() {
  doGCCycle();
  doGCCycle();
  doGCCycle();
}

void Manager::setQuantumLength(
    unsigned int quantum_) {
  pimpl -> quantum_length = quantum_;
}

void Manager::setWaitLength(
    unsigned int length_) {
  pimpl -> wait_length = static_cast<int>(length_) - 100;
}

bool Manager::isZombie(
    const ProtoObject* object_) const {
  OGCASSERT(object_ != 0);
  return IS_ECRU(object_);
}

std::ostream& Manager::test_testMark(
    std::ostream& os_) const {
  os_ << "GC {" << std::endl;
  os_ << "  white: "; pimpl -> white.test_gcTestMark(os_) << std::endl;
  os_ << "  grey: "; pimpl -> grey.test_gcTestMark(os_) << std::endl;
  os_ << "  black: "; pimpl -> black.test_gcTestMark(os_) << std::endl;
  os_ << "  rooting: "; pimpl -> rooting.test_gcTestMark(os_) << std::endl;
  os_ << "  ecru: "; pimpl -> ecru.test_gcTestMark(os_) << std::endl;
  os_ << "  finalized: "; pimpl -> finalized.test_gcTestMark(os_) << std::endl;
  os_ << "}" << std::endl;
  return os_;
}

} /* -- namespace OndraGC */
