#ifdef CPU_CPP

bool CPUDebugger::property(unsigned id, string &name, string &value) {
  unsigned n = 0;

  //internal
  if(id == n++) { name = "S-CPU MDR"; value = string("0x", strhex<2>(mdr())); return true; }

  //$2181-2183
  if(id == n++) { name = "$2181-$2183"; value = ""; return true; }
  if(id == n++) { name = "WRAM Address"; value = string("0x", strhex<6>(wram_address())); return true; }

  //$4016
  if(id == n++) { name = "$4016"; value = ""; return true; }
  if(id == n++) { name = "Joypad Strobe Latch"; value = joypad_strobe_latch(); return true; }

  //$4200
  if(id == n++) { name = "$4200"; value = ""; return true; }
  if(id == n++) { name = "NMI Enable"; value = nmi_enable(); return true; }
  if(id == n++) { name = "H-IRQ Enable"; value = hirq_enable(); return true; }
  if(id == n++) { name = "V-IRQ Enable"; value = virq_enable(); return true; }
  if(id == n++) { name = "Auto Joypad Poll"; value = auto_joypad_poll(); return true; }

  //$4201
  if(id == n++) { name = "$4201"; value = ""; return true; }
  if(id == n++) { name = "PIO"; value = string("0x", strhex<2>(pio_bits())); return true; }

  //$4202
  if(id == n++) { name = "$4202"; value = ""; return true; }
  if(id == n++) { name = "Multiplicand"; value = string("0x", strhex<2>(multiplicand())); return true; }

  //$4203
  if(id == n++) { name = "$4203"; value = ""; return true; }
  if(id == n++) { name = "Multiplier"; value = string("0x", strhex<2>(multiplier())); return true; }

  //$4204-$4205
  if(id == n++) { name = "$4204-$4205"; value = ""; return true; }
  if(id == n++) { name = "Dividend"; value = string("0x", strhex<4>(dividend())); return true; }

  //$4206
  if(id == n++) { name = "$4206"; value = ""; return true; }
  if(id == n++) { name = "Divisor"; value = string("0x", strhex<2>(divisor())); return true; }

  //$4207-$4208
  if(id == n++) { name = "$4207-$4208"; value = ""; return true; }
  if(id == n++) { name = "H-Time"; value = string("0x", strhex<4>(htime())); return true; }

  //$4209-$420a
  if(id == n++) { name = "$4209-$420a"; value = ""; return true; }
  if(id == n++) { name = "V-Time"; value = string("0x", strhex<4>(vtime())); return true; }

  //$420b
  if(id == n++) { name = "$420b"; value = ""; return true; }
  if(id == n++) { name = "DMA Enable"; value = string("0x", strhex<2>(dma_enable())); return true; }

  //$420c
  if(id == n++) { name = "$420c"; value = ""; return true; }
  if(id == n++) { name = "HDMA Enable"; value = string("0x", strhex<2>(hdma_enable())); return true; }

  //$420d
  if(id == n++) { name = "$420d"; value = ""; return true; }
  if(id == n++) { name = "FastROM Enable"; value = fastrom_enable(); return true; }

  for(unsigned i = 0; i < 8; i++) {
    if(id == n++) { name = string() << "DMA Channel " << i; return true; }

    //$43x0
    if(id == n++) { name = "Direction"; value = dma_direction(i); return true; }
    if(id == n++) { name = "Indirect"; value = dma_indirect(i); return true; }
    if(id == n++) { name = "Reverse Transfer"; value = dma_reverse_transfer(i); return true; }
    if(id == n++) { name = "Fixed Transfer"; value = dma_fixed_transfer(i); return true; }
    if(id == n++) { name = "Transfer Mode"; value = dma_transfer_mode(i); return true; }

    //$43x1
    if(id == n++) { name = "B-Bus Address"; value = string("0x", strhex<4>(dma_bbus_address(i))); return true; }

    //$43x2-$43x3
    if(id == n++) { name = "A-Bus Address"; value = string("0x", strhex<4>(dma_abus_address(i))); return true; }

    //$43x4
    if(id == n++) { name = "A-Bus Bank"; value = string("0x", strhex<2>(dma_abus_bank(i))); return true; }

    //$43x5-$43x6
    if(id == n++) { name = "Transfer Size / Indirect Address"; value = string("0x", strhex<4>(dma_transfer_size(i))); return true; }

    //$43x7
    if(id == n++) { name = "Indirect Bank"; value = string("0x", strhex<2>(dma_indirect_bank(i))); return true; }

    //$43x8-$43x9
    if(id == n++) { name = "Table Address"; value = string("0x", strhex<4>(dma_table_address(i))); return true; }

    //$43xa
    if(id == n++) { name = "Line Counter"; value = string("0x", strhex<2>(dma_line_counter(i))); return true; }
  }

  return false;
}

