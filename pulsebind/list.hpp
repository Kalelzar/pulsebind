#ifndef LIST_H
#define LIST_H

#include <cstdlib>

struct List {
  void** array = nullptr;
  size_t size = 0;
  size_t capacity = 0;
};

extern "C" List newList(size_t capacity);
extern "C" void listAdd(List& l, void* data);
extern "C" void* listGet(List& l, size_t ind);
extern "C" void deleteList(List& l);

#endif /* LIST_H */
