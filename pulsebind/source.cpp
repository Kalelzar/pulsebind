#include "source.hpp"
#include <cstring>

#include "callback.hpp"
#include "sink.hpp"

namespace Pulsebind {

extern "C" List get_hardware_sources(PulseAudio &pa) {
  List l = new_list(4);

  pa_operation *op =
      pa_context_get_source_info_list(pa.context, &onSourceInfo, &l);

  iterate(pa, op);

  pa_operation_unref(op);

  return l;
}

extern "C" HardwareSource *get_hardware_source_by_name(List list,
                                                       const char *name) {
  for (size_t i = 0; i < list.size; i++) {
    HardwareSource *hs = (HardwareSource *)list_get(list, i);
    if (strcmp(hs->name, name) == 0) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" HardwareSource *get_hardware_source_by_id(List list,
                                                     uint32_t index) {
  for (size_t i = 0; i < list.size; i++) {
    HardwareSource *hs = (HardwareSource *)list_get(list, i);
    if (hs->id == index) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" HardwareSource *
get_hardware_source_by_description(List list, const char *desc) {
  for (size_t i = 0; i < list.size; i++) {
    HardwareSource *hs = (HardwareSource *)list_get(list, i);
    if (strcmp(hs->description, desc) == 0) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" void hardware_source_set_volume(PulseAudio &pa,
                                           HardwareSource *source,
                                           uint32_t volume) {

  pa_volume_t new_volume = denormalize_volume(volume);

  if (new_volume > PA_VOLUME_MAX) {
    new_volume = PA_VOLUME_MAX;
  }

  pa_cvolume *new_cvolume =
      pa_cvolume_set(&source->volume, source->volume.channels, new_volume);
  hardware_source_set_cvolume(pa, source, *new_cvolume);
}

extern "C" void hardware_source_set_cvolume(PulseAudio &pa,
                                            HardwareSource *source,
                                            pa_cvolume cvol) {
  pa_operation *set = pa_context_set_source_volume_by_index(
      pa.context, source->id, &cvol, &onSuccess, nullptr);
  pa_operation *get = pa_context_get_source_info_by_index(
      pa.context, source->id, &onUpdateHWSource, source);
  iterate(pa, set);
  iterate(pa, get);
  pa_operation_unref(set);
  pa_operation_unref(get);
}

extern "C" void hardware_source_set_mute(PulseAudio &pa, HardwareSource *source,
                                         bool mute) {
  pa_operation *op = pa_context_set_source_mute_by_index(
      pa.context, source->id, (int)mute, &onSuccess, nullptr);
  iterate(pa, op);
  pa_operation_unref(op);
}

extern "C" void free_hardware_source(HardwareSource &source) {
  free((void *)source.name);
  free((void *)source.description);
}

extern "C" void free_hardware_sources(List &list) {
  for (size_t i = 0; i < list.size; i++) {
    if (list_get(list, i)) {
      free_hardware_source(*((HardwareSource *)list_get(list, i)));
    }
  }
  delete_list(list);
}

} // namespace Pulsebind
