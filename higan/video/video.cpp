#include <emulator/emulator.hpp>

namespace Emulator {

Video video;

Video::~Video() {
  reset();
}

auto Video::refresh(uint32* input, uint pitch, uint width, uint height) -> void {
  if(this->width != width || this->height != height) {
    delete output;
    output = new uint32[width * height]();
    this->width = width;
    this->height = height;
  }

  pitch >>= 2;  //bytes to words
  for(uint y : range(height)) {
    auto source = input + y * pitch;
    auto target = output + y * width;

    if(!effects.interframeBlending) {
      for(uint x : range(width)) {
        auto color = palette[*source++];
        *target++ = color;
      }
    } else {
      for(uint x : range(width)) {
        auto a = *target;
        auto b = palette[*source++];
        *target++ = (a + b - ((a ^ b) & 0x01010101)) >> 1;
      }
    }
  }

  if(effects.colorBleed) {
    for(uint y : range(height)) {
      auto target = output + y * width;
      for(uint x : range(width)) {
        auto a = target[x];
        auto b = target[x + (x != width - 1)];
        target[x] = (a + b - ((a ^ b) & 0x01010101)) >> 1;
      }
    }
  }

  interface->videoRefresh(output, width * sizeof(uint32), width, height);
}

auto Video::reset() -> void {
  interface = nullptr;
  delete output;
  output = nullptr;
  delete palette;
  palette = nullptr;
  width = 0;
  height = 0;
  effects.colorBleed = false;
  effects.interframeBlending = false;
}

auto Video::setInterface(Interface* interface) -> void {
  this->interface = interface;
}

auto Video::setPalette(uint32 size, function<uint64 (uint32)> callback) -> void {
  delete palette;
  palette = new uint32[size];
  for(auto index : range(size)) {
    //convert color from 16-bits/channel to 8-bits/channel; force alpha to 1.0
    uint64 color = callback(index);
    palette[index] = 255u << 24 | color.bits(40,47) << 16 | color.bits(24,31) << 8 | color.bits(8,15) << 0;
  }
  colors = size;
}

auto Video::setEffect(Effect effect, const any& value) -> void {
  if(effect == Effect::ColorBleed && value.is<bool>()) {
    effects.colorBleed = value.get<bool>();
  }

  if(effect == Effect::InterframeBlending && value.is<bool>()) {
    effects.interframeBlending = value.get<bool>();
  }
}

}
