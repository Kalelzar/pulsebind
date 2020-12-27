#include "client.hpp"
#include <cstring>

#include "callback.hpp"

namespace Pulsebind {
extern "C" List get_clients(PulseAudio &pa) {
  List l = new_list(4);
  pa_operation *op =
      pa_context_get_client_info_list(pa.context, &onClientInfo, &l);

  iterate(pa, op);

  pa_operation_unref(op);

  return l;
}

extern "C" Client *get_client_by_name(List list, const char *name) {
  for (size_t i = 0; i < list.size; i++) {
    Client *hs = (Client *)list_get(list, i);
    if (strcmp(hs->name, name) == 0) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" Client *get_client_by_id(List list, uint32_t index){
  for (size_t i = 0; i < list.size; i++) {
    Client *hs = (Client *)list_get(list, i);
    if (hs->id == index) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" void free_client(Client &client){
  free((void *)client.name);
}

extern "C" void free_clients(List &list){
  for (size_t i = 0; i < list.size; i++) {
    if (list_get(list, i)) {
      free_client(*((Client*)list_get(list, i)));
    }
  }
  delete_list(list);
}


} // namespace Pulsebind
