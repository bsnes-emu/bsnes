#include <gba/gba.hpp>

namespace GBA {

System system;

void System::BIOS::load(const uint8_t *biosdata, unsigned biossize) {
  memcpy(data, biosdata, min(size, biossize));

  string sha256 = nall::sha256(data, size);
  if(sha256 != "fd2547724b505f487e6dcb29ec2ecff3af35a841a77ab2e85fd87350abd36570") {
    interface->message("Warning: Game Boy Advance BIOS SHA256 sum is incorrect.");
  }
}

System::BIOS::BIOS() {
  data = new uint8[size = 16384]();
}

void System::init() {
}

void System::term() {
}

void System::power() {
  bus.power();
  cpu.power();
  ppu.power();
  apu.power();
  scheduler.power();
}

void System::run() {
  scheduler.enter();
}

}
