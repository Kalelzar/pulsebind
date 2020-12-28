#include "softwareSource.hpp"
#include <cstring>

#include "callback.hpp"
#include "client.hpp"
#include "sink.hpp"

namespace Pulsebind {

extern "C" List getSourceOutputs(PulseAudio &pa) {
  List l = newList(4);

  pa_operation *op = pa_context_get_source_output_info_list(
      pa.context, &onSourceOutputInfo, &l);

  iterate(pa, op);

  pa_operation_unref(op);

  return l;
}

extern "C" SourceOutput *getSourceOutputByName(List list, const char *name) {
  for (size_t i = 0; i < list.size; i++) {
    SourceOutput *hs = (SourceOutput *)listGet(list, i);
    if (strcmp(hs->name, name) == 0) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" SourceOutput *getSourceOutputById(List list, uint32_t index) {
  for (size_t i = 0; i < list.size; i++) {
    SourceOutput *hs = (SourceOutput *)listGet(list, i);
    if (hs->id == index) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" SourceOutput *
getSourceOutputByDescription(List sources, List clients, const char *desc) {
  for (size_t i = 0; i < sources.size; i++) {
    SourceOutput *hs = (SourceOutput *)listGet(sources, i);
    if (!hs)
      continue;
    Client *client = (Client *)getClientById(clients, hs->client);
    if (!client)
      continue;
    if (strcmp(client->name, desc) == 0) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" void sourceOutputSetVolume(PulseAudio &pa, SourceOutput *source,
                                      uint32_t volume) {

  pa_volume_t new_volume = denormalizeVolume(volume);

  if (new_volume > PA_VOLUME_MAX) {
    new_volume = PA_VOLUME_MAX;
  }

  pa_cvolume *new_cvolume =
      pa_cvolume_set(&source->volume, source->volume.channels, new_volume);
  sourceOutputSetCVolume(pa, source, *new_cvolume);
}

extern "C" void sourceOutputSetCVolume(PulseAudio &pa, SourceOutput *source,
                                       pa_cvolume cvol) {
  pa_operation *set = pa_context_set_source_output_volume(
      pa.context, source->id, &cvol, &onSuccess, nullptr);
  pa_operation *get = pa_context_get_source_output_info(
      pa.context, source->id, &onUpdateSWSource, source);
  iterate(pa, set);
  iterate(pa, get);
  pa_operation_unref(set);
  pa_operation_unref(get);
}

extern "C" void sourceOutputSetMute(PulseAudio &pa, SourceOutput *source,
                                    bool mute) {
  pa_operation *op = pa_context_set_source_output_mute(
      pa.context, source->id, (int)mute, &onSuccess, nullptr);
  iterate(pa, op);
  pa_operation_unref(op);
}

extern "C" void sourceOutputSetSource(PulseAudio &pa, SourceOutput *source,
                                      uint32_t hw_source) {
  pa_operation *set = pa_context_move_source_output_by_index(
      pa.context, source->id, hw_source, &onSuccess, nullptr);
  iterate(pa, set);
  source->source = hw_source;
  pa_operation_unref(set);
}

extern "C" void freeSourceOutput(SourceOutput &source) {
  free((void *)source.name);
}

extern "C" void freeSourceOutputs(List &list) {
  for (size_t i = 0; i < list.size; i++) {
    if (listGet(list, i)) {
      freeSourceOutput(*((SourceOutput *)listGet(list, i)));
    }
  }
  deleteList(list);
}

} // namespace Pulsebind
