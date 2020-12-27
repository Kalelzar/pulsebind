#ifndef LIST_H
#define LIST_H

#include <cstdlib>

struct List {
  void**array = nullptr;
  size_t size = 0;
  size_t capacity = 0;
};

extern "C" List new_list(size_t capacity);
extern "C" void list_add(List& l, void* data);
extern "C" void* list_get(List& l, size_t ind);
extern "C" void delete_list(List& l);

#endif /* LIST_H */
