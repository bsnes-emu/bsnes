auto MSU1::serialize(serializer& s) -> void {
  Thread::serialize(s);

  s.integer(mmio.dataSeekOffset);
  s.integer(mmio.dataReadOffset);

  s.integer(mmio.audioPlayOffset);
  s.integer(mmio.audioLoopOffset);

  s.integer(mmio.audioTrack);
  s.integer(mmio.audioVolume);

  s.integer(mmio.audioResumeTrack);
  s.integer(mmio.audioResumeOffset);

  s.integer(mmio.dataBusy);
  s.integer(mmio.audioBusy);
  s.integer(mmio.audioRepeat);
  s.integer(mmio.audioPlay);
  s.integer(mmio.audioError);

  dataOpen();
  audioOpen();
}
