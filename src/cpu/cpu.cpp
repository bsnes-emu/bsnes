#include "../base.h"
#include "dcpu.cpp"

CPU::CPU() {
  cpu_version = 1;
  mmio = &mmio_unmapped;
}
