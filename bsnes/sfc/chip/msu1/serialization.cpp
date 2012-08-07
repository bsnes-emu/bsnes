#ifdef MSU1_CPP

void MSU1::serialize(serializer &s) {
  Thread::serialize(s);

  s.integer(boot);

  s.integer(mmio.data_offset);
  s.integer(mmio.audio_offset);
  s.integer(mmio.audio_loop_offset);

  s.integer(mmio.audio_track);
  s.integer(mmio.audio_volume);

  s.integer(mmio.data_busy);
  s.integer(mmio.audio_busy);
  s.integer(mmio.audio_repeat);
  s.integer(mmio.audio_play);
  s.integer(mmio.audio_error);

  data_open();
  audio_open();
}

#endif
