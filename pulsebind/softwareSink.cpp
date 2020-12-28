#include "softwareSink.hpp"
#include <cstring>

#include "callback.hpp"
#include "client.hpp"
#include "sink.hpp"

namespace Pulsebind {

extern "C" List getSinkInputs(PulseAudio &pa) {
  List l = newList(4);
  if (!l.array)
    return l;
  pa_operation *op =
      pa_context_get_sink_input_info_list(pa.context, &onSinkInputInfo, &l);

  iterate(pa, op);

  pa_operation_unref(op);

  return l;
}

extern "C" SinkInput *getSinkInputByName(List list, const char *name) {
  if (!list.array)
    return nullptr;

  for (size_t i = 0; i < list.size; i++) {
    SinkInput *hs = (SinkInput *)listGet(list, i);
    if (strcmp(hs->name, name) == 0) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" SinkInput *getSinkInputById(List list, uint32_t index) {
  if (!list.array)
    return nullptr;
  for (size_t i = 0; i < list.size; i++) {
    SinkInput *hs = (SinkInput *)listGet(list, i);
    if (hs->id == index) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" SinkInput *getSinkInputByDescription(List sinks, List clients,
                                                const char *desc) {
  for (size_t i = 0; i < sinks.size; i++) {
    SinkInput *hs = (SinkInput *)listGet(sinks, i);
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

extern "C" void sinkInputSetVolume(PulseAudio &pa, SinkInput *sink,
                                   uint32_t volume) {

  pa_volume_t new_volume = denormalizeVolume(volume);

  if (new_volume > PA_VOLUME_MAX) {
    new_volume = PA_VOLUME_MAX;
  }

  pa_cvolume *new_cvolume =
      pa_cvolume_set(&sink->volume, sink->volume.channels, new_volume);
  sinkInputSetCVolume(pa, sink, *new_cvolume);
}

extern "C" void sinkInputSetCVolume(PulseAudio &pa, SinkInput *sink,
                                    pa_cvolume cvol) {
  pa_operation *set = pa_context_set_sink_input_volume(
      pa.context, sink->id, &cvol, &onSuccess, nullptr);
  pa_operation *get = pa_context_get_sink_input_info(pa.context, sink->id,
                                                     &onUpdateSWSink, sink);
  iterate(pa, set);
  iterate(pa, get);
  pa_operation_unref(set);
  pa_operation_unref(get);
}

extern "C" void sinkInputSetMute(PulseAudio &pa, SinkInput *sink, bool mute) {
  pa_operation *op = pa_context_set_sink_input_mute(
      pa.context, sink->id, (int)mute, &onSuccess, nullptr);
  iterate(pa, op);
  pa_operation_unref(op);
}

extern "C" void sinkInputSetSink(PulseAudio &pa, SinkInput *sink,
                                 uint32_t hw_sink) {
  pa_operation *set = pa_context_move_sink_input_by_index(
      pa.context, sink->id, hw_sink, &onSuccess, nullptr);
  iterate(pa, set);
  sink->sink = hw_sink;
  pa_operation_unref(set);
}

extern "C" void freeSinkInput(SinkInput &sink) { free((void *)sink.name); }

extern "C" void freeSinkInputs(List &list) {
  if (!list.array)
    return;
  for (size_t i = 0; i < list.size; i++) {
    if (listGet(list, i)) {
      freeSinkInput(*((SinkInput *)listGet(list, i)));
    }
  }
  deleteList(list);
}

} // namespace Pulsebind
