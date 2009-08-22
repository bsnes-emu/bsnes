#ifdef SYSTEM_CPP

Debugger debugger;

uint8 Debugger::read(Debugger::MemorySource source, unsigned addr) {
  switch(source) {
    case CPUBus: {
      //do not read from memory-mapped registers that could affect program behavior
      if(((addr - 0x2000) & 0x40c000) == 0x000000) break;  //$00-3f:2000-5fff MMIO
      return bus.read(addr & 0xffffff);
    } break;

    case APURAM: {
      return memory::apuram.read(addr & 0xffff);
    } break;

    case VRAM: {
      return memory::vram.read(addr & 0xffff);
    } break;

    case OAM: {
      if(addr & 0x0200) return memory::oam.read(0x0200 + (addr & 0x1f));
      return memory::oam.read(addr & 0x01ff);
    } break;

    case CGRAM: {
      return memory::cgram.read(addr & 0x01ff);
    } break;
  }

  return 0x00;
}

Debugger::Debugger() {
  break_event = None;

  for(unsigned n = 0; n < Breakpoints; n++) {
    breakpoint[n].enabled = false;
    breakpoint[n].addr = 0;
    breakpoint[n].data = -1;
    breakpoint[n].mode = Breakpoint::Exec;
    breakpoint[n].source = Breakpoint::CPUBus;
    breakpoint[n].counter = 0;
  }
  breakpoint_hit = 0;

  step_cpu = false;
  step_smp = false;
}

#endif
