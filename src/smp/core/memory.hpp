alwaysinline uint8 op_readpc() {
  return op_read(regs.pc++);
}

alwaysinline uint8 op_readstack() {
  return op_read(0x0100 | ++regs.sp);
}

alwaysinline void op_writestack(uint8 data) {
  op_write(0x0100 | regs.sp--, data);
}

alwaysinline uint8 op_readaddr(uint16 addr) {
  return op_read(addr);
}

alwaysinline void op_writeaddr(uint16 addr, uint8 data) {
  op_write(addr, data);
}

alwaysinline uint8 op_readdp(uint8 addr) {
  return op_read((unsigned(regs.p.p) << 8) + addr);
}

alwaysinline void op_writedp(uint8 addr, uint8 data) {
  op_write((unsigned(regs.p.p) << 8) + addr, data);
}
