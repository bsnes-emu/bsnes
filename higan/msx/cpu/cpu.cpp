#include <msx/msx.hpp>

namespace MSX {

CPU cpu;
#include "memory.cpp"
#include "serialization.cpp"

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
  instruction();
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
}

auto CPU::synchronizing() const -> bool {
  return scheduler.synchronizing();
}

auto CPU::power() -> void {
}

}
