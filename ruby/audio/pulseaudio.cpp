#include <pulse/pulseaudio.h>

struct AudioPulseAudio : Audio {
  ~AudioPulseAudio() { term(); }

  struct {
    pa_mainloop* mainloop = nullptr;
    pa_context* context = nullptr;
    pa_stream* stream = nullptr;
    pa_sample_spec spec;
    pa_buffer_attr buffer_attr;
    bool first;
  } device;

  struct {
    uint32_t* data = nullptr;
    size_t size;
    unsigned offset;
  } buffer;

  struct {
    bool synchronize = false;
    unsigned frequency = 48000;
    unsigned latency = 60;
  } settings;

  auto cap(const string& name) -> bool {
    if(name == Audio::Synchronize) return true;
    if(name == Audio::Frequency) return true;
    if(name == Audio::Latency) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Audio::Synchronize) return settings.synchronize;
    if(name == Audio::Frequency) return settings.frequency;
    if(name == Audio::Latency) return settings.latency;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Audio::Synchronize && value.is<bool>()) {
      settings.synchronize = value.get<bool>();
      return true;
    }

    if(name == Audio::Frequency && value.is<unsigned>()) {
      settings.frequency = value.get<unsigned>();
      if(device.stream) {
        pa_operation_unref(pa_stream_update_sample_rate(device.stream, settings.frequency, NULL, NULL));
      }
      return true;
    }

    if(name == Audio::Latency && value.is<unsigned>()) {
      settings.latency = value.get<unsigned>();
      if(device.stream) {
        device.buffer_attr.tlength = pa_usec_to_bytes(settings.latency * PA_USEC_PER_MSEC, &device.spec);
        pa_stream_set_buffer_attr(device.stream, &device.buffer_attr, NULL, NULL);
      }
      return true;
    }

    return false;
  }

  auto sample(uint16_t left, uint16_t right) -> void {
    pa_stream_begin_write(device.stream, (void**)&buffer.data, &buffer.size);
    buffer.data[buffer.offset++] = left + (right << 16);
    if((buffer.offset + 1) * pa_frame_size(&device.spec) <= buffer.size) return;

    while(true) {
      if(device.first) {
        device.first = false;
        pa_mainloop_iterate(device.mainloop, 0, NULL);
      } else {
        pa_mainloop_iterate(device.mainloop, 1, NULL);
      }
      unsigned length = pa_stream_writable_size(device.stream);
      if(length >= buffer.offset * pa_frame_size(&device.spec)) break;
      if(settings.synchronize == false) {
        buffer.offset = 0;
        return;
      }
    }

    pa_stream_write(device.stream, (const void*)buffer.data, buffer.offset * pa_frame_size(&device.spec), NULL, 0LL, PA_SEEK_RELATIVE);
    buffer.data = 0;
    buffer.offset = 0;
  }

  auto clear() -> void {
  }

  auto init() -> bool {
    device.mainloop = pa_mainloop_new();

    device.context = pa_context_new(pa_mainloop_get_api(device.mainloop), "ruby::pulseaudio");
    pa_context_connect(device.context, NULL, PA_CONTEXT_NOFLAGS, NULL);

    pa_context_state_t cstate;
    do {
      pa_mainloop_iterate(device.mainloop, 1, NULL);
      cstate = pa_context_get_state(device.context);
      if(!PA_CONTEXT_IS_GOOD(cstate)) return false;
    } while(cstate != PA_CONTEXT_READY);

    device.spec.format = PA_SAMPLE_S16LE;
    device.spec.channels = 2;
    device.spec.rate = settings.frequency;
    device.stream = pa_stream_new(device.context, "audio", &device.spec, NULL);

    device.buffer_attr.maxlength = -1;
    device.buffer_attr.tlength = pa_usec_to_bytes(settings.latency * PA_USEC_PER_MSEC, &device.spec);
    device.buffer_attr.prebuf = -1;
    device.buffer_attr.minreq = -1;
    device.buffer_attr.fragsize = -1;

    pa_stream_flags_t flags = (pa_stream_flags_t)(PA_STREAM_ADJUST_LATENCY | PA_STREAM_VARIABLE_RATE);
    pa_stream_connect_playback(device.stream, NULL, &device.buffer_attr, flags, NULL, NULL);

    pa_stream_state_t sstate;
    do {
      pa_mainloop_iterate(device.mainloop, 1, NULL);
      sstate = pa_stream_get_state(device.stream);
      if(!PA_STREAM_IS_GOOD(sstate)) return false;
    } while(sstate != PA_STREAM_READY);

    buffer.size = 960;
    buffer.offset = 0;
    device.first = true;

    return true;
  }

  auto term() -> void {
    if(buffer.data) {
      pa_stream_cancel_write(device.stream);
      buffer.data = nullptr;
    }

    if(device.stream) {
      pa_stream_disconnect(device.stream);
      pa_stream_unref(device.stream);
      device.stream = nullptr;
    }

    if(device.context) {
      pa_context_disconnect(device.context);
      pa_context_unref(device.context);
      device.context = nullptr;
    }

    if(device.mainloop) {
      pa_mainloop_free(device.mainloop);
      device.mainloop = nullptr;
    }
  }
};
