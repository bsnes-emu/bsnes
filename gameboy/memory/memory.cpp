#include <gameboy.hpp>

#define MEMORY_CPP
namespace GameBoy {

Bus bus;

uint8_t& Memory::operator[](unsigned addr) {
  return data[addr];
}

void Memory::allocate(unsigned size_) {
  free();
  size = size_;
  data = new uint8_t[size]();
}

void Memory::copy(const uint8_t *data_, unsigned size_) {
  free();
  size = size_;
  data = new uint8_t[size];
  memcpy(data, data_, size);
}

void Memory::free() {
  if(data) {
    delete[] data;
    data = 0;
  }
}

Memory::Memory() {
  data = 0;
  size = 0;
}

Memory::~Memory() {
  free();
}

//

uint8 Bus::read(uint16 addr) {
  if(/* addr >= 0x0000 && */ addr <= 0x7fff) {  //Cartridge ROM
    return cartrom[addr];
  }

  if(/* addr >= 0x8000 && */ addr <= 0x9fff) {  //VRAM
    return vram[addr & 0x1fff];
  }

  if(/* addr >= 0xa000 && */ addr <= 0xbfff) {  //Cartridge RAM
    if(cartram.size == 0) return 0x00;
    return cartram[addr & 0x1fff];
  }

  if(/* addr >= 0xc000 && */ addr <= 0xdfff) {  //WRAM
    return wram[addr & 0x1fff];
  }

  if(/* addr >= 0xe000 && */ addr <= 0xfdff) {  //WRAM (mirror)
    return wram[addr & 0x1fff];
  }

  if(/* addr >= 0xfe00 && */ addr <= 0xfe9f) {  //OAM
    return oam[addr & 0xff];
  }

  if(/* addr >= 0xfea0 && */ addr <= 0xfeff) {  //unmapped
    return 0x00;
  }

  if(/* addr >= 0xff00 && */ addr <= 0xff7f) {  //MMIO
    return 0x00;
  }

  if(/* addr >= 0xff80 && */ addr <= 0xfffe) {  //HRAM
    return hram[addr & 0x7f];
  }

  //addr == 0xffff (interrupt enable register)
  return 0x00;
}

void Bus::write(uint16 addr, uint8 data) {
  if(/* addr >= 0x0000 && */ addr <= 0x7fff) {  //Cartridge ROM
    return;
  }

  if(/* addr >= 0x8000 && */ addr <= 0x9fff) {  //VRAM
    vram[addr & 0x1fff] = data;
    return;
  }

  if(/* addr >= 0xa000 && */ addr <= 0xbfff) {  //Cartridge RAM
    if(cartram.size == 0) return;
    cartram[addr & 0x1fff] = data;
    return;
  }

  if(/* addr >= 0xc000 && */ addr <= 0xdfff) {  //WRAM
    wram[addr & 0x1fff] = data;
    return;
  }

  if(/* addr >= 0xe000 && */ addr <= 0xfdff) {  //WRAM (mirror)
    wram[addr & 0x1fff] = data;
    return;
  }

  if(/* addr >= 0xfe00 && */ addr <= 0xfe9f) {  //OAM
    oam[addr & 0xff] = data;
    return;
  }

  if(/* addr >= 0xfea0 && */ addr <= 0xfeff) {  //unmapped
    return;
  }

  if(/* addr >= 0xff00 && */ addr <= 0xff7f) {  //MMIO
    return;
  }

  if(/* addr >= 0xff80 && */ addr <= 0xfffe) {  //HRAM
    hram[addr & 0x7f] = data;
    return;
  }

  //addr == 0xffff (interrupt enable register)
  return;
}

Bus::Bus() {
  vram.allocate(8192);
  wram.allocate(8192);
  oam.allocate ( 160);
  hram.allocate( 128);
}

}
