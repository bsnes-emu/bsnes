#include <sfc/sfc.hpp>

namespace SuperFamicom {

CPU cpu;

#include "serialization.cpp"
#include "dma.cpp"
#include "memory.cpp"
#include "mmio.cpp"
#include "timing.cpp"

CPU::CPU() : queue(512, {&CPU::queue_event, this}) {
  PPUcounter::scanline = {&CPU::scanline, this};
}

auto CPU::step(uint clocks) -> void {
  smp.clock -= clocks * (uint64)smp.frequency;
  ppu.clock -= clocks;
  for(uint i = 0; i < coprocessors.size(); i++) {
    auto& chip = *coprocessors[i];
    chip.clock -= clocks * (uint64)chip.frequency;
  }
  device.controllerPort1->clock -= clocks * (uint64)device.controllerPort1->frequency;
  device.controllerPort2->clock -= clocks * (uint64)device.controllerPort2->frequency;
  synchronizeDevices();
}

auto CPU::synchronizeSMP() -> void {
  if(SMP::Threaded == true) {
    if(smp.clock < 0) co_switch(smp.thread);
  } else {
    while(smp.clock < 0) smp.main();
  }
}

auto CPU::synchronizePPU() -> void {
  if(PPU::Threaded == true) {
    if(ppu.clock < 0) co_switch(ppu.thread);
  } else {
    while(ppu.clock < 0) ppu.main();
  }
}

auto CPU::synchronizeCoprocessors() -> void {
  for(uint i = 0; i < coprocessors.size(); i++) {
    auto& chip = *coprocessors[i];
    if(chip.clock < 0) co_switch(chip.thread);
  }
}

auto CPU::synchronizeDevices() -> void {
  if(device.controllerPort1->clock < 0) co_switch(device.controllerPort1->thread);
  if(device.controllerPort2->clock < 0) co_switch(device.controllerPort2->thread);
}

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
  if(status.nmi_pending) {
    status.nmi_pending = false;
    regs.vector = (regs.e == false ? 0xffea : 0xfffa);
    interrupt();
  }

  if(status.irq_pending) {
    status.irq_pending = false;
    regs.vector = (regs.e == false ? 0xffee : 0xfffe);
    interrupt();
  }

  instruction();
}

auto CPU::enable() -> void {
  function<auto (uint, uint8) -> uint8> reader{&CPU::mmio_read, (CPU*)&cpu};
  function<auto (uint, uint8) -> void> writer{&CPU::mmio_write, (CPU*)&cpu};

  bus.map(reader, writer, 0x00, 0x3f, 0x2140, 0x2183);
  bus.map(reader, writer, 0x80, 0xbf, 0x2140, 0x2183);

  bus.map(reader, writer, 0x00, 0x3f, 0x4016, 0x4017);
  bus.map(reader, writer, 0x80, 0xbf, 0x4016, 0x4017);

  bus.map(reader, writer, 0x00, 0x3f, 0x4200, 0x421f);
  bus.map(reader, writer, 0x80, 0xbf, 0x4200, 0x421f);

  bus.map(reader, writer, 0x00, 0x3f, 0x4300, 0x437f);
  bus.map(reader, writer, 0x80, 0xbf, 0x4300, 0x437f);

  reader = [](uint addr, uint8 data) { return cpu.wram[addr]; };
  writer = [](uint addr, uint8 data) { cpu.wram[addr] = data; };

  bus.map(reader, writer, 0x00, 0x3f, 0x0000, 0x1fff, 0x002000);
  bus.map(reader, writer, 0x80, 0xbf, 0x0000, 0x1fff, 0x002000);
  bus.map(reader, writer, 0x7e, 0x7f, 0x0000, 0xffff, 0x020000);
}

auto CPU::power() -> void {
  regs.a = 0x0000;
  regs.x = 0x0000;
  regs.y = 0x0000;
  regs.s = 0x01ff;

  reset();
}

auto CPU::reset() -> void {
  create(Enter, system.cpuFrequency());
  coprocessors.reset();
  PPUcounter::reset();

  regs.pc = 0x000000;
  regs.x.h = 0x00;
  regs.y.h = 0x00;
  regs.s.h = 0x01;
  regs.d = 0x0000;
  regs.db = 0x00;
  regs.p = 0x34;
  regs.e = 1;
  regs.mdr = 0x00;
  regs.wai = false;

  regs.pc.l = bus.read(0xfffc, regs.mdr);
  regs.pc.h = bus.read(0xfffd, regs.mdr);
  regs.pc.b = 0x00;

  status.nmi_valid = false;
  status.nmi_line = false;
  status.nmi_transition = false;
  status.nmi_pending = false;

  status.irq_valid = false;
  status.irq_line = false;
  status.irq_transition = false;
  status.irq_pending = false;

  status.irq_lock = false;
  status.hdma_pending = false;

  status.wram_addr = 0x000000;

  status.joypad_strobe_latch = 0;

  status.nmi_enabled = false;
  status.virq_enabled = false;
  status.hirq_enabled = false;
  status.auto_joypad_poll_enabled = false;

  status.pio = 0xff;

  status.htime = 0x0000;
  status.vtime = 0x0000;

  status.rom_speed = 8;

  status.joy1l = status.joy1h = 0x00;
  status.joy2l = status.joy2h = 0x00;
  status.joy3l = status.joy3h = 0x00;
  status.joy4l = status.joy4h = 0x00;

  dma_reset();
}

}
