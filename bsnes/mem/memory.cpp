#include "../base.h"
#include "../cpu/g65816.h"
extern g65816 *gx816;
extern emustate emu_state;
extern debugstate debugger;

void g65816::InitializeROM(byte memory_map) {
  memset(rom, 0, 0x600000);
  map = memory_map;
}

void g65816::InitializeWRAM(byte value) {
  memset(wram, value, 0x020000);
  memset(sram, 0x00,  0x0e0000);
}

/***********************
 *** SNES Memory Map ***
 **************************************************
 *** 00-3f  0000-1fff  First 8k WRAM            ***
 ***        2000-5fff  MMIO                     ***
 ***        6000-7fff  Expansion RAM (Unmapped) ***
 ***        8000-ffff  Cartridge ROM            ***
 *** 40-7d  0000-ffff  Cartridge ROM            ***
 *** 7e-7f  0000-ffff  128k WRAM                ***
 *** 80-bf  0000-1fff  First 8k WRAM            ***
 ***        2000-5fff  MMIO                     ***
 ***        6000-7fff  Expansion RAM (Unmapped) ***
 ***        8000-ffff  Cartridge ROM            ***
 *** c0-ff  0000-ffff  Cartridge ROM            ***
**************************************************/

ulong g65816::mirror_offset(ulong addr) {
byte db;
word a;
ulong r = 0;
  db = (addr >> 16) & 0xff;
  a  = (addr & 0xffff);
  if(db >= 0x00 && db <= 0x3f) {
    if(a >= 0x0000 && a <= 0x1fff) {
      r = 0x7e0000 | (a & 0x1fff);
    } else if(a >= 0x2000 && a <= 0x5fff) {
      r = a;
    } else if(a >= 0x6000 && a <= 0x7fff) {
      r = (db << 16) | a;
    } else if(a >= 0x8000 && a <= 0xffff) {
      r = (db << 16) | a;
    }
  } else if(db >= 0x40 && db <= 0x7d) {
      r = (db << 16) | a;
  } else if(db >= 0x7e && db <= 0x7f) {
    r = addr;
  } else if(db >= 0x80 && db <= 0xbf) {
    if(a >= 0x0000 && a <= 0x1fff) {
      r = 0x7e0000 | (a & 0x1fff);
    } else if(a >= 0x2000 && a <= 0x5fff) {
      r = a;
    } else if(a >= 0x6000 && a <= 0x7fff) {
      r = (db << 16) | a;
    } else if(a >= 0x8000 && a <= 0xffff) {
      r = ((db & 0x7f) << 16) | a;
    }
  } else if(db >= 0xc0 && db <= 0xff) {
    r = addr;
  }

  return r;
}

