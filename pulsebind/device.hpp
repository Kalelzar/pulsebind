#ifndef DEVICE_H
#define DEVICE_H

#include "pulseaudio.hpp"
#include "sink.hpp"
#include "softwareSink.hpp"
#include "softwareSource.hpp"
#include "source.hpp"

namespace Pulsebind {

enum DeviceType { SINK, SINK_INPUT, SOURCE, SOURCE_OUTPUT, UNDEFINED };

struct Device {

  DeviceType type;

  union {
    Sink *sink;
    Source *source;
    SinkInput *sinkInput;
    SourceOutput *sourceOutput;
  } device;
};

extern "C" uint32_t deviceGetID(Device &d);
extern "C" const char *deviceGetName(Device &d);
extern "C" const char *deviceGetDescription(Device &d, const List clients);
extern "C" bool deviceGetMute(Device &d);
extern "C" int deviceGetVolumePercent(Device &d);
extern "C" pa_cvolume deviceGetVolume(Device &d);

extern "C" void deviceSetMute(PulseAudio &pa, Device &d, const bool mute);
extern "C" void deviceSetVolumePercent(PulseAudio &pa, Device &d,
                                       const int vol);
extern "C" void deviceSetVolume(PulseAudio &pa, Device &d, pa_cvolume &vol);

extern "C" const char *formatDevice(PulseAudio &pa, Device &d, const char *fmt);
extern "C" void printDevice(PulseAudio &pa, Device &d, const char *fmt);

extern "C" Device newDevice(PulseAudio &pa, DeviceType deviceT,
                            void *deviceSrc);

} // namespace Pulsebind

#endif /* DEVICE_H */
