#include <../base.hpp>

#define CPU_CPP
namespace SNES {

void CPU::power() {
  cpu_version = config.cpu.version;
}

void CPU::reset() {
}

CPU::CPU() {
}

CPU::~CPU() {
}
};

