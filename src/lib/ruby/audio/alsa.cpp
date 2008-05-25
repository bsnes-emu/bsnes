#include <alsa/asoundlib.h>

#include <ruby/ruby.h>

namespace ruby {

#include "alsa.h"

class pAudioALSA {
public:
  AudioALSA &self;

  struct {
    snd_pcm_t *handle;
    snd_pcm_format_t format;
    int channels;
    const char *name;
    unsigned latency;
  } device;

  struct {
    uint16_t *data;
    unsigned length;
    unsigned size;
  } buffer;

  struct {
    unsigned frequency;
  } settings;

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

    buffer.data[buffer.length++] = left;
    buffer.data[buffer.length++] = right;
    if(buffer.length + 2 < buffer.size) return; //will crash in some cases if not stopped two before

    snd_pcm_sframes_t written = snd_pcm_writei(device.handle, buffer.data, buffer.length);
    if(written < 0) {
      snd_pcm_recover(device.handle, written, 1);
      //no samples written, drop one sample to prevent possible emulation stall
      buffer.length -= 2;
      memmove(buffer.data, buffer.data + 2, buffer.length * sizeof(uint16_t));
    } else if(written < buffer.length) {
      //only some samples written
      buffer.length -= written;
      memmove(buffer.data, buffer.data + written, buffer.length * sizeof(uint16_t));
    } else {
      //all samples written
      buffer.length = 0;
    }
  }

  bool init() {
    buffer.data = new uint16_t[buffer.size];

    if(snd_pcm_open(&device.handle, device.name, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK) < 0) {
      //failed to initialize
      term();
      return false;
    }

    if(snd_pcm_set_params(device.handle, device.format, SND_PCM_ACCESS_RW_INTERLEAVED,
      device.channels, settings.frequency, 1, device.latency) < 0) {
      //failed to set device parameters
      term();
      return false;
    }

    return true;
  }

  void term() {
    if(device.handle) {
      snd_pcm_drain(device.handle);
      snd_pcm_close(device.handle);
      device.handle = 0;
    }

    if(buffer.data) {
      delete[] buffer.data;
      buffer.data = 0;
	}
  }

  pAudioALSA(AudioALSA &self_) : self(self_) {
    device.handle = 0;
    device.format = SND_PCM_FORMAT_S16_LE;
    device.channels = 2;
    device.name = "default";
    device.latency = 90;

    buffer.data = 0;
    buffer.length = 0;
    buffer.size = device.latency * 32;

    settings.frequency = 22050;
  }

  ~pAudioALSA() {
    term();
  }
};

bool AudioALSA::cap(Setting setting) { return p.cap(setting); }
uintptr_t AudioALSA::get(Setting setting) { return p.get(setting); }
bool AudioALSA::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
void AudioALSA::sample(uint16_t left, uint16_t right) { p.sample(left, right); }
bool AudioALSA::init() { return p.init(); }
void AudioALSA::term() { p.term(); }
AudioALSA::AudioALSA() : p(*new pAudioALSA(*this)) {}
AudioALSA::~AudioALSA() { delete &p; }

} //namespace ruby
