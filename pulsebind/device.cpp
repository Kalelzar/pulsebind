#include "device.hpp"
#include "client.hpp"
#include "pulseaudio.hpp"

#include <cstdio>
#include <cstring>

namespace Pulsebind {

extern "C" uint32_t deviceGetID(Device &d) {
  switch (d.type) {
  case SINK:
    return d.device.sink->id;
    break;
  case SOURCE:
    return d.device.source->id;
    break;
  case SINK_INPUT:
    return d.device.sinkInput->id;
    break;
  case SOURCE_OUTPUT:
    return d.device.sourceOutput->id;
    break;
  case UNDEFINED:
    return UINT32_MAX;
    break;
  }

  return UINT32_MAX;
}

extern "C" const char *deviceGetName(Device &d) {

  switch (d.type) {
  case SINK:
    return d.device.sink->name;
    break;
  case SOURCE:
    return d.device.source->name;
    break;
  case SINK_INPUT:
    return d.device.sinkInput->name;
    break;
  case SOURCE_OUTPUT:
    return d.device.sourceOutput->name;
    break;
  case UNDEFINED:
    return nullptr;
    break;
  }

  return nullptr;
}

extern "C" const char *deviceGetDescription(Device &d, const List clients) {
  switch (d.type) {
  case SINK:
    return d.device.sink->description;
    break;
  case SOURCE:
    return d.device.source->description;
    break;
  case SINK_INPUT:
    return ((Client *)getClientById(clients, d.device.sinkInput->client))->name;
    break;
  case SOURCE_OUTPUT:
    return ((Client *)getClientById(clients, d.device.sourceOutput->client))
        ->name;
    break;
  case UNDEFINED:
    return nullptr;
    break;
  }

  return nullptr;
}

extern "C" bool deviceGetMute(Device &d) {
  switch (d.type) {
  case SINK:
    return d.device.sink->mute;
    break;
  case SOURCE:
    return d.device.source->mute;
    break;
  case SINK_INPUT:
    return d.device.sinkInput->mute;
    break;
  case SOURCE_OUTPUT:
    return d.device.sourceOutput->mute;
    break;
  case UNDEFINED:
    return true;
    break;
  }

  return true;
}

extern "C" int deviceGetVolumePercent(Device &d) {
  switch (d.type) {
  case SINK:
    return d.device.sink->volumePercent;
    break;
  case SOURCE:
    return d.device.source->volumePercent;
    break;
  case SINK_INPUT:
    return d.device.sinkInput->volumePercent;
    break;
  case SOURCE_OUTPUT:
    return d.device.sourceOutput->volumePercent;
    break;
  case UNDEFINED:
    return -1;
    break;
  }

  return -1;
}

extern "C" pa_cvolume deviceGetVolume(Device &d) {
  switch (d.type) {
  case SINK:
    return d.device.sink->volume;
    break;
  case SOURCE:
    return d.device.source->volume;
    break;
  case SINK_INPUT:
    return d.device.sinkInput->volume;
    break;
  case SOURCE_OUTPUT:
    return d.device.sourceOutput->volume;
    break;
  default:
    return *((pa_cvolume *)0xdeadbeef);
  }

  return *((pa_cvolume *)0xdeadbeef);
}

extern "C" void deviceSetMute(PulseAudio &pa, Device &d, const bool mute) {
  switch (d.type) {
  case SINK:
    sinkSetMute(pa, d.device.sink, mute);
    break;
  case SOURCE:
    sourceSetMute(pa, d.device.source, mute);
    break;
  case SINK_INPUT:
    sinkInputSetMute(pa, d.device.sinkInput, mute);
    break;
  case SOURCE_OUTPUT:
    sourceOutputSetMute(pa, d.device.sourceOutput, mute);
    break;
  case UNDEFINED:
    break;
  }
}

extern "C" void deviceSetVolumePercent(PulseAudio &pa, Device &d,
                                       const int vol) {
  switch (d.type) {
  case SINK:
    sinkSetVolume(pa, d.device.sink, vol);
    break;
  case SOURCE:
    sourceSetVolume(pa, d.device.source, vol);
    break;
  case SINK_INPUT:
    sinkInputSetVolume(pa, d.device.sinkInput, vol);
    break;
  case SOURCE_OUTPUT:
    sourceOutputSetVolume(pa, d.device.sourceOutput, vol);
    break;
  case UNDEFINED:
    break;
  }
}

extern "C" void deviceSetVolume(PulseAudio &pa, Device &d,
                                pa_cvolume &cvolume) {
  switch (d.type) {
  case SINK:
    sinkSetCVolume(pa, d.device.sink, cvolume);
    break;
  case SOURCE:
    sourceSetCVolume(pa, d.device.source, cvolume);
    break;
  case SINK_INPUT:
    sinkInputSetCVolume(pa, d.device.sinkInput, cvolume);
    break;
  case SOURCE_OUTPUT:
    sourceOutputSetCVolume(pa, d.device.sourceOutput, cvolume);
    break;
  case UNDEFINED:
    break;
  }
}

/**
 * ~i - ID
 * ~t - Device type
 * ~n - Name
 * ~d - Description/Client name
 * ~m - Mute status (true/false)
 * ~M - Mute status (muted/not muted)
 * ~v - Volume Percent
 * ~V - Volume for each channel
 * ~c - Channel count
 * ~C - Client ID (SW sink/source only, "" on HW sink/source)
 * ~s - Sink/Source (SW only / same as ~i for HW)
 */
extern "C" const char *formatDevice(PulseAudio &pa, Device &d,
                                    const char *fmt) {
  // 4KB should be more than enough.
  // TODO: Write an overload for this function where the user can pass their
  // own pointer to write to.
  char *str = (char *)calloc(4096, sizeof(char));

  size_t fmtl = strlen(fmt);

  size_t point = 0;

  List clients = getClients(pa);

  for (size_t i = 0; i < fmtl; i++) {

    char next = fmt[i];

    if (next != '~') {
      str[point++] = next;
    } else {
      i++;
      char format = fmt[i];
      const char *buf;
      pa_cvolume cv;
      switch (format) {
      case 'i':
        point += snprintf(str + point, 4096 - point, "%u", deviceGetID(d));
        break;
      case 'n':
        buf = deviceGetName(d);
        strncpy(str + point, buf, 4096 - point);
        point += (strlen(buf));
        break;
      case 't':
        switch (d.type) {
        case DeviceType::SINK:
          strncpy(str + point, "Sink", 4);
          point += 4;
          break;
        case DeviceType::SOURCE:
          strncpy(str + point, "Source", 6);
          point += 6;
          break;
        case DeviceType::SOURCE_OUTPUT:
          strncpy(str + point, "Source Output", 13);
          point += 13;
          break;
        case DeviceType::SINK_INPUT:
          strncpy(str + point, "Sink Input", 10);
          point += 10;
          break;
        case DeviceType::UNDEFINED:
          strncpy(str + point, "Undefined", 9);
          point += 9;
          break;
        }
        break;
      case 'd':
        buf = deviceGetDescription(d, clients);
        strncpy(str + point, buf, 4096 - point);
        point += (strlen(buf));
        break;
      case 'm':
        if (deviceGetMute(d)) {
          strncpy(str + point, "true", 4);
          point += 4;
        } else {
          strncpy(str + point, "false", 5);
          point += 5;
        }
        break;
      case 'M':
        if (deviceGetMute(d)) {
          strncpy(str + point, "muted", 5);
          point += 5;
        } else {
          strncpy(str + point, "not muted", 9);
          point += 9;
        }
        break;
      case 'v':
        point += snprintf(str + point, 4096 - point, "%d",
                          deviceGetVolumePercent(d));
        break;
      case 'V':
        str[point++] = '[';
        cv = deviceGetVolume(d);
        for (int c = 0; c < cv.channels; c++) {
          point += snprintf(str + point, 4096 - point, "%d",
                            normalizeVolume(cv.values[c]));
          if (c + 1 != cv.channels) {
            str[point++] = ' ';
          }
        }
        str[point++] = ']';
        break;
      case 'c':
        cv = deviceGetVolume(d);
        point += snprintf(str + point, 4096 - point, "%u", cv.channels);
        break;
      case 'C':
        if (d.type == DeviceType::SOURCE_OUTPUT ||
            d.type == DeviceType::SINK_INPUT) {
          point += snprintf(str + point, 4096 - point, "%u",
                            d.device.sinkInput->client);
        }
        break;
      case 's':
        if (d.type == DeviceType::SOURCE_OUTPUT ||
            d.type == DeviceType::SINK_INPUT) {
          point += snprintf(str + point, 4096 - point, "%u",
                            d.device.sinkInput->sink);
        } else {
          point += snprintf(str + point, 4096 - point, "%u", d.device.sink->id);
        }
        break;
      default:
        str[point++] = next;
        str[point++] = format;
      }
    }
  }

  str[point] = '\0';

  freeClients(clients);
  return str;
};

extern "C" void printDevice(PulseAudio &pa, Device &d, const char *fmt) {
  const char *str = formatDevice(pa, d, fmt);
  printf("%s", str);
  free((void *)str);
}

extern "C" Device newDevice(PulseAudio &pa, DeviceType deviceT,
                            void *deviceSrc) {
  Device d;
  d.type = deviceT;
  switch (deviceT) {
  case SINK:
    d.device.sink = (Sink *)deviceSrc;
    break;
  case SINK_INPUT:
    d.device.sinkInput = (SinkInput *)deviceSrc;
    break;
  case SOURCE:
    d.device.source = (Source *)deviceSrc;
    break;
  case SOURCE_OUTPUT:
    d.device.sourceOutput = (SourceOutput *)deviceSrc;
    break;
  case UNDEFINED:
    break;
  }
  return d;
}

} // namespace Pulsebind
