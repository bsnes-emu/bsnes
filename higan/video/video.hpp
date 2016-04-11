#pragma once

namespace Emulator {

struct Interface;

struct Video {
  enum class Effect : uint {
    ColorBleed,
    InterframeBlending,
  };

  ~Video();
  auto refresh(uint32* input, uint pitch, uint width, uint height) -> void;
  auto reset() -> void;
  auto setInterface(Interface* interface) -> void;
  auto setPalette(uint32 size, function<uint64 (uint32)> callback) -> void;
  auto setEffect(Effect effect, const any& value) -> void;

private:
  Emulator::Interface* interface = nullptr;
  uint32* output = nullptr;
  uint32* palette = nullptr;
  uint width = 0;
  uint height = 0;
  uint colors = 0;

  struct Effects {
    bool colorBleed = false;
    bool interframeBlending = false;
  } effects;
};

extern Video video;

}
