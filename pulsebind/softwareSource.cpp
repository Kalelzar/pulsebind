#include "softwareSource.hpp"
#include <cstring>

#include "callback.hpp"
#include "client.hpp"
#include "sink.hpp"

namespace Pulsebind {

extern "C" List get_software_sources(PulseAudio &pa) {
  List l = new_list(4);

  pa_operation *op = pa_context_get_source_output_info_list(
      pa.context, &onSourceOutputInfo, &l);

  iterate(pa, op);

  pa_operation_unref(op);

  return l;
}

extern "C" SoftwareSource *get_software_source_by_name(List list,
                                                       const char *name) {
  for (size_t i = 0; i < list.size; i++) {
    SoftwareSource *hs = (SoftwareSource *)list_get(list, i);
    if (strcmp(hs->name, name) == 0) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" SoftwareSource *get_software_source_by_id(List list,
                                                     uint32_t index) {
  for (size_t i = 0; i < list.size; i++) {
    SoftwareSource *hs = (SoftwareSource *)list_get(list, i);
    if (hs->id == index) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" SoftwareSource *
get_software_source_by_description(List sources, List clients,
                                   const char *desc) {
  for (size_t i = 0; i < sources.size; i++) {
    SoftwareSource *hs = (SoftwareSource *)list_get(sources, i);
    if (!hs)
      continue;
    Client *client = (Client *)get_client_by_id(clients, hs->client);
    if (!client)
      continue;
    if (strcmp(client->name, desc) == 0) {
      return hs;
    }
  }

  return nullptr;
}

extern "C" void software_source_set_volume(PulseAudio &pa,
                                           SoftwareSource *source,
                                           uint32_t volume) {

  pa_volume_t new_volume = denormalize_volume(volume);

  if (new_volume > PA_VOLUME_MAX) {
    new_volume = PA_VOLUME_MAX;
  }

  pa_cvolume *new_cvolume =
      pa_cvolume_set(&source->volume, source->volume.channels, new_volume);
  software_source_set_cvolume(pa, source, *new_cvolume);
}

extern "C" void software_source_set_cvolume(PulseAudio &pa,
                                            SoftwareSource *source,
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

extern "C" void software_source_set_mute(PulseAudio &pa, SoftwareSource *source,
                                         bool mute) {
  pa_operation *op = pa_context_set_source_output_mute(
      pa.context, source->id, (int)mute, &onSuccess, nullptr);
  iterate(pa, op);
  pa_operation_unref(op);
}

extern "C" void software_source_set_hw_source(PulseAudio &pa,
                                              SoftwareSource *source,
                                              uint32_t hw_source) {
  pa_operation *set = pa_context_move_source_output_by_index(
      pa.context, source->id, hw_source, &onSuccess, nullptr);
  iterate(pa, set);
  source->source = hw_source;
  pa_operation_unref(set);
}

extern "C" void free_software_source(SoftwareSource &source) {
  free((void *)source.name);
}

extern "C" void free_software_sources(List &list) {
  for (size_t i = 0; i < list.size; i++) {
    if (list_get(list, i)) {
      free_software_source(*((SoftwareSource *)list_get(list, i)));
    }
  }
  delete_list(list);
}

} // namespace Pulsebind
