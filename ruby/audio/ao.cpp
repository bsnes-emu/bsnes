#include <ao/ao.h>

struct AudioAO : Audio {
  AudioAO() { initialize(); }
  ~AudioAO() { terminate(); }

  auto driver() -> string override { return "libao"; }
  auto ready() -> bool override { return _ready; }

  auto hasFrequencies() -> bool override { return true; }

  auto availableFrequencies() -> vector<double> override {
    return {44100.0, 48000.0, 96000.0};
  }

  auto setFrequency(double frequency) -> bool override {
    if(frequency == Audio::frequency()) return true;
    if(!Audio::setFrequency(frequency)) return false;
    return initialize();
  }

  auto output(const double samples[]) -> void override {
    uint32_t sample = 0;
    sample |= (uint16_t)sclamp<16>(samples[0] * 32767.0) <<  0;
    sample |= (uint16_t)sclamp<16>(samples[1] * 32767.0) << 16;
    ao_play(_interface, (char*)&sample, 4);
  }

private:
  auto initialize() -> bool {
    terminate();

    ao_initialize();

    int driverID = ao_default_driver_id();
    if(driverID < 0) return false;

    ao_sample_format format;
    format.bits = 16;
    format.channels = 2;
    format.rate = (uint)_frequency;
    format.byte_format = AO_FMT_LITTLE;
    format.matrix = nullptr;

    ao_info* information = ao_driver_info(driverID);
    if(!information) return false;
    string device = information->short_name;

    ao_option* options = nullptr;
    if(device == "alsa") {
      ao_append_option(&options, "buffer_time", "100000");  //100ms latency (default was 500ms)
    }

    _interface = ao_open_live(driverID, &format, options);
    if(!_interface) return false;

    return _ready = true;
  }

  auto terminate() -> void {
    _ready = false;
    if(_interface) {
      ao_close(_interface);
      _interface = nullptr;
    }
    ao_shutdown();
  }

  bool _ready = false;

  ao_device* _interface = nullptr;
};
