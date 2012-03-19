#include "registers.cpp"

void ARM::power() {
  processor.power();
  pipeline.reload = true;
  exception = false;
  r(15).modify = [&] { pipeline.reload = true; };
}
