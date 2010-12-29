//4194304hz (4 * 1024 * 1024)
//70224 clocks/frame
//  456 clocks/scanline
//  154 scanlines/frame

#ifdef CPU_CPP

#include "opcode.cpp"

void CPU::add_clocks(unsigned clocks) {
  clock += clocks;

  if(clock >= 456) scanline();
}

void CPU::scanline() {
  clock -= 456;

  lcd.status.ly++;
  if(lcd.status.ly >= 154) frame();
}

void CPU::frame() {
  lcd.status.ly -= 154;
  scheduler.exit();
}

#endif
