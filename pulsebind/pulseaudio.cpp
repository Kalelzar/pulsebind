#include "pulseaudio.hpp"
#include "callback.hpp"

namespace Pulsebind {

extern "C" PulseAudio new_pulse(const char *client) noexcept {
  PulseAudio pa;

  pa.mainloop = pa_mainloop_new();
  pa.mainloop_api = pa_mainloop_get_api(pa.mainloop);
  pa.context = pa_context_new(pa.mainloop_api, client);
  pa_context_set_state_callback(pa.context, &onStateChanged, &pa);
  pa.state = CONNECTING;

  if (pa_context_connect(pa.context, nullptr, PA_CONTEXT_NOFLAGS, nullptr) <
      0) {
    pa.state = ERROR;
    return pa;
  }

  while (pa.state == CONNECTING) {
    if (pa_mainloop_iterate(pa.mainloop, 1, &pa.retval) < 0) {
      pa.state = ERROR;
      return pa;
    }
  }

  return pa;
}

extern "C" void delete_pulse(PulseAudio &pa) noexcept {
  pa_context_set_state_callback(pa.context, nullptr, nullptr);

  if (pa.state == CONNECTED)
    pa_context_disconnect(pa.context);

  pa_context_unref(pa.context);
  pa_mainloop_free(pa.mainloop);
}

void iterate(PulseAudio &pa, pa_operation *op) noexcept {
  while (pa_operation_get_state(op) == PA_OPERATION_RUNNING) {
    pa_mainloop_iterate(pa.mainloop, 1, &pa.retval);
  }
}

} // namespace Pulsebind
