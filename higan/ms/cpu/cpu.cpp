#include <ms/ms.hpp>

namespace MasterSystem {

CPU cpu;

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
}

}
