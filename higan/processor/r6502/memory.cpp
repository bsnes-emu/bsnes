auto R6502::op_readpc() -> uint8 {
  return op_read(regs.pc);
}

auto R6502::op_readpci() -> uint8 {
  return op_read(regs.pc++);
}

auto R6502::op_readsp() -> uint8 {
  return op_read(0x0100 | ++regs.s);
}

auto R6502::op_readzp(uint8 addr) -> uint8 {
  return op_read(addr);
}

//

auto R6502::op_writesp(uint8 data) -> void {
  op_write(0x0100 | regs.s--, data);
}

auto R6502::op_writezp(uint8 addr, uint8 data) -> void {
  op_write(addr, data);
}

//

auto R6502::op_page(uint16 x, uint16 y) -> void {
  if((x & 0xff00) != (y & 0xff00)) op_read((x & 0xff00) | (y & 0x00ff));
}

auto R6502::op_page_always(uint16 x, uint16 y) -> void {
  op_read((x & 0xff00) | (y & 0x00ff));
}
