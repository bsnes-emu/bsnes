#include "../../base.h"

uint8 bAPUSkip::spcram_read (uint16 addr) { return 0xff; }
void  bAPUSkip::spcram_write(uint16 addr, uint8 value) {}

/*
  This routine is very serious. It will eat holes through
  the ROM to skip APU test conditions. Or in other words,
  it will disable and/or force branches when neccesary.

  It can very easily break or corrupt a game and prevent it
  from being playable until the ROM is reloaded (ROM writes
  are only performed in memory, of course).

  However, this kind of brute force approach is required to
  get many games playable without proper SPC700 emulation.
*/
uint8 bAPUSkip::port_read(uint8 port) {
  port &= 3;
_apu_port *p = &apu_port[port];
int i, x, y, z, t;
uint32 addr;

  addr = cpu->regs.pc.d;
  p->read_addr[p->read_pos & 31] = addr;

//- lda $214x
//  cmp $214x
//  bne -
//  cmp ???
//  beq/bne -
__test1:
//look for an lda/cmp read pattern
  if(addr == p->read_addr[(p->read_pos - 1) & 31])goto __test2;
  if(addr != p->read_addr[(p->read_pos - 2) & 31])goto __test2;
  if(addr == p->read_addr[(p->read_pos - 3) & 31])goto __test2;
  if(addr != p->read_addr[(p->read_pos - 4) & 31])goto __test2;
  if(p->read_addr[(p->read_pos - 1) & 31] != p->read_addr[(p->read_pos - 3) & 31])goto __test2;
//try and find compare opcode
  for(i=0;i<24;i++) {
    x = mem_bus->read(addr + i);
    if(x == OP_CMP_CONST || x == OP_CPX_CONST || x == OP_CPY_CONST)break;
    if(x == OP_CMP_ADDR  || x == OP_CPX_ADDR  || x == OP_CPY_ADDR) break;
    if(x == OP_CMP_LONG)break;
  }
  if(i == 24)goto __test2;
//seek to next opcode
  if(x == OP_CMP_CONST) {
    i += (cpu->regs.p.m)?2:3;
  } else if(x == OP_CPX_CONST || x == OP_CPY_CONST) {
    i += (cpu->regs.p.x)?2:3;
  } else if(x == OP_CMP_ADDR || x == OP_CPX_ADDR || x == OP_CPY_ADDR) {
    i += 3;
  } else { //(x == OP_CMP_LONG) {
    i += 4;
  }
  x = mem_bus->read(addr + i);
  if(x == OP_BNE) {
    mem_bus->cart->write_protect(false);
    mem_bus->write(addr + i,     OP_NOP);
    mem_bus->write(addr + i + 1, OP_NOP);
    mem_bus->cart->write_protect(true);
  } else if(x == OP_BEQ) {
    mem_bus->cart->write_protect(false);
    mem_bus->write(addr + i, OP_BRA);
    mem_bus->cart->write_protect(true);
  } else goto __test2;
  goto __pass;

//- lda $214x
//  cmp ???
//  beq/bne -
__test2:
//look for a repeated read pattern
  if(addr != p->read_addr[(p->read_pos - 1) & 31])goto __test3;
  if(addr != p->read_addr[(p->read_pos - 2) & 31])goto __test3;
  if(addr != p->read_addr[(p->read_pos - 3) & 31])goto __test3;
  if(addr != p->read_addr[(p->read_pos - 4) & 31])goto __test3;
  if(addr != p->read_addr[(p->read_pos - 5) & 31])goto __test3;
  if(addr != p->read_addr[(p->read_pos - 6) & 31])goto __test3;
  if(addr != p->read_addr[(p->read_pos - 7) & 31])goto __test3;
  if(addr != p->read_addr[(p->read_pos - 8) & 31])goto __test3;
//try and find compare opcode
  for(i=0;i<24;i++) {
    x = mem_bus->read(addr + i);
    if(x == OP_CMP_CONST || x == OP_CPX_CONST || x == OP_CPY_CONST)break;
    if(x == OP_CMP_ADDR  || x == OP_CPX_ADDR  || x == OP_CPY_ADDR) break;
    if(x == OP_CMP_LONG)break;
  }
  if(i == 24)goto __test3;
//seek to next opcode
  if(x == OP_CMP_CONST) {
    i += (cpu->regs.p.m)?2:3;
  } else if(x == OP_CPX_CONST || x == OP_CPY_CONST) {
    i += (cpu->regs.p.x)?2:3;
  } else if(x == OP_CMP_ADDR || x == OP_CPX_ADDR || x == OP_CPY_ADDR) {
    i += 3;
  } else if(x == OP_CMP_LONG) {
    i += 4;
  }
  x = mem_bus->read(addr + i);
  if(x == OP_BNE) {
    mem_bus->cart->write_protect(false);
    mem_bus->write(addr + i,     OP_NOP);
    mem_bus->write(addr + i + 1, OP_NOP);
    mem_bus->cart->write_protect(true);
  } else if(x == OP_BEQ) {
    mem_bus->cart->write_protect(false);
    mem_bus->write(addr + i, OP_BRA);
    mem_bus->cart->write_protect(true);
  } else goto __test3;
  goto __pass;

//fallback
__test3:
  if(p->pos < 4) {
    if(!(port & 1)) {
      p->value = cpu->regs.a.l;
    } else {
      p->value = cpu->regs.a.h;
    }
  } else if(p->pos < 8) {
    if(!(port & 1)) {
      p->value = cpu->regs.x.l;
    } else {
      p->value = cpu->regs.x.h;
    }
  } else if(p->pos < 12) {
    if(!(port & 1)) {
      p->value = cpu->regs.y.l;
    } else {
      p->value = cpu->regs.y.h;
    }
  } else if(p->pos < 16) {
    p->value = rand();
  }
  if(++p->pos == 16)p->pos = 0;

__pass:
  p->read_pos++;
  p->read_pos &= 31;
  return p->value;
}

void bAPUSkip::port_write(uint8 port, uint8 value) {
  port &= 3;
  apu_port[port].value = value;
}

void bAPUSkip::run() {
  snes->notify(SNES::APU_EXEC_OPCODE_BEGIN);
  snes->notify(SNES::APU_EXEC_OPCODE_END);
}

void bAPUSkip::power() {
  reset();
}

void bAPUSkip::reset() {
  regs.a  = 0x00;
  regs.x  = 0x00;
  regs.y  = 0x00;
  regs.sp = 0x00;
  regs.p  = 0x02;
  regs.pc = 0xffc0;

  memset(&apu_port[0], 0, sizeof(_apu_port));
  memset(&apu_port[1], 0, sizeof(_apu_port));
  memset(&apu_port[2], 0, sizeof(_apu_port));
  memset(&apu_port[3], 0, sizeof(_apu_port));
}
