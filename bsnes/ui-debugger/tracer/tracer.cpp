#include "../base.hpp"
Tracer *tracer = nullptr;

void Tracer::resetMask() {
  memset(cpuMask, 0, 0x200000);
  memset(smpMask, 0, 0x2000);
}

bool Tracer::maskCPU(uint24 addr) {
  if(mask == false) return false;
  if(cpuMask[addr >> 3] & (1 << (addr & 7))) return true;
  cpuMask[addr >> 3] |= 1 << (addr & 7);
  return false;
}

bool Tracer::maskSMP(uint16 addr) {
  if(mask == false) return false;
  if(smpMask[addr >> 3] & (1 << (addr & 7))) return true;
  smpMask[addr >> 3] |= 1 << (addr & 7);
  return false;
}

bool Tracer::enabled() {
  return fp.open();
}

void Tracer::enable(bool state) {
  if(state == false) {
    debugger->print("Tracer disabled\n");
    fp.close();
    return;
  }

  //try not to overwrite existing traces: scan from 001-999.
  //if all files exist, use 000, even if it overwrites another log.
  unsigned n = 1;
  do {
    if(file::exists({ interface->pathName, "debug/trace-", decimal<3, '0'>(n), ".log" }) == false) break;
  } while(++n <= 999);

  string filename = { interface->pathName, "debug/trace-", decimal<3, '0'>(n), ".log" };
  if(fp.open(filename, file::mode::write) == false) return;
  debugger->print("Tracing to ", filename, "\n");
}

Tracer::Tracer() {
  mask = false;
  cpuMask = new uint8_t[0x200000]();
  smpMask = new uint8_t[0x2000]();
}

Tracer::~Tracer() {
  delete[] cpuMask;
  delete[] smpMask;
}
