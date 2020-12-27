#ifndef SOFTWARESINK_H
#define SOFTWARESINK_H

#include "list.hpp"
#include "pulseaudio.hpp"

namespace Pulsebind {

struct SoftwareSink {
  uint32_t id;
  uint32_t sink;
  uint32_t client;
  const char *name;
  bool mute;
  pa_cvolume volume;
  pa_volume_t volumeAvg;
  int volumePercent;
};

extern "C" List get_software_sinks(PulseAudio &p);
extern "C" SoftwareSink *get_software_sink_by_name(List list, const char *name);
extern "C" SoftwareSink *get_software_sink_by_id(List list, uint32_t index);
extern "C" SoftwareSink *
get_software_sink_by_description(List sinks, List clients, const char *desc);

extern "C" void software_sink_set_volume(PulseAudio &pa, SoftwareSink *sink,
                                         uint32_t volume);
extern "C" void software_sink_set_cvolume(PulseAudio &pa, SoftwareSink *sink,
                                          pa_cvolume cvol);
extern "C" void software_sink_set_mute(PulseAudio &pa, SoftwareSink *sink,
                                       bool mute);

extern "C" void software_sink_set_hw_sink(PulseAudio &pa, SoftwareSink *sink,
                                          uint32_t hw_sink);

extern "C" void free_software_sink(SoftwareSink &sink);

extern "C" void free_software_sinks(List &list);

} // namespace Pulsebind

#endif /* SOFTWARESINK_H */
