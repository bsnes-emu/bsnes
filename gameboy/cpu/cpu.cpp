//Sharp LR35902 @ 4.19MHz

#include <gameboy.hpp>

#define CPU_CPP
namespace GameBoy {

CPU cpu;

void CPU::main() {
  while(true) {
    uint8 opcode = bus.read(r.pc++);
    switch(opcode) {
      case 0x00: break;  //NOP
    }
  }
}

void CPU::power() {
  reset();
}

void CPU::reset() {
  r.pc = 0x0100;
}

}
