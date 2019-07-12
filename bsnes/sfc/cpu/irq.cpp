//called once every four clock cycles;
//as NMI steps by scanlines (divisible by 4) and IRQ by PPU 4-cycle dots.
//
//ppu.(vh)counter(n) returns the value of said counters n-clocks before current time;
//it is used to emulate hardware communication delay between opcode and interrupt units.
auto CPU::pollInterrupts() -> void {
  //NMI hold
  if(status.nmiHold.lower() && io.nmiEnable) status.nmiTransition = true;

  //NMI test
  if(status.nmiValid.flip(vcounter(2) >= ppu.vdisp())) {
    if(status.nmiLine = status.nmiValid) status.nmiHold = true;  //hold /NMI for four cycles
  }

  //IRQ hold
  status.irqHold = false;
  if(status.irqLine && io.irqEnable) status.irqTransition = true;

  //IRQ test
  if(status.irqValid.raise(io.irqEnable
  && (!io.virqEnable || vcounter(10) == io.vtime)
  && (!io.hirqEnable || hcounter(10) == io.htime)
  && (vcounter(6) || hcounter(6))  //IRQs cannot trigger on last dot of fields
  )) status.irqLine = status.irqHold = true;  //hold /IRQ for four cycles
}

auto CPU::nmitimenUpdate(uint8 data) -> void {
  io.hirqEnable = bit1(data,4);
  io.virqEnable = bit1(data,5);
  io.irqEnable = io.hirqEnable || io.virqEnable;

  if(io.virqEnable && !io.hirqEnable && status.irqLine) {
    status.irqTransition = true;
  } else if(!io.irqEnable) {
    status.irqLine = false;
    status.irqTransition = false;
  }

  if(io.nmiEnable.raise(bit1(data,7)) && status.nmiLine) {
    status.nmiTransition = true;
  }

  status.irqLock = true;
}

auto CPU::rdnmi() -> bool {
  bool result = status.nmiLine;
  if(!status.nmiHold) {
    status.nmiLine = false;
  }
  return result;
}

auto CPU::timeup() -> bool {
  bool result = status.irqLine;
  if(!status.irqHold) {
    status.irqLine = false;
    status.irqTransition = false;
  }
  return result;
}

auto CPU::nmiTest() -> bool {
  if(!status.nmiTransition) return false;
  status.nmiTransition = false;
  r.wai = false;
  return true;
}

auto CPU::irqTest() -> bool {
  if(!status.irqTransition && !r.irq) return false;
  status.irqTransition = false;
  r.wai = false;
  return !r.p.i;
}
