void sCPU::update_interrupts() {
  status.nmi_trigger_pos = (status.vcounter == status.vblstart) ? 2 : IRQ_TRIGGER_NEVER;

  if(irq_pos_valid() == true) {
    status.virq_trigger_pos = status.virq_pos;
    status.hirq_trigger_pos = 4 * ((status.hirq_enabled) ? (status.hirq_pos + 1) : 0);
  } else {
    status.virq_trigger_pos = IRQ_TRIGGER_NEVER;
    status.hirq_trigger_pos = IRQ_TRIGGER_NEVER;
  }
}

alwaysinline void sCPU::poll_interrupts(uint clocks) {
  clocks >>= 1;
  while(clocks--) {
    status.hclock += 2;
    if(status.hclock >= status.line_clocks) { scanline(); }

  //NMI tick
    if(counter.nmi_fire) {
      counter.nmi_fire -= 2;
      if(counter.nmi_fire == 0) {
        if(status.nmi_enabled == true && status.nmi_line == 1) {
          status.nmi_line = 0;
          status.nmi_transition = 1;
        }
      }
    }

  //IRQ tick
    if(counter.irq_fire) {
      counter.irq_fire -= 2;
      if(counter.irq_fire == 0) {
        if(status.virq_enabled == true || status.hirq_enabled == true) {
          status.irq_line = 0;
          status.irq_transition = 1;
        }
      }
    }

  //NMI test
    if(status.hclock == status.nmi_trigger_pos) {
      status.nmi_read  = 0;
      counter.nmi_fire = 4;
    }

  //IRQ test
    if(status.hclock == 10) { status.irq_lock = false; }

    if(status.hirq_trigger_pos == IRQ_TRIGGER_NEVER) { continue; }
    if(status.virq_enabled == false && status.hirq_enabled == false) { continue; }
    if(status.irq_lock == true) { continue; }

  uint vpos = status.vcounter, hpos = status.hclock;
    timeshift_backward(10, vpos, hpos);

  bool trigger_irq = true;
    if(status.virq_enabled == true && vpos != status.virq_trigger_pos)trigger_irq = false;
    if(status.hirq_enabled == true && hpos != status.hirq_trigger_pos)trigger_irq = false;

    if(trigger_irq == true) {
      status.irq_lock  = true;
      status.irq_read  = 0;
      counter.irq_fire = 4;
    }
  }
}

bool sCPU::nmi_edge() { return (counter.nmi_fire != 0); }
bool sCPU::irq_edge() { return (counter.irq_fire != 0); }

void sCPU::irqpos_update(uint16 addr) {
uint vpos = status.vcounter, hpos = status.hclock;
  timeshift_backward(10, vpos, hpos);
  if(hpos < status.hirq_trigger_pos) { status.irq_lock = false; }
}
