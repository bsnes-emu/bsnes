#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned snes_library_revision();
void snes_init();
void snes_term();
void snes_unload();
void snes_run();
void snes_set_controller_port_device(bool port, unsigned device);

typedef void (*snes_video_refresh_t)(uint16_t *data, unsigned pitch, unsigned *line, unsigned width, unsigned height);
typedef void (*snes_audio_sample_t)(uint16_t left, uint16_t right);
typedef void (*snes_input_poll_t)();
typedef int16_t (*snes_input_state_t)(bool port, unsigned device, unsigned index, unsigned id);

void snes_set_video_refresh(snes_video_refresh_t);
void snes_set_audio_sample(snes_audio_sample_t);
void snes_set_input_poll(snes_input_poll_t);
void snes_set_input_state(snes_input_state_t);

void snes_load_cartridge_normal(
  const char *rom_xml, uint8_t *rom_data, unsigned rom_size
);

void snes_load_cartridge_bsx_slotted(
  const char *rom_xml, uint8_t *rom_data, unsigned rom_size,
  const char *bsx_xml, uint8_t *bsx_data, unsigned bsx_size
);

void snes_load_cartridge_bsx(
  const char *rom_xml, uint8_t *rom_data, unsigned rom_size,
  const char *bsx_xml, uint8_t *bsx_data, unsigned bsx_size
);

void snes_load_cartridge_sufami_turbo(
  const char *rom_xml, uint8_t *rom_data, unsigned rom_size,
  const char *sta_xml, uint8_t *sta_data, unsigned sta_size,
  const char *stb_xml, uint8_t *stb_data, unsigned stb_size
);

void snes_load_cartridge_super_game_boy(
  const char *rom_xml, uint8_t *rom_data, unsigned rom_size,
  const char *dmg_xml, uint8_t *dmg_data, unsigned dmg_size
);

uint8_t* snes_get_cartridge_ram_data();
unsigned snes_get_cartridge_ram_size();
uint8_t* snes_get_cartridge_rtc_data();
unsigned snes_get_cartridge_rtc_size();
uint8_t* snes_get_bsx_ram_data();
unsigned snes_get_bsx_ram_size();
uint8_t* snes_get_bsx_pram_data();
unsigned snes_get_bsx_pram_size();
uint8_t* snes_get_sufami_turbo_a_ram_data();
unsigned snes_get_sufami_turbo_a_ram_size();
uint8_t* snes_get_sufami_turbo_b_ram_data();
unsigned snes_get_sufami_turbo_b_ram_size();
uint8_t* snes_get_game_boy_ram_data();
unsigned snes_get_game_boy_ram_size();
uint8_t* snes_get_game_boy_rtc_data();
unsigned snes_get_game_boy_rtc_size();

#ifdef __cplusplus
}
#endif
