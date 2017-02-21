auto PSG::Tone::run() -> void {
  clock = 0;
  if(--counter) return;

  clock = 1;
  counter = pitch;
  output ^= 1;
}

auto PSG::Tone::power() -> void {
  volume = ~0;
  counter = 0;
  pitch = 0;
  clock = 0;
  output = 0;

  left = 1;
  right = 1;
}
