#include <sfc/sfc.hpp>

namespace SuperFamicom {

ICD icd;

#if defined(CORE_GB)

#include "platform.cpp"
#include "interface.cpp"
#include "io.cpp"
#include "serialization.cpp"

auto ICD::Enter() -> void {
  while(true) {
    if(scheduler.synchronizing()) GameBoy::system.runToSave();
    scheduler.synchronize();
    icd.main();
  }
}

auto ICD::main() -> void {
  if(r6003 & 0x80) {
    GameBoy::system.run();
    step(GameBoy::system._clocksExecuted);
    GameBoy::system._clocksExecuted = 0;
  } else {  //DMG halted
    stream->sample(0.0, 0.0);
    step(2);  //two clocks per audio sample
  }
  synchronize(cpu);
}

auto ICD::load() -> bool {
  GameBoy::superGameBoy = this;
  GameBoy::system.load(&gameBoyInterface, GameBoy::System::Model::SuperGameBoy, cartridge.pathID());
  return cartridge.loadGameBoy();
}

auto ICD::unload() -> void {
  GameBoy::system.save();
  GameBoy::system.unload();
}

auto ICD::power() -> void {
  //SGB1 uses CPU oscillator; SGB2 uses dedicated oscillator
  create(ICD::Enter, (Frequency ? Frequency : system.cpuFrequency()) / 5.0);
  stream = Emulator::audio.createStream(2, frequency() / 2.0);
  stream->addHighPassFilter(20.0, Emulator::Filter::Order::First);
  stream->addDCRemovalFilter();

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

auto ICD::reset() -> void {
  create(ICD::Enter, (Frequency ? Frequency : system.cpuFrequency()) / 5.0);

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
