auto VDP::DMA::run() -> void {
  if(!io.enable || io.wait) return;

  if(!vdp.io.command.bit(5)) return;
  if(io.mode <= 1) return load();
  if(io.mode == 2) return fill();
  if(!vdp.io.command.bit(4)) return;
  if(io.mode == 3) return copy();
}

auto VDP::DMA::load() -> void {
  cpu.wait |= Wait::VDP_DMA;

  auto data = cpu.readWord(io.mode.bit(0) << 23 | io.source << 1);
  vdp.writeDataPort(data);

  io.source.bits(0,15)++;
  if(--io.length == 0) {
    vdp.io.command.bit(5) = 0;
    cpu.wait &=~ Wait::VDP_DMA;
  }
}

//todo: supposedly, this can also write to VSRAM and CRAM (undocumented)
auto VDP::DMA::fill() -> void {
  if(vdp.io.command.bits(0,3) == 1) {
    vdp.vram.writeByte(vdp.io.address, io.fill);
  }

  io.source.bits(0,15)++;
  vdp.io.address += vdp.io.dataIncrement;
  if(--io.length == 0) {
    vdp.io.command.bit(5) = 0;
  }
}

//note: this can only copy to VRAM
auto VDP::DMA::copy() -> void {
  auto data = vdp.vram.readByte(io.source);
  vdp.vram.writeByte(vdp.io.address, data);

  io.source.bits(0,15)++;
  vdp.io.address += vdp.io.dataIncrement;
  if(--io.length == 0) {
    vdp.io.command.bit(5) = 0;
  }
}

auto VDP::DMA::power() -> void {
  memory::fill(&io, sizeof(IO));
}
