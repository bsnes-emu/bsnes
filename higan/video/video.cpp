#include <emulator/emulator.hpp>

namespace Emulator {

Video video;

Video::~Video() {
  reset();
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

auto Video::setPalette() -> void {
  if(!interface) return;

  delete palette;
  colors = interface->videoColors();
  palette = new uint32[colors];
  for(auto index : range(colors)) {
    uint64 color = interface->videoColor(index);
    uint16 b = color.bits( 0,15);
    uint16 g = color.bits(16,31);
    uint16 r = color.bits(32,47);
    uint16 a = 0xffff;

    if(saturation != 1.0) {
      uint16 grayscale = uclamp<16>((r + g + b) / 3);
      double inverse = max(0.0, 1.0 - saturation);
      r = uclamp<16>(r * saturation + grayscale * inverse);
      g = uclamp<16>(g * saturation + grayscale * inverse);
      b = uclamp<16>(b * saturation + grayscale * inverse);
    }

    if(gamma != 1.0) {
      double reciprocal = 1.0 / 32767.0;
      r = r > 32767 ? r : uint16(32767 * pow(r * reciprocal, gamma));
      g = g > 32767 ? g : uint16(32767 * pow(g * reciprocal, gamma));
      b = b > 32767 ? b : uint16(32767 * pow(b * reciprocal, gamma));
    }

    if(luminance != 1.0) {
      r = uclamp<16>(r * luminance);
      g = uclamp<16>(g * luminance);
      b = uclamp<16>(b * luminance);
    }

    //convert color from 16-bits/channel to 8-bits/channel; force alpha to 1.0
    palette[index] = a.byte(1) << 24 | r.byte(1) << 16 | g.byte(1) << 8 | b.byte(1) << 0;
  }
}

auto Video::setSaturation(double saturation) -> void {
  this->saturation = saturation;
}

auto Video::setGamma(double gamma) -> void {
  this->gamma = gamma;
}

auto Video::setLuminance(double luminance) -> void {
  this->luminance = luminance;
}

auto Video::setEffect(Effect effect, const any& value) -> void {
  if(effect == Effect::ColorBleed && value.is<bool>()) {
    effects.colorBleed = value.get<bool>();
  }

  if(effect == Effect::InterframeBlending && value.is<bool>()) {
    effects.interframeBlending = value.get<bool>();
  }
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

}
