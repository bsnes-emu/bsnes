void sCPU::update_interrupts() {
  status.nmi_read_pos = (status.vcounter == status.vblstart) ? 2 : IRQ_TRIGGER_NEVER;
  status.nmi_line_pos = (status.vcounter == status.vblstart) ? 6 : IRQ_TRIGGER_NEVER;

  if(irq_pos_valid() == false) {
    status.irq_read_pos = IRQ_TRIGGER_NEVER;
    status.irq_line_pos = IRQ_TRIGGER_NEVER;
    return;
  }

uint vpos = status.virq_pos;
uint hpos = 4 * ((status.hirq_enabled) ? (status.hirq_pos + 1) : 0);
  timeshift_forward(10, vpos, hpos);
  if(!status.virq_enabled || (status.virq_enabled && status.vcounter == vpos)) {
    status.irq_read_pos = hpos;
  } else {
    status.irq_read_pos = IRQ_TRIGGER_NEVER;
  }

  timeshift_forward(4, vpos, hpos);
  if(!status.virq_enabled || (status.virq_enabled && status.vcounter == vpos)) {
    status.irq_line_pos = hpos;
  } else {
    status.irq_line_pos = IRQ_TRIGGER_NEVER;
  }
}

alwaysinline void sCPU::poll_interrupts(uint clocks) {
  if(status.hclock + clocks >= status.line_clocks) {
    clocks = (status.hclock + clocks) - status.line_clocks;
    poll_interrupts_range(status.line_clocks - status.hclock);
    scanline();
    status.irq_lock = false;
    if(clocks == 0) { return; }
  }

  poll_interrupts_range(clocks);
  status.hclock += clocks;
}

alwaysinline void sCPU::poll_interrupts_range(uint clocks) {
int start, end;
  if(status.hclock == 0) {
    start = -1;
    end   = clocks;
  } else {
    start = status.hclock;
    end   = status.hclock + clocks;
  }

  if(start < status.nmi_read_pos && status.nmi_read_pos <= end) {
    status.nmi_read = 0;
  }

  if(start < status.nmi_line_pos && status.nmi_line_pos <= end) {
    if(status.nmi_enabled == true) {
      if(status.nmi_line == 1) {
        status.nmi_transition = 1;
      }
      status.nmi_line = 0;
    }
  }

  if(status.virq_enabled == false && status.hirq_enabled == false) {
    return;
  }

  if(status.hirq_enabled == false) {
    if(status.irq_lock == false) {
      if(end >= status.irq_read_pos) {
        status.irq_read = 0;
      }

      if(end >= status.irq_line_pos) {
        status.irq_lock = true;
        status.irq_line = 0;
        status.irq_transition = 1;
      }
    }
  } else {
    if(start < status.irq_read_pos && status.irq_read_pos <= end) {
      status.irq_read = 0;
    }

    if(start < status.irq_line_pos && status.irq_line_pos <= end) {
      status.irq_lock = true;
      status.irq_line = 0;
      status.irq_transition = 1;
    }
  }
}

bool sCPU::nmi_edge() {
uint vpos = status.vcounter, hpos = status.hclock;
  if(hpos == status.nmi_read_pos)return true;
  timeshift_backward(2, vpos, hpos);
  if(hpos == status.nmi_read_pos)return true;
  return false;
}

bool sCPU::irq_edge() {
uint vpos = status.vcounter, hpos = status.hclock;
  if(hpos == status.irq_read_pos)return true;
  timeshift_backward(2, vpos, hpos);
  if(hpos == status.irq_read_pos)return true;
  return false;
}

void sCPU::irqpos_update(uint16 addr) {
  if(status.hclock < status.irq_line_pos) { status.irq_lock = false; }
}
