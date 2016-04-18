#include <pulse/simple.h>
#include <pulse/error.h>

struct AudioPulseAudioSimple : Audio {
  ~AudioPulseAudioSimple() { term(); }

  struct {
    pa_simple* handle = nullptr;
    pa_sample_spec spec;
  } device;

  struct {
    uint32_t* data = nullptr;
    unsigned offset = 0;
  } buffer;

  struct {
    unsigned frequency = 48000;
  } settings;

  auto cap(const string& name) -> bool {
    if(name == Audio::Frequency) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Audio::Frequency) return settings.frequency;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Audio::Frequency && value.is<unsigned>()) {
      settings.frequency = value.get<unsigned>();
      if(device.handle) init();
      return true;
    }

    return false;
  }

  auto sample(int16_t left, int16_t right) -> void {
    if(!device.handle) return;

    buffer.data[buffer.offset++] = (uint16_t)left << 0 | (uint16_t)right << 16;
    if(buffer.offset >= 64) {
      int error;
      pa_simple_write(device.handle, (const void*)buffer.data, buffer.offset * sizeof(uint32_t), &error);
      buffer.offset = 0;
    }
  }

  auto clear() -> void {
  }

  auto init() -> bool {
    device.spec.format   = PA_SAMPLE_S16LE;
    device.spec.channels = 2;
    device.spec.rate     = settings.frequency;

    int error = 0;
    device.handle = pa_simple_new(
      0,                         //default server
      "ruby::pulseaudiosimple",  //application name
      PA_STREAM_PLAYBACK,        //direction
      0,                         //default device
      "audio",                   //stream description
      &device.spec,              //sample format
      0,                         //default channel map
      0,                         //default buffering attributes
      &error                     //error code
    );
    if(!device.handle) {
      fprintf(stderr, "ruby::pulseaudiosimple failed to initialize - %s\n", pa_strerror(error));
      return false;
    }

    buffer.data = new uint32_t[64];
    buffer.offset = 0;
    return true;
  }

  auto term() -> void {
    if(device.handle) {
      int error;
      pa_simple_flush(device.handle, &error);
      pa_simple_free(device.handle);
      device.handle = nullptr;
    }

    if(buffer.data) {
      delete[] buffer.data;
      buffer.data = nullptr;
    }
  }
};
