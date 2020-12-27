#include "device.hpp"
#include "client.hpp"

#include <cstdio>
#include <cstring>

namespace Pulsebind {

extern "C" uint32_t deviceGetID(Device &d) {
  switch (d.type) {
  case HW_SINK:
    return d.device.hw_sink->id;
    break;
  case HW_SOURCE:
    return d.device.hw_source->id;
    break;
  case SW_SINK:
    return d.device.sw_sink->id;
    break;
  case SW_SOURCE:
    return d.device.sw_source->id;
    break;
  case UNDEFINED:
    return UINT32_MAX;
    break;
  }

  return UINT32_MAX;
}

extern "C" const char *deviceGetName(Device &d) {

  switch (d.type) {
  case HW_SINK:
    return d.device.hw_sink->name;
    break;
  case HW_SOURCE:
    return d.device.hw_source->name;
    break;
  case SW_SINK:
    return d.device.sw_sink->name;
    break;
  case SW_SOURCE:
    return d.device.sw_source->name;
    break;
  case UNDEFINED:
    return nullptr;
    break;
  }

  return nullptr;
}

extern "C" const char *deviceGetDescription(Device &d, const List clients) {
  switch (d.type) {
  case HW_SINK:
    return d.device.hw_sink->description;
    break;
  case HW_SOURCE:
    return d.device.hw_source->description;
    break;
  case SW_SINK:
    return ((Client *)get_client_by_id(clients, d.device.sw_sink->client))
        ->name;
    break;
  case SW_SOURCE:
    return ((Client *)get_client_by_id(clients, d.device.sw_source->client))
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
  case HW_SINK:
    return d.device.hw_sink->mute;
    break;
  case HW_SOURCE:
    return d.device.hw_source->mute;
    break;
  case SW_SINK:
    return d.device.sw_sink->mute;
    break;
  case SW_SOURCE:
    return d.device.sw_source->mute;
    break;
  case UNDEFINED:
    return true;
    break;
  }

  return true;
}

extern "C" int deviceGetVolumePercent(Device &d) {
  switch (d.type) {
  case HW_SINK:
    return d.device.hw_sink->volumePercent;
    break;
  case HW_SOURCE:
    return d.device.hw_source->volumePercent;
    break;
  case SW_SINK:
    return d.device.sw_sink->volumePercent;
    break;
  case SW_SOURCE:
    return d.device.sw_source->volumePercent;
    break;
  case UNDEFINED:
    return -1;
    break;
  }

  return -1;
}

extern "C" pa_cvolume deviceGetVolume(Device &d) {
  switch (d.type) {
  case HW_SINK:
    return d.device.hw_sink->volume;
    break;
  case HW_SOURCE:
    return d.device.hw_source->volume;
    break;
  case SW_SINK:
    return d.device.sw_sink->volume;
    break;
  case SW_SOURCE:
    return d.device.sw_source->volume;
    break;
  default:
    return *((pa_cvolume *)0xdeadbeef);
  }

  return *((pa_cvolume *)0xdeadbeef);
}

extern "C" void deviceSetMute(PulseAudio &pa, Device &d, const bool mute) {
  switch (d.type) {
  case HW_SINK:
    hardware_sink_set_mute(pa, d.device.hw_sink, mute);
    break;
  case HW_SOURCE:
    hardware_source_set_mute(pa, d.device.hw_source, mute);
    break;
  case SW_SINK:
    software_sink_set_mute(pa, d.device.sw_sink, mute);
    break;
  case SW_SOURCE:
    software_source_set_mute(pa, d.device.sw_source, mute);
    break;
  case UNDEFINED:
    break;
  }
}

extern "C" void deviceSetVolumePercent(PulseAudio &pa, Device &d,
                                       const int vol) {
  switch (d.type) {
  case HW_SINK:
    hardware_sink_set_volume(pa, d.device.hw_sink, vol);
    break;
  case HW_SOURCE:
    hardware_source_set_volume(pa, d.device.hw_source, vol);
    break;
  case SW_SINK:
    software_sink_set_volume(pa, d.device.sw_sink, vol);
    break;
  case SW_SOURCE:
    software_source_set_volume(pa, d.device.sw_source, vol);
    break;
  case UNDEFINED:
    break;
  }
}

extern "C" void deviceSetVolume(PulseAudio &pa, Device &d,
                                pa_cvolume &cvolume) {
  switch (d.type) {
  case HW_SINK:
    hardware_sink_set_cvolume(pa, d.device.hw_sink, cvolume);
    break;
  case HW_SOURCE:
    hardware_source_set_cvolume(pa, d.device.hw_source, cvolume);
    break;
  case SW_SINK:
    software_sink_set_cvolume(pa, d.device.sw_sink, cvolume);
    break;
  case SW_SOURCE:
    software_source_set_cvolume(pa, d.device.sw_source, cvolume);
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
 * ~M - Mute status (1/0)
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

  List clients = get_clients(pa);

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
        case DeviceType::HW_SINK:
          strncpy(str+point, "Sink", 4);
          point+=4;
          break;
        case DeviceType::HW_SOURCE:
          strncpy(str+point, "Source", 6);
          point+=6;
          break;
        case DeviceType::SW_SOURCE:
          strncpy(str+point, "Source Output", 13);
          point+=13;
          break;
        case DeviceType::SW_SINK:
          strncpy(str+point, "Sink Input", 10);
          point+=10;
          break;
        case DeviceType::UNDEFINED:
          strncpy(str+point, "Undefined", 9);
          point+=9;
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
          str[point++] = '1';
        } else {
          str[point++] = '0';
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
                            normalize_volume(cv.values[c]));
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
        if (d.type == DeviceType::SW_SOURCE || d.type == DeviceType::SW_SINK) {
          point += snprintf(str + point, 4096 - point, "%u",
                            d.device.sw_sink->client);
        }
        break;
      case 's':
        if (d.type == DeviceType::SW_SOURCE || d.type == DeviceType::SW_SINK) {
          point +=
              snprintf(str + point, 4096 - point, "%u", d.device.sw_sink->sink);
        } else {
          point +=
              snprintf(str + point, 4096 - point, "%u", d.device.hw_sink->id);
        }
        break;
      default:
        str[point++] = next;
        str[point++] = format;
      }
    }
  }

  str[point] = '\0';

  free_clients(clients);
  return str;
};

extern "C" void printDevice(PulseAudio &pa, Device &d, const char *fmt) {
  const char *str = formatDevice(pa, d, fmt);
  printf("%s", str);
  free((void *)str);
}

} // namespace Pulsebind
