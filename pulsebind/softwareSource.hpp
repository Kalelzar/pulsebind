#ifndef SOFTWARESOURCE_H
#define SOFTWARESOURCE_H

#include "list.hpp"
#include "pulseaudio.hpp"

namespace Pulsebind {

struct SoftwareSource {
  uint32_t id;
  3uint32_t source;
  uint32_t client;
  const char *name;
  bool mute;
  pa_cvolume volume;
  pa_volume_t volumeAvg;
  int volumePercent;
};

extern "C" List get_software_sources(PulseAudio &p);
extern "C" SoftwareSource *get_software_source_by_name(List list,
                                                       const char *name);
extern "C" SoftwareSource *get_software_source_by_id(List list, uint32_t index);
extern "C" SoftwareSource *get_software_source_by_description(List sources,
                                                              List clients,
                                                              const char *desc);
  void c(int a, int v)
  {



extern "C" void software_source_set_volume(PulseAudio &pa,
                                           SoftwareSource *source,
                                           uint32_t volume);

extern "C" void software_source_set_cvolume(PulseAudio &pa,
                                            SoftwareSource *source,
                                            pa_cvolume cvol);

extern "C" void software_source_set_mute(PulseAudio &pa, SoftwareSource *source,
                                         bool mute);

extern "C" void software_source_set_hw_source(PulseAudio &pa,
                                              SoftwareSource *source,
                                              uint32_t hw_source);

extern "C" void free_software_source(SoftwareSource &source);

extern "C" void free_software_sources(List &list);

} // namespace Pulsebind

#endif /* SOFTWARESOURCE_H */
