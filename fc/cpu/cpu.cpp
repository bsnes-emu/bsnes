#include <fc/fc.hpp>

namespace Famicom {

#include "timing.cpp"
#include "serialization.cpp"
CPU cpu;

auto CPU::Enter() -> void {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    cpu.main();
  }
}

auto CPU::main() -> void {
  if(status.interrupt_pending) {
    interrupt();
    return;
  }

  exec();
}

auto CPU::add_clocks(uint clocks) -> void {
  apu.clock -= clocks;
  if(apu.clock < 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(apu.thread);

  ppu.clock -= clocks;
  if(ppu.clock < 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(ppu.thread);

  cartridge.clock -= clocks;
  if(cartridge.clock < 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cartridge.thread);
}

auto CPU::power() -> void {
  R6502::power();

  for(auto addr : range(0x0800)) ram[addr] = 0xff;
  ram[0x0008] = 0xf7;
  ram[0x0009] = 0xef;
  ram[0x000a] = 0xdf;
  ram[0x000f] = 0xbf;
}

auto CPU::reset() -> void {
  R6502::reset();
  create(CPU::Enter, 21477272);

  regs.pc  = bus.read(0xfffc) << 0;
  regs.pc |= bus.read(0xfffd) << 8;

  status.interrupt_pending = false;
  status.nmi_pending = false;
  status.nmi_line = 0;
  status.irq_line = 0;
  status.irq_apu_line = 0;

  status.rdy_line = 1;
  status.rdy_addr_valid = false;
  status.rdy_addr_value = 0x0000;

  status.oam_dma_pending = false;
  status.oam_dma_page = 0x00;

  status.controller_latch = false;
  status.controller_port0 = 0;
  status.controller_port1 = 0;
}

auto CPU::debugger_read(uint16 addr) -> uint8 {
  return bus.read(addr);
}

auto CPU::ram_read(uint16 addr) -> uint8 {
  return ram[addr & 0x07ff];
}

auto CPU::ram_write(uint16 addr, uint8 data) -> void {
  ram[addr & 0x07ff] = data;
}

auto CPU::read(uint16 addr) -> uint8 {
  if(addr == 0x4016) {
    return (mdr() & 0xc0) | input.data(0);
  }

  if(addr == 0x4017) {
    return (mdr() & 0xc0) | input.data(1);
  }

  return apu.read(addr);
}

auto CPU::write(uint16 addr, uint8 data) -> void {
  if(addr == 0x4014) {
    status.oam_dma_page = data;
    status.oam_dma_pending = true;
  }

  if(addr == 0x4016) {
    input.latch(data & 0x01);
  }

  return apu.write(addr, data);
}

}
