//called once every four clock cycles;
//as NMI steps by scanlines (divisible by 4) and IRQ by PPU 4-cycle dots.
//
//ppu.(vh)counter(n) returns the value of said counters n-clocks before current time;
//it is used to emulate hardware communication delay between opcode and interrupt units.
auto CPU::pollInterrupts() -> void {
  //NMI hold
  if(status.nmiHold) {
    status.nmiHold = false;
    if(status.nmiEnabled) status.nmiTransition = true;
  }

  //NMI test
  bool nmiValid = vcounter(2) >= ppu.vdisp();
  if(!status.nmiValid && nmiValid) {
    //0->1 edge sensitive transition
    status.nmiLine = true;
    status.nmiHold = true;  //hold /NMI for four cycles
  } else if(status.nmiValid && !nmiValid) {
    //1->0 edge sensitive transition
    status.nmiLine = false;
  }
  status.nmiValid = nmiValid;

  //IRQ hold
  status.irqHold = false;
  if(status.irqLine) {
    if(status.virqEnabled || status.hirqEnabled) status.irqTransition = true;
  }

  //IRQ test
  bool irqValid = status.virqEnabled || status.hirqEnabled;
  if(irqValid) {
    if((status.virqEnabled && vcounter(10) != (status.virqPos))
    || (status.hirqEnabled && hcounter(10) != (status.hirqPos + 1) * 4)
    || (status.virqPos && vcounter(6) == 0)  //IRQs cannot trigger on last dot of field
    ) irqValid = false;
  }
  if(!status.irqValid && irqValid) {
    //0->1 edge sensitive transition
    status.irqLine = true;
    status.irqHold = true;  //hold /IRQ for four cycles
  }
  status.irqValid = irqValid;
}

auto CPU::nmitimenUpdate(uint8 data) -> void {
  bool nmiEnabled  = status.nmiEnabled;
  bool virqEnabled = status.virqEnabled;
  bool hirqEnabled = status.hirqEnabled;
  status.nmiEnabled  = data & 0x80;
  status.virqEnabled = data & 0x20;
  status.hirqEnabled = data & 0x10;

  //0->1 edge sensitive transition
  if(!nmiEnabled && status.nmiEnabled && status.nmiLine) {
    status.nmiTransition = true;
  }

  //?->1 level sensitive transition
  if(status.virqEnabled && !status.hirqEnabled && status.irqLine) {
    status.irqTransition = true;
  }

  if(!status.virqEnabled && !status.hirqEnabled) {
    status.irqLine = false;
    status.irqTransition = false;
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
