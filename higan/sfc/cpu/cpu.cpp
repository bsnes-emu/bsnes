#include <sfc/sfc.hpp>

namespace SuperFamicom {

CPU cpu;
#include "dma.cpp"
#include "memory.cpp"
#include "mmio.cpp"
#include "timing.cpp"
#include "irq.cpp"
#include "joypad.cpp"
#include "serialization.cpp"

auto CPU::interruptPending() const -> bool { return status.interrupt_pending; }
auto CPU::pio() const -> uint8 { return status.pio; }
auto CPU::joylatch() const -> bool { return status.joypad_strobe_latch; }

CPU::CPU() {
  PPUcounter::scanline = {&CPU::scanline, this};
}

auto CPU::step(uint clocks) -> void {
  smp.clock -= clocks * (uint64)smp.frequency;
  ppu.clock -= clocks;
  for(auto chip : coprocessors) {
    chip->clock -= clocks * (uint64)chip->frequency;
  }
  device.controllerPort1->clock -= clocks * (uint64)device.controllerPort1->frequency;
  device.controllerPort2->clock -= clocks * (uint64)device.controllerPort2->frequency;
  synchronizeDevices();
}

auto CPU::synchronizeSMP() -> void {
  if(SMP::Threaded) {
    if(smp.clock < 0) co_switch(smp.thread);
  } else {
    while(smp.clock < 0) smp.main();
  }
}

auto CPU::synchronizePPU() -> void {
  if(PPU::Threaded) {
    if(ppu.clock < 0) co_switch(ppu.thread);
  } else {
    while(ppu.clock < 0) ppu.main();
  }
}

auto CPU::synchronizeCoprocessors() -> void {
  for(auto chip : coprocessors) {
    if(chip->clock < 0) co_switch(chip->thread);
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
  if(status.interrupt_pending) {
    status.interrupt_pending = false;
    if(status.nmi_pending) {
      status.nmi_pending = false;
      regs.vector = !regs.e ? 0xffea : 0xfffa;
      interrupt();
      debugger.op_nmi();
    } else if(status.irq_pending) {
      status.irq_pending = false;
      regs.vector = !regs.e ? 0xffee : 0xfffe;
      interrupt();
      debugger.op_irq();
    } else if(status.reset_pending) {
      status.reset_pending = false;
      addClocks(132);
      regs.vector = 0xfffc;
      interrupt();
    } else if(status.power_pending) {
      status.power_pending = false;
      addClocks(186);
      regs.pc.l = bus.read(0xfffc, regs.mdr);
      regs.pc.h = bus.read(0xfffd, regs.mdr);
    }
  }

  debugger.op_exec(regs.pc.d);
  instruction();
}

auto CPU::enable() -> void {
  function<auto (uint24, uint8) -> uint8> reader;
  function<auto (uint24, uint8) -> void> writer;

  reader = {&CPU::apuPortRead, this};
  writer = {&CPU::apuPortWrite, this};
  bus.map(reader, writer, 0x00, 0x3f, 0x2140, 0x217f);
  bus.map(reader, writer, 0x80, 0xbf, 0x2140, 0x217f);

  reader = {&CPU::cpuPortRead, this};
  writer = {&CPU::cpuPortWrite, this};
  bus.map(reader, writer, 0x00, 0x3f, 0x2180, 0x2183);
  bus.map(reader, writer, 0x80, 0xbf, 0x2180, 0x2183);

  bus.map(reader, writer, 0x00, 0x3f, 0x4016, 0x4017);
  bus.map(reader, writer, 0x80, 0xbf, 0x4016, 0x4017);

  bus.map(reader, writer, 0x00, 0x3f, 0x4200, 0x421f);
  bus.map(reader, writer, 0x80, 0xbf, 0x4200, 0x421f);

  reader = {&CPU::dmaPortRead, this};
  writer = {&CPU::dmaPortWrite, this};
  bus.map(reader, writer, 0x00, 0x3f, 0x4300, 0x437f);
  bus.map(reader, writer, 0x80, 0xbf, 0x4300, 0x437f);

  reader = [](uint24 addr, uint8) -> uint8 { return cpu.wram[addr]; };
  writer = [](uint24 addr, uint8 data) -> void { cpu.wram[addr] = data; };
  bus.map(reader, writer, 0x00, 0x3f, 0x0000, 0x1fff, 0x002000);
  bus.map(reader, writer, 0x80, 0xbf, 0x0000, 0x1fff, 0x002000);
  bus.map(reader, writer, 0x7e, 0x7f, 0x0000, 0xffff, 0x020000);
}

auto CPU::power() -> void {
  for(auto& byte : wram) byte = random(0x55);

  //CPU
  regs.a = regs.x = regs.y = 0x0000;
  regs.s = 0x01ff;

  //DMA
  for(auto& channel : this->channel) {
    channel.direction = 1;
    channel.indirect = true;
    channel.unused = true;
    channel.reverse_transfer = true;
    channel.fixed_transfer = true;
    channel.transfer_mode = 7;

    channel.dest_addr = 0xff;

    channel.source_addr = 0xffff;
    channel.source_bank = 0xff;

    channel.transfer_size = 0xffff;
    channel.indirect_bank = 0xff;

    channel.hdma_addr = 0xffff;
    channel.line_counter = 0xff;
    channel.unknown = 0xff;
  }

  status.power_pending = true;
  status.interrupt_pending = true;
}

auto CPU::reset() -> void {
  create(Enter, system.cpuFrequency());
  coprocessors.reset();
  PPUcounter::reset();

  //CPU
  regs.pc     = 0x000000;
  regs.x.h    = 0x00;
  regs.y.h    = 0x00;
  regs.s.h    = 0x01;
  regs.d      = 0x0000;
  regs.db     = 0x00;
  regs.p      = 0x34;
  regs.e      = 1;
  regs.mdr    = 0x00;
  regs.wai    = false;
  regs.vector = 0xfffc;  //reset vector address

  //$2140-217f
  for(auto& port : status.port) port = 0x00;

  //$2181-$2183
  status.wram_addr = 0x000000;

  //$4016-$4017
  status.joypad_strobe_latch = 0;
  status.joypad1_bits = ~0;
  status.joypad2_bits = ~0;

  //$4200
  status.nmi_enabled = false;
  status.hirq_enabled = false;
  status.virq_enabled = false;
  status.auto_joypad_poll = false;

  //$4201
  status.pio = 0xff;

  //$4202-$4203
  status.wrmpya = 0xff;
  status.wrmpyb = 0xff;

  //$4204-$4206
  status.wrdiva = 0xffff;
  status.wrdivb = 0xff;

  //$4207-$420a
  status.hirq_pos = 0x01ff;
  status.virq_pos = 0x01ff;

  //$420d
  status.rom_speed = 8;

  //$4214-$4217
  status.rddiv = 0x0000;
  status.rdmpy = 0x0000;

  //$4218-$421f
  status.joy1 = 0x0000;
  status.joy2 = 0x0000;
  status.joy3 = 0x0000;
  status.joy4 = 0x0000;

  //ALU
  alu.mpyctr = 0;
  alu.divctr = 0;
  alu.shift = 0;

  //DMA
  for(auto& channel : this->channel) {
    channel.dma_enabled = false;
    channel.hdma_enabled = false;

    channel.hdma_completed = false;
    channel.hdma_do_transfer = false;
  }

  pipe.valid = false;
  pipe.addr = 0;
  pipe.data = 0;

  //Timing
  status.clock_count = 0;
  status.line_clocks = lineclocks();

  status.irq_lock = false;
  status.dram_refresh_position = (cpu_version == 1 ? 530 : 538);
  status.dram_refreshed = false;

  status.hdma_init_position = (cpu_version == 1 ? 12 + 8 - dmaCounter() : 12 + dmaCounter());
  status.hdma_init_triggered = false;

  status.hdma_position = 1104;
  status.hdma_triggered = false;

  status.nmi_valid      = false;
  status.nmi_line       = false;
  status.nmi_transition = false;
  status.nmi_pending    = false;
  status.nmi_hold       = false;

  status.irq_valid      = false;
  status.irq_line       = false;
  status.irq_transition = false;
  status.irq_pending    = false;
  status.irq_hold       = false;

  status.reset_pending = !status.power_pending;
  status.interrupt_pending = true;

  status.dma_active   = false;
  status.dma_counter  = 0;
  status.dma_clocks   = 0;
  status.dma_pending  = false;
  status.hdma_pending = false;
  status.hdma_mode    = 0;

  status.auto_joypad_active  = false;
  status.auto_joypad_latch   = false;
  status.auto_joypad_counter = 0;
  status.auto_joypad_clock   = 0;
}

}
