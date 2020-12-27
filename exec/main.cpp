#include <iostream>
#include <pulsebind/client.hpp>
#include <pulsebind/device.hpp>
#include <pulsebind/pulseaudio.hpp>
#include <pulsebind/server.hpp>
#include <pulsebind/sink.hpp>
#include <pulsebind/softwareSink.hpp>
#include <pulsebind/softwareSource.hpp>
#include <pulsebind/source.hpp>

#include <cmath>

int main() {
  Pulsebind::PulseAudio pa = Pulsebind::new_pulse("pulsebind");
  if (pa.state == Pulsebind::ERROR)
    return 1;

  List hardware_sinks = Pulsebind::get_hardware_sinks(pa);

  for (size_t i = 0; i < hardware_sinks.size; i++) {
    Pulsebind::HardwareSink *sink =
        (Pulsebind::HardwareSink *)list_get(hardware_sinks, i);
    Pulsebind::Device d = {Pulsebind::DeviceType::HW_SINK, sink};
    Pulsebind::printDevice(
        pa, d,
        "\n~t: ID: ~i, Name: ~n, Desc: ~d, Mute: ~m, Mute(n): ~M, Volume: ~v%, "
        "CVolume: ~V, Channels: ~c, Client: ~C, Sink/Source: ~s\n");
  }

  Pulsebind::free_hardware_sinks(hardware_sinks);

  List software_sinks = Pulsebind::get_software_sinks(pa);

  for (size_t i = 0; i < software_sinks.size; i++) {
    Pulsebind::SoftwareSink *sink =
        (Pulsebind::SoftwareSink *)list_get(software_sinks, i);
    Pulsebind::Device d;
    d.type = Pulsebind::DeviceType::SW_SINK;
    d.device.sw_sink = sink;
    Pulsebind::printDevice(
        pa, d,
        "\n~t: ID: ~i, Name: ~n, Desc: ~d, Mute: ~m, Mute(n): ~M, Volume: ~v%, "
        "CVolume: ~V, Channels: ~c, Client: ~C, Sink/Source: ~s\n");
  }

  Pulsebind::free_software_sinks(software_sinks);

  List hardware_sources = Pulsebind::get_hardware_sources(pa);

  for (size_t i = 0; i < hardware_sources.size; i++) {
    Pulsebind::HardwareSource *source =
        (Pulsebind::HardwareSource *)list_get(hardware_sources, i);
    Pulsebind::Device d;
    d.type = Pulsebind::DeviceType::HW_SOURCE;
    d.device.hw_source = source;
    Pulsebind::printDevice(
        pa, d,
        "\n~t: ID: ~i, Name: ~n, Desc: ~d, Mute: ~m, Mute(n): ~M, Volume: ~v%, "
        "CVolume: ~V, Channels: ~c, Client: ~C, Sink/Source: ~s\n");
  }

  Pulsebind::free_hardware_sources(hardware_sources);

  List software_sources = Pulsebind::get_software_sources(pa);

  for (size_t i = 0; i < software_sources.size; i++) {
    Pulsebind::SoftwareSource *source =
        (Pulsebind::SoftwareSource *)list_get(software_sources, i);
    Pulsebind::Device d;
    d.type = Pulsebind::DeviceType::SW_SOURCE;
    d.device.sw_source = source;
    Pulsebind::printDevice(
        pa, d,
        "\n~t: ID: ~i, Name: ~n, Desc: ~d, Mute: ~m, Mute(n): ~M, Volume: ~v%, "
        "CVolume: ~V, Channels: ~c, Client: ~C, Sink/Source: ~s\n");
  }

  Pulsebind::free_software_sources(software_sources);

  Pulsebind::delete_pulse(pa);

  return 0;
}
