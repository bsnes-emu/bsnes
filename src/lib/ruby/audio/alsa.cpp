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
    snd_pcm_uframes_t buffer_size;
    snd_pcm_uframes_t period_size;
    int channels;
    const char *name;
    unsigned latency;
  } device;

  struct {
    uint32_t *data;
    unsigned length;
  } buffer;

  struct {
    bool synchronize;
    unsigned frequency;
  } settings;

  bool cap(Audio::Setting setting) {
    if(setting == Audio::Synchronize) return true;
    if(setting == Audio::Frequency) return true;
    return false;
  }

  uintptr_t get(Audio::Setting setting) {
    if(setting == Audio::Synchronize) return settings.synchronize;
    if(setting == Audio::Frequency) return settings.frequency;
    return false;
  }

  bool set(Audio::Setting setting, uintptr_t param) {
    if(setting == Audio::Synchronize) {
      settings.synchronize = param;
      if(device.handle) {
        term();
        init();
      }
      return true;
    }
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

    buffer.data[buffer.length++] = left + (right << 16);
    if(buffer.length < device.period_size) return;

    if(settings.synchronize == false) {
      snd_pcm_avail_update(device.handle);
      snd_pcm_sframes_t delay;
      snd_pcm_delay(device.handle, &delay);
      if(delay < 0) {
        snd_pcm_prepare(device.handle);
      } else if(delay > device.buffer_size - device.period_size) {
        buffer.length = 0;
        return;
      }
    }

    uint32_t *buffer_ptr = buffer.data;
    int i = 4;

    while((buffer.length > 0) && i--) {
      snd_pcm_sframes_t written = snd_pcm_writei(device.handle, buffer_ptr, buffer.length);
      if(written < 0) {
        //no samples written
        snd_pcm_recover(device.handle, written, 1);
      } else if(written <= buffer.length) {
        buffer.length -= written;
        buffer_ptr += written;
      }
    }

  /*if(i < 0) {
      if(buffer.data == buffer_ptr) {
        buffer.length--;
        buffer_ptr++;
      }
      memmove(buffer.data, buffer_ptr, buffer.length * sizeof(uint32_t));
    }*/
  }

  bool init() {
    if(snd_pcm_open(&device.handle, device.name, SND_PCM_STREAM_PLAYBACK, 0) < 0) {
      term();
      return false;
    }

    /* //below code will not work with 24khz mode (ALSA library bug)
    if(snd_pcm_set_params(device.handle, device.format, SND_PCM_ACCESS_RW_INTERLEAVED,
      device.channels, settings.frequency, 1, device.latency) < 0) {
      //failed to set device parameters
      term();
      return false;
    }

    if(snd_pcm_get_params(device.handle, &device.buffer_size, &device.period_size) < 0) {
      device.period_size = device.latency * 1e-6 * settings.frequency / 4;
    }*/

    snd_pcm_hw_params_t *hwparams;
    snd_pcm_sw_params_t *swparams;
    unsigned rate = settings.frequency;
    unsigned buffer_time = device.latency;
    unsigned period_time = device.latency / 4;

    snd_pcm_hw_params_alloca(&hwparams);
    if(snd_pcm_hw_params_any(device.handle, hwparams) < 0) {
      term();
      return false;
    }

    if(snd_pcm_hw_params_set_access(device.handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0
    || snd_pcm_hw_params_set_format(device.handle, hwparams, device.format) < 0
    || snd_pcm_hw_params_set_channels(device.handle, hwparams, device.channels) < 0
    || snd_pcm_hw_params_set_rate_near(device.handle, hwparams, &rate, 0) < 0
    || snd_pcm_hw_params_set_period_time_near(device.handle, hwparams, &period_time, 0) < 0
    || snd_pcm_hw_params_set_buffer_time_near(device.handle, hwparams, &buffer_time, 0) < 0
    ) {
      term();
      return false;
    }

    if(snd_pcm_hw_params(device.handle, hwparams) < 0) {
      term();
      return false;
    }

    if(snd_pcm_get_params(device.handle, &device.buffer_size, &device.period_size) < 0) {
      term();
      return false;
    }

    snd_pcm_sw_params_alloca(&swparams);
    if(snd_pcm_sw_params_current(device.handle, swparams) < 0) {
      term();
      return false;
    }

    if(snd_pcm_sw_params_set_start_threshold(device.handle, swparams,
      (device.buffer_size / device.period_size) * device.period_size) < 0
    ) {
      term();
      return false;
    }

    if(snd_pcm_sw_params(device.handle, swparams) < 0) {
      term();
      return false;
    }

    buffer.data = new uint32_t[device.period_size];
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
    device.latency = 60000;

    buffer.data = 0;
    buffer.length = 0;

    settings.synchronize = false;
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
