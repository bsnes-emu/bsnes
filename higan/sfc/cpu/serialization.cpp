auto CPU::serialize(serializer& s) -> void {
  R65816::serialize(s);
  Thread::serialize(s);
  PPUcounter::serialize(s);

  s.array(wram);

  s.integer(version);

  s.integer(status.interruptPending);

  s.integer(status.clockCount);
  s.integer(status.lineClocks);

  s.integer(status.irqLock);

  s.integer(status.dramRefreshPosition);
  s.integer(status.dramRefreshed);

  s.integer(status.hdmaInitPosition);
  s.integer(status.hdmaInitTriggered);

  s.integer(status.hdmaPosition);
  s.integer(status.hdmaTriggered);

  s.integer(status.nmiValid);
  s.integer(status.nmiLine);
  s.integer(status.nmiTransition);
  s.integer(status.nmiPending);
  s.integer(status.nmiHold);

  s.integer(status.irqValid);
  s.integer(status.irqLine);
  s.integer(status.irqTransition);
  s.integer(status.irqPending);
  s.integer(status.irqHold);

  s.integer(status.powerPending);
  s.integer(status.resetPending);

  s.integer(status.dmaActive);
  s.integer(status.dmaCounter);
  s.integer(status.dmaClocks);
  s.integer(status.dmaPending);
  s.integer(status.hdmaPending);
  s.integer(status.hdmaMode);

  s.integer(status.autoJoypadActive);
  s.integer(status.autoJoypadLatch);
  s.integer(status.autoJoypadCounter);
  s.integer(status.autoJoypadClock);

  s.array(status.port);

  s.integer(status.wramAddress);

  s.integer(status.joypadStrobeLatch);
  s.integer(status.joypad1_bits);
  s.integer(status.joypad2_bits);

  s.integer(status.nmiEnabled);
  s.integer(status.hirqEnabled);
  s.integer(status.virqEnabled);
  s.integer(status.autoJoypadPoll);

  s.integer(status.pio);

  s.integer(status.wrmpya);
  s.integer(status.wrmpyb);

  s.integer(status.wrdiva);
  s.integer(status.wrdivb);

  s.integer(status.hirqPos);
  s.integer(status.virqPos);

  s.integer(status.romSpeed);

  s.integer(status.rddiv);
  s.integer(status.rdmpy);

  s.integer(status.joy1);
  s.integer(status.joy2);
  s.integer(status.joy3);
  s.integer(status.joy4);

  s.integer(alu.mpyctr);
  s.integer(alu.divctr);
  s.integer(alu.shift);

  for(uint n : range(8)) {
    s.integer(channel[n].dmaEnabled);
    s.integer(channel[n].hdmaEnabled);
    s.integer(channel[n].direction);
    s.integer(channel[n].indirect);
    s.integer(channel[n].unused);
    s.integer(channel[n].reverseTransfer);
    s.integer(channel[n].fixedTransfer);
    s.integer(channel[n].transferMode);
    s.integer(channel[n].targetAddress);
    s.integer(channel[n].sourceAddress);
    s.integer(channel[n].sourceBank);
    s.integer(channel[n].transferSize);
    s.integer(channel[n].indirectBank);
    s.integer(channel[n].hdmaAddress);
    s.integer(channel[n].lineCounter);
    s.integer(channel[n].unknown);
    s.integer(channel[n].hdmaCompleted);
    s.integer(channel[n].hdmaDoTransfer);
  }

  s.integer(pipe.valid);
  s.integer(pipe.addr);
  s.integer(pipe.data);
}
