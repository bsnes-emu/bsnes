#if defined(FAVOR_ACCURACY)
  #include "irqtiming_accurate.cpp"
#elif defined(FAVOR_SPEED)
  #include "irqtiming_fast.cpp"
#endif

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
    if(status.irq_line == 1 && irq_edge()) {
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
