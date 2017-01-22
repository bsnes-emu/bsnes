#include <md/md.hpp>

namespace MegaDrive {

YM2612 ym2612;

auto YM2612::Enter() -> void {
  while(true) scheduler.synchronize(), ym2612.main();
}

auto YM2612::main() -> void {
  step(1);
}

auto YM2612::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto YM2612::power() -> void {
  create(YM2612::Enter, system.colorburst() * 15.0 / 7.0);
}

}
