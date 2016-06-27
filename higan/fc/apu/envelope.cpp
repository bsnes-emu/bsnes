auto APU::Envelope::volume() const -> uint {
  return useSpeedAsVolume ? speed : decayVolume;
}

auto APU::Envelope::clock() -> void {
  if(reloadDecay) {
    reloadDecay = false;
    decayVolume = 0x0f;
    decayCounter = speed + 1;
    return;
  }

  if(--decayCounter == 0) {
    decayCounter = speed + 1;
    if(decayVolume || loopMode) decayVolume--;
  }
}

auto APU::Envelope::power() -> void {
}

auto APU::Envelope::reset() -> void {
  speed = 0;
  useSpeedAsVolume = 0;
  loopMode = 0;
  reloadDecay = 0;
  decayCounter = 0;
  decayVolume = 0;
}
