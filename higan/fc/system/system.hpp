#include "peripherals.hpp"

struct System {
  auto loaded() const -> bool { return _loaded; }

  auto run() -> void;
  auto runToSave() -> void;

  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto init() -> void;
  auto term() -> void;

  //video.cpp
  auto configureVideoPalette() -> void;
  auto configureVideoEffects() -> void;

  //serialization.cpp
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

  auto serialize(serializer&) -> void;
  auto serializeAll(serializer&) -> void;
  auto serializeInit() -> void;

  struct Information {
    string manifest;
  } information;

private:
  bool _loaded = false;
  uint _serializeSize = 0;
};

extern System system;
