#include "libsnes.hpp"
#include <snes/snes.hpp>

struct context_t {
  snes_video_refresh_t video_refresh;
  snes_audio_sample_t audio_sample;
  snes_input_poll_t input_poll;
  snes_input_state_t input_state;

  string xml_cartridge;
  string xml_bsx_cartridge;
  string xml_sufami_turbo_cartridge_a;
  string xml_sufami_turbo_cartridge_b;
  string xml_game_boy_cartridge;
};

static context_t context = { 0, 0, 0, 0 };

struct Interface : public SNES::Interface {
  void video_refresh(uint16_t *data, unsigned pitch, unsigned *line, unsigned width, unsigned height) {
    return context.video_refresh(data, pitch, line, width, height);
  }

  void audio_sample(uint16_t left, uint16_t right) {
    return context.audio_sample(left, right);
  }

  void input_poll() {
    return context.input_poll();
  }

  int16_t input_poll(bool port, SNES::Input::Device device, unsigned index, unsigned id) {
    return context.input_state(port, (unsigned)device, index, id);
  }
};

static Interface interface;

unsigned snes_library_revision() {
  return 0;
}

void snes_init() {
  SNES::system.init(&interface);
  SNES::input.port_set_device(0, SNES::Input::Device::Joypad);
  SNES::input.port_set_device(1, SNES::Input::Device::Joypad);
}

void snes_term() {
  SNES::system.term();
}

bool snes_load() {
  lstring list = { context.xml_cartridge };
  SNES::cartridge.load(SNES::Cartridge::Mode::Normal, list);
  SNES::system.power();
  return true;
}

void snes_unload() {
  SNES::cartridge.unload();
}

void snes_run() {
  SNES::system.run();
}

void snes_load_cartridge(
  const char *xml,
  uint8_t *rom_data, unsigned rom_size,
  uint8_t *ram_data, unsigned ram_size,
  uint8_t *rtc_data, unsigned rtc_size
) {
  context.xml_cartridge = xml;
  SNES::memory::cartrom.map(rom_data, rom_size);
  SNES::memory::cartram.map(ram_data, ram_size);
  SNES::memory::cartrtc.map(rtc_data, rtc_size);
}

void snes_load_bsx_cartridge(
  const char *xml,
  uint8_t *flash_data, unsigned flash_size,
  uint8_t *ram_data, unsigned ram_size,
  uint8_t *pram_data, unsigned pram_size
) {
  context.xml_bsx_cartridge = xml;
  SNES::memory::bsxflash.map(flash_data, flash_size);
  SNES::memory::bsxram.map(ram_data, ram_size);
  SNES::memory::bsxpram.map(pram_data, pram_size);
}

void snes_load_sufami_turbo_cartridge_a(
  const char *xml,
  uint8_t *rom_data, unsigned rom_size,
  uint8_t *ram_data, unsigned ram_size
) {
  context.xml_sufami_turbo_cartridge_a = xml;
  SNES::memory::stArom.map(rom_data, rom_size);
  SNES::memory::stAram.map(ram_data, ram_size);
}

void snes_load_sufami_turbo_cartridge_b(
  const char *xml,
  uint8_t *rom_data, unsigned rom_size,
  uint8_t *ram_data, unsigned ram_size
) {
  context.xml_sufami_turbo_cartridge_b = xml;
  SNES::memory::stBrom.map(rom_data, rom_size);
  SNES::memory::stBram.map(ram_data, ram_size);
}

void snes_load_game_boy_cartridge(
  const char *xml,
  uint8_t *rom_data, unsigned rom_size,
  uint8_t *ram_data, unsigned ram_size,
  uint8_t *rtc_data, unsigned rtc_size
) {
  context.xml_game_boy_cartridge = xml;
  SNES::memory::gbrom.map(rom_data, rom_size);
  SNES::memory::gbram.map(ram_data, ram_size);
  SNES::memory::gbrtc.map(rtc_data, rtc_size);
}

void snes_set_video_refresh(snes_video_refresh_t video_refresh) {
  context.video_refresh = video_refresh;
}

void snes_set_audio_sample(snes_audio_sample_t audio_sample) {
  context.audio_sample = audio_sample;
}

void snes_set_input_poll(snes_input_poll_t input_poll) {
  context.input_poll = input_poll;
}

void snes_set_input_state(snes_input_state_t input_state) {
  context.input_state = input_state;
}
