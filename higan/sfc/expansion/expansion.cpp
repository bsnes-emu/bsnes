#include <sfc/sfc.hpp>

namespace SuperFamicom {

Expansion::Expansion() {
  if(!thread) create(Expansion::Enter, 1);
}

auto Expansion::Enter() -> void {
  while(true) scheduler.synchronize(), peripherals.expansionPort->main();
}

auto Expansion::main() -> void {
  step(1);
}

}
