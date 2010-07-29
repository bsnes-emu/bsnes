class Interface;

class System : property<System> {
public:
  enum class Region : unsigned { NTSC = 0, PAL = 1, Autodetect = 2 };
  enum class ExpansionPortDevice : unsigned { None = 0, BSX = 1 };

  void run();
  void runtosave();

  void init(Interface*);
  void term();
  void power();
  void reset();
  void unload();

  void frame();
  void scanline();

  //return *active* system information (settings are cached upon power-on)
  readonly<Region> region;
  readonly<ExpansionPortDevice> expansion;
  readonly<unsigned> serialize_size;

  serializer serialize();
  bool unserialize(serializer&);

  System();
  virtual ~System() {}

private:
  Interface *interface;
  void runthreadtosave();

  void serialize(serializer&);
  void serialize_all(serializer&);
  void serialize_init();

  friend class Cartridge;
  friend class Video;
  friend class Audio;
  friend class Input;
  friend class StateManager;
  friend void threadentry_cop();
};

#include <snes/video/video.hpp>
#include <snes/audio/audio.hpp>
#include <snes/input/input.hpp>

#include <snes/config/config.hpp>
#include <snes/debugger/debugger.hpp>
#include <snes/interface/interface.hpp>
#include <snes/scheduler/scheduler.hpp>

extern System system;
