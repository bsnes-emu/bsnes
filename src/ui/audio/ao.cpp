uint AudioAO::object_count = 0;

void AudioAO::run(uint32 sample) {
  ao_play(audio_device, (char *)&sample, 4); //This may need to be byte swapped for Big Endian
}

void AudioAO::set_frequency(uint freq) {
  Audio::set_frequency(freq);
  driver_format.rate = frequency;
  if(audio_device) {
    term();
    init();
  }
}

void AudioAO::init() {
  audio_device = ao_open_live(driver_id, &driver_format, 0);
  if(audio_device) {
    ao_info *di = ao_driver_info(driver_id);
  } else {
    dprintf("libao: failed to open audio device.\n");
  }
}

void AudioAO::term() {
  if(audio_device) {
    ao_close(audio_device);
    audio_device = 0;
  }
}

void AudioAO::set_driver(const char *name) {
  driver_id = ao_driver_id(name);
  if(driver_id < 0) { //If a driver by that name doesn't exist, we use the default driver for that OS
    dprintf("libao: %s is not a valid audio driver ... using default driver.\n", name);
    driver_id = ao_default_driver_id();
  }
  if(audio_device) {
    term();
    init();
  }
}

AudioAO::AudioAO() {
  if(!object_count++) {
    ao_initialize();
  }

  driver_id = ao_default_driver_id();
  driver_format.bits = 16;
  driver_format.channels = 2;
  driver_format.rate = frequency;
  driver_format.byte_format = AO_FMT_LITTLE;
  audio_device = 0;
}

AudioAO::~AudioAO() {
  if(audio_device) {
    ao_close(audio_device);
  }
  if(!--object_count) {
    ao_shutdown();
  }
}
