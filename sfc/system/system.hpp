struct Interface;

struct System : property<System> {
  enum class Region : unsigned { NTSC = 0, PAL = 1, Autodetect = 2 };
  enum class ExpansionPortDevice : unsigned { None = 0, Satellaview = 1 };

  void run();
  void runtosave();

  void init();
  void term();
  void load();
  void unload();
  void power();
  void reset();

  void frame();
  void scanline();

  //return *active* system information (settings are cached upon power-on)
  readonly<Region> region;
  readonly<ExpansionPortDevice> expansion;
  readonly<unsigned> cpu_frequency;
  readonly<unsigned> apu_frequency;
  readonly<unsigned> serialize_size;

  serializer serialize();
  bool unserialize(serializer&);

  System();

private:
  void runthreadtosave();

  void serialize(serializer&);
  void serialize_all(serializer&);
  void serialize_init();

  friend class Cartridge;
  friend class Video;
  friend class Audio;
  friend class Input;
};

extern System system;

#include "video.hpp"
#include "audio.hpp"
#include "input.hpp"

#include <sfc/scheduler/scheduler.hpp>

struct Configuration {
  Input::Device controller_port1 = Input::Device::Joypad;
  Input::Device controller_port2 = Input::Device::Joypad;
  System::ExpansionPortDevice expansion_port = System::ExpansionPortDevice::Satellaview;
  System::Region region = System::Region::Autodetect;
  bool random = true;
};

extern Configuration configuration;

struct Random {
  void seed(unsigned seed) {
    iter = seed;
  }

  unsigned operator()(unsigned result) {
    if(configuration.random == false) return result;
    return iter = (iter >> 1) ^ (((iter & 1) - 1) & 0xedb88320);
  }

  void serialize(serializer& s) {
    s.integer(iter);
  }

private:
  unsigned iter = 0;
};

extern Random random;
