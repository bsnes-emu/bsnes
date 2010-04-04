#include "libsnes.hpp"
#include <snes/snes.hpp>

#include <nall/info/snes.hpp>
using namespace nall;

struct Interface : public SNES::Interface {
  snes_video_refresh_t pvideo_refresh;
  snes_audio_sample_t paudio_sample;
  snes_input_poll_t pinput_poll;
  snes_input_state_t pinput_state;

  void video_refresh(uint16_t *data, unsigned pitch, unsigned *line, unsigned width, unsigned height) {
    return pvideo_refresh(data, pitch, line, width, height);
  }

  void audio_sample(uint16_t left, uint16_t right) {
    return paudio_sample(left, right);
  }

  void input_poll() {
    return pinput_poll();
  }

  int16_t input_poll(bool port, SNES::Input::Device device, unsigned index, unsigned id) {
    return pinput_state(port, (unsigned)device, index, id);
  }

  Interface() : pvideo_refresh(0), paudio_sample(0), pinput_poll(0), pinput_state(0) {
  }
};

static Interface interface;

unsigned snes_library_revision() {
  return 1;
}

void snes_init() {
  SNES::system.init(&interface);
  SNES::input.port_set_device(0, SNES::Input::Device::Joypad);
  SNES::input.port_set_device(1, SNES::Input::Device::Joypad);
}

void snes_term() {
  SNES::system.term();
}

void snes_unload() {
  SNES::cartridge.unload();
}

void snes_run() {
  SNES::system.run();
}

void snes_set_video_refresh(snes_video_refresh_t video_refresh) {
  interface.pvideo_refresh = video_refresh;
}

void snes_set_audio_sample(snes_audio_sample_t audio_sample) {
  interface.paudio_sample = audio_sample;
}

void snes_set_input_poll(snes_input_poll_t input_poll) {
  interface.pinput_poll = input_poll;
}

void snes_set_input_state(snes_input_state_t input_state) {
  interface.pinput_state = input_state;
}

void snes_load_cartridge_normal(
  const char *rom_xml, uint8_t *rom_data, unsigned rom_size
) {
  SNES::memory::cartrom.copy(rom_data, rom_size);
  string xmlrom = rom_xml ? string(rom_xml) : snes_information(rom_data, rom_size).xml_memory_map;
  SNES::cartridge.load(SNES::Cartridge::Mode::Normal, { xmlrom });
  SNES::system.power();
}

void snes_load_cartridge_bsx_slotted(
  const char *rom_xml, uint8_t *rom_data, unsigned rom_size,
  const char *bsx_xml, uint8_t *bsx_data, unsigned bsx_size
) {
  SNES::memory::cartrom.copy(rom_data, rom_size);
  string xmlrom = rom_xml ? string(rom_xml) : snes_information(rom_data, rom_size).xml_memory_map;
  SNES::memory::bsxflash.copy(bsx_data, bsx_size);
  string xmlbsx = bsx_xml ? string(bsx_xml) : snes_information(bsx_data, bsx_size).xml_memory_map;
  SNES::cartridge.load(SNES::Cartridge::Mode::BsxSlotted, { xmlrom, xmlbsx });
  SNES::system.power();
}

void snes_load_cartridge_bsx(
  const char *rom_xml, uint8_t *rom_data, unsigned rom_size,
  const char *bsx_xml, uint8_t *bsx_data, unsigned bsx_size
) {
  SNES::memory::cartrom.copy(rom_data, rom_size);
  string xmlrom = rom_xml ? string(rom_xml) : snes_information(rom_data, rom_size).xml_memory_map;
  SNES::memory::bsxflash.copy(bsx_data, bsx_size);
  string xmlbsx = bsx_xml ? string(bsx_xml) : snes_information(bsx_data, bsx_size).xml_memory_map;
  SNES::cartridge.load(SNES::Cartridge::Mode::Bsx, { xmlrom, xmlbsx });
  SNES::system.power();
}

void snes_load_cartridge_sufami_turbo(
  const char *rom_xml, uint8_t *rom_data, unsigned rom_size,
  const char *sta_xml, uint8_t *sta_data, unsigned sta_size,
  const char *stb_xml, uint8_t *stb_data, unsigned stb_size
) {
  SNES::memory::cartrom.copy(rom_data, rom_size);
  string xmlrom = rom_xml ? string(rom_xml) : snes_information(rom_data, rom_size).xml_memory_map;
  SNES::memory::stArom.copy(sta_data, sta_size);
  string xmlsta = sta_xml ? string(sta_xml) : snes_information(sta_data, sta_size).xml_memory_map;
  SNES::memory::stBrom.copy(stb_data, stb_size);
  string xmlstb = stb_xml ? string(stb_xml) : snes_information(stb_data, stb_size).xml_memory_map;
  SNES::cartridge.load(SNES::Cartridge::Mode::SufamiTurbo, { xmlrom, xmlsta, xmlstb });
  SNES::system.power();
}

void snes_load_cartridge_super_game_boy(
  const char *rom_xml, uint8_t *rom_data, unsigned rom_size,
  const char *dmg_xml, uint8_t *dmg_data, unsigned dmg_size
) {
  SNES::memory::cartrom.copy(rom_data, rom_size);
  string xmlrom = rom_xml ? string(rom_xml) : snes_information(rom_data, rom_size).xml_memory_map;
  SNES::memory::gbrom.copy(dmg_data, dmg_size);
  string xmldmg = dmg_xml ? string(dmg_xml) : snes_information(dmg_data, dmg_size).xml_memory_map;
  SNES::cartridge.load(SNES::Cartridge::Mode::SuperGameBoy, { xmlrom, xmldmg });
  SNES::system.power();
}

uint8_t* snes_get_cartridge_ram_data() { return SNES::memory::cartram.data(); }
unsigned snes_get_cartridge_ram_size() { return SNES::memory::cartram.size(); }
uint8_t* snes_get_cartridge_rtc_data() { return SNES::memory::cartrtc.data(); }
unsigned snes_get_cartridge_rtc_size() { return SNES::memory::cartrtc.size(); }
uint8_t* snes_get_bsx_ram_data() { return SNES::memory::bsxram.data(); }
unsigned snes_get_bsx_ram_size() { return SNES::memory::bsxram.size(); }
uint8_t* snes_get_bsx_pram_data() { return SNES::memory::bsxpram.data(); }
unsigned snes_get_bsx_pram_size() { return SNES::memory::bsxpram.size(); }
uint8_t* snes_get_sufami_turbo_a_ram_data() { return SNES::memory::stAram.data(); }
unsigned snes_get_sufami_turbo_a_ram_size() { return SNES::memory::stAram.size(); }
uint8_t* snes_get_sufami_turbo_b_ram_data() { return SNES::memory::stBram.data(); }
unsigned snes_get_sufami_turbo_b_ram_size() { return SNES::memory::stBram.size(); }
uint8_t* snes_get_game_boy_ram_data() { return SNES::memory::gbram.data(); }
unsigned snes_get_game_boy_ram_size() { return SNES::memory::gbram.size(); }
uint8_t* snes_get_game_boy_rtc_data() { return SNES::memory::gbrtc.data(); }
unsigned snes_get_game_boy_rtc_size() { return SNES::memory::gbrtc.size(); }
