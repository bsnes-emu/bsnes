auto Program::rewindMode(Rewind::Mode mode) -> void {
  rewind.mode = mode;
  rewind.counter = 0;
}

auto Program::rewindReset() -> void {
  rewindMode(Rewind::Mode::Playing);
  rewind.history.reset();
  rewind.frequency = settings.emulator.rewind.frequency;
  rewind.length = settings.emulator.rewind.length;
}

auto Program::rewindRun() -> void {
  if(rewind.frequency == 0) return;  //rewind disabled?

  if(rewind.mode == Rewind::Mode::Rewinding) {
    if(rewind.history.size() == 0) return rewindMode(Rewind::Mode::Playing);  //nothing left to rewind?
    if(++rewind.counter < rewind.frequency / 4) return;

    rewind.counter = 0;
    auto t = rewind.history.takeLast();
    serializer s{t.data(), t.size()};  //convert serializer::Save to serializer::Load
    if(!rewind.history) {
      showMessage("Rewind history exhausted");
      rewindReset();
    }
    emulator->unserialize(s);
    return;
  }

  if(rewind.mode == Rewind::Mode::Playing) {
    if(++rewind.counter < rewind.frequency) return;

    rewind.counter = 0;
    if(rewind.history.size() >= rewind.length) {
      rewind.history.takeFirst();
    }
    auto s = emulator->serialize();
    rewind.history.append(s);
    return;
  }
}