void CPUDebugger::op_step() {
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
  CPU::op_step();
  synchronize_smp();
}

uint8 CPUDebugger::op_read(uint32 addr) {
  uint8 data = CPU::op_read(addr);
  usage[addr] |= UsageRead;
  debugger.breakpoint_test(Debugger::Breakpoint::Source::CPUBus, Debugger::Breakpoint::Mode::Read, addr, data);
  return data;
}

void CPUDebugger::op_write(uint32 addr, uint8 data) {
  CPU::op_write(addr, data);
  usage[addr] |= UsageWrite;
  usage[addr] &= ~UsageExec;
  debugger.breakpoint_test(Debugger::Breakpoint::Source::CPUBus, Debugger::Breakpoint::Mode::Write, addr, data);
}

CPUDebugger::CPUDebugger() {
  usage = new uint8[1 << 24]();
  opcode_pc = 0x8000;
}

CPUDebugger::~CPUDebugger() {
  delete[] usage;
}

//internal
unsigned CPUDebugger::mdr() { return regs.mdr; }

//$2181-$2183
unsigned CPUDebugger::wram_address() { return status.wram_addr; }

//$4016
bool CPUDebugger::joypad_strobe_latch() { return status.joypad_strobe_latch; }

//$4200
bool CPUDebugger::nmi_enable() { return status.nmi_enabled; }
bool CPUDebugger::hirq_enable() { return status.hirq_enabled; }
bool CPUDebugger::virq_enable() { return status.virq_enabled; }
bool CPUDebugger::auto_joypad_poll() { return status.auto_joypad_poll; }

//$4201
unsigned CPUDebugger::pio_bits() { return status.pio; }

//$4202
unsigned CPUDebugger::multiplicand() { return status.wrmpya; }

//$4203
unsigned CPUDebugger::multiplier() { return status.wrmpyb; }

//$4204-$4205
unsigned CPUDebugger::dividend() { return status.wrdiva; }

//$4206
unsigned CPUDebugger::divisor() { return status.wrdivb; }

//$4207-$4208
unsigned CPUDebugger::htime() { return status.hirq_pos; }

//$4209-$420a
unsigned CPUDebugger::vtime() { return status.virq_pos; }

//$420b
unsigned CPUDebugger::dma_enable() {
  unsigned result = 0;
  for(unsigned n = 0; n < 8; n++) {
    result |= channel[n].dma_enabled << n;
  }
  return result;
}

//$420c
unsigned CPUDebugger::hdma_enable() {
  unsigned result = 0;
  for(unsigned n = 0; n < 8; n++) {
    result |= channel[n].hdma_enabled << n;
  }
  return result;
}

//$420d
bool CPUDebugger::fastrom_enable() { return status.rom_speed; }

//$43x0
bool CPUDebugger::dma_direction(unsigned n) { return channel[n].direction; }
bool CPUDebugger::dma_indirect(unsigned n) { return channel[n].hdma_indirect; }
bool CPUDebugger::dma_reverse_transfer(unsigned n) { return channel[n].reversexfer; }
bool CPUDebugger::dma_fixed_transfer(unsigned n) { return channel[n].fixedxfer; }
unsigned CPUDebugger::dma_transfer_mode(unsigned n) { return channel[n].xfermode; }

//$43x1
unsigned CPUDebugger::dma_bbus_address(unsigned n) { return 0x2100 + channel[n].destaddr; }

//$43x2-$43x3
unsigned CPUDebugger::dma_abus_address(unsigned n) { return channel[n].srcaddr; }

//$43x4
unsigned CPUDebugger::dma_abus_bank(unsigned n) { return channel[n].srcbank; }

//$43x5-$43x6
unsigned CPUDebugger::dma_transfer_size(unsigned n) { return channel[n].xfersize; }

//$43x7
unsigned CPUDebugger::dma_indirect_bank(unsigned n) { return channel[n].hdma_ibank; }

//$43x8-$43x9
unsigned CPUDebugger::dma_table_address(unsigned n) { return channel[n].hdma_addr; }

//$43xa
unsigned CPUDebugger::dma_line_counter(unsigned n) { return channel[n].hdma_line_counter; }

#endif
