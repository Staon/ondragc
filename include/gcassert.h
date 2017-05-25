#ifndef OndraGCINCLUDE_GCASSERT_H_
#define OndraGCINCLUDE_GCASSERT_H_

#include <ondrart/oassert.h>

#define OGCASSERT(expression_) \
  OASSERT_1(expression_)

#define OGCASSERT_NEVER() \
  OASSERT_NEVER()

#endif /* -- OndraGCINCLUDE_GCASSERT_H_ */
