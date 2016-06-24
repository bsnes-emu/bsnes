#include <sfc/sfc.hpp>

namespace SuperFamicom {

ICD2 icd2;

#if defined(SFC_SUPERGAMEBOY)

#include "interface.cpp"
#include "mmio.cpp"
#include "serialization.cpp"

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
    stream->sample(0.0, 0.0);
    step(2);  //two clocks per audio sample
  }
  synchronizeCPU();
}

auto ICD2::init() -> void {
}

auto ICD2::load() -> bool {
  bind = GameBoy::interface->bind;
  hook = GameBoy::interface->hook;
  GameBoy::interface->bind = this;
  GameBoy::interface->hook = this;
  GameBoy::interface->load(GameBoy::ID::SuperGameBoy);
  return cartridge.loadGameBoy();
}

auto ICD2::unload() -> void {
  GameBoy::interface->unload();
  GameBoy::interface->bind = bind;
  GameBoy::interface->hook = hook;
}

auto ICD2::power() -> void {
}

auto ICD2::reset(bool soft) -> void {
  create(ICD2::Enter, cpu.frequency / 5);
  if(!soft) stream = Emulator::audio.createStream(2, cpu.frequency / 10);

  r6003 = 0x00;
  r6004 = 0xff;
  r6005 = 0xff;
  r6006 = 0xff;
  r6007 = 0xff;
  for(auto& r : r7000) r = 0x00;
  mltReq = 0;

  for(auto& n : output) n = 0xff;
  readBank = 0;
  readAddress = 0;
  writeBank = 0;
  writeAddress = 0;

  packetSize = 0;
  joypID = 3;
  joyp15Lock = 0;
  joyp14Lock = 0;
  pulseLock = true;

  GameBoy::system.init();
  GameBoy::system.power();
}

#endif

}
