//nmiPoll() and irqPoll() are called once every four clock cycles;
//as NMI steps by scanlines (divisible by 4) and IRQ by PPU 4-cycle dots.
//
//ppu.(vh)counter(n) returns the value of said counters n-clocks before current time;
//it is used to emulate hardware communication delay between opcode and interrupt units.

auto CPU::nmiPoll() -> void {
  //NMI hold
  if(status.nmiHold.lower() && io.nmiEnable) {
    status.nmiTransition = 1;
  }

  //NMI test
  if(status.nmiValid.flip(vcounter(2) >= ppu.vdisp())) {
    if(status.nmiLine = status.nmiValid) status.nmiHold = 1;  //hold /NMI for four cycles
  }
}

auto CPU::irqPoll() -> void {
  //IRQ hold
  status.irqHold = 0;
  if(status.irqLine && io.irqEnable) {
    status.irqTransition = 1;
  }

  //IRQ test
  //the IRQ unit's H and V counters are driven by the PPU's blanking signals and
  //differ from the PPU's own at the start of each scanline: at hclock 2 they
  //still read the previous line's V and H=339, at hclock 6 and 10 both read H=0
  //on the first scanline of a field V is cleared at hclock 2 (end of vblank)
  //instead of advancing at hclock 6, so V=0 IRQs raise one poll earlier, and
  //V=last,H=339 can never match: H=339 is only seen once V is already 0
  uint vtime = vcounter(10);
  if(vcounter() == 0 && hcounter() == 6) vtime = 0;
  //H=0 is held for two polls, so HTIME=0 (stored as 4) matches one poll earlier
  //than HTIME>=1
  uint htime = hcounter(io.htime == 4 ? 6 : 10);
  if(status.irqValid.raise(io.irqEnable
  && (!io.virqEnable || vtime == io.vtime)
  && (!io.hirqEnable || htime == io.htime)
  )) status.irqLine = status.irqHold = 1;  //hold /IRQ for four cycles
}

auto CPU::nmitimenUpdate(uint8 data) -> void {
  io.hirqEnable = data & 0x10;
  io.virqEnable = data & 0x20;
  io.irqEnable = io.hirqEnable || io.virqEnable;

  if(io.virqEnable && !io.hirqEnable && status.irqLine) {
    status.irqTransition = 1;
  } else if(!io.irqEnable) {
    status.irqLine = 0;
    status.irqTransition = 0;
  }

  if(io.nmiEnable.raise(data & 0x80) && status.nmiLine) {
    status.nmiTransition = 1;
  }

  status.irqLock = 1;
}

auto CPU::rdnmi() -> bool {
  bool result = status.nmiLine;
  if(!status.nmiHold) {
    status.nmiLine = 0;
  }
  return result;
}

auto CPU::timeup() -> bool {
  bool result = status.irqLine;
  if(!status.irqHold) {
    status.irqLine = 0;
    status.irqTransition = 0;
  }
  return result;
}

auto CPU::nmiTest() -> bool {
  if(!status.nmiTransition) return 0;
  status.nmiTransition = 0;
  r.wai = 0;
  return 1;
}

auto CPU::irqTest() -> bool {
  if(!status.irqTransition && !r.irq) return 0;
  status.irqTransition = 0;
  r.wai = 0;
  return !r.p.i;
}

//used to test for NMI/IRQ, which can trigger on the edge of every opcode.
//test one cycle early to simulate two-stage pipeline of the 65816 CPU.
//
//status.irqLock is used to simulate hardware delay before interrupts can
//trigger during certain events (immediately after DMA, writes to $4200, etc)
auto CPU::lastCycle() -> void {
  if(!status.irqLock) {
    if(nmiTest()) status.nmiPending = 1, status.interruptPending = 1;
    if(irqTest()) status.irqPending = 1, status.interruptPending = 1;
  }
}
