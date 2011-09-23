void CPU::serialize(serializer &s) {
  Processor::serialize(s);

  s.array(ram);

  s.integer(regs.mdr);
  s.integer(regs.pc);
  s.integer(regs.a);
  s.integer(regs.x);
  s.integer(regs.y);
  s.integer(regs.s);
  s.integer(regs.p.n);
  s.integer(regs.p.v);
  s.integer(regs.p.d);
  s.integer(regs.p.i);
  s.integer(regs.p.z);
  s.integer(regs.p.c);

  s.integer(abs.w);
  s.integer(iabs.w);
  s.integer(rd);
  s.integer(zp);
  s.integer(aa);

  s.integer(status.interrupt_pending);
  s.integer(status.nmi_pending);
  s.integer(status.nmi_line);
  s.integer(status.irq_line);
  s.integer(status.irq_apu_line);

  s.integer(status.rdy_line);
  s.integer(status.rdy_addr.valid);
  s.integer(status.rdy_addr.value);

  s.integer(status.oam_dma_pending);
  s.integer(status.oam_dma_page);

  s.integer(status.controller_latch);
  s.integer(status.controller_port0);
  s.integer(status.controller_port1);
}
