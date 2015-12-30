auto CPU::op_read(uint16 addr) -> uint8 {
  if(status.oam_dma_pending) {
    status.oam_dma_pending = false;
    op_read(addr);
    oam_dma();
  }

  while(status.rdy_line == 0) {
    regs.mdr = bus.read(status.rdy_addr_valid ? status.rdy_addr_value : addr);
    add_clocks(12);
  }

  regs.mdr = bus.read(addr);
  add_clocks(12);
  return regs.mdr;
}

auto CPU::op_write(uint16 addr, uint8 data) -> void {
  bus.write(addr, regs.mdr = data);
  add_clocks(12);
}

auto CPU::last_cycle() -> void {
  status.interrupt_pending = ((status.irq_line | status.irq_apu_line) & ~regs.p.i) | status.nmi_pending;
}

auto CPU::nmi(uint16& vector) -> void {
  if(status.nmi_pending) {
    status.nmi_pending = false;
    vector = 0xfffa;
  }
}

auto CPU::oam_dma() -> void {
  for(uint n : range(256)) {
    uint8 data = op_read((status.oam_dma_page << 8) + n);
    op_write(0x2004, data);
  }
}

auto CPU::set_nmi_line(bool line) -> void {
  //edge-sensitive (0->1)
  if(!status.nmi_line && line) status.nmi_pending = true;
  status.nmi_line = line;
}

auto CPU::set_irq_line(bool line) -> void {
  //level-sensitive
  status.irq_line = line;
}

auto CPU::set_irq_apu_line(bool line) -> void {
  //level-sensitive
  status.irq_apu_line = line;
}

auto CPU::set_rdy_line(bool line) -> void {
  status.rdy_line = line;
}

auto CPU::set_rdy_addr(bool valid, uint16 value) -> void {
  status.rdy_addr_valid = valid;
  status.rdy_addr_value = value;
}
