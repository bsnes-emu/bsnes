#include <pulse/simple.h>
#include <pulse/error.h>

namespace ruby {

#include "pulseaudio.hpp"

class pAudioPulseAudio {
public:
  struct {
    pa_simple *handle;
    pa_sample_spec spec;
  } device;

  struct {
    uint32_t *data;
    unsigned offset;
  } buffer;

  struct {
    unsigned frequency;
  } settings;

  AudioPulseAudio &self;

  bool cap(Audio::Setting setting) {
    if(setting == Audio::Frequency) return true;
    return false;
  }

  uintptr_t get(Audio::Setting setting) {
    if(setting == Audio::Frequency) return settings.frequency;
    return false;
  }

  bool set(Audio::Setting setting, uintptr_t param) {
    if(setting == Audio::Frequency) {
      settings.frequency = param;
      if(device.handle) {
        term();
        init();
      }
      return true;
    }

    return false;
  }

  void sample(uint16_t left, uint16_t right) {
    if(!device.handle) return;

    buffer.data[buffer.offset++] = left + (right << 16);
    if(buffer.offset >= 64) {
      int error;
      pa_simple_write(device.handle, (const void*)buffer.data, buffer.offset * sizeof(uint32_t), &error);
      buffer.offset = 0;
    }
  }

  bool init() {
    device.spec.format   = PA_SAMPLE_S16LE;
    device.spec.channels = 2;
    device.spec.rate     = settings.frequency;

    int error = 0;
    device.handle = pa_simple_new(
      0,                   //default server
      "ruby::pulseaudio",  //application name
      PA_STREAM_PLAYBACK,  //direction
      0,                   //default device
      "audio",             //stream description
      &device.spec,        //sample format
      0,                   //default channel map
      0,                   //default buffering attributes
      &error               //error code
    );
    if(!device.handle) {
      fprintf(stderr, "ruby::pulseaudio failed to initialize - %s\n", pa_strerror(error));
      return false;
    }

    buffer.data = new uint32_t[64];
    buffer.offset = 0;
    return true;
  }

  void term() {
    if(device.handle) {
      int error;
      pa_simple_flush(device.handle, &error);
      pa_simple_free(device.handle);
      device.handle = 0;
    }

    if(buffer.data) {
      delete[] buffer.data;
      buffer.data = 0;
    }
  }

  pAudioPulseAudio(AudioPulseAudio &self_) : self(self_) {
    device.handle = 0;
    buffer.data = 0;
    settings.frequency = 22050;
  }

  ~pAudioPulseAudio() {
    term();
  }
};

bool AudioPulseAudio::cap(Setting setting) { return p.cap(setting); }
uintptr_t AudioPulseAudio::get(Setting setting) { return p.get(setting); }
bool AudioPulseAudio::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
void AudioPulseAudio::sample(uint16_t left, uint16_t right) { return p.sample(left, right); }
bool AudioPulseAudio::init() { return p.init(); }
void AudioPulseAudio::term() { p.term(); }
AudioPulseAudio::AudioPulseAudio() : p(*new pAudioPulseAudio(*this)) {}
AudioPulseAudio::~AudioPulseAudio() { delete &p; }

} //namespace ruby
