#include <../base.hpp>

#define SGB_CPP
namespace SNES {

SuperGameBoy sgb;

void SuperGameBoy::enter() {
  while(true) {
    if(sgb_run) {
      unsigned samples = sgb_run(samplebuffer, 16);
      scheduler.addclocks_cop(samples * 10);
      scheduler.sync_copcpu();
    } else {
      scheduler.addclocks_cop(64 * 1024 * 1024);
      scheduler.sync_copcpu();
    }
  }
}

uint8_t SuperGameBoy::read(unsigned addr) {
  addr &= 0xffff;
  if(sgb_read) return sgb_read(addr);
  return 0x00;
}

void SuperGameBoy::write(unsigned addr, uint8_t data) {
  addr &= 0xffff;
  if(sgb_write) return sgb_write(addr, data);
}

void SuperGameBoy::init() {
  if(libsgb.open("SuperGameBoy")) {
    sgb_init  = libsgb.sym("sgb_init");
    sgb_term  = libsgb.sym("sgb_term");
    sgb_power = libsgb.sym("sgb_power");
    sgb_reset = libsgb.sym("sgb_reset");
    sgb_read  = libsgb.sym("sgb_read");
    sgb_write = libsgb.sym("sgb_write");
    sgb_run   = libsgb.sym("sgb_run");
  }
}

void SuperGameBoy::enable() {
}

void SuperGameBoy::power() {
  bus.map(Bus::MapDirect, 0x00, 0x3f, 0x6000, 0x7fff, *this);
  bus.map(Bus::MapDirect, 0x80, 0xbf, 0x6000, 0x7fff, *this);

  if(sgb_init) {
    sgb_init(SGB2,
      memory::gbrom.data(), memory::gbrom.size(),
      memory::gbram.data(), memory::gbram.size()
    );
  }

  if(sgb_power) sgb_power();
}

void SuperGameBoy::reset() {
  if(sgb_reset) sgb_reset();
}

};
