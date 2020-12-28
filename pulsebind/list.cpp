#include "list.hpp"
#include <cstring>

extern "C" List new_list(size_t capacity) {
  List list;
  list.capacity = capacity;
  list.size = 0;
  list.array = (void **)calloc(capacity, sizeof(void *));
  return list;
}

inline void expand_list(List &list) {
  if (!list.array)
    return;
  int newCapacity = list.capacity ? list.capacity * 2 : 4;
  void **newArray = (void **)realloc(list.array, sizeof(void *) * newCapacity);
  if(!newArray) return;
  list.array = newArray;
  list.capacity = newCapacity;
}

extern "C" void list_add(List &list, void *data) {
  if (!list.array)
    return;

  if (list.size + 1 >= list.capacity) {
    expand_list(list);
    if(list.size + 1 >= list.capacity) return;
  }

  list.array[list.size++] = data;
}

extern "C" void delete_list(List &list) {
  if (!list.array)
    return;
  for (size_t i = 0; i < list.size; i++) {
    free(list.array[i]);
  }
  free(list.array);
  list.array = nullptr;
}

extern "C" void *list_get(List &list, size_t index) {
  if (!list.array)
    return nullptr;
  return list.array[index];
}
