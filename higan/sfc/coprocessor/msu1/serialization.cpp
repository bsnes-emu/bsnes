auto MSU1::serialize(serializer& s) -> void {
  Thread::serialize(s);

  s.integer(io.dataSeekOffset);
  s.integer(io.dataReadOffset);

  s.integer(io.audioPlayOffset);
  s.integer(io.audioLoopOffset);

  s.integer(io.audioTrack);
  s.integer(io.audioVolume);

  s.integer(io.audioResumeTrack);
  s.integer(io.audioResumeOffset);

  s.integer(io.audioError);
  s.integer(io.audioPlay);
  s.integer(io.audioRepeat);
  s.integer(io.audioBusy);
  s.integer(io.dataBusy);

  dataOpen();
  audioOpen();
}
