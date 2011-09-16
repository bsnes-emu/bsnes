uint8 CPU::op_read(uint16 addr) {
  if(status.oam_dma_pending) {
    status.oam_dma_pending = false;
    op_read(addr);
    oam_dma();
  }

  while(status.rdy_line == 0) {
    regs.mdr = bus.read(status.rdy_addr ? status.rdy_addr() : addr);
    add_clocks(12);
  }

  regs.mdr = bus.read(addr);
  add_clocks(12);
  return regs.mdr;
}

void CPU::op_write(uint16 addr, uint8 data) {
  add_clocks(12);
  bus.write(addr, regs.mdr = data);
}

//

uint8 CPU::op_readpc() {
  return op_read(regs.pc);
}

uint8 CPU::op_readpci() {
  return op_read(regs.pc++);
}

uint8 CPU::op_readsp() {
  return op_read(0x0100 | ++regs.s);
}

uint8 CPU::op_readzp(uint8 addr) {
  return op_read(addr);
}

//

void CPU::op_writesp(uint8 data) {
  op_write(0x0100 | regs.s--, data);
}

void CPU::op_writezp(uint8 addr, uint8 data) {
  op_write(addr, data);
}

//

void CPU::op_page(uint16 x, uint16 y) {
  if((x & 0xff00) != (y & 0xff00)) op_read((x & 0xff00) | (y & 0x00ff));
}

void CPU::op_page_always(uint16 x, uint16 y) {
  op_read((x & 0xff00) | (y & 0x00ff));
}
