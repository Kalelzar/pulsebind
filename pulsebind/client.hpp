#ifndef CLIENT_H
#define CLIENT_H


#include "list.hpp"
#include "pulseaudio.hpp"

namespace Pulsebind {

struct Client {
  const char* name;
  uint32_t id;
};

extern "C" List get_clients(PulseAudio &p);
extern "C" Client *get_client_by_name(List list, const char *name);
extern "C" Client *get_client_by_id(List list, uint32_t index);

extern "C" void free_client(Client &client);

extern "C" void free_clients(List &list);

} // namespace Pulsebind
#endif /* CLIENT_H */
