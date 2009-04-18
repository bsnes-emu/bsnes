#include <../base.hpp>
#define CPU_CPP

void CPU::power() {
  cpu_version = snes.config.cpu.version;
}

void CPU::reset() {
}

CPU::CPU() {
}

CPU::~CPU() {
}
