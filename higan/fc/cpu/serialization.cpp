void CPU::serialize(serializer &s) {
  R6502::serialize(s);
  Thread::serialize(s);

  s.array(ram);

  s.integer(status.interrupt_pending);
  s.integer(status.nmi_pending);
  s.integer(status.nmi_line);
  s.integer(status.irq_line);
  s.integer(status.irq_apu_line);

  s.integer(status.rdy_line);
  bool rdy_addr_valid = status.rdy_addr;
  unsigned rdy_addr_value = 0;
  if(rdy_addr_valid) rdy_addr_value = status.rdy_addr();
  s.integer(rdy_addr_valid);
  s.integer(rdy_addr_value);
  if(rdy_addr_valid) status.rdy_addr = rdy_addr_value;

  s.integer(status.oam_dma_pending);
  s.integer(status.oam_dma_page);

  s.integer(status.controller_latch);
  s.integer(status.controller_port0);
  s.integer(status.controller_port1);
}
