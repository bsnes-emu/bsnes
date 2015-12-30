alwaysinline auto op_readpc() -> uint8 {
  return op_read((regs.pc.b << 16) + regs.pc.w++);
}

alwaysinline auto op_readstack() -> uint8 {
  regs.e ? regs.s.l++ : regs.s.w++;
  return op_read(regs.s.w);
}

alwaysinline auto op_readstackn() -> uint8 {
  return op_read(++regs.s.w);
}

alwaysinline auto op_readaddr(uint32 addr) -> uint8 {
  return op_read(addr & 0xffff);
}

alwaysinline auto op_readlong(uint32 addr) -> uint8 {
  return op_read(addr & 0xffffff);
}

alwaysinline auto op_readdbr(uint32 addr) -> uint8 {
  return op_read(((regs.db << 16) + addr) & 0xffffff);
}

alwaysinline auto op_readpbr(uint32 addr) -> uint8 {
  return op_read((regs.pc.b << 16) + (addr & 0xffff));
}

alwaysinline auto op_readdp(uint32 addr) -> uint8 {
  if(regs.e && regs.d.l == 0x00) {
    return op_read((regs.d & 0xff00) + ((regs.d + (addr & 0xffff)) & 0xff));
  } else {
    return op_read((regs.d + (addr & 0xffff)) & 0xffff);
  }
}

alwaysinline auto op_readsp(uint32 addr) -> uint8 {
  return op_read((regs.s + (addr & 0xffff)) & 0xffff);
}

alwaysinline auto op_writestack(uint8 data) -> void {
  op_write(regs.s.w, data);
  regs.e ? regs.s.l-- : regs.s.w--;
}

alwaysinline auto op_writestackn(uint8 data) -> void {
  op_write(regs.s.w--, data);
}

alwaysinline auto op_writeaddr(uint32 addr, uint8 data) -> void {
  op_write(addr & 0xffff, data);
}

alwaysinline auto op_writelong(uint32 addr, uint8 data) -> void {
  op_write(addr & 0xffffff, data);
}

alwaysinline auto op_writedbr(uint32 addr, uint8 data) -> void {
  op_write(((regs.db << 16) + addr) & 0xffffff, data);
}

alwaysinline auto op_writepbr(uint32 addr, uint8 data) -> void {
  op_write((regs.pc.b << 16) + (addr & 0xffff), data);
}

alwaysinline auto op_writedp(uint32 addr, uint8 data) -> void {
  if(regs.e && regs.d.l == 0x00) {
    op_write((regs.d & 0xff00) + ((regs.d + (addr & 0xffff)) & 0xff), data);
  } else {
    op_write((regs.d + (addr & 0xffff)) & 0xffff, data);
  }
}

alwaysinline auto op_writesp(uint32 addr, uint8 data) -> void {
  op_write((regs.s + (addr & 0xffff)) & 0xffff, data);
}
