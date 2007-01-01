void sCPU::update_interrupts() {
  status.vnmi_trigger_pos = status.vblstart;

  if(irq_pos_valid() == true) {
    status.virq_trigger_pos = status.virq_pos;
    status.hirq_trigger_pos = 4 * ((status.hirq_enabled) ? (status.hirq_pos + 1) : 0);
  } else {
    status.virq_trigger_pos = IRQ_TRIGGER_NEVER;
    status.hirq_trigger_pos = IRQ_TRIGGER_NEVER;
  }
}

alwaysinline
void sCPU::poll_interrupts() {
uint vpos = status.vcounter, hpos = status.hclock;

//NMI test
  timeshift_backward(2, vpos, hpos);
bool nmi_valid = (vpos >= status.vnmi_trigger_pos);
  if(status.nmi_valid == false && nmi_valid == true) {
  //0->1 edge sensitive transition
    status.nmi_line = true;
    counter.nmi_hold = 6;
  } else if(status.nmi_valid == true && nmi_valid == false) {
  //1->0 edge sensitive transition
    status.nmi_line = false;
  }
  status.nmi_valid = nmi_valid;

//NMI hold
  if(counter.nmi_hold) {
    counter.nmi_hold -= 2;
    if(counter.nmi_hold == 0) {
      if(status.nmi_enabled == true) { status.nmi_transition = true; }
    }
  }

//IRQ test
  timeshift_backward(8, vpos, hpos);
bool irq_valid = (status.virq_enabled == true || status.hirq_enabled == true);
  if(irq_valid == true) {
    if(status.virq_enabled == true && vpos != status.virq_trigger_pos) { irq_valid = false; }
    if(status.hirq_enabled == true && hpos != status.hirq_trigger_pos) { irq_valid = false; }
  }
  if(status.irq_valid == false && irq_valid == true) {
  //0->1 edge sensitive transition
    status.irq_line = true;
    counter.irq_hold = 6;
  }
  status.irq_valid = irq_valid;

//IRQ hold
  if(counter.irq_hold) { counter.irq_hold -= 2; }
  if(status.irq_line == true && counter.irq_hold == 0) {
    if(status.virq_enabled == true || status.hirq_enabled == true) { status.irq_transition = true; }
  }
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
  counter.set(counter.irq_delay, 2);
}

void sCPU::hvtime_update(uint16 addr) {
  update_interrupts();
}

bool sCPU::rdnmi() {
bool result = status.nmi_line;
  if(counter.nmi_hold == 0) {
    status.nmi_line = false;
  }
  return result;
}

bool sCPU::timeup() {
bool result = status.irq_line;
  if(counter.irq_hold == 0) {
    status.irq_line = false;
    status.irq_transition = false;
  }
  return result;
}
