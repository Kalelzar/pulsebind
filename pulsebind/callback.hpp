#ifndef CALLBACK_H
#define CALLBACK_H

#include <pulse/pulseaudio.h>

namespace Pulsebind {

void onStateChanged(pa_context *context, void *data);
void onServerInfo(pa_context *context, const pa_server_info *info, void *data);
void onSinkInfo(pa_context *context, const pa_sink_info *info, int eol,
                void *data);
  void onUpdateHWSink(pa_context *context, const pa_sink_info *info, int eol,
                void *data);
void onSinkInputInfo(pa_context *context, const pa_sink_input_info *info,
                     int eol, void *data);
void onUpdateSWSink(pa_context *context, const pa_sink_input_info *info,
                     int eol, void *data);


void onSourceInfo(pa_context *context, const pa_source_info *info, int eol,
                  void *data);
  void onUpdateHWSource(pa_context *context, const pa_source_info *info, int eol,
                void *data);
void onSourceOutputInfo(pa_context *context, const pa_source_output_info *info,
                  int eol, void *data);

void onUpdateSWSource(pa_context *context, const pa_source_output_info *info,
                  int eol, void *data);

void onClientInfo(pa_context *context, const pa_client_info *info, int eol,
                  void *data);

void onSuccess(pa_context *context, int success, void *data);
} // namespace Pulsebind

#endif /* CALLBACK_H */
