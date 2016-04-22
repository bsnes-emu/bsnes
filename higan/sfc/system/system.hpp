struct Interface;

#include "peripherals.hpp"

struct System {
  enum class Region : bool { NTSC = 0, PAL = 1 };

  auto loaded() const -> bool;
  auto region() const -> Region;
  auto cpuFrequency() const -> uint;
  auto apuFrequency() const -> uint;

  auto run() -> void;
  auto runToSave() -> void;

  auto init() -> void;
  auto term() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  //video.cpp
  auto configureVideoPalette() -> void;
  auto configureVideoEffects() -> void;

  //serialization.cpp
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

  struct Information {
    string manifest;
  } information;

private:
  auto serialize(serializer&) -> void;
  auto serializeAll(serializer&) -> void;
  auto serializeInit() -> void;

  bool _loaded = false;
  Region _region = Region::NTSC;
  uint _cpuFrequency = 0;
  uint _apuFrequency = 0;
  uint _serializeSize = 0;

  friend class Cartridge;
  friend class Device;
};

extern System system;

struct Random {
  auto seed(uint seed) -> void;
  auto operator()(uint result) -> uint;
  auto serialize(serializer& s) -> void;

private:
  uint iter = 0;
};

extern Random random;
