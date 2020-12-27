#ifndef SINK_H
#define SINK_H

#include "list.hpp"
#include "pulseaudio.hpp"

namespace Pulsebind {

struct HardwareSink {
  uint32_t id;
  const char *name;
  const char *description;
  bool mute;
  pa_volume_t baseVolume;
  pa_cvolume volume;
  pa_volume_t volumeAvg;
  int volumePercent;
};

extern "C" List get_hardware_sinks(PulseAudio &p);
extern "C" HardwareSink *get_hardware_sink_by_name(List list, const char *name);
extern "C" HardwareSink *get_hardware_sink_by_id(List list, uint32_t index);
extern "C" HardwareSink *get_hardware_sink_by_description(List list,
                                                          const char *desc);

extern "C" void hardware_sink_set_volume(PulseAudio &pa, HardwareSink *sink,
                                         uint32_t volume);
extern "C" void hardware_sink_set_cvolume(PulseAudio &pa, HardwareSink *sink,
                                          pa_cvolume cvol);

extern "C" void hardware_sink_set_mute(PulseAudio &pa, HardwareSink *sink,
                                       bool mute);

extern "C" void free_hardware_sink(HardwareSink &sink);

extern "C" void free_hardware_sinks(List &list);

extern "C" int normalize_volume(pa_volume_t t);
extern "C" pa_volume_t denormalize_volume(int v);

} // namespace Pulsebind

#endif /* SINK_H */
