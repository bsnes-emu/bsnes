struct Configuration {
  unsigned controller_port1;
  unsigned controller_port2;
  unsigned expansion_port;
  unsigned region;

  struct CPU {
    unsigned version;
    unsigned ntsc_clock_rate;
    unsigned pal_clock_rate;
    unsigned alu_mul_delay;
    unsigned alu_div_delay;
    unsigned wram_init_value;
  } cpu;

  struct SMP {
    unsigned ntsc_clock_rate;
    unsigned pal_clock_rate;
  } smp;

  struct PPU {
    bool bg1_enabled[2];
    bool bg2_enabled[2];
    bool bg3_enabled[2];
    bool bg4_enabled[2];
    bool oam_enabled[4];
  } ppu;

  struct PPU1 {
    unsigned version;
  } ppu1;

  struct PPU2 {
    unsigned version;
  } ppu2;

  struct SuperFX {
    unsigned speed;
  } superfx;

  Configuration();
};

extern Configuration config;
