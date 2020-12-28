#include <iostream>
#include <pulsebind/softwareSink.hpp>
#include <pulsebind/softwareSource.hpp>
#include <pulsebind/client.hpp>
#include <pulsebind/device.hpp>
#include <pulsebind/pulseaudio.hpp>
#include <pulsebind/server.hpp>
#include <pulsebind/sink.hpp>
#include <pulsebind/source.hpp>

#include <cmath>

int main() {
  Pulsebind::PulseAudio pa = Pulsebind::newPulse("pulsebind");
  if (pa.state == Pulsebind::ERROR)
    return 1;

  List sinks = Pulsebind::getSinks(pa);

  for (size_t i = 0; i < sinks.size; i++) {
    Pulsebind::Sink *sink = (Pulsebind::Sink *)listGet(sinks, i);
    Pulsebind::Device d = {Pulsebind::DeviceType::SINK, sink};
    Pulsebind::printDevice(
        pa, d,
        "\n~t: ID: ~i, Name: ~n, Desc: ~d, Mute: ~m, Mute(n): ~M, Volume: ~v%, "
        "CVolume: ~V, Channels: ~c, Client: ~C, Sink/Source: ~s\n");
  }

  Pulsebind::freeSinks(sinks);

  List sinkInputs = Pulsebind::getSinkInputs(pa);

  for (size_t i = 0; i < sinkInputs.size; i++) {
    Pulsebind::SinkInput *sinkInput =
        (Pulsebind::SinkInput *)listGet(sinkInputs, i);
    Pulsebind::Device d;
    d.type = Pulsebind::DeviceType::SINK_INPUT;
    d.device.sinkInput = sinkInput;
    Pulsebind::printDevice(
        pa, d,
        "\n~t: ID: ~i, Name: ~n, Desc: ~d, Mute: ~m, Mute(n): ~M, Volume: ~v%, "
        "CVolume: ~V, Channels: ~c, Client: ~C, Sink/Source: ~s\n");
  }

  Pulsebind::freeSinkInputs(sinkInputs);

  List sources = Pulsebind::getSources(pa);

  for (size_t i = 0; i < sources.size; i++) {
    Pulsebind::Source *source = (Pulsebind::Source *)listGet(sources, i);
    Pulsebind::Device d;
    d.type = Pulsebind::DeviceType::SOURCE;
    d.device.source = source;
    Pulsebind::printDevice(
        pa, d,
        "\n~t: ID: ~i, Name: ~n, Desc: ~d, Mute: ~m, Mute(n): ~M, Volume: ~v%, "
        "CVolume: ~V, Channels: ~c, Client: ~C, Sink/Source: ~s\n");
  }

  Pulsebind::freeSources(sources);

  List sourceOutputs = Pulsebind::getSourceOutputs(pa);

  for (size_t i = 0; i < sourceOutputs.size; i++) {
    Pulsebind::SourceOutput *sourceOutput =
        (Pulsebind::SourceOutput *)listGet(sourceOutputs, i);
    Pulsebind::Device d;
    d.type = Pulsebind::DeviceType::SOURCE_OUTPUT;
    d.device.sourceOutput = sourceOutput;
    Pulsebind::printDevice(
        pa, d,
        "\n~t: ID: ~i, Name: ~n, Desc: ~d, Mute: ~m, Mute(n): ~M, Volume: ~v%, "
        "CVolume: ~V, Channels: ~c, Client: ~C, Sink/Source: ~s\n");
  }

  Pulsebind::freeSourceOutputs(sourceOutputs);

  Pulsebind::deletePulse(pa);

  return 0;
}
