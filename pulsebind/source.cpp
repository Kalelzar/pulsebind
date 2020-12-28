#include "source.hpp"
#include <cstring>

#include "callback.hpp"
#include "sink.hpp"

namespace Pulsebind {

extern "C" List getSources(PulseAudio &pa) {
  List l = newList(4);

  pa_operation *op =
      pa_context_get_source_info_list(pa.context, &onSourceInfo, &l);

  iterate(pa, op);

  pa_operation_unref(op);

  return l;
}

extern "C" Source *getSourceByName(List list, const char *name) {
  for (size_t i = 0; i < list.size; i++) {
    Source *hs = (Source *)listGet(list, i);
    if (strcmp(hs->name, name) == 0) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" Source *getSourceById(List list, uint32_t index) {
  for (size_t i = 0; i < list.size; i++) {
    Source *hs = (Source *)listGet(list, i);
    if (hs->id == index) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" Source *getSourceByDescription(List list, const char *desc) {
  for (size_t i = 0; i < list.size; i++) {
    Source *hs = (Source *)listGet(list, i);
    if (strcmp(hs->description, desc) == 0) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" void sourceSetVolume(PulseAudio &pa, Source *source,
                                uint32_t volume) {

  pa_volume_t newVolume = denormalizeVolume(volume);

  if (newVolume > PA_VOLUME_MAX) {
    newVolume = PA_VOLUME_MAX;
  }

  pa_cvolume *new_cvolume =
      pa_cvolume_set(&source->volume, source->volume.channels, newVolume);
  sourceSetCVolume(pa, source, *new_cvolume);
}

extern "C" void sourceSetCVolume(PulseAudio &pa, Source *source,
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

extern "C" void sourceSetMute(PulseAudio &pa, Source *source, bool mute) {
  pa_operation *op = pa_context_set_source_mute_by_index(
      pa.context, source->id, (int)mute, &onSuccess, nullptr);
  iterate(pa, op);
  pa_operation_unref(op);
}

extern "C" void freeSource(Source &source) {
  free((void *)source.name);
  free((void *)source.description);
}

extern "C" void freeSources(List &list) {
  for (size_t i = 0; i < list.size; i++) {
    if (listGet(list, i)) {
      freeSource(*((Source *)listGet(list, i)));
    }
  }
  deleteList(list);
}

} // namespace Pulsebind
