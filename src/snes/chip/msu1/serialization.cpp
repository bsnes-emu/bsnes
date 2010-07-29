#ifdef MSU1_CPP

void MSU1::serialize(serializer &s) {
  Processor::serialize(s);

  s.integer(mmio.data_offset);
  s.integer(mmio.audio_offset);
  s.integer(mmio.audio_track);
  s.integer(mmio.audio_volume);
  s.integer(mmio.data_busy);
  s.integer(mmio.audio_busy);
  s.integer(mmio.audio_repeat);
  s.integer(mmio.audio_play);

  if(datafile.open()) datafile.close();
  if(datafile.open(string() << cartridge.basename() << ".msu", file::mode_read)) {
    datafile.seek(mmio.data_offset);
  }

  if(audiofile.open()) audiofile.close();
  char track[16];
  sprintf(track, "-%u", mmio.audio_track);
  if(audiofile.open(string() << cartridge.basename() << track << ".wav", file::mode_read)) {
    audiofile.seek(mmio.audio_offset);
  }
}

#endif