ulong g65816::convert_offset(byte read_mode, ulong addr, bool mirror) {
byte db;
  switch(read_mode) {
  case MEMMODE_DP:
    addr = (regs.d + (addr & 0xff)) & 0xffff;
    break;
  case MEMMODE_DPX:
    addr = (regs.d + regs.x + (addr & 0xff)) & 0xffff;
    break;
  case MEMMODE_DPY:
    addr = (regs.d + regs.y + (addr & 0xff)) & 0xffff;
    break;
  case MEMMODE_IDP:
    addr = (regs.d + (addr & 0xff)) & 0xffff;
    addr = mem_read(MEMMODE_LONG, MEMSIZE_WORD, addr);
    addr |= (regs.db << 16);
    break;
  case MEMMODE_IDPX:
    addr = (regs.d + regs.x + (addr & 0xff)) & 0xffff;
    addr = mem_read(MEMMODE_LONG, MEMSIZE_WORD, addr);
    addr |= (regs.db << 16);
    break;
  case MEMMODE_IDPY:
    addr = (regs.d + (addr & 0xff)) & 0xffff;
    addr = mem_read(MEMMODE_LONG, MEMSIZE_WORD, addr);
    addr += (regs.db << 16) + regs.y;
    if((addr >> 16) != regs.db)index_bank_crossed = true;
    else                       index_bank_crossed = false;
    break;
  case MEMMODE_ILDP:
    addr = (regs.d + (addr & 0xff));
    addr = mem_read(MEMMODE_LONG, MEMSIZE_LONG, addr);
    break;
  case MEMMODE_ILDPY:
    addr = (regs.d + (addr & 0xff));
    addr = mem_read(MEMMODE_LONG, MEMSIZE_LONG, addr);
    addr += regs.y;
    break;
  case MEMMODE_ADDR:
    addr = addr & 0xffff;
    addr |= (regs.db << 16);
    break;
  case MEMMODE_ADDR_PC:
    addr = addr & 0xffff;
    addr |= (regs.pc & 0xff0000);
    break;
  case MEMMODE_ADDRX:
    addr = (regs.db << 16) + (addr & 0xffff);
    addr += regs.x;
    if((addr >> 16) != regs.db)index_bank_crossed = true;
    else                       index_bank_crossed = false;
    break;
  case MEMMODE_ADDRY:
    addr = (regs.db << 16) + (addr & 0xffff);
    addr += regs.y;
    if((addr >> 16) != regs.db)index_bank_crossed = true;
    else                       index_bank_crossed = false;
    break;
  case MEMMODE_IADDRX:
    addr += regs.x;
    addr = mem_read(MEMMODE_LONG, MEMSIZE_WORD, (addr & 0x00ffff));
    addr |= (regs.pc & 0xff0000);
    break;
  case MEMMODE_ILADDR:
    addr = mem_read(MEMMODE_LONG, MEMSIZE_LONG, addr);
    break;
  case MEMMODE_IADDR_PC:
    addr |= (regs.pc & 0xff0000);
    break;
  case MEMMODE_IADDRX_PC:
    addr += regs.x;
    addr |= (regs.pc & 0xff0000);
    break;
  case MEMMODE_ILADDR_PC:
    break;
  case MEMMODE_LONG:
    break;
  case MEMMODE_LONGX:
    addr += regs.x;
    break;
  case MEMMODE_SR:
    addr = (regs.s + (addr & 0xff)) & 0xffff;
    break;
  case MEMMODE_ISRY:
    addr = (regs.s + (addr & 0xff)) & 0xffff;
    addr = mem_read(MEMMODE_LONG, MEMSIZE_WORD, addr);
    addr += (regs.db << 16) + regs.y;
    break;
  }

  if(mirror == true) {
    return mirror_offset(addr);
  } else {
    return addr;
  }
}

ulong g65816::adjust_base_offset(byte read_mode, ulong addr) {
byte db;
  switch(read_mode) {
  case MEMMODE_DP:
    addr = (regs.d + (addr & 0xff)) & 0xffff;
    break;
  case MEMMODE_DPX:
    addr = (regs.d + regs.x + (addr & 0xff)) & 0xffff;
    break;
  case MEMMODE_DPY:
    addr = (regs.d + regs.y + (addr & 0xff)) & 0xffff;
    break;
  case MEMMODE_IDP:
    addr = (regs.d + (addr & 0xff)) & 0xffff;
    break;
  case MEMMODE_IDPX:
    addr = (regs.d + regs.x + (addr & 0xff)) & 0xffff;
    break;
  case MEMMODE_IDPY:
    addr = (regs.d + (addr & 0xff)) & 0xffff;
    break;
  case MEMMODE_ILDP:
    addr = (regs.d + (addr & 0xff));
    break;
  case MEMMODE_ILDPY:
    addr = (regs.d + (addr & 0xff));
    break;
  case MEMMODE_ADDR:
    addr = addr & 0xffff;
    addr |= (regs.db << 16);
    break;
  case MEMMODE_ADDR_PC:
    addr = addr & 0xffff;
    addr |= (regs.pc & 0xff0000);
    break;
  case MEMMODE_ADDRX:
    addr = (regs.db << 16) | (addr & 0xffff);
    addr += regs.x;
    if((addr >> 16) != regs.db)index_bank_crossed = true;
    else                       index_bank_crossed = false;
    break;
  case MEMMODE_ADDRY:
    addr = (regs.db << 16) + (addr & 0xffff);
    addr += regs.y;
    if((addr >> 16) != regs.db)index_bank_crossed = true;
    else                       index_bank_crossed = false;
    break;
  case MEMMODE_IADDRX:
    addr += regs.x;
    addr &= 0xffff;
    break;
  case MEMMODE_ILADDR:
    addr &= 0xffff;
    break;
  case MEMMODE_IADDR_PC:
    addr |= (regs.pc & 0xff0000);
    break;
  case MEMMODE_IADDRX_PC:
    addr += regs.x;
    addr |= (regs.pc & 0xff0000);
    break;
  case MEMMODE_ILADDR_PC:
    break;
  case MEMMODE_LONG:
    break;
  case MEMMODE_LONGX:
    addr += regs.x;
    break;
  case MEMMODE_SR:
    addr = (regs.s + (addr & 0xff)) & 0xffff;
    break;
  case MEMMODE_ISRY:
    addr = (regs.s + (addr & 0xff)) & 0xffff;
    break;
  }
  return addr;
}

