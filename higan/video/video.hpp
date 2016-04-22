#pragma once

namespace Emulator {

struct Interface;

struct Video {
  enum class Effect : uint {
    ColorBleed,
    InterframeBlending,
  };

  ~Video();

  auto reset() -> void;
  auto setInterface(Interface* interface) -> void;

  auto setPalette() -> void;
  auto setSaturation(double saturation) -> void;
  auto setGamma(double gamma) -> void;
  auto setLuminance(double luminance) -> void;

  auto setEffect(Effect effect, const any& value) -> void;

  auto refresh(uint32* input, uint pitch, uint width, uint height) -> void;

private:
  Emulator::Interface* interface = nullptr;
  uint32* output = nullptr;
  uint32* palette = nullptr;
  uint width = 0;
  uint height = 0;
  uint colors = 0;

  double saturation = 1.0;
  double gamma = 1.0;
  double luminance = 1.0;

  struct Effects {
    bool colorBleed = false;
    bool interframeBlending = false;
  } effects;
};

extern Video video;

}
