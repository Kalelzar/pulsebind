#ifndef SOFTWARESINK_H
#define SOFTWARESINK_H

#include "list.hpp"
#include "pulseaudio.hpp"

namespace Pulsebind {

struct SinkInput {
  uint32_t id;
  uint32_t sink;
  uint32_t client;
  const char *name;
  bool mute;
  pa_cvolume volume;
  pa_volume_t volumeAvg;
  int volumePercent;
};

extern "C" List getSinkInputs(PulseAudio &p);
extern "C" SinkInput *getSinkInputByName(List list, const char *name);
extern "C" SinkInput *getSinkInputById(List list, uint32_t index);
extern "C" SinkInput *getSinkInputByDescription(List sinks, List clients,
                                                const char *desc);

extern "C" void sinkInputSetVolume(PulseAudio &pa, SinkInput *sink,
                                   uint32_t volume);
extern "C" void sinkInputSetCVolume(PulseAudio &pa, SinkInput *sink,
                                    pa_cvolume cvol);
extern "C" void sinkInputSetMute(PulseAudio &pa, SinkInput *sink, bool mute);

extern "C" void sinkInputSetSink(PulseAudio &pa, SinkInput *sink,
                                 uint32_t hw_sink);

extern "C" void freeSinkInput(SinkInput &sink);

extern "C" void freeSinkInputs(List &list);

} // namespace Pulsebind

#endif /* SOFTWARESINK_H */
