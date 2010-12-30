#include <gameboy.hpp>

#define CPU_CPP
namespace GameBoy {

#include "core/core.cpp"
#include "mmio/mmio.cpp"
#include "timing/timing.cpp"
CPU cpu;

void CPU::Main() {
  cpu.main();
}

void CPU::main() {
  while(true) {
  //print(disassemble(r[PC]), "\n");
    interrupt_test();
    uint8 opcode = op_read(r[PC]++);
    (this->*opcode_table[opcode])();
  }
}

void CPU::interrupt_test() {
  if(status.ime) {
    if(status.interrupt_request_vblank && status.interrupt_enable_vblank) {
      status.interrupt_request_vblank = 0;
      return interrupt_exec(0x0040);
    }

    if(status.interrupt_request_stat && status.interrupt_enable_stat) {
      status.interrupt_request_stat = 0;
      return interrupt_exec(0x0048);
    }

    if(status.interrupt_request_timer && status.interrupt_enable_timer) {
      status.interrupt_request_timer = 0;
      return interrupt_exec(0x0050);
    }

    if(status.interrupt_request_serial && status.interrupt_enable_serial) {
      status.interrupt_request_serial = 0;
      return interrupt_exec(0x0058);
    }

    if(status.interrupt_request_joypad && status.interrupt_enable_joypad) {
      status.interrupt_request_joypad = 0;
      return interrupt_exec(0x0060);
    }
  }
}

void CPU::interrupt_exec(uint16 pc) {
  status.ime = 0;
  op_write(--r[SP], r[PC] >> 8);
  op_write(--r[SP], r[PC] >> 0);
  r[PC] = pc;
  op_io();
}

void CPU::power() {
  for(unsigned n = 0xc000; n <= 0xdfff; n++) bus.mmio[n] = this;  //WRAM
  for(unsigned n = 0xe000; n <= 0xfdff; n++) bus.mmio[n] = this;  //WRAM (mirror)
  for(unsigned n = 0xff00; n <= 0xff0f; n++) bus.mmio[n] = this;  //MMIO
  for(unsigned n = 0xff80; n <= 0xffff; n++) bus.mmio[n] = this;  //HRAM+IE

  for(unsigned n = 0; n < 8192; n++) wram[n] = 0x00;
  for(unsigned n = 0; n <  128; n++) hram[n] = 0x00;

  reset();
}

void CPU::reset() {
  create(Main, 4 * 1024 * 1024);

  r[PC] = 0x0100;
  r[SP] = 0xfffe;
  r[AF] = 0x0000;
  r[BC] = 0x0000;
  r[DE] = 0x0000;
  r[HL] = 0x0000;

  status.ime = 0;
  status.timer0 = 0;
  status.timer1 = 0;
  status.timer2 = 0;
  status.timer3 = 0;

  status.p15 = 0;
  status.p14 = 0;

  status.div = 0;

  status.tima = 0;

  status.tma = 0;

  status.timer_enable = 0;
  status.timer_clock = 0;

  status.interrupt_request_joypad = 0;
  status.interrupt_request_serial = 0;
  status.interrupt_request_timer = 0;
  status.interrupt_request_stat = 0;
  status.interrupt_request_vblank = 0;

  status.interrupt_enable_joypad = 0;
  status.interrupt_enable_serial = 0;
  status.interrupt_enable_timer = 0;
  status.interrupt_enable_stat = 0;
  status.interrupt_enable_vblank = 0;
}

CPU::CPU() {
  initialize_opcode_table();
}

}
