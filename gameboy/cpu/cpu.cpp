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
    print(disassemble(r[PC]), "\n");

    uint8 opcode = op_read(r[PC]++);
    (this->*opcode_table[opcode])();

    opcode_counter++;
  }
}

void CPU::power() {
  for(unsigned n = 0xc000; n <= 0xdfff; n++) bus.mmio[n] = this;  //WRAM
  for(unsigned n = 0xe000; n <= 0xfdff; n++) bus.mmio[n] = this;  //WRAM (mirror)
  for(unsigned n = 0xff80; n <= 0xfffe; n++) bus.mmio[n] = this;  //HRAM

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
}

CPU::CPU() {
  initialize_opcode_table();
}

}
