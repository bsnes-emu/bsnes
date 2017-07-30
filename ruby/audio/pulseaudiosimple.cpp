#include <pulse/simple.h>
#include <pulse/error.h>

struct AudioPulseAudioSimple : Audio {
  AudioPulseAudioSimple() { initialize(); }
  ~AudioPulseAudioSimple() { terminate(); }

  auto ready() -> bool { return _ready; }

  auto information() -> Information {
    Information information;
    information.devices = {"Default"};
    information.frequencies = {44100.0, 48000.0, 96000.0};
    information.latencies = {40};
    information.channels = {2};
    return information;
  }

  auto blocking() -> bool { return true; }
  auto channels() -> uint { return 2; }
  auto frequency() -> double { return _frequency; }
  auto latency() -> uint { return 40; }

  auto setFrequency(double frequency) -> bool {
    if(_frequency == frequency) return true;
    _frequency = frequency;
    return initialize();
  }

  auto output(const double samples[]) -> void {
    if(!ready()) return;

    _buffer[_offset++] = uint16_t(samples[0] * 32768.0) << 0 | uint16_t(samples[1] * 32768.0) << 16;
    if(_offset >= 64) {
      int error;
      pa_simple_write(_interface, (const void*)_buffer, _offset * sizeof(uint32_t), &error);
      _offset = 0;
    }
  }

private:
  auto initialize() -> bool {
    terminate();

    pa_sample_spec specification;
    specification.format = PA_SAMPLE_S16LE;
    specification.channels = 2;
    specification.rate = (uint)_frequency;

    int error = 0;
    _interface = pa_simple_new(
      0,                         //default server
      "ruby::pulseAudioSimple",  //application name
      PA_STREAM_PLAYBACK,        //direction
      0,                         //default device
      "audio",                   //stream description
      &specification,            //sample format
      0,                         //default channel map
      0,                         //default buffering attributes
      &error                     //error code
    );
    if(!_interface) return false;

    _buffer = new uint32_t[64]();
    _offset = 0;
    return _ready = true;
  }

  auto terminate() -> void {
    _ready = false;

    if(_interface) {
      int error;
      pa_simple_flush(_interface, &error);
      pa_simple_free(_interface);
      _interface = nullptr;
    }

    if(_buffer) {
      delete[] _buffer;
      _buffer = nullptr;
    }
  }

  bool _ready = false;
  double _frequency = 48000.0;

  pa_simple* _interface = nullptr;

  uint32_t* _buffer = nullptr;
  uint _offset = 0;
};
