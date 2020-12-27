#ifndef SOURCE_H
#define SOURCE_H

#include "list.hpp"
#include "pulseaudio.hpp"

namespace Pulsebind {

struct HardwareSource {
  uint32_t id;
  const char *name;
  const char *description;
  bool mute;
  pa_volume_t baseVolume;
  pa_cvolume volume;
  pa_volume_t volumeAvg;
  int volumePercent;
};

extern "C" List get_hardware_sources(PulseAudio &p);
extern "C" HardwareSource *get_hardware_source_by_name(List list,
                                                       const char *name);
extern "C" HardwareSource *get_hardware_source_by_id(List list, uint32_t index);
extern "C" HardwareSource *get_hardware_source_by_description(List list,
                                                              const char *desc);

extern "C" void hardware_source_set_volume(PulseAudio &pa,
                                           HardwareSource *source,
                                           uint32_t volume);
extern "C" void hardware_source_set_cvolume(PulseAudio &pa,
                                            HardwareSource *source,
                                            pa_cvolume cvol);
extern "C" void hardware_source_set_mute(PulseAudio &pa, HardwareSource *source,
                                         bool mute);

extern "C" void free_hardware_source(HardwareSource &source);

extern "C" void free_hardware_sources(List &list);

} // namespace Pulsebind

#endif /* SOURCE_H */
