/*****
 * These 3 functions control bus timing for the CPU.
 * cpu_io is an I/O cycle, and always 6 clock cycles long.
 * mem_read / mem_write indicate memory access bus cycles.
 * they are either 6, 8, or 12 bus cycles long, depending
 * both on location and the $420d.d0 FastROM enable bit.
 *****/

void sCPU::op_io() {
  status.clock_count = 6;
  precycle_edge();
  add_clocks(6);
  cycle_edge();
}

uint8 sCPU::op_read(uint32 addr) {
  status.clock_count = r_mem->speed(addr);
  precycle_edge();
  add_clocks(status.clock_count - 4);
  regs.mdr = r_mem->read(addr);
  add_clocks(4);
  cycle_edge();
  return regs.mdr;
}

void sCPU::op_write(uint32 addr, uint8 data) {
  status.clock_count = r_mem->speed(addr);
  precycle_edge();
  add_clocks(status.clock_count);
  regs.mdr = data;
  r_mem->write(addr, regs.mdr);
  cycle_edge();
}

//

alwaysinline uint8 sCPU::op_readpc() {
  return op_read((regs.pc.b << 16) + regs.pc.w++);
}

alwaysinline uint8 sCPU::op_readstack() {
  if(regs.e) {
    regs.s.l++;
  } else {
    regs.s.w++;
  }
  return op_read(regs.s.w);
}

alwaysinline uint8 sCPU::op_readstackn() {
  return op_read(++regs.s.w);
}

alwaysinline uint8 sCPU::op_readaddr(uint32 addr) {
  return op_read(addr & 0xffff);
}

alwaysinline uint8 sCPU::op_readlong(uint32 addr) {
  return op_read(addr & 0xffffff);
}

alwaysinline uint8 sCPU::op_readdbr(uint32 addr) {
  return op_read(((regs.db << 16) + addr) & 0xffffff);
}

alwaysinline uint8 sCPU::op_readpbr(uint32 addr) {
  return op_read((regs.pc.b << 16) + (addr & 0xffff));
}

alwaysinline uint8 sCPU::op_readdp(uint32 addr) {
  if(regs.e && regs.d.l == 0x00) {
    return op_read((regs.d & 0xff00) + ((regs.d + (addr & 0xffff)) & 0xff));
  } else {
    return op_read((regs.d + (addr & 0xffff)) & 0xffff);
  }
}

alwaysinline uint8 sCPU::op_readsp(uint32 addr) {
  return op_read((regs.s + (addr & 0xffff)) & 0xffff);
}

alwaysinline void sCPU::op_writestack(uint8 data) {
  op_write(regs.s.w, data);
  if(regs.e) {
    regs.s.l--;
  } else {
    regs.s.w--;
  }
}

alwaysinline void sCPU::op_writestackn(uint8 data) {
  op_write(regs.s.w--, data);
}

alwaysinline void sCPU::op_writeaddr(uint32 addr, uint8 data) {
  op_write(addr & 0xffff, data);
}

alwaysinline void sCPU::op_writelong(uint32 addr, uint8 data) {
  op_write(addr & 0xffffff, data);
}

alwaysinline void sCPU::op_writedbr(uint32 addr, uint8 data) {
  op_write(((regs.db << 16) + addr) & 0xffffff, data);
}

alwaysinline void sCPU::op_writepbr(uint32 addr, uint8 data) {
  op_write((regs.pc.b << 16) + (addr & 0xffff), data);
}

alwaysinline void sCPU::op_writedp(uint32 addr, uint8 data) {
  if(regs.e && regs.d.l == 0x00) {
    op_write((regs.d & 0xff00) + ((regs.d + (addr & 0xffff)) & 0xff), data);
  } else {
    op_write((regs.d + (addr & 0xffff)) & 0xffff, data);
  }
}

alwaysinline void sCPU::op_writesp(uint32 addr, uint8 data) {
  op_write((regs.s + (addr & 0xffff)) & 0xffff, data);
}
