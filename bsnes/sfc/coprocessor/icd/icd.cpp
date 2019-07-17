#include <sfc/sfc.hpp>

namespace SuperFamicom {

ICD icd;
#include "interface.cpp"
#include "io.cpp"
#include "boot-roms.cpp"
#include "serialization.cpp"

namespace SameBoy {
  static auto hreset(GB_gameboy_t*) -> void {
    icd.ly++;
    icd.ppuScanline();
  }

  static auto vreset(GB_gameboy_t*) -> void {
    icd.ly = 0;
    icd.ppuScanline();
  }

  static auto icd_pixel(GB_gameboy_t*, uint8_t pixel) -> void {
    icd.ppuOutput(pixel);
  }

  static auto joyp_write(GB_gameboy_t*, uint8_t value) -> void {
    bool p14 = value & 0x10;
    bool p15 = value & 0x20;
    icd.joypWrite(p14, p15);
  }

  static auto rgb_encode(GB_gameboy_t*, uint8_t r, uint8_t g, uint8_t b) -> uint32_t {
    return r << 16 | g << 8 | b << 0;
  }

  static auto sample(GB_gameboy_t*, GB_sample_t* sample) -> void {
    float left  = sample->left  / 32768.0f;
    float right = sample->right / 32768.0f;
    icd.apuOutput(left, right);
  }

  static auto vblank(GB_gameboy_t*) -> void {
  }
}

auto ICD::Enter() -> void {
  while(true) {
    scheduler.synchronize();
    icd.main();
  }
}

auto ICD::main() -> void {
  if(r6003 & 0x80) {
    auto clocks = GB_run(&sameboy);
    step(clocks >> 1);
  } else {  //DMG halted
    stream->sample(float(0.0), float(0.0));
    step(128);
  }
  synchronize(cpu);
}

auto ICD::load() -> bool {
  information = {};

  GB_random_set_enabled(false);
  if(Frequency == 0) {
    GB_init(&sameboy, GB_MODEL_SGB_NO_SFC);
    GB_load_boot_rom_from_buffer(&sameboy, (const unsigned char*)&SGB1BootROM[0], 256);
    GB_set_sample_rate(&sameboy, uint(system.cpuFrequency() / 5.0 / 128.0));
  } else {
    GB_init(&sameboy, GB_MODEL_SGB2_NO_SFC);
    GB_load_boot_rom_from_buffer(&sameboy, (const unsigned char*)&SGB2BootROM[0], 256);
    GB_set_sample_rate(&sameboy, uint(Frequency / 5.0 / 128.0));
  }
  GB_set_highpass_filter_mode(&sameboy, GB_HIGHPASS_ACCURATE);
  GB_set_icd_hreset_callback(&sameboy, &SameBoy::hreset);
  GB_set_icd_vreset_callback(&sameboy, &SameBoy::vreset);
  GB_set_icd_pixel_callback(&sameboy, &SameBoy::icd_pixel);
  GB_set_joyp_write_callback(&sameboy, &SameBoy::joyp_write);
  GB_set_rgb_encode_callback(&sameboy, &SameBoy::rgb_encode);
  GB_apu_set_sample_callback(&sameboy, &SameBoy::sample);
  GB_set_vblank_callback(&sameboy, &SameBoy::vblank);
  GB_set_pixels_output(&sameboy, &bitmap[0]);
  if(auto loaded = platform->load(ID::GameBoy, "Game Boy", "gb")) {
    information.pathID = loaded.pathID;
  } else return unload(), false;
  if(auto fp = platform->open(pathID(), "manifest.bml", File::Read, File::Required)) {
    auto manifest = fp->reads();
    cartridge.slotGameBoy.load(manifest);
  } else return unload(), false;
  if(auto fp = platform->open(pathID(), "program.rom", File::Read, File::Required)) {
    auto size = fp->size();
    auto data = (uint8_t*)malloc(size);
    cartridge.information.sha256 = Hash::SHA256({data, (uint64_t)size}).digest();
    fp->read(data, size);
    GB_load_rom_from_buffer(&sameboy, data, size);
  } else return unload(), false;
  ly = 0;
  return true;
}

auto ICD::unload() -> void {
  GB_free(&sameboy);
}

auto ICD::power() -> void {
  //SGB1 uses CPU oscillator; SGB2 uses dedicated oscillator
  create(ICD::Enter, (Frequency ? Frequency : system.cpuFrequency()) / 5.0);
  stream = Emulator::audio.createStream(2, uint((Frequency ? Frequency : system.cpuFrequency()) / 5.0 / 128.0));

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

  GB_reset(&sameboy);
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

  GB_reset(&sameboy);
}

}
