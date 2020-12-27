#ifndef PULSEAUDIO_H
#define PULSEAUDIO_H

#include <pulse/pulseaudio.h>

namespace Pulsebind {

enum State { CONNECTING, CONNECTED, ERROR, INVALID };

/**
 * A container storing the needed data for manipulating the pulseaudio server
 * via the asynchronous C library.
 * Must be created via {@code new_pulse}
 * and freed with {@code delete_pulse}.
 */
struct PulseAudio {
  pa_mainloop *mainloop;
  pa_mainloop_api *mainloop_api;
  pa_context *context;
  int retval;

  State state = INVALID;
};

/**
 * Connect to a local PulseAudio server with the provided client name.
 * must be freed with <code> delete_pulse </code>
 *
 * If the connection failed then PulseAudio::state will be ERROR
 * else it will be CONNECTED
 * @param client
 */
extern "C" PulseAudio new_pulse(const char *client) noexcept;

/**
 * Disconnect and free all necessary components.
 * @param p the connection to end.
 */
extern "C" void delete_pulse(PulseAudio &p) noexcept;

void iterate(PulseAudio &pa, pa_operation *op) noexcept;

} // namespace Pulsebind
#endif /* PULSEAUDIO_H */
