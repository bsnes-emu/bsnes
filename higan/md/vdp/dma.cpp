auto VDP::dmaRun() -> void {
  if(!io.dmaEnable) return;
  if(io.command.bits(4,5) != 2) return;

  if(io.dmaMode <= 1) return dmaLoad();
  if(io.dmaMode == 2) return dmaFill();
  if(io.dmaMode == 3) return dmaCopy();
}

auto VDP::dmaLoad() -> void {
  cpu.wait |= Wait::VDP_DMA;

  auto data = cpu.readWord(io.dmaMode.bit(0) << 23 | io.dmaSource << 1);
  writeDataPort(data);

  io.dmaSource.bits(0,15)++;
  if(--io.dmaLength == 0) {
    io.command.bit(5) = 0;
    cpu.wait &=~ Wait::VDP_DMA;
  }
}

auto VDP::dmaFill() -> void {
  if(io.dmaFillWait) return;

  auto data = io.dmaFillByte;
  writeDataPort(data << 8 | data << 0);

  io.dmaSource.bits(0,15)++;
  if(--io.dmaLength == 0) {
    io.command.bit(5) = 0;
  }
}

auto VDP::dmaCopy() -> void {
}