ulong g65816::read_indirect_address(byte read_mode, ulong addr) {
byte db;
  switch(read_mode) {
  case MEMMODE_IDP:
    addr = mem_read(MEMMODE_LONG, MEMSIZE_WORD, addr);
    addr |= (regs.db << 16);
    break;
  case MEMMODE_IDPX:
    addr = mem_read(MEMMODE_LONG, MEMSIZE_WORD, addr);
    addr |= (regs.db << 16);
    break;
  case MEMMODE_IDPY:
    addr = mem_read(MEMMODE_LONG, MEMSIZE_WORD, addr);
    addr += (regs.db << 16) + regs.y;
    if((addr >> 16) != regs.db)index_bank_crossed = true;
    else                       index_bank_crossed = false;
    break;
  case MEMMODE_ILDP:
    addr = mem_read(MEMMODE_LONG, MEMSIZE_LONG, addr);
    break;
  case MEMMODE_ILDPY:
    addr = mem_read(MEMMODE_LONG, MEMSIZE_LONG, addr);
    addr += regs.y;
    break;
  case MEMMODE_IADDRX:
    addr = mem_read(MEMMODE_LONG, MEMSIZE_WORD, (addr & 0x00ffff));
    addr |= (regs.pc & 0xff0000);
    break;
  case MEMMODE_ILADDR:
    addr = mem_read(MEMMODE_LONG, MEMSIZE_LONG, addr);
    break;
  case MEMMODE_ISRY:
    addr = mem_read(MEMMODE_LONG, MEMSIZE_WORD, addr);
    addr += (regs.db << 16) + regs.y;
    break;
  default:
    dprintf("* Error: Invalid read_mode for g65816::read_indirect_address [%d]", read_mode);
    break;
  }
  return addr;
}

ulong g65816::get_dc(byte read_mode, ulong addr) {
  regs.dc = convert_offset(read_mode, addr, false);
  return regs.dc;
}

byte g65816::mem_getbyte_direct(ulong addr, byte access_mode) {
byte db;
word a;
  db = (addr >> 16) & 0xff;
  a  = (addr & 0xffff);
  if(db == 0x00 && a >= 0x2000 && a <= 0x5fff) {
    return mmio_read(addr);
  }

  if(db == 0x7e || db == 0x7f) {
    return wram[addr & 0x01ffff];
  } else if(db != 0x7e && db != 0x7f && a >= 0x8000 && a <= 0xffff) {
    return rom_read(addr, MEMSIZE_BYTE);
  } else if(db >= 0x30 && db <= 0x3f) {
    if(a >= 0x6000 && a <= 0x7fff) {
      addr = ((db - 0x30) * 0x2000) + (a - 0x6000);
      addr &= (sram_size - 1);
      return sram[addr];
    }
  } else if(db >= 0x70 && db <= 0x7d) {
    addr -= 0x700000;
    addr &= (sram_size - 1);
    return sram[addr];
  } else if(db >= 0xc0 && db <= 0xff) {
    return rom_read(addr, MEMSIZE_BYTE);
  }
  return 0;
}

