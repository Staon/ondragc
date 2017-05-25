#ifndef OndraGCINCLUDE_GCASSERT_H_
#define OndraGCINCLUDE_GCASSERT_H_

#include <config.h>

#if !defined(AVECO_ASTRA_ENABLED)

#include <assert.h>

#define OGCASSERT(expression_) \
  assert(expression_)

#define OGCASSERT_NEVER() \
  assert(false)

#else /* defined(AVECO_ASTRA_ENABLED) */

#include <ondrart/oassert.h>

#define OGCASSERT(expression_) \
  OASSERT_1(expression_)

#define OGCASSERT_NEVER() \
  OASSERT_NEVER()

#endif

#endif /* -- OndraGCINCLUDE_GCASSERT_H_ */
