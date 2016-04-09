auto SuperFX::step(unsigned clocks) -> void {
  if(regs.romcl) {
    regs.romcl -= min(clocks, regs.romcl);
    if(regs.romcl == 0) {
      regs.sfr.r = 0;
      regs.romdr = bus_read((regs.rombr << 16) + regs.r[14]);
    }
  }

  if(regs.ramcl) {
    regs.ramcl -= min(clocks, regs.ramcl);
    if(regs.ramcl == 0) {
      bus_write(0x700000 + (regs.rambr << 16) + regs.ramar, regs.ramdr);
    }
  }

  Cothread::step(clocks);
  synchronizeCPU();
}

auto SuperFX::rombuffer_sync() -> void {
  if(regs.romcl) step(regs.romcl);
}

auto SuperFX::rombuffer_update() -> void {
  regs.sfr.r = 1;
  regs.romcl = regs.clsr ? 5 : 6;
}

auto SuperFX::rombuffer_read() -> uint8 {
  rombuffer_sync();
  return regs.romdr;
}

auto SuperFX::rambuffer_sync() -> void {
  if(regs.ramcl) step(regs.ramcl);
}

auto SuperFX::rambuffer_read(uint16 addr) -> uint8 {
  rambuffer_sync();
  return bus_read(0x700000 + (regs.rambr << 16) + addr);
}

auto SuperFX::rambuffer_write(uint16 addr, uint8 data) -> void {
  rambuffer_sync();
  regs.ramcl = regs.clsr ? 5 : 6;
  regs.ramar = addr;
  regs.ramdr = data;
}

auto SuperFX::r14_modify(uint16 data) -> void {
  regs.r[14].data = data;
  rombuffer_update();
}

auto SuperFX::r15_modify(uint16 data) -> void {
  regs.r[15].data = data;
  r15_modified = true;
}

auto SuperFX::timing_reset() -> void {
  r15_modified = false;

  regs.romcl = 0;
  regs.romdr = 0;

  regs.ramcl = 0;
  regs.ramar = 0;
  regs.ramdr = 0;
}
