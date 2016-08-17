#include <ms/ms.hpp>

namespace MasterSystem {

VDP vdp;

auto VDP::Enter() -> void {
  while(true) scheduler.synchronize(), vdp.main();
}

auto VDP::main() -> void {
}

auto VDP::step(uint clocks) -> void {
  Thread::step(clocks);
}

}
