#include <md/md.hpp>

namespace MegaDrive {

PSG psg;

auto PSG::Enter() -> void {
  while(true) scheduler.synchronize(), psg.main();
}

auto PSG::main() -> void {
  step(frequency);
}

auto PSG::step(uint clocks) -> void {
}

auto PSG::power() -> void {
}

auto PSG::reset() -> void {
  create(PSG::Enter, system.colorburst());
}

}
