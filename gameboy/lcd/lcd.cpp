#include <gameboy.hpp>

#define LCD_CPP
namespace GameBoy {

#include "mmio/mmio.cpp"
LCD lcd;

void LCD::power() {
  for(unsigned n = 0xa000; n <= 0xbfff; n++) bus.mmio[n] = this;  //VRAM
  for(unsigned n = 0xff40; n <= 0xff4f; n++) bus.mmio[n] = this;  //MMIO
  for(unsigned n = 0xfe00; n <= 0xfe9f; n++) bus.mmio[n] = this;  //OAM

  reset();
}

void LCD::reset() {
  status.ly = 0;
}

}
