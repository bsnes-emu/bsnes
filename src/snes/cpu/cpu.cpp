#include <snes/snes.hpp>

#define CPU_CPP
namespace SNES {

#if defined(DEBUGGER)
  #include "cpu-debugger.cpp"
#endif

void CPU::power() {
  cpu_version = config.cpu.version;
}

void CPU::reset() {
  PPUCounter::reset();
}

void CPU::serialize(serializer &s) {
  PPUCounter::serialize(s);
  s.integer(cpu_version);
}

CPU::CPU() {
}

CPU::~CPU() {
}

}
