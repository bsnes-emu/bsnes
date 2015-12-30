alwaysinline auto op_readpc() -> uint8 {
  return op_read(regs.pc++);
}

alwaysinline auto op_readsp() -> uint8 {
  return op_read(0x0100 | ++regs.s);
}

alwaysinline auto op_writesp(uint8 data) -> void {
  return op_write(0x0100 | regs.s--, data);
}

alwaysinline auto op_readdp(uint8 addr) -> uint8 {
  return op_read((regs.p.p << 8) + addr);
}

alwaysinline auto op_writedp(uint8 addr, uint8 data) -> void {
  return op_write((regs.p.p << 8) + addr, data);
}
