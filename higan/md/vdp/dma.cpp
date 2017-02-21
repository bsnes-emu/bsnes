auto VDP::DMA::run() -> void {
  if(!io.enable || io.wait) return;
  if(!vdp.io.command.bit(5)) return;

  if(io.mode <= 1) return load();
  if(io.mode == 2) return fill();
  if(io.mode == 3) return copy();
}

auto VDP::DMA::load() -> void {
  cpu.wait |= Wait::VDP_DMA;

  auto data = busCPU.readWord(io.mode.bit(0) << 23 | io.source << 1);
  vdp.writeDataPort(data);

  io.source.bits(0,15)++;
  if(--io.length == 0) {
    vdp.io.command.bit(5) = 0;
    cpu.wait &=~ Wait::VDP_DMA;
  }
}

auto VDP::DMA::fill() -> void {
  auto data = io.fill;
  vdp.writeDataPort(data << 8 | data << 0);

  io.source.bits(0,15)++;
  if(--io.length == 0) {
    vdp.io.command.bit(5) = 0;
  }
}

auto VDP::DMA::copy() -> void {
  auto data = vdp.vram[io.source.bits(0,14)];
  vdp.writeDataPort(data);

  io.source.bits(0,15)++;
  if(--io.length == 0) {
    vdp.io.command.bit(5) = 0;
  }
}

auto VDP::DMA::power() -> void {
  memory::fill(&io, sizeof(IO));
}
