#include <nes/nes.hpp>

namespace Famicom {

#include "core/core.cpp"
#include "memory/memory.cpp"
#include "serialization.cpp"
CPU cpu;

void CPU::Main() {
  cpu.main();
}

void CPU::main() {
//trace = true;
//FILE *fp = fopen("/home/byuu/Desktop/log.txt", "wb");

  unsigned lpc = 0xffff;
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    if(status.interrupt_pending) {
      interrupt();
      continue;
    }

    if(trace) {
      if(lpc != regs.pc) { print(disassemble(), "\n"); } lpc = regs.pc;
    //if(lpc != regs.pc) { fprintf(fp, "%s\n", (const char*)disassemble()); fflush(fp); } lpc = regs.pc;
    }

    op_exec();
  }
}

void CPU::add_clocks(unsigned clocks) {
  apu.clock -= clocks;
  if(apu.clock < 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(apu.thread);

  ppu.clock -= clocks;
  if(ppu.clock < 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(ppu.thread);

  cartridge.clock -= clocks;
  if(cartridge.clock < 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cartridge.thread);
}

void CPU::power() {
  regs.a = 0x00;
  regs.x = 0x00;
  regs.y = 0x00;
  regs.s = 0x00;
  regs.p = 0x04;

  for(unsigned addr = 0; addr < 0x0800; addr++) ram[addr] = 0xff;
  ram[0x0008] = 0xf7;
  ram[0x0009] = 0xef;
  ram[0x000a] = 0xdf;
  ram[0x000f] = 0xbf;
}

void CPU::reset() {
  create(CPU::Main, 21477272);

  regs.mdr = 0x00;
  regs.s -= 3;
  regs.p.i = 1;

  regs.pc  = bus.read(0xfffc) << 0;
  regs.pc |= bus.read(0xfffd) << 8;

  status.interrupt_pending = false;
  status.nmi_pending = false;
  status.nmi_line = 0;
  status.irq_line = 0;
  status.irq_apu_line = 0;

  status.rdy_line = 1;
  status.rdy_addr = { false, 0x0000 };

  status.oam_dma_pending = false;
  status.oam_dma_page = 0x00;

  status.controller_latch = false;
  status.controller_port0 = 0;
  status.controller_port1 = 0;
}

uint8 CPU::mdr() const {
  return regs.mdr;
}

void CPU::set_rdy_line(bool line) {
  status.rdy_line = line;
}

void CPU::set_rdy_addr(optional<uint16> addr) {
  status.rdy_addr = addr;
}

uint8 CPU::ram_read(uint16 addr) {
  return ram[addr & 0x07ff];
}

void CPU::ram_write(uint16 addr, uint8 data) {
  ram[addr & 0x07ff] = data;
}

uint8 CPU::read(uint16 addr) {
  if(addr == 0x4016) {
    return (mdr() & 0xc0) | input.data(0);
  }

  if(addr == 0x4017) {
    return (mdr() & 0xc0) | input.data(1);
  }

  return apu.read(addr);
}

void CPU::write(uint16 addr, uint8 data) {
  if(addr == 0x4014) {
    status.oam_dma_page = data;
    status.oam_dma_pending = true;
  }

  if(addr == 0x4016) {
    input.latch(data & 0x01);
  }

  return apu.write(addr, data);
}

void CPU::oam_dma() {
  for(unsigned n = 0; n < 256; n++) {
    uint8 data = op_read((status.oam_dma_page << 8) + n);
    op_write(0x2004, data);
  }
}

}
