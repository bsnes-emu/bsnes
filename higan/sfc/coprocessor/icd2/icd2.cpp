#include <sfc/sfc.hpp>

namespace SuperFamicom {

#include "interface/interface.cpp"
#include "mmio/mmio.cpp"
#include "serialization.cpp"
ICD2 icd2;

auto ICD2::Enter() -> void {
  while(true) {
    if(scheduler.synchronizing()) GameBoy::system.runToSave();
    scheduler.synchronize();
    icd2.main();
  }
}

auto ICD2::main() -> void {
  if(r6003 & 0x80) {
    GameBoy::system.run();
    step(GameBoy::system._clocksExecuted);
    GameBoy::system._clocksExecuted = 0;
  } else {  //DMG halted
    audio.coprocessorSample(0, 0);
    step(1);
  }
  synchronizeCPU();
}

auto ICD2::init() -> void {
}

auto ICD2::load() -> void {
  bind = GameBoy::interface->bind;
  hook = GameBoy::interface->hook;
  GameBoy::interface->bind = this;
  GameBoy::interface->hook = this;
  interface->loadRequest(ID::GameBoy, "Game Boy", "gb", false);
  GameBoy::interface->load(GameBoy::ID::SuperGameBoy);
  cartridge.loadGameBoy();
}

auto ICD2::unload() -> void {
  GameBoy::interface->unload();
  GameBoy::interface->bind = bind;
  GameBoy::interface->hook = hook;
}

auto ICD2::power() -> void {
  audio.coprocessorEnable(true);
  audio.coprocessorFrequency(2 * 1024 * 1024);
}

auto ICD2::reset() -> void {
  create(ICD2::Enter, cpu.frequency / 5);

  r6003 = 0x00;
  r6004 = 0xff;
  r6005 = 0xff;
  r6006 = 0xff;
  r6007 = 0xff;
  for(auto& r : r7000) r = 0x00;
  mlt_req = 0;

  for(auto& n : output) n = 0xff;
  read_bank = 0;
  read_addr = 0;
  write_bank = 0;
  write_addr = 0;

  packetsize = 0;
  joyp_id = 3;
  joyp15lock = 0;
  joyp14lock = 0;
  pulselock = true;

  GameBoy::system.init();
  GameBoy::system.power();
}

}
