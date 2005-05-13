#include "../base.h"
#include "dcpu.cpp"

CPU::CPU() {
  mmio = &mmio_unmapped;
}
