#ifdef SCPU_CPP

void sCPU::update_interrupts() {
  unsigned vtime = status.virq_pos;
  unsigned htime = status.hirq_enabled ? status.hirq_pos : 0;
  unsigned vlimit = (snes.region() == SNES::NTSC ? 525 : 625) >> 1;

  //an IRQ for the very last dot of a field cannot trigger an IRQ
  if((vtime == (vlimit - 1) && htime == 339 && ppu.interlace() == false)
  || (vtime == vlimit && htime == 339)
  ) {
    vtime = 0x03ff;
    htime = 0x03ff;
  }

  status.virq_trigger_pos = vtime;
  status.hirq_trigger_pos = 4 * (status.hirq_enabled ? htime + 1 : 0);
}

alwaysinline void sCPU::poll_interrupts() {
  uint16_t vpos, hpos;

  //NMI hold
  if(status.nmi_hold) {
    status.nmi_hold -= 2;
    if(status.nmi_hold == 0) {
      if(status.nmi_enabled == true) status.nmi_transition = true;
    }
  }

  //NMI test
  vpos = ppu.vcounter(2);
  hpos = ppu.hcounter(2);
  bool nmi_valid = (vpos >= (!ppu.overscan() ? 225 : 240));
  if(status.nmi_valid == false && nmi_valid == true) {
    //0->1 edge sensitive transition
    status.nmi_line = true;
    status.nmi_hold = 4;
  } else if(status.nmi_valid == true && nmi_valid == false) {
    //1->0 edge sensitive transition
    status.nmi_line = false;
  }
  status.nmi_valid = nmi_valid;

  //IRQ hold
  if(status.irq_hold) status.irq_hold -= 2;
  if(status.irq_line == true && status.irq_hold == 0) {
    if(status.virq_enabled == true || status.hirq_enabled == true) status.irq_transition = true;
  }

  //IRQ test
  vpos = ppu.vcounter(10);
  hpos = ppu.hcounter(10);
  bool irq_valid = (status.virq_enabled == true || status.hirq_enabled == true);
  if(irq_valid == true) {
    if(status.virq_enabled == true && vpos != status.virq_trigger_pos) irq_valid = false;
    if(status.hirq_enabled == true && hpos != status.hirq_trigger_pos) irq_valid = false;
  }
  if(status.irq_valid == false && irq_valid == true) {
    //0->1 edge sensitive transition
    status.irq_line = true;
    status.irq_hold = 4;
  }
  status.irq_valid = irq_valid;
}

void sCPU::nmitimen_update(uint8 data) {
  bool nmi_enabled  = status.nmi_enabled;
  bool virq_enabled = status.virq_enabled;
  bool hirq_enabled = status.hirq_enabled;
  status.nmi_enabled  = !!(data & 0x80);
  status.virq_enabled = !!(data & 0x20);
  status.hirq_enabled = !!(data & 0x10);

  //0->1 edge sensitive transition
  if(nmi_enabled == false && status.nmi_enabled == true && status.nmi_line == true) {
    status.nmi_transition = true;
  }

  //?->1 level sensitive transition
  if(status.virq_enabled == true && status.hirq_enabled == false && status.irq_line == true) {
    status.irq_transition = true;
  }

  if(status.virq_enabled == false && status.hirq_enabled == false) {
    status.irq_line = false;
    status.irq_transition = false;
  }

  update_interrupts();
  status.irq_lock = true;
  delta.enqueue(EventIrqLockRelease, 2);
}

void sCPU::hvtime_update(uint16 addr) {
  update_interrupts();
}

bool sCPU::rdnmi() {
  bool result = status.nmi_line;
  if(status.nmi_hold == 0) {
    status.nmi_line = false;
  }
  return result;
}

bool sCPU::timeup() {
  bool result = status.irq_line;
  if(status.irq_hold == 0) {
    status.irq_line = false;
    status.irq_transition = false;
  }
  return result;
}

alwaysinline bool sCPU::nmi_test() {
  if(status.nmi_transition == false) return false;
  status.nmi_transition = false;
  event.wai = false;
  return true;
}

alwaysinline bool sCPU::irq_test() {
  if(status.irq_transition == false) return false;
  status.irq_transition = false;
  event.wai = false;
  return !regs.p.i;
}

#endif  //ifdef SCPU_CPP
