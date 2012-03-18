#include <gba/gba.hpp>

#define SYSTEM_CPP
namespace GBA {

System system;

void System::BIOS::load(const uint8_t *newdata, unsigned newsize) {
  if(data) delete[] data;
  data = new uint8[size = newsize];
  memcpy(data, newdata, newsize);

  string sha256 = nall::sha256(data, size);
  if(size != 16384 || sha256 != "fd2547724b505f487e6dcb29ec2ecff3af35a841a77ab2e85fd87350abd36570") {
    interface->message("Warning: Game Boy Advance BIOS SHA256 sum is incorrect.");
  }
}

System::BIOS::BIOS() {
  data = nullptr;
  size = 0u;
}

System::BIOS::~BIOS() {
  delete[] data;
}

void System::run() {
  scheduler.enter();
}

void System::init() {
}

void System::term() {
}

void System::load() {
}

void System::unload() {
  cartridge.unload();
}

void System::power() {
  cartridge.power();
  cpu.power();
  ppu.power();
  apu.power();
  scheduler.power();
}

}
