#include <pulse/simple.h>
#include <pulse/error.h>

struct AudioPulseAudioSimple : AudioDriver {
  AudioPulseAudioSimple& self = *this;
  AudioPulseAudioSimple(Audio& super) : AudioDriver(super) {}
  ~AudioPulseAudioSimple() { terminate(); }

  auto create() -> bool override {
    super.setBlocking(true);
    super.setFrequency(48000);
    return initialize();
  }

  auto driver() -> string override { return "PulseAudio Simple"; }
  auto ready() -> bool override { return _ready; }

  auto hasFrequencies() -> vector<uint> override {
    return {44100, 48000, 96000};
  }

  auto setFrequency(uint frequency) -> bool override { return initialize(); }

  auto output(const double samples[]) -> void override {
    if(!ready()) return;

    _buffer[_offset]  = (uint16_t)sclamp<16>(samples[0] * 32767.0) <<  0;
    _buffer[_offset] |= (uint16_t)sclamp<16>(samples[1] * 32767.0) << 16;
    if(++_offset >= 64) {
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
    specification.rate = self.frequency;

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

  pa_simple* _interface = nullptr;

  uint32_t* _buffer = nullptr;
  uint _offset = 0;
};
