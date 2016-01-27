#include <processor/processor.hpp>
#include "v30mz.hpp"

namespace Processor {

auto V30MZ::exec() -> void {
  step(1);
}

auto V30MZ::power() -> void {
  r.ax = 0x0000;
  r.cx = 0x0000;
  r.dx = 0x0000;
  r.bx = 0x0000;
  r.sp = 0x0000;
  r.bp = 0x0000;
  r.si = 0x0000;
  r.di = 0x0000;
  r.es = 0x0000;
  r.cs = 0xffff;
  r.ss = 0x0000;
  r.ds = 0x0000;
}

}
