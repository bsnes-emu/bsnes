struct Interface;

#include "video.hpp"
#include "audio.hpp"
#include "device.hpp"

struct System : property<System> {
  enum class Region : uint { NTSC = 0, PAL = 1, Autodetect = 2 };

  System();

  auto run() -> void;
  auto runToSave() -> void;

  auto init() -> void;
  auto term() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  //return *active* system information (settings are cached upon power-on)
  readonly<Region> region;
  readonly<Device::ID> expansionPort;

  readonly<uint> cpuFrequency;
  readonly<uint> apuFrequency;
  readonly<uint> serializeSize;

  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

  struct Information {
    string manifest;
  } information;

private:
  auto runThreadToSave() -> void;

  auto serialize(serializer&) -> void;
  auto serializeAll(serializer&) -> void;
  auto serializeInit() -> void;

  friend class Cartridge;
  friend class Video;
  friend class Audio;
  friend class Device;
};

extern System system;

#include <sfc/scheduler/scheduler.hpp>

struct Configuration {
  Device::ID controllerPort1 = Device::ID::None;
  Device::ID controllerPort2 = Device::ID::None;
  Device::ID expansionPort = Device::ID::None;
  System::Region region = System::Region::Autodetect;
  bool random = true;
};

extern Configuration configuration;

struct Random {
  auto seed(uint seed) -> void {
    iter = seed;
  }

  auto operator()(uint result) -> uint {
    if(configuration.random == false) return result;
    return iter = (iter >> 1) ^ (((iter & 1) - 1) & 0xedb88320);
  }

  auto serialize(serializer& s) -> void {
    s.integer(iter);
  }

private:
  uint iter = 0;
};

extern Random random;
