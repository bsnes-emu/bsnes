#include <ao/ao.h>

struct AudioAO : Audio {
  ~AudioAO() { term(); }

  int driver_id;
  ao_sample_format driver_format;
  ao_device* audio_device = nullptr;

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
      if(audio_device) init();
      return true;
    }

    return false;
  }

  auto sample(int16_t left, int16_t right) -> void {
    uint32_t samp = (uint16_t)left << 0 | (uint16_t)right << 0;
    ao_play(audio_device, (char*)&samp, 4); //This may need to be byte swapped for Big Endian
  }

  auto clear() -> void {
  }

  auto init() -> bool {
    ao_initialize();

    driver_id = ao_default_driver_id(); //ao_driver_id((const char*)driver)
    if(driver_id < 0) return false;

    driver_format.bits = 16;
    driver_format.channels = 2;
    driver_format.rate = settings.frequency;
    driver_format.byte_format = AO_FMT_LITTLE;

    ao_option* options = nullptr;
    ao_info *di = ao_driver_info(driver_id);
    if(!di) return false;
    if(!strcmp(di->short_name, "alsa")) {
      ao_append_option(&options, "buffer_time", "100000"); //100ms latency (default was 500ms)
    }

    audio_device = ao_open_live(driver_id, &driver_format, options);
    if(!audio_device) return false;

    return true;
  }

  auto term() -> void {
    if(audio_device) {
      ao_close(audio_device);
      audio_device = nullptr;
    }
    ao_shutdown();
  }
};
