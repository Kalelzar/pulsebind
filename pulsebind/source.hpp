#ifndef SOURCE_H
#define SOURCE_H

#include "list.hpp"
#include "pulseaudio.hpp"

namespace Pulsebind {

struct Source {
  uint32_t id;
  const char *name;
  const char *description;
  bool mute;
  pa_volume_t baseVolume;
  pa_cvolume volume;
  pa_volume_t volumeAvg;
  int volumePercent;
};

extern "C" List getSources(PulseAudio &p);
extern "C" Source *getSourceByName(List list,
                                                       const char *name);
extern "C" Source *getSourceById(List list, uint32_t index);
extern "C" Source *getSourceByDescription(List list,
                                                              const char *desc);

extern "C" void sourceSetVolume(PulseAudio &pa,
                                           Source *source,
                                           uint32_t volume);
extern "C" void sourceSetCVolume(PulseAudio &pa,
                                            Source *source,
                                            pa_cvolume cvol);
extern "C" void sourceSetMute(PulseAudio &pa, Source *source,
                                         bool mute);

extern "C" void freeSource(Source &source);

extern "C" void freeSources(List &list);

} // namespace Pulsebind

#endif /* SOURCE_H */
