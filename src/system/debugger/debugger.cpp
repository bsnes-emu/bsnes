#ifdef SYSTEM_CPP

Debugger debugger;

void Debugger::breakpoint_test(Debugger::Breakpoint::Source source, Debugger::Breakpoint::Mode mode, unsigned addr, uint8 data) {
  for(unsigned i = 0; i < Breakpoints; i++) {
    if(breakpoint[i].enabled == false) continue;

    //shadow S-CPU WRAM addresses ($00-3f|80-bf:0000-1fff mirrors $7e:0000-1fff)
    if(source == Debugger::Breakpoint::Source::CPUBus && (
        ((breakpoint[i].addr & 0x40e000) == 0x000000) ||  //$00-3f|80-bf:0000-1fff
        ((breakpoint[i].addr & 0xffe000) == 0x7e0000)     //$7e:0000-1fff
      )
    ) {
      if((breakpoint[i].addr & 0x1fff) != (addr & 0x1fff)) continue;
    } else {
      if(breakpoint[i].addr != addr) continue;
    }

    if(breakpoint[i].data != -1 && breakpoint[i].data != data) continue;
    if(breakpoint[i].source != source) continue;
    if(breakpoint[i].mode != mode) continue;

    breakpoint[i].counter++;
    breakpoint_hit = i;
    break_event = BreakEvent::BreakpointHit;
    scheduler.exit(Scheduler::ExitReason::DebuggerEvent);
    break;
  }
}

uint8 Debugger::read(Debugger::MemorySource source, unsigned addr) {
  switch(source) {
    case MemorySource::CPUBus: {
      //do not read from memory-mapped registers that could affect program behavior
      if(((addr - 0x2000) & 0x40c000) == 0x000000) break;  //$00-3f:2000-5fff MMIO
      return bus.read(addr & 0xffffff);
    } break;

    case MemorySource::APURAM: {
      return memory::apuram.read(addr & 0xffff);
    } break;

    case MemorySource::VRAM: {
      return memory::vram.read(addr & 0xffff);
    } break;

    case MemorySource::OAM: {
      if(addr & 0x0200) return memory::oam.read(0x0200 + (addr & 0x1f));
      return memory::oam.read(addr & 0x01ff);
    } break;

    case MemorySource::CGRAM: {
      return memory::cgram.read(addr & 0x01ff);
    } break;
  }

  return 0x00;
}

void Debugger::write(Debugger::MemorySource source, unsigned addr, uint8 data) {
  switch(source) {
    case MemorySource::CPUBus: {
      //do not write to memory-mapped registers that could affect program behavior
      if(((addr - 0x2000) & 0x40c000) == 0x000000) break;  //$00-3f:2000-5fff MMIO
      memory::cartrom.write_protect(false);
      bus.write(addr & 0xffffff, data);
      memory::cartrom.write_protect(true);
    } break;

    case MemorySource::APURAM: {
      memory::apuram.write(addr & 0xffff, data);
    } break;

    case MemorySource::VRAM: {
      memory::vram.write(addr & 0xffff, data);
    } break;

    case MemorySource::OAM: {
      if(addr & 0x0200) memory::oam.write(0x0200 + (addr & 0x1f), data);
      else memory::oam.write(addr & 0x01ff, data);
    } break;

    case MemorySource::CGRAM: {
      memory::cgram.write(addr & 0x01ff, data);
    } break;
  }
}

Debugger::Debugger() {
  break_event = BreakEvent::None;

  for(unsigned n = 0; n < Breakpoints; n++) {
    breakpoint[n].enabled = false;
    breakpoint[n].addr = 0;
    breakpoint[n].data = -1;
    breakpoint[n].mode = Breakpoint::Mode::Exec;
    breakpoint[n].source = Breakpoint::Source::CPUBus;
    breakpoint[n].counter = 0;
  }
  breakpoint_hit = 0;

  step_cpu = false;
  step_smp = false;
}

#endif
