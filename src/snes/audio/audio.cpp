#ifdef SNES_CPP

void SNES::Audio::update(uint16 l_sample, uint16 r_sample) {
  snesinterface.audio_sample(l_sample, r_sample);
}

void SNES::Audio::init() {
}

#endif  //ifdef SNES_CPP
