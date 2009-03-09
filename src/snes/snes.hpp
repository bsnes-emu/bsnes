#include "interface/interface.hpp"
#include "scheduler/scheduler.hpp"
#include "tracer/tracer.hpp"

class VideoFilter;

class SNES {
public:
  enum Region { NTSC = 0, PAL = 1 };
  enum RegionAutodetect { Autodetect = 2 };
  enum ExpansionPortDevice { ExpansionNone = 0, ExpansionBSX = 1 };

  struct Config {
    unsigned controller_port1;
    unsigned controller_port2;
    unsigned expansion_port;
    unsigned region;

    struct File {
      bool autodetect_type;
      bool bypass_patch_crc32;
    } file;

    struct Path {
      string base;     //binary path
      string user;     //user profile path (bsnes.cfg, ...)
      string current;  //current working directory (path to currently loaded cartridge)
      string rom, save, patch, cheat, data;
      string bsx, st;
    } path;

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

    struct PPU1 {
      unsigned version;
    } ppu1;

    struct PPU2 {
      unsigned version;
    } ppu2;
  } config;

  //system functions
  virtual void run();
  virtual void runtoframe();

  virtual void init();
  virtual void term();
  virtual void power();
  virtual void reset();

  virtual void frame();
  virtual void scanline();

  //return *active* region / expansion port device information
  //settings cached upon power-on
  Region region() const;
  ExpansionPortDevice expansion() const;

  #include "video/video.hpp"
  #include "audio/audio.hpp"
  #include "input/input.hpp"

  SNES();
  virtual ~SNES() {}

private:
  unsigned snes_region;
  unsigned snes_expansion;
};

extern SNES snes;
