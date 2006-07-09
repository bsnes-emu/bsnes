void sCPU::update_interrupts() {
  if(status.vcounter == (!overscan() ? 225 : 240)) {
    status.nmi_read_pos = 2;
    status.nmi_line_pos = 6;
  } else {
    status.nmi_read_pos = -64;
    status.nmi_line_pos = -64;
  }

  if(irq_pos_valid() == false) {
    status.irq_read_pos = -64;
    status.irq_line_pos = -64;
    return;
  }

uint vpos = status.virq_pos;
uint hpos = (status.hirq_enabled) ? status.hirq_pos : 0;
  hpos = (hpos != 0) ? ((hpos << 2) + 14) : 10;
  if(hpos >= status.line_clocks) {
    hpos -= status.line_clocks;
    if(++vpos >= status.field_lines) {
      vpos = 0;
    }
  }

  if((status.virq_enabled == true && status.vcounter == vpos) || status.virq_enabled == false) {
    status.irq_read_pos = hpos;
  } else {
    status.irq_read_pos = -64;
  }

  hpos += 4;
  if(hpos >= status.line_clocks) {
    hpos -= status.line_clocks;
    if(++vpos >= status.field_lines) {
      vpos = 0;
    }
  }

  if((status.virq_enabled == true && status.vcounter == vpos) || status.virq_enabled == false) {
    status.irq_line_pos = hpos;
  } else {
    status.irq_line_pos = -64;
  }
}

void sCPU::poll_interrupts(int clocks) {
int16 start, end;
  if(status.hclock == 0) {
    start = -1;
    end   = clocks;
  } else {
    start = status.hclock;
    end   = status.hclock + clocks;
  }

//NMI read test
  if(start < status.nmi_read_pos && status.nmi_read_pos <= end) {
  //nmi_read lowers even when NMI interrupts are disabled via $4200.d7
    status.nmi_read = 0;
  }

//NMI line test
  if(start < status.nmi_line_pos && status.nmi_line_pos <= end) {
    if(status.nmi_enabled == true) {
      if(status.nmi_line == 1) {
        status.nmi_transition = 1;
      }
      status.nmi_line = 0;
    }
  }

//IRQ read test
  if(start < status.irq_read_pos && status.irq_read_pos <= end) {
    if(status.virq_enabled == true || status.hirq_enabled == true) {
      status.irq_read = 0;
    }
  }

//IRQ line test
  if(start < status.irq_line_pos && status.irq_line_pos <= end) {
    if(status.virq_enabled == true || status.hirq_enabled == true) {
      status.irq_line = 0;
      status.irq_transition = 1;
    }
  }
}

bool sCPU::nmi_read_pos_match(uint offset) {
uint16 v = !overscan() ? 225 : 240;
uint16 h = 2 + offset;
  return (status.vcounter == v && status.hclock == h);
}

bool sCPU::irq_read_pos_match(uint offset) {
  if(irq_pos_valid() == false)return false;

uint vpos = status.virq_pos;
uint hpos = (status.hirq_enabled) ? status.hirq_pos : 0;
  hpos  = (hpos != 0) ? ((hpos << 2) + 14) : 10;
  hpos += offset;
  if(hpos >= status.line_clocks) {
    hpos -= status.line_clocks;
    if(++vpos >= status.field_lines) {
      vpos = 0;
    }
  }

  if((status.virq_enabled == true && status.vcounter == vpos) || status.virq_enabled == false) {
    return (status.hclock == hpos);
  }

  return false;
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

bool sCPU::nmi_test() {
  if(status.nmi_transition == 0)return false;

  status.nmi_transition = 0;
  event.wai = false;
  return true;
}

bool sCPU::irq_test() {
  if(status.irq_transition == 1)goto irq_trigger;

  if(status.irq_read == 0) {
    if(status.irq_line == 1 && (irq_read_pos_match(0) || irq_read_pos_match(2))) {
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
