auto VDP::dmaRun() -> void {
  if(!io.dmaEnable) return;
  if(!io.dmaActive) return;

  if(io.dmaMode == 2) return dmaFillVRAM();
}

auto VDP::dmaFillVRAM() -> void {
  auto address = io.address.bits(1,15);
  auto data = io.dmaFillWord;
  if(io.address.bit(0)) data = data >> 8 | data << 8;
  vram[address] = data;
  io.address += io.dataIncrement;

  if(--io.dmaLength == 0) {
    io.dmaActive = false;
  }
}
