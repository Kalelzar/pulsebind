#include "sink.hpp"
#include <cstring>

#include "callback.hpp"

namespace Pulsebind {

extern "C" List get_hardware_sinks(PulseAudio &pa) {
  List l = new_list(4);
  pa_operation *op = pa_context_get_sink_info_list(pa.context, &onSinkInfo, &l);

  iterate(pa, op);

  pa_operation_unref(op);

  return l;
}

extern "C" HardwareSink *get_hardware_sink_by_name(List list,
                                                   const char *name) {
  for (size_t i = 0; i < list.size; i++) {
    HardwareSink *hs = (HardwareSink *)list_get(list, i);
    if (strcmp(hs->name, name) == 0) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" HardwareSink *get_hardware_sink_by_id(List list, uint32_t index) {
  for (size_t i = 0; i < list.size; i++) {
    HardwareSink *hs = (HardwareSink *)list_get(list, i);
    if (hs->id == index) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" HardwareSink *get_hardware_sink_by_description(List list,
                                                          const char *desc) {
  for (size_t i = 0; i < list.size; i++) {
    HardwareSink *hs = (HardwareSink *)list_get(list, i);
    if (strcmp(hs->description, desc) == 0) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" void hardware_sink_set_volume(PulseAudio &pa, HardwareSink *sink,
                                         uint32_t volume) {

  pa_volume_t new_volume = denormalize_volume(volume);

  if (new_volume > PA_VOLUME_MAX) {
    new_volume = PA_VOLUME_MAX;
  }

  pa_cvolume *new_cvolume =
      pa_cvolume_set(&sink->volume, sink->volume.channels, new_volume);
  hardware_sink_set_cvolume(pa, sink, *new_cvolume);
}

extern "C" void hardware_sink_set_cvolume(PulseAudio &pa, HardwareSink *sink,
                                          pa_cvolume cvol) {
  pa_operation *set = pa_context_set_sink_volume_by_index(
      pa.context, sink->id, &cvol, &onSuccess, nullptr);
  pa_operation *get = pa_context_get_sink_info_by_index(pa.context, sink->id,
                                                     &onUpdateHWSink, sink);
  iterate(pa, set);
  iterate(pa, get);
  pa_operation_unref(set);
  pa_operation_unref(get);
}

extern "C" void hardware_sink_set_mute(PulseAudio &pa, HardwareSink *sink, bool mute) {
  pa_operation *op = pa_context_set_sink_mute_by_index(
      pa.context, sink->id, (int)mute, &onSuccess, nullptr);
  iterate(pa, op);
  pa_operation_unref(op);
}

extern "C" void free_hardware_sink(HardwareSink &sink) {
  free((void *)sink.name);
  free((void *)sink.description);
}

extern "C" void free_hardware_sinks(List &list) {
  for (size_t i = 0; i < list.size; i++) {
    if (list_get(list, i)) {
      free_hardware_sink(*((HardwareSink *)list_get(list, i)));
    }
  }
  delete_list(list);
}

extern "C" int normalize_volume(pa_volume_t t) {
  return (int)((double)t * 100.0 / PA_VOLUME_NORM);
}

extern "C" pa_volume_t denormalize_volume(int v) {
  return (pa_volume_t)((double)v * PA_VOLUME_NORM / 100.0);
}

} // namespace Pulsebind
