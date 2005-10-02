#include "../base.h"
#include "dcpu.cpp"

CPU::CPU() {
  cpu_version = 2;
  mmio = &mmio_unmapped;
}
