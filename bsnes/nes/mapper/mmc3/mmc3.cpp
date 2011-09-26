MMC3 mmc3;

void MMC3::main() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    if(irq_delay) irq_delay--;
    cpu.set_irq_line(irq_line);
    tick();
  }
}

void MMC3::irq_test(uint16 addr) {
  if(!(chr_abus & 0x1000) && (addr & 0x1000)) {
    if(irq_delay == 0) {
      if(irq_counter == 0) {
        irq_counter = irq_latch;
      } else if(--irq_counter == 0) {
        if(irq_enable) irq_line = 1;
      }
    }
    irq_delay = 6;
  }
  chr_abus = addr;
}

unsigned MMC3::prg_addr(uint16 addr) {
  if((addr & 0xe000) == 0x8000) {
    if((bank_select & 0x40) != 0) return (0x3e << 13) | (addr & 0x1fff);
    return (prg_bank[0] << 13) | (addr & 0x1fff);
  }

  if((addr & 0xe000) == 0xa000) {
    return (prg_bank[1] << 13) | (addr & 0x1fff);
  }

  if((addr & 0xe000) == 0xc000) {
    if((bank_select & 0x40) == 0) return (0x3e << 13) | (addr & 0x1fff);
    return (prg_bank[0] << 13) | (addr & 0x1fff);
  }

  if((addr & 0xe000) == 0xe000) {
    return (0x3f << 13) | (addr & 0x1fff);
  }

  throw;
}

uint8 MMC3::prg_read(unsigned addr) {
  if((addr & 0xe000) == 0x6000) {  //$6000-7fff
    if(prg_ram_enable) {
      return prg_ram[addr & 0x1fff];
    }
  }

  if(addr & 0x8000) {  //$8000-ffff
    return Mapper::prg_read(prg_addr(addr));
  }

  return cpu.mdr();
}

void MMC3::prg_write(unsigned addr, uint8 data) {
  if((addr & 0xe000) == 0x6000) {  //$6000-7fff
    if(prg_ram_enable && prg_ram_write_protect == false) {
      prg_ram[addr & 0x1fff] = data;
    }
  }

  switch(addr & 0xe001) {
  case 0x8000:
    bank_select = data & 0xc7;
    break;

  case 0x8001:
    switch(bank_select & 7) {
    case 0: chr_bank[0] = data & ~1; break;
    case 1: chr_bank[1] = data & ~1; break;
    case 2: chr_bank[2] = data; break;
    case 3: chr_bank[3] = data; break;
    case 4: chr_bank[4] = data; break;
    case 5: chr_bank[5] = data; break;
    case 6: prg_bank[0] = data & 0x3f; break;
    case 7: prg_bank[1] = data & 0x3f; break;
    }
    break;

  case 0xa000:
    mirror_select = data & 0x01;
    break;

  case 0xa001:
    prg_ram_enable = data & 0x80;
    prg_ram_write_protect = data & 0x40;
    break;

  case 0xc000:
    irq_latch = data;
    break;

  case 0xc001:
    irq_counter = 0;
    break;

  case 0xe000:
    irq_enable = false;
    irq_line = 0;
    cpu.set_irq_line(irq_line);
    break;

  case 0xe001:
    irq_enable = true;
    break;
  }
}

unsigned MMC3::chr_addr(uint16 addr) {
  if((bank_select & 0x80) == 0) {
    if(addr <= 0x07ff) return (chr_bank[0] << 10) | (addr & 0x07ff);
    if(addr <= 0x0fff) return (chr_bank[1] << 10) | (addr & 0x07ff);
    if(addr <= 0x13ff) return (chr_bank[2] << 10) | (addr & 0x03ff);
    if(addr <= 0x17ff) return (chr_bank[3] << 10) | (addr & 0x03ff);
    if(addr <= 0x1bff) return (chr_bank[4] << 10) | (addr & 0x03ff);
    if(addr <= 0x1fff) return (chr_bank[5] << 10) | (addr & 0x03ff);
  }

  if((bank_select & 0x80) != 0) {
    if(addr <= 0x03ff) return (chr_bank[2] << 10) | (addr & 0x03ff);
    if(addr <= 0x07ff) return (chr_bank[3] << 10) | (addr & 0x03ff);
    if(addr <= 0x0bff) return (chr_bank[4] << 10) | (addr & 0x03ff);
    if(addr <= 0x0fff) return (chr_bank[5] << 10) | (addr & 0x03ff);
    if(addr <= 0x17ff) return (chr_bank[0] << 10) | (addr & 0x07ff);
    if(addr <= 0x1fff) return (chr_bank[1] << 10) | (addr & 0x07ff);
  }

  throw;
}

uint8 MMC3::chr_read(unsigned addr) {
  irq_test(addr);
  if(addr & 0x2000) return ppu.ciram_read(ciram_addr(addr));
  return Mapper::chr_read(chr_addr(addr));
}

void MMC3::chr_write(unsigned addr, uint8 data) {
  irq_test(addr);
  if(addr & 0x2000) return ppu.ciram_write(ciram_addr(addr), data);
  return Mapper::chr_write(chr_addr(addr), data);
}

unsigned MMC3::ciram_addr(uint13 addr) {
  if(mirror_select == 0) return ((addr & 0x0400) >> 0) | (addr & 0x03ff);
  if(mirror_select == 1) return ((addr & 0x0800) >> 1) | (addr & 0x03ff);
  throw;
}

unsigned MMC3::ram_size() {
  return 8192u;
}

uint8* MMC3::ram_data() {
  return prg_ram;
}

void MMC3::power() {
  reset();
}

void MMC3::reset() {
  bank_select = 0;

  prg_bank[0] = 0;
  prg_bank[1] = 0;

  chr_bank[0] = 0;
  chr_bank[1] = 0;
  chr_bank[2] = 0;
  chr_bank[3] = 0;
  chr_bank[4] = 0;
  chr_bank[5] = 0;

  mirror_select = 0;
  prg_ram_enable = 1;
  prg_ram_write_protect = 0;

  irq_latch = 0x00;
  irq_counter = 0x00;
  irq_enable = false;
  irq_delay = 0;
  irq_line = 0;

  chr_abus = 0;
}

void MMC3::serialize(serializer &s) {
  s.array(prg_ram);

  s.integer(bank_select);
  s.array(prg_bank);
  s.array(chr_bank);
  s.integer(mirror_select);
  s.integer(prg_ram_enable);
  s.integer(prg_ram_write_protect);
  s.integer(irq_latch);
  s.integer(irq_counter);
  s.integer(irq_enable);
  s.integer(irq_delay);
  s.integer(irq_line);

  s.integer(chr_abus);
}