byte g65816::mem_getbyte(ulong addr, byte access_mode) {
int i;
byte r;
  r = mem_getbyte_direct(addr, access_mode);

  if(debug_get_state() == DEBUGMODE_DISABLED)return r;

  if(access_mode == MEMACCESS_DEBUGGER)return r; //don't report breakpoint hits from debugger

  for(i=0;i<16;i++) {
    if(bp_list[i].flags & BP_READ) {
      if(bp_list[i].offset == addr) {
        if(bp_list[i].flags & BP_VAL) {
          if(bp_list[i].value == r) {
            dprintf("* breakpoint %d hit -- read match access [%0.2x]", i, r);
            bp_list[i].hit_count++;
            debugger.refresh_bp = true;
            debug_set_state(DEBUGMODE_WAIT);
            disas_g65816_op();
          }
        } else {
          dprintf("* breakpoint %d hit -- read access", i);
          bp_list[i].hit_count++;
          debugger.refresh_bp = true;
          debug_set_state(DEBUGMODE_WAIT);
          disas_g65816_op();
        }
      }
    }
  }
  return r;
}

void g65816::mem_putbyte_direct(ulong addr, byte value, byte access_mode) {
byte db;
word a;
  db = (addr >> 16) & 0xff;
  a  = (addr & 0xffff);
  if(db == 0x00 && a >= 0x2000 && a <= 0x5fff) {
    mmio_write(a, value);
  } else if(db == 0x7e || db == 0x7f) {
    wram[addr & 0x01ffff] = value;
  } else if(db >= 0x30 && db <= 0x3f) {
    if(a >= 0x6000 && a <= 0x7fff) {
      addr = ((db - 0x30) * 0x2000) + (a - 0x6000);
      addr &= (sram_size - 1);
      sram[addr] = value;
    }
  } else if(db >= 0x70 && db <= 0x7d) {
    addr -= 0x700000;
    addr &= (sram_size - 1);
    sram[addr] = value;
  }

  if(access_mode == MEMACCESS_DEBUGGER) {
    if(gx816->map == MEMMAP_LOROM) {
      if((db >= 0x00 && db <= 0x5f) || (db >= 0x80 && db <= 0xdf)) {
        if(addr >= 0x8000 && addr <= 0xffff) {
          rom_write(addr, value, MEMSIZE_BYTE);
        }
      }
    } else if(gx816->map == MEMMAP_HIROM) {
      if(db >= 0xc0 && db <= 0xff) {
        rom_write(addr, value, MEMSIZE_BYTE);
      }
    }
  }
}

void g65816::mem_putbyte(ulong addr, byte value, byte access_mode) {
int i;
  mem_putbyte_direct(addr, value, access_mode);

  if(debug_get_state() == DEBUGMODE_DISABLED)return;

  if(access_mode == MEMACCESS_DEBUGGER)return; //don't report breakpoint hits from debugger

  for(i=0;i<16;i++) {
    if(bp_list[i].flags & BP_WRITE) {
      if(bp_list[i].offset == addr) {
        if(bp_list[i].flags & BP_VAL) {
          if(bp_list[i].value == value) {
            dprintf("* breakpoint %d hit -- write match access [%0.2x]", i, value);
            bp_list[i].hit_count++;
            debugger.refresh_bp = true;
            debug_set_state(DEBUGMODE_WAIT);
            disas_g65816_op();
          }
        } else {
          dprintf("* breakpoint %d hit -- write access", i);
          bp_list[i].hit_count++;
          debugger.refresh_bp = true;
          debug_set_state(DEBUGMODE_WAIT);
          disas_g65816_op();
        }
      }
    }
  }
}

ulong g65816::mem_read(byte read_mode, byte read_size, ulong addr, byte access_mode) {
ulong r;
  addr = convert_offset(read_mode, addr);

  switch(read_size) {
  case MEMSIZE_BYTE:
    r = mem_getbyte(addr,     access_mode);
    break;
  case MEMSIZE_WORD:
    r = mem_getbyte(addr,     access_mode) |
        mem_getbyte(addr + 1, access_mode)<<8;
    break;
  case MEMSIZE_LONG:
    r = mem_getbyte(addr,     access_mode) |
        mem_getbyte(addr + 1, access_mode)<<8 |
        mem_getbyte(addr + 2, access_mode)<<16;
    break;
  }

  return r;
}

