#include <ms/ms.hpp>

namespace MasterSystem {

PSG psg;

auto PSG::Enter() -> void {
  while(true) scheduler.synchronize(), psg.main();
}

auto PSG::main() -> void {
}

auto PSG::step(uint clocks) -> void {
  Thread::step(clocks);
}

}
