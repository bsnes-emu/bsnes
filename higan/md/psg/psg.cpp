#include <md/md.hpp>

namespace MegaDrive {

PSG psg;

auto PSG::Enter() -> void {
  while(true) scheduler.synchronize(), psg.main();
}

auto PSG::main() -> void {
  step(1);
}

auto PSG::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto PSG::power() -> void {
}

auto PSG::reset() -> void {
  create(PSG::Enter, system.colorburst());
}

}
