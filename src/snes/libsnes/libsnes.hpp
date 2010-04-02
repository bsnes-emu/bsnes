#include <stdint.h>

extern "C" unsigned snes_library_revision();
extern "C" void snes_init();
extern "C" void snes_term();
extern "C" bool snes_load();
extern "C" void snes_unload();
extern "C" void snes_run();

extern "C" void snes_load_cartridge(
  const char *xml,
  uint8_t *rom_data, unsigned rom_size,
  uint8_t *ram_data, unsigned ram_size,
  uint8_t *rtc_data, unsigned rtc_size
);

extern "C" void snes_load_bsx_cartridge(
  const char *xml,
  uint8_t *flash_data, unsigned flash_size,
  uint8_t *ram_data, unsigned ram_size,
  uint8_t *pram_data, unsigned pram_size
);

extern "C" void snes_load_sufami_turbo_cartridge_a(
  const char *xml,
  uint8_t *rom_data, unsigned rom_size,
  uint8_t *ram_data, unsigned ram_size
);

extern "C" void snes_load_sufami_turbo_cartridge_b(
  const char *xml,
  uint8_t *rom_data, unsigned rom_size,
  uint8_t *ram_data, unsigned ram_size
);

extern "C" void snes_load_game_boy_cartridge(
  const char *xml,
  uint8_t *rom_data, unsigned rom_size,
  uint8_t *ram_data, unsigned ram_size,
  uint8_t *rtc_data, unsigned rtc_size
);

typedef void (*snes_video_refresh_t)(uint16_t *data, unsigned pitch, unsigned *line, unsigned width, unsigned height);
typedef void (*snes_audio_sample_t)(uint16_t left, uint16_t right);
typedef void (*snes_input_poll_t)();
typedef int16_t (*snes_input_state_t)(bool port, unsigned device, unsigned index, unsigned id);

extern "C" void snes_set_video_refresh(snes_video_refresh_t);
extern "C" void snes_set_audio_sample(snes_audio_sample_t);
extern "C" void snes_set_input_poll(snes_input_poll_t);
extern "C" void snes_set_input_state(snes_input_state_t);
