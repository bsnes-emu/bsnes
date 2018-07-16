auto CPU::serialize(serializer& s) -> void {
  LR35902::serialize(s);
  Thread::serialize(s);

  s.array(wram);
  s.array(hram);

  s.integer(status.clock);

  s.integer(status.p15);
  s.integer(status.p14);
  s.integer(status.joyp);

  s.integer(status.serialData);
  s.integer(status.serialBits);

  s.integer(status.serialTransfer);
  s.integer(status.serialClock);

  s.integer(status.div);
  s.integer(status.tima);
  s.integer(status.tma);
  s.integer(status.timerEnable);
  s.integer(status.timerClock);

  s.integer(status.interruptRequestJoypad);
  s.integer(status.interruptRequestSerial);
  s.integer(status.interruptRequestTimer);
  s.integer(status.interruptRequestStat);
  s.integer(status.interruptRequestVblank);

  s.integer(status.speedDouble);
  s.integer(status.speedSwitch);

  s.integer(status.dmaSource);
  s.integer(status.dmaTarget);
  s.integer(status.dmaMode);
  s.integer(status.dmaLength);
  s.integer(status.dmaCompleted);

  s.integer(status.ff6c);

  s.integer(status.wramBank);

  s.integer(status.ff72);
  s.integer(status.ff73);
  s.integer(status.ff74);
  s.integer(status.ff75);

  s.integer(status.interruptEnableJoypad);
  s.integer(status.interruptEnableSerial);
  s.integer(status.interruptEnableTimer);
  s.integer(status.interruptEnableStat);
  s.integer(status.interruptEnableVblank);
}
