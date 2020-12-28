#ifndef SINK_H
#define SINK_H

#include "list.hpp"
#include "pulseaudio.hpp"

namespace Pulsebind {

struct Sink {
  uint32_t id;
  const char *name;
  const char *description;
  bool mute;
  pa_volume_t baseVolume;
  pa_cvolume volume;
  pa_volume_t volumeAvg;
  int volumePercent;
};

extern "C" List getSinks(PulseAudio &p);
extern "C" Sink *getSinkByName(List list, const char *name);
extern "C" Sink *getSinkById(List list, uint32_t index);
extern "C" Sink *getSinkByDescription(List list,
                                                          const char *desc);

extern "C" void sinkSetVolume(PulseAudio &pa, Sink *sink,
                                         uint32_t volume);
extern "C" void sinkSetCVolume(PulseAudio &pa, Sink *sink,
                                          pa_cvolume cvol);

extern "C" void sinkSetMute(PulseAudio &pa, Sink *sink,
                                       bool mute);

extern "C" void freeSink(Sink &sink);

extern "C" void freeSinks(List &list);

extern "C" int normalizeVolume(pa_volume_t t);
extern "C" pa_volume_t denormalizeVolume(int v);

} // namespace Pulsebind

#endif /* SINK_H */
