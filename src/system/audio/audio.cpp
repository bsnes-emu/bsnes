#ifdef SYSTEM_CPP

Audio audio;

void Audio::sample(uint16 l_sample, uint16 r_sample) {
  system.interface->audio_sample(l_sample, r_sample);
}

void Audio::init() {
}

#endif
