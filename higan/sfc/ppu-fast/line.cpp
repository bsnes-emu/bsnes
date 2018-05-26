auto PPU::Line::render() -> void {
  for(uint x : range(512)) {
    outputLo[x] = 0x7ffff;
    outputHi[x] = 0x7ffff;
  }
}
