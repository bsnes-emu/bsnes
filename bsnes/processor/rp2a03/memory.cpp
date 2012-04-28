uint8 RP2A03::op_readpc() {
  return op_read(regs.pc);
}

uint8 RP2A03::op_readpci() {
  return op_read(regs.pc++);
}

uint8 RP2A03::op_readsp() {
  return op_read(0x0100 | ++regs.s);
}

uint8 RP2A03::op_readzp(uint8 addr) {
  return op_read(addr);
}

//

void RP2A03::op_writesp(uint8 data) {
  op_write(0x0100 | regs.s--, data);
}

void RP2A03::op_writezp(uint8 addr, uint8 data) {
  op_write(addr, data);
}

//

void RP2A03::op_page(uint16 x, uint16 y) {
  if((x & 0xff00) != (y & 0xff00)) op_read((x & 0xff00) | (y & 0x00ff));
}

void RP2A03::op_page_always(uint16 x, uint16 y) {
  op_read((x & 0xff00) | (y & 0x00ff));
}
