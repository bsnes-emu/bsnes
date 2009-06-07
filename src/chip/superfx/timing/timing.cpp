void SuperFX::add_clocks(unsigned clocks) {
  if(regs.romcl) {
    regs.romcl -= min(clocks, regs.romcl);
    if(regs.romcl == 0) {
      regs.sfr.r = 0;
      regs.romdr = superfxbus.read((regs.rombr << 16) + regs.r[14]);
    }
  }

  if(regs.ramcl) {
    regs.ramcl -= min(clocks, regs.ramcl);
    if(regs.ramcl == 0) {
      superfxbus.write(0x700000 + (regs.rambr << 16) + regs.ramar, regs.ramdr);
    }
  }

  scheduler.addclocks_cop(clocks);
  scheduler.sync_copcpu();
}

void SuperFX::rombuffer_sync() {
  if(regs.romcl) add_clocks(regs.romcl);
}

void SuperFX::rombuffer_update() {
  regs.sfr.r = 1;
  regs.romcl = memory_access_speed;
}

uint8_t SuperFX::rombuffer_read() {
  rombuffer_sync();
  return regs.romdr;
}

void SuperFX::rambuffer_sync() {
  if(regs.ramcl) add_clocks(regs.ramcl);
}

uint8_t SuperFX::rambuffer_read(uint16_t addr) {
  rambuffer_sync();
  return superfxbus.read(0x700000 + (regs.rambr << 16) + addr);
}

void SuperFX::rambuffer_write(uint16_t addr, uint8_t data) {
  rambuffer_sync();
  regs.ramcl = memory_access_speed;
  regs.ramar = addr;
  regs.ramdr = data;
}

void SuperFX::r14_modify(uint16_t data) {
  regs.r[14].data = data;
  rombuffer_update();
}

void SuperFX::r15_modify(uint16_t data) {
  regs.r[15].data = data;
  r15_modified = true;
}

void SuperFX::timing_reset() {
  cache_access_speed  = config.superfx.slow_cache_speed;
  memory_access_speed = config.superfx.slow_memory_speed;
  r15_modified = false;

  regs.romcl = 0;
  regs.romdr = 0;

  regs.ramcl = 0;
  regs.ramar = 0;
  regs.ramdr = 0;
}
