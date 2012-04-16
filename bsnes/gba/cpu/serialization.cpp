void CPU::serialize(serializer &s) {
  ARM::serialize(s);
  Thread::serialize(s);

  s.array(iwram,  32 * 1024);
  s.array(ewram, 256 * 1024);

  for(auto &dma : regs.dma) {
    s.integer(dma.source);
    s.integer(dma.target);
    s.integer(dma.length);
    s.integer(dma.control.targetmode);
    s.integer(dma.control.sourcemode);
    s.integer(dma.control.repeat);
    s.integer(dma.control.size);
    s.integer(dma.control.drq);
    s.integer(dma.control.timingmode);
    s.integer(dma.control.irq);
    s.integer(dma.control.enable);
    s.integer(dma.run.target);
    s.integer(dma.run.source);
    s.integer(dma.run.length);
  }

  for(auto &timer : regs.timer) {
    s.integer(timer.period);
    s.integer(timer.reload);
    s.integer(timer.control.frequency);
    s.integer(timer.control.cascade);
    s.integer(timer.control.irq);
    s.integer(timer.control.enable);
  }

  for(auto &flag : regs.keypad.control.flag) s.integer(flag);
  s.integer(regs.keypad.control.enable);
  s.integer(regs.keypad.control.condition);

  s.integer(regs.ime);

  s.integer(regs.irq.enable.vblank);
  s.integer(regs.irq.enable.hblank);
  s.integer(regs.irq.enable.vcoincidence);
  for(auto &flag : regs.irq.enable.timer) s.integer(flag);
  s.integer(regs.irq.enable.serial);
  for(auto &flag : regs.irq.enable.dma) s.integer(flag);
  s.integer(regs.irq.enable.keypad);
  s.integer(regs.irq.enable.cartridge);

  s.integer(regs.irq.flag.vblank);
  s.integer(regs.irq.flag.hblank);
  s.integer(regs.irq.flag.vcoincidence);
  for(auto &flag : regs.irq.flag.timer) s.integer(flag);
  s.integer(regs.irq.flag.serial);
  for(auto &flag : regs.irq.flag.dma) s.integer(flag);
  s.integer(regs.irq.flag.keypad);
  s.integer(regs.irq.flag.cartridge);

  for(auto &flag : regs.wait.control.nwait) s.integer(flag);
  for(auto &flag : regs.wait.control.swait) s.integer(flag);
  s.integer(regs.wait.control.phi);
  s.integer(regs.wait.control.prefetch);
  s.integer(regs.wait.control.gametype);

  s.integer(regs.memory.control.disable);
  s.integer(regs.memory.control.unknown1);
  s.integer(regs.memory.control.ewram);
  s.integer(regs.memory.control.ewramwait);
  s.integer(regs.memory.control.unknown2);

  s.integer(regs.postboot);
  s.integer((unsigned&)regs.mode);
  s.integer(regs.clock);

  s.integer(pending.dma.vblank);
  s.integer(pending.dma.hblank);
  s.integer(pending.dma.hdma);
}
