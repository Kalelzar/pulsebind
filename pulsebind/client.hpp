#ifndef CLIENT_H
#define CLIENT_H


#include "list.hpp"
#include "pulseaudio.hpp"

namespace Pulsebind {

struct Client {
  const char* name;
  uint32_t id;
};

extern "C" List getClients(PulseAudio &p);
extern "C" Client *getClientByName(List list, const char *name);
extern "C" Client *getClientById(List list, uint32_t index);

extern "C" void freeClient(Client &client);

extern "C" void freeClients(List &list);

} // namespace Pulsebind
#endif /* CLIENT_H */
