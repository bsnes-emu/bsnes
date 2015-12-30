struct MMC3 : Chip {
  MMC3(Board& board) : Chip(board) {
  }

  auto main() -> void {
    while(true) {
      if(scheduler.sync == Scheduler::SynchronizeMode::All) {
        scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
      }

      if(irq_delay) irq_delay--;
      cpu.set_irq_line(irq_line);
      tick();
    }
  }

  auto irq_test(uint addr) -> void {
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

  auto prg_addr(uint addr) const -> uint {
    switch((addr >> 13) & 3) {
    case 0:
      if(prg_mode == 1) return (0x3e << 13) | (addr & 0x1fff);
      return (prg_bank[0] << 13) | (addr & 0x1fff);
    case 1:
      return (prg_bank[1] << 13) | (addr & 0x1fff);
    case 2:
      if(prg_mode == 0) return (0x3e << 13) | (addr & 0x1fff);
      return (prg_bank[0] << 13) | (addr & 0x1fff);
    case 3:
      return (0x3f << 13) | (addr & 0x1fff);
    }
  }

  auto chr_addr(uint addr) const -> uint {
    if(chr_mode == 0) {
      if(addr <= 0x07ff) return (chr_bank[0] << 10) | (addr & 0x07ff);
      if(addr <= 0x0fff) return (chr_bank[1] << 10) | (addr & 0x07ff);
      if(addr <= 0x13ff) return (chr_bank[2] << 10) | (addr & 0x03ff);
      if(addr <= 0x17ff) return (chr_bank[3] << 10) | (addr & 0x03ff);
      if(addr <= 0x1bff) return (chr_bank[4] << 10) | (addr & 0x03ff);
      if(addr <= 0x1fff) return (chr_bank[5] << 10) | (addr & 0x03ff);
    } else {
      if(addr <= 0x03ff) return (chr_bank[2] << 10) | (addr & 0x03ff);
      if(addr <= 0x07ff) return (chr_bank[3] << 10) | (addr & 0x03ff);
      if(addr <= 0x0bff) return (chr_bank[4] << 10) | (addr & 0x03ff);
      if(addr <= 0x0fff) return (chr_bank[5] << 10) | (addr & 0x03ff);
      if(addr <= 0x17ff) return (chr_bank[0] << 10) | (addr & 0x07ff);
      if(addr <= 0x1fff) return (chr_bank[1] << 10) | (addr & 0x07ff);
    }
  }

  auto ciram_addr(uint addr) const -> uint {
    if(mirror == 0) return ((addr & 0x0400) >> 0) | (addr & 0x03ff);
    if(mirror == 1) return ((addr & 0x0800) >> 1) | (addr & 0x03ff);
  }

  auto ram_read(uint addr) -> uint8 {
    if(ram_enable) return board.prgram.data[addr & 0x1fff];
    return 0x00;
  }

  auto ram_write(uint addr, uint8 data) -> void {
    if(ram_enable && !ram_write_protect) board.prgram.data[addr & 0x1fff] = data;
  }

  auto reg_write(uint addr, uint8 data) -> void {
    switch(addr & 0xe001) {
    case 0x8000:
      chr_mode = data & 0x80;
      prg_mode = data & 0x40;
      bank_select = data & 0x07;
      break;

    case 0x8001:
      switch(bank_select) {
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
      mirror = data & 0x01;
      break;

    case 0xa001:
      ram_enable = data & 0x80;
      ram_write_protect = data & 0x40;
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
      break;

    case 0xe001:
      irq_enable = true;
      break;
    }
  }

  auto power() -> void {
  }

  auto reset() -> void {
    chr_mode = 0;
    prg_mode = 0;
    bank_select = 0;
    prg_bank[0] = 0;
    prg_bank[1] = 0;
    chr_bank[0] = 0;
    chr_bank[1] = 0;
    chr_bank[2] = 0;
    chr_bank[3] = 0;
    chr_bank[4] = 0;
    chr_bank[5] = 0;
    mirror = 0;
    ram_enable = 1;
    ram_write_protect = 0;
    irq_latch = 0;
    irq_counter = 0;
    irq_enable = false;
    irq_delay = 0;
    irq_line = 0;

    chr_abus = 0;
  }

  auto serialize(serializer& s) -> void {
    s.integer(chr_mode);
    s.integer(prg_mode);
    s.integer(bank_select);
    s.array(prg_bank);
    s.array(chr_bank);
    s.integer(mirror);
    s.integer(ram_enable);
    s.integer(ram_write_protect);
    s.integer(irq_latch);
    s.integer(irq_counter);
    s.integer(irq_enable);
    s.integer(irq_delay);
    s.integer(irq_line);

    s.integer(chr_abus);
  }

  bool chr_mode;
  bool prg_mode;
  uint3 bank_select;
  uint8 prg_bank[2];
  uint8 chr_bank[6];
  bool mirror;
  bool ram_enable;
  bool ram_write_protect;
  uint8 irq_latch;
  uint8 irq_counter;
  bool irq_enable;
  uint irq_delay;
  bool irq_line;

  uint16 chr_abus;
};
