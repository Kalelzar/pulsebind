#include "server.hpp"
#include "callback.hpp"
namespace Pulsebind {

extern "C" Server getServerInfo(PulseAudio &pa) noexcept {
  Server s;

  pa_operation *op = pa_context_get_server_info(pa.context, &onServerInfo, &s);
  iterate(pa, op);
  pa_operation_unref(op);

  return s;
}

extern "C" void deleteServer(Server &s) noexcept {
  free((void *)s.hostName);
  free((void *)s.userName);
  free((void *)s.serverName);
  free((void *)s.serverVersion);
  free((void *)s.defaultSinkName);
  free((void *)s.defaultSourceName);
}

} // namespace Pulsebind
