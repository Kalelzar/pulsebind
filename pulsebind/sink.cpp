#include "sink.hpp"
#include <cstring>

#include "callback.hpp"

namespace Pulsebind {

extern "C" List getSinks(PulseAudio &pa) {
  List l = newList(4);
  pa_operation *op = pa_context_get_sink_info_list(pa.context, &onSinkInfo, &l);

  iterate(pa, op);

  pa_operation_unref(op);

  return l;
}

extern "C" Sink *getSinkByName(List list, const char *name) {
  for (size_t i = 0; i < list.size; i++) {
    Sink *hs = (Sink *)listGet(list, i);
    if (strcmp(hs->name, name) == 0) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" Sink *getSinkById(List list, uint32_t index) {
  for (size_t i = 0; i < list.size; i++) {
    Sink *hs = (Sink *)listGet(list, i);
    if (hs->id == index) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" Sink *getSinkByDescription(List list, const char *desc) {
  for (size_t i = 0; i < list.size; i++) {
    Sink *hs = (Sink *)listGet(list, i);
    if (strcmp(hs->description, desc) == 0) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" void sinkSetVolume(PulseAudio &pa, Sink *sink, uint32_t volume) {

  pa_volume_t new_volume = denormalizeVolume(volume);

  if (new_volume > PA_VOLUME_MAX) {
    new_volume = PA_VOLUME_MAX;
  }

  pa_cvolume *new_cvolume =
      pa_cvolume_set(&sink->volume, sink->volume.channels, new_volume);
  sinkSetCVolume(pa, sink, *new_cvolume);
}

extern "C" void sinkSetCVolume(PulseAudio &pa, Sink *sink, pa_cvolume cvol) {
  pa_operation *set = pa_context_set_sink_volume_by_index(
      pa.context, sink->id, &cvol, &onSuccess, nullptr);
  pa_operation *get = pa_context_get_sink_info_by_index(pa.context, sink->id,
                                                        &onUpdateHWSink, sink);
  iterate(pa, set);
  iterate(pa, get);
  pa_operation_unref(set);
  pa_operation_unref(get);
}

extern "C" void sinkSetMute(PulseAudio &pa, Sink *sink, bool mute) {
  pa_operation *op = pa_context_set_sink_mute_by_index(
      pa.context, sink->id, (int)mute, &onSuccess, nullptr);
  iterate(pa, op);
  pa_operation_unref(op);
}

extern "C" void freeSink(Sink &sink) {
  free((void *)sink.name);
  free((void *)sink.description);
}

extern "C" void freeSinks(List &list) {
  for (size_t i = 0; i < list.size; i++) {
    if (listGet(list, i)) {
      freeSink(*((Sink *)listGet(list, i)));
    }
  }
  deleteList(list);
}

extern "C" int normalizeVolume(pa_volume_t t) {
  return (int)((double)t * 100.0 / PA_VOLUME_NORM);
}

extern "C" pa_volume_t denormalizeVolume(int v) {
  return (pa_volume_t)((double)v * PA_VOLUME_NORM / 100.0);
}

} // namespace Pulsebind
