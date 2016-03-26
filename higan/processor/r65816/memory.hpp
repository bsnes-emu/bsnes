alwaysinline auto readpc() -> uint8 {
  return read((regs.pc.b << 16) + regs.pc.w++);
}

alwaysinline auto readstack() -> uint8 {
  regs.e ? regs.s.l++ : regs.s.w++;
  return read(regs.s.w);
}

alwaysinline auto readstackn() -> uint8 {
  return read(++regs.s.w);
}

alwaysinline auto readaddr(uint32 addr) -> uint8 {
  return read(addr & 0xffff);
}

alwaysinline auto readlong(uint32 addr) -> uint8 {
  return read(addr & 0xffffff);
}

alwaysinline auto readdbr(uint32 addr) -> uint8 {
  return read(((regs.db << 16) + addr) & 0xffffff);
}

alwaysinline auto readpbr(uint32 addr) -> uint8 {
  return read((regs.pc.b << 16) + (addr & 0xffff));
}

alwaysinline auto readdp(uint32 addr) -> uint8 {
  if(regs.e && regs.d.l == 0x00) {
    return read((regs.d & 0xff00) + ((regs.d + (addr & 0xffff)) & 0xff));
  } else {
    return read((regs.d + (addr & 0xffff)) & 0xffff);
  }
}

alwaysinline auto readsp(uint32 addr) -> uint8 {
  return read((regs.s + (addr & 0xffff)) & 0xffff);
}

alwaysinline auto writestack(uint8 data) -> void {
  write(regs.s.w, data);
  regs.e ? regs.s.l-- : regs.s.w--;
}

alwaysinline auto writestackn(uint8 data) -> void {
  write(regs.s.w--, data);
}

alwaysinline auto writeaddr(uint32 addr, uint8 data) -> void {
  write(addr & 0xffff, data);
}

alwaysinline auto writelong(uint32 addr, uint8 data) -> void {
  write(addr & 0xffffff, data);
}

alwaysinline auto writedbr(uint32 addr, uint8 data) -> void {
  write(((regs.db << 16) + addr) & 0xffffff, data);
}

alwaysinline auto writepbr(uint32 addr, uint8 data) -> void {
  write((regs.pc.b << 16) + (addr & 0xffff), data);
}

alwaysinline auto writedp(uint32 addr, uint8 data) -> void {
  if(regs.e && regs.d.l == 0x00) {
    write((regs.d & 0xff00) + ((regs.d + (addr & 0xffff)) & 0xff), data);
  } else {
    write((regs.d + (addr & 0xffff)) & 0xffff, data);
  }
}

alwaysinline auto writesp(uint32 addr, uint8 data) -> void {
  write((regs.s + (addr & 0xffff)) & 0xffff, data);
}
