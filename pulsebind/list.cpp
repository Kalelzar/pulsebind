#include "list.hpp"
#include <cstring>

extern "C" List newList(size_t capacity) {
  List list;
  list.capacity = capacity;
  list.size = 0;
  list.array = (void **)calloc(capacity, sizeof(void *));
  return list;
}

inline void expandList(List &list) {
  if (!list.array)
    return;
  int newCapacity = list.capacity ? list.capacity * 2 : 4;
  void **newArray = (void **)realloc(list.array, sizeof(void *) * newCapacity);
  if(!newArray) return;
  list.array = newArray;
  list.capacity = newCapacity;
}

extern "C" void listAdd(List &list, void *data) {
  if (!list.array)
    return;

  if (list.size + 1 >= list.capacity) {
    expandList(list);
    if(list.size + 1 >= list.capacity) return;
  }

  list.array[list.size++] = data;
}

extern "C" void deleteList(List &list) {
  if (!list.array)
    return;
  for (size_t i = 0; i < list.size; i++) {
    free(list.array[i]);
  }
  free(list.array);
  list.array = nullptr;
}

extern "C" void *listGet(List &list, size_t index) {
  if (!list.array)
    return nullptr;
  return list.array[index];
}