void g65816::mem_write(byte write_mode, byte write_size, ulong addr, ulong value, byte access_mode) {
  addr = convert_offset(write_mode, addr);

  switch(write_size) {
  case MEMSIZE_BYTE:
    mem_putbyte(addr,   value,     access_mode);
    break;
  case MEMSIZE_WORD:
    mem_putbyte(addr,   value,     access_mode);
    mem_putbyte(addr+1, value>>8,  access_mode);
    break;
  case MEMSIZE_LONG:
    mem_putbyte(addr,   value,     access_mode);
    mem_putbyte(addr+1, value>>8,  access_mode);
    mem_putbyte(addr+2, value>>16, access_mode);
    break;
  }

  debugger.refresh_mem = true;
}

ulong g65816::rom_read(ulong addr, byte read_size) {
ulong r;
  if(map == MEMMAP_LOROM) {
    if(read_size == MEMSIZE_BYTE) {
      r = rom[((addr & 0x7f0000) >> 1) | (addr & 0x7fff)];
    } else if(read_size == MEMSIZE_WORD) {
      r = rom[((addr & 0x7f0000) >> 1) | (addr & 0x7fff)] |
          rom[(((addr + 1) & 0x7f0000) >> 1) | ((addr + 1) & 0x7fff)] << 8;
    } else if(read_size == MEMSIZE_LONG) {
      r = rom[((addr & 0x7f0000) >> 1) | (addr & 0x7fff)] |
          rom[(((addr + 1) & 0x7f0000) >> 1) | ((addr + 1) & 0x7fff)] << 8 |
          rom[(((addr + 2) & 0x7f0000) >> 1) | ((addr + 2) & 0x7fff)] << 16;
    }
  } else if(map == MEMMAP_HIROM) {
    if(read_size == MEMSIZE_BYTE) {
      r = rom[addr & 0x3fffff];
    } else if(read_size == MEMSIZE_WORD) {
      r = rom[addr & 0x3fffff] |
          rom[(addr + 1) & 0x3fffff] << 8;
    } else if(read_size == MEMSIZE_LONG) {
      r = rom[addr & 0x3fffff] |
          rom[(addr + 1) & 0x3fffff] << 8 |
          rom[(addr + 2) & 0x3fffff] << 16;
    }
  }
  return r;
}

void g65816::rom_write(ulong addr, ulong v, byte write_size) {
  if(map == MEMMAP_LOROM) {
    if(write_size == MEMSIZE_BYTE) {
      rom[((addr & 0x7f0000) >> 1) | (addr & 0x7fff)] = v;
    } else if(write_size == MEMSIZE_WORD) {
      rom[((addr & 0x7f0000) >> 1) | (addr & 0x7fff)] = v;
      rom[(((addr + 1) & 0x7f0000) >> 1) | ((addr + 1) & 0x7fff)] = v >> 8;
    } else if(write_size == MEMSIZE_LONG) {
      rom[((addr & 0x7f0000) >> 1) | (addr & 0x7fff)] = v;
      rom[(((addr + 1) & 0x7f0000) >> 1) | ((addr + 1) & 0x7fff)] = v >> 8;
      rom[(((addr + 2) & 0x7f0000) >> 1) | ((addr + 2) & 0x7fff)] = v >> 16;
    }
  } else if(map == MEMMAP_HIROM) {
    if(write_size == MEMSIZE_BYTE) {
      rom[addr & 0x3fffff] = v;
    } else if(write_size == MEMSIZE_WORD) {
      rom[addr & 0x3fffff] = v;
      rom[(addr + 1) & 0x3fffff] = v >> 8;
    } else if(write_size == MEMSIZE_LONG) {
      rom[addr & 0x3fffff] = v;
      rom[(addr + 1) & 0x3fffff] = v >> 8;
      rom[(addr + 2) & 0x3fffff] = v >> 16;
    }
  }
}
