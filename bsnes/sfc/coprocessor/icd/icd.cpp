#include <sfc/sfc.hpp>

namespace SuperFamicom {

ICD icd;

#include "platform.cpp"
#include "interface.cpp"
#include "io.cpp"
#include "boot-roms.cpp"
#include "serialization.cpp"

namespace SameBoy {
  static auto rgbEncode(GB_gameboy_t*, uint8_t r, uint8_t g, uint8_t b) -> uint32_t {
    return r << 16 | g << 8 | b << 0;
  }
}

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
    stream->sample(float(0.0), float(0.0));
    step(2);  //two clocks per audio sample
  }
  synchronize(cpu);
}

auto ICD::load() -> bool {
  information = {};

  GB_random_set_enabled(false);
  GB_init(&sameboy, GB_MODEL_DMG_B);
  if(Frequency == 0) {
    GB_load_boot_rom_from_buffer(&sameboy, (const unsigned char*)&SGB1BootROM, 256);
  } else {
    GB_load_boot_rom_from_buffer(&sameboy, (const unsigned char*)&SGB2BootROM, 256);
  }
  GB_set_pixels_output(&sameboy, bitmap);
  GB_set_rgb_encode_callback(&sameboy, SameBoy::rgbEncode);
  if(auto loaded = platform->load(ID::GameBoy, "Game Boy", "gb")) {
    information.pathID = loaded.pathID;
  } else return false;
  if(auto fp = platform->open(pathID(), "program.rom", File::Read, File::Required)) {
    auto size = fp->size();
    auto data = (uint8_t*)malloc(size);
    fp->read(data, size);
    GB_load_rom_from_buffer(&sameboy, data, size);
  } else return false;
  GameBoy::superGameBoy = this;
  GameBoy::system.load(&gameBoyInterface, GameBoy::System::Model::SuperGameBoy, cartridge.pathID());
  return cartridge.loadGameBoy();
}

auto ICD::unload() -> void {
  GB_free(&sameboy);
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

}
