#include "client.hpp"
#include <cstring>

#include "callback.hpp"

namespace Pulsebind {
extern "C" List getClients(PulseAudio &pa) {
  List l = newList(4);
  pa_operation *op =
      pa_context_get_client_info_list(pa.context, &onClientInfo, &l);

  iterate(pa, op);

  pa_operation_unref(op);

  return l;
}

extern "C" Client *getClientByName(List list, const char *name) {
  for (size_t i = 0; i < list.size; i++) {
    Client *hs = (Client *)listGet(list, i);
    if (strcmp(hs->name, name) == 0) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" Client *getClientById(List list, uint32_t index){
  for (size_t i = 0; i < list.size; i++) {
    Client *hs = (Client *)listGet(list, i);
    if (hs->id == index) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" void freeClient(Client &client){
  free((void *)client.name);
}

extern "C" void freeClients(List &list){
  for (size_t i = 0; i < list.size; i++) {
    if (listGet(list, i)) {
      freeClient(*((Client*)listGet(list, i)));
    }
  }
  deleteList(list);
}


} // namespace Pulsebind
