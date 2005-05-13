#include "../base.h"

void Clock::enable_overscan(bool n) {
  status.overscan = n;
}

void Clock::enable_interlace(bool n) {
  status.interlace = n;
}

bool Clock::overscan() {
  return status.overscan;
}

bool Clock::interlace() {
  return status.interlace;
}

bool Clock::interlace_field() {
  return status.interlace_field;
}

uint16 Clock::vcounter() {
  return status.vcounter;
}

uint16 Clock::hcounter() {
  return status.hcounter;
}

uint16 Clock::hcycles() {
  return status.hcycles;
}

uint16 Clock::visible_scanlines() {
  return (status.overscan)?239:224;
}

void Clock::set_frameskip(uint8 fs) {}

Clock::Clock() {
  status.overscan        = false;
  status.interlace       = false;
  status.interlace_field = 0;

  status.vcounter = 0;
  status.hcounter = 0;
  status.hcycles  = 0;
}
