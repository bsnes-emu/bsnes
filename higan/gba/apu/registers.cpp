APU::Registers::SoundBias::operator uint16_t() const {
  return (
    (level     <<  0)
  | (amplitude << 14)
  );
}

auto APU::Registers::SoundBias::operator=(uint16 source) -> uint16 {
  level     = (source >>  0) & 1023;
  amplitude = (source >> 14) &    3;
  return operator uint16_t();
}
