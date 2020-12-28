#ifndef SOURCEOUTPUT_H
#define SOURCEOUTPUT_H

#include "list.hpp"
#include "pulseaudio.hpp"

namespace Pulsebind {

struct SourceOutput {
  uint32_t id;
  uint32_t source;
  uint32_t client;
  const char *name;
  bool mute;
  pa_cvolume volume;
  pa_volume_t volumeAvg;
  int volumePercent;
};

extern "C" List getSourceOutputs(PulseAudio &p);
extern "C" SourceOutput *getSourceOutputByName(List list,
                                                       const char *name);
extern "C" SourceOutput *getSourceOutputById(List list, uint32_t index);
extern "C" SourceOutput *getSourceOutputByDescription(List sources,
                                                              List clients,
                                                              const char *desc);

extern "C" void sourceOutputSetVolume(PulseAudio &pa,
                                           SourceOutput *source,
                                           uint32_t volume);

extern "C" void sourceOutputSetCVolume(PulseAudio &pa,
                                            SourceOutput *source,
                                            pa_cvolume cvol);

extern "C" void sourceOutputSetMute(PulseAudio &pa, SourceOutput *source,
                                         bool mute);

extern "C" void sourceOutputSetSource(PulseAudio &pa,
                                              SourceOutput *source,
                                              uint32_t hw_source);

extern "C" void freeSourceOutput(SourceOutput &source);

extern "C" void freeSourceOutputs(List &list);

} // namespace Pulsebind

#endif /* SOURCEOUTPUT_H */
