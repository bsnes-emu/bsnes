void sCPU::update_interrupts() {
  status.nmi_trigger_pos = (status.vcounter == status.vblstart) ? 2 : IRQ_TRIGGER_NEVER;

  if(irq_pos_valid() == true) {
    status.virq_trigger_pos = status.virq_pos;
    status.hirq_trigger_pos = (status.hirq_enabled) ? ((status.hirq_pos + 1) * 4) : 0;
  } else {
    status.virq_trigger_pos = IRQ_TRIGGER_NEVER;
    status.hirq_trigger_pos = IRQ_TRIGGER_NEVER;
  }
}

alwaysinline void sCPU::nmi_tick() {
  counter.nmi_fire -= 2;
  if(counter.nmi_fire != 0) { return; }

  if(status.nmi_enabled == true && status.nmi_line == 1) {
    status.nmi_line = 0;
    status.nmi_transition = 1;
  }
}

alwaysinline void sCPU::irq_tick() {
  counter.irq_fire -= 2;
  if(counter.irq_fire != 0) { return; }

  if(status.virq_enabled == true || status.hirq_enabled == true) {
    status.irq_line = 0;
    status.irq_transition = 1;
  }
}

alwaysinline void sCPU::poll_interrupts() {
  if(status.hclock == status.nmi_trigger_pos) {
    status.nmi_read  = 0;
    counter.nmi_fire = 4;
  }

  if(status.hclock == 10) { status.irq_lock = false; }

  if(status.hirq_trigger_pos == IRQ_TRIGGER_NEVER)return;
  if(status.virq_enabled == false && status.hirq_enabled == false)return;
  if(status.irq_lock == true)return;

uint vpos = status.vcounter;
uint hpos = status.hclock;
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

bool sCPU::irq_pos_valid() {
uint vpos   = status.virq_pos;
uint hpos   = (status.hirq_enabled) ? status.hirq_pos : 0;
uint vlimit = region_scanlines() >> 1;
//positions that can never be latched
//vlimit = 262/NTSC, 312/PAL
//PAL results are unverified on hardware
  if(vpos == 240 && hpos == 339 && interlace() == false && interlace_field() == 1)return false;
  if(vpos == (vlimit - 1) && hpos == 339 && interlace() == false)return false;
  if(vpos == vlimit && interlace() == false)return false;
  if(vpos == vlimit && hpos == 339)return false;
  if(vpos  > vlimit)return false;
  if(hpos  > 339)return false;

  return true;
}

alwaysinline bool sCPU::nmi_test() {
  if(status.nmi_transition == 0)return false;

  status.nmi_transition = 0;
  event.wai = false;
  return true;
}

alwaysinline bool sCPU::irq_test() {
  if(status.irq_transition == 1)goto irq_trigger;

  if(status.irq_read == 0) {
    if(status.irq_line == 1 && counter.irq_fire) {
      return false;
    }
    goto irq_trigger;
  }

  if(status.irq_line == 0) {
    status.irq_line = 1;
    goto irq_trigger;
  }

  return false;

irq_trigger:
  status.irq_transition = 0;
  event.wai = false;
  return (regs.p.i) ? false : true;
}
