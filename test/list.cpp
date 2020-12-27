#include "../lib/googletest/googletest/include/gtest/gtest.h"
#include "../lib/rapidcheck/extras/gtest/include/rapidcheck/gtest.h"

#include <pulsebind/list.hpp>

RC_GTEST_PROP(List, AllocN, (unsigned char N)){
  size_t size = (size_t) N;
  List l = new_list(size);
  RC_ASSERT(l.capacity == size);
  RC_ASSERT(l.size == 0);
  RC_ASSERT((int*) l.array != nullptr);
  delete_list(l);
}

RC_GTEST_PROP(List, AddNSubsequentInts, (unsigned char N)){
  size_t size = (size_t) N;
  List l = new_list(size);
  RC_ASSERT(l.capacity == size);
  RC_ASSERT(l.size == 0);
  RC_ASSERT((int*) l.array != nullptr);

  for(size_t i = 0; i < size; i++){
    size_t* iptr = (size_t*) malloc(sizeof(size_t));
    *iptr = i;
    list_add(l, (void*) iptr);
    RC_ASSERT(l.size == i+1);
  }

  for(size_t i = 0; i < size; i++){
    RC_ASSERT(*((size_t*) list_get(l, i)) == i);
  }

  delete_list(l);
}
