#ifdef CPU_CPP

void sCPUDebugger::op_step() {
  bool break_event = false;

  usage[regs.pc] &= ~(UsageFlagM | UsageFlagX);
  usage[regs.pc] |= UsageExec | (regs.p.m << 1) | (regs.p.x << 0);
  opcode_pc = regs.pc;

  if(debugger.step_cpu) {
    debugger.break_event = Debugger::BreakEvent::CPUStep;
    scheduler.exit(Scheduler::ExitReason::DebuggerEvent);
  } else {
    debugger.breakpoint_test(Debugger::Breakpoint::Source::CPUBus, Debugger::Breakpoint::Mode::Exec, regs.pc, 0x00);
  }

  if(step_event) step_event();
  sCPU::op_step();
  synchronize_smp();
}

uint8 sCPUDebugger::op_read(uint32 addr) {
  uint8 data = sCPU::op_read(addr);
  usage[addr] |= UsageRead;
  debugger.breakpoint_test(Debugger::Breakpoint::Source::CPUBus, Debugger::Breakpoint::Mode::Read, addr, data);
  return data;
}

void sCPUDebugger::op_write(uint32 addr, uint8 data) {
  sCPU::op_write(addr, data);
  usage[addr] |= UsageWrite;
  usage[addr] &= ~UsageExec;
  debugger.breakpoint_test(Debugger::Breakpoint::Source::CPUBus, Debugger::Breakpoint::Mode::Write, addr, data);
}

sCPUDebugger::sCPUDebugger() {
  usage = new uint8[1 << 24]();
  opcode_pc = 0x8000;
}

sCPUDebugger::~sCPUDebugger() {
  delete[] usage;
}

//===========
//CPUDebugger
//===========

//internal
unsigned sCPUDebugger::mdr() { return regs.mdr; }

//$2181-$2183
unsigned sCPUDebugger::wram_address() { return status.wram_addr; }

//$4016
bool sCPUDebugger::joypad_strobe_latch() { return status.joypad_strobe_latch; }

//$4200
bool sCPUDebugger::nmi_enable() { return status.nmi_enabled; }
bool sCPUDebugger::hirq_enable() { return status.hirq_enabled; }
bool sCPUDebugger::virq_enable() { return status.virq_enabled; }
bool sCPUDebugger::auto_joypad_poll() { return status.auto_joypad_poll; }

//$4201
unsigned sCPUDebugger::pio_bits() { return status.pio; }

//$4202
unsigned sCPUDebugger::multiplicand() { return status.wrmpya; }

//$4203
unsigned sCPUDebugger::multiplier() { return status.wrmpyb; }

//$4204-$4205
unsigned sCPUDebugger::dividend() { return status.wrdiva; }

//$4206
unsigned sCPUDebugger::divisor() { return status.wrdivb; }

//$4207-$4208
unsigned sCPUDebugger::htime() { return status.hirq_pos; }

//$4209-$420a
unsigned sCPUDebugger::vtime() { return status.virq_pos; }

//$420b
unsigned sCPUDebugger::dma_enable() {
  unsigned result = 0;
  for(unsigned n = 0; n < 8; n++) {
    result |= channel[n].dma_enabled << n;
  }
  return result;
}

//$420c
unsigned sCPUDebugger::hdma_enable() {
  unsigned result = 0;
  for(unsigned n = 0; n < 8; n++) {
    result |= channel[n].hdma_enabled << n;
  }
  return result;
}

//$420d
bool sCPUDebugger::fastrom_enable() { return status.rom_speed; }

//$43x0
bool sCPUDebugger::dma_direction(unsigned n) { return channel[n].direction; }
bool sCPUDebugger::dma_indirect(unsigned n) { return channel[n].hdma_indirect; }
bool sCPUDebugger::dma_reverse_transfer(unsigned n) { return channel[n].reversexfer; }
bool sCPUDebugger::dma_fixed_transfer(unsigned n) { return channel[n].fixedxfer; }
unsigned sCPUDebugger::dma_transfer_mode(unsigned n) { return channel[n].xfermode; }

//$43x1
unsigned sCPUDebugger::dma_bbus_address(unsigned n) { return 0x2100 + channel[n].destaddr; }

//$43x2-$43x3
unsigned sCPUDebugger::dma_abus_address(unsigned n) { return channel[n].srcaddr; }

//$43x4
unsigned sCPUDebugger::dma_abus_bank(unsigned n) { return channel[n].srcbank; }

//$43x5-$43x6
unsigned sCPUDebugger::dma_transfer_size(unsigned n) { return channel[n].xfersize; }

//$43x7
unsigned sCPUDebugger::dma_indirect_bank(unsigned n) { return channel[n].hdma_ibank; }

//$43x8-$43x9
unsigned sCPUDebugger::dma_table_address(unsigned n) { return channel[n].hdma_addr; }

//$43xa
unsigned sCPUDebugger::dma_line_counter(unsigned n) { return channel[n].hdma_line_counter; }

#endif
