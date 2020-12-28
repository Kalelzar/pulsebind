#include "callback.hpp"
#include "client.hpp"
#include "list.hpp"
#include "pulseaudio.hpp"
#include "server.hpp"
#include "sink.hpp"
#include "softwareSink.hpp"
#include "softwareSource.hpp"
#include "source.hpp"

#include <cmath>
#include <cstring>

namespace Pulsebind {

void onStateChanged(pa_context *context, void *data) {
  PulseAudio *pa = (PulseAudio *)data;
  switch (pa_context_get_state(context)) {
  case PA_CONTEXT_READY:
    pa->state = CONNECTED;
    break;
  case PA_CONTEXT_FAILED:
    pa->state = ERROR;
    break;
  default:
    break;
  }
}

inline char *copyCString(const char *cstring) noexcept {
  size_t size = strlen(cstring);
  char *cstr = (char *)calloc(size + 1, sizeof(char));
  if (!cstr)
    return nullptr;
  strncpy(cstr, cstring, size);
  cstr[size] = '\0';
  return cstr;
}

void onServerInfo(pa_context *context, const pa_server_info *info, void *data) {
  Server *s = (Server *)data;
  s->hostName = copyCString(info->host_name);
  s->userName = copyCString(info->user_name);
  s->serverName = copyCString(info->server_name);
  s->serverVersion = copyCString(info->server_version);
  s->cookie = info->cookie;
  s->defaultSinkName = copyCString(info->default_sink_name);
  s->defaultSourceName = copyCString(info->default_source_name);
}

void onSinkInfo(pa_context *context, const pa_sink_info *info, int eol,
                void *data) {

  if (eol != 0)
    return;

  Sink *s = (Sink *)malloc(sizeof(Sink));

  if (!s)
    return;

  List list = *((List *)data);

  s->name = copyCString(info->name);
  s->description = copyCString(info->description);
  s->id = info->index;
  s->mute = info->mute == 1;
  s->baseVolume = info->base_volume;
  s->volume = info->volume;
  s->volumeAvg = pa_cvolume_avg(&(s->volume));
  s->volumePercent = normalizeVolume(s->volumeAvg);

  listAdd(list, (void *)s);
  *((List *)data) = list;
}

void onUpdateHWSink(pa_context *context, const pa_sink_info *info, int eol,
                    void *data) {

  if (eol != 0)
    return;

  Sink *sws = (Sink *)data;
  if (sws->id != info->index)
    return;
  sws->mute = info->mute;
  sws->baseVolume = info->base_volume;
  sws->volume = info->volume;
  sws->volumeAvg = pa_cvolume_avg(&(info->volume));
  sws->volumePercent = normalizeVolume(sws->volumeAvg);
}

void onUpdateHWSource(pa_context *context, const pa_source_info *info, int eol,
                      void *data) {

  if (eol != 0)
    return;

  Source *sws = (Source *)data;
  if (sws->id != info->index)
    return;
  sws->mute = info->mute;
  sws->baseVolume = info->base_volume;
  sws->volume = info->volume;
  sws->volumeAvg = pa_cvolume_avg(&(info->volume));
  sws->volumePercent = normalizeVolume(sws->volumeAvg);
}

void onSinkInputInfo(pa_context *context, const pa_sink_input_info *info,
                     int eol, void *data) {

  if (eol != 0)
    return;

  SinkInput *s = (SinkInput *)malloc(sizeof(SinkInput));

  if (!s)
    return;

  List list = *((List *)data);

  s->name = copyCString(info->name);
  s->client = info->client;
  s->sink = info->sink;
  s->id = info->index;
  s->mute = info->mute == 1;
  s->volume = info->volume;
  s->volumeAvg = pa_cvolume_avg(&(s->volume));
  s->volumePercent = normalizeVolume(s->volumeAvg);

  listAdd(list, (void *)s);
  *((List *)data) = list;
}

void onSourceInfo(pa_context *context, const pa_source_info *info, int eol,
                  void *data) {

  if (eol != 0)
    return;

  Source *s = (Source *)malloc(sizeof(Source));

  if (!s)
    return;

  List list = *((List *)data);

  s->name = copyCString(info->name);
  s->description = copyCString(info->description);
  s->id = info->index;
  s->mute = info->mute == 1;
  s->baseVolume = info->base_volume;
  s->volume = info->volume;
  s->volumeAvg = pa_cvolume_avg(&(s->volume));
  s->volumePercent = normalizeVolume(s->volumeAvg);

  listAdd(list, (void *)s);
  *((List *)data) = list;
}

void onSourceOutputInfo(pa_context *context, const pa_source_output_info *info,
                        int eol, void *data) {

  if (eol != 0)
    return;

  SourceOutput *s = (SourceOutput *)malloc(sizeof(SourceOutput));

  if (!s)
    return;

  List list = *((List *)data);

  s->name = copyCString(info->name);
  s->client = info->client;
  s->source = info->source;
  s->id = info->index;
  s->mute = info->mute == 1;
  s->volume = info->volume;
  s->volumeAvg = pa_cvolume_avg(&(s->volume));
  s->volumePercent = normalizeVolume(s->volumeAvg);

  listAdd(list, (void *)s);
  *((List *)data) = list;
}

void onUpdateSWSource(pa_context *context, const pa_source_output_info *info,
                      int eol, void *data) {

  if (eol != 0)
    return;

  SourceOutput *sws = (SourceOutput *)data;
  if (sws->id != info->index)
    return;
  sws->mute = info->mute;
  sws->client = info->index;
  sws->source = info->source;
  sws->volume = info->volume;
  sws->volumeAvg = pa_cvolume_avg(&(info->volume));
  sws->volumePercent = normalizeVolume(sws->volumeAvg);
}

void onUpdateSWSink(pa_context *context, const pa_sink_input_info *info,
                    int eol, void *data) {

  if (eol != 0)
    return;

  SinkInput *sws = (SinkInput *)data;
  if (sws->id != info->index)
    return;
  sws->mute = info->mute;
  sws->client = info->index;
  sws->sink = info->sink;
  sws->volume = info->volume;
  sws->volumeAvg = pa_cvolume_avg(&(info->volume));
  sws->volumePercent = normalizeVolume(sws->volumeAvg);
}

void onClientInfo(pa_context *context, const pa_client_info *info, int eol,
                  void *data) {

  if (eol != 0)
    return;

  Client *s = (Client *)malloc(sizeof(Client));

  if (!s)
    return;

  List list = *((List *)data);

  s->name = copyCString(info->name);
  s->id = info->index;

  listAdd(list, (void *)s);
  *((List *)data) = list;
}

void onSuccess(pa_context *context, int success, void *data) {}

} // namespace Pulsebind
