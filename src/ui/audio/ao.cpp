#include "ao.h"

uint AudioAO::object_count = 0;

void AudioAO::sample(uint16 l_sample, uint16 r_sample) {
uint32 samp = (l_sample << 0) + (r_sample << 16);
  ao_play(audio_device, (char*)&samp, 4); //This may need to be byte swapped for Big Endian
}

void AudioAO::update_frequency() {
  Audio::update_frequency();
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

AudioAO::AudioAO(const char *driver) {
  if(!object_count++) {
    ao_initialize();
  }

  driver_id = (driver && *driver) ? ao_driver_id(driver) : ao_default_driver_id();
  if(driver_id < 0) { driver_id = ao_default_driver_id(); } //fallback on default if driver doesn't exist
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
