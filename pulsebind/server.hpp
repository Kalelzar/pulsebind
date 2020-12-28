#ifndef SERVER_H
#define SERVER_H

#include "pulseaudio.hpp"
#include <pulse/pulseaudio.h>

namespace Pulsebind {


struct Server {
  uint32_t cookie;
  const char *hostName;
  const char *userName;
  const char *serverName;
  const char *serverVersion;
  const char *defaultSinkName;
  const char *defaultSourceName;
};

extern "C" Server getServerInfo(PulseAudio &p) noexcept;
extern "C" void deleteServer(Server &s) noexcept;

} // namespace Pulsebind

#endif /* SERVER_H */
