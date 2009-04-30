#include <libgambatte/include/gambatte.h>

namespace SNES {

class GambatteVideo : public Gambatte::VideoBlitter {
public:
  unsigned bufferWidth, bufferHeight;
  uint32_t *buffer;

  void setBufferDimensions(unsigned width, unsigned height) {
    if(buffer) delete[] buffer;
    buffer = new uint32_t[width * height];
    bufferWidth = width;
    bufferHeight = height;
  }

  const Gambatte::PixelBuffer inBuffer() {
    Gambatte::PixelBuffer pb;
    pb.pixels = (void*)buffer;
    pb.format = Gambatte::PixelBuffer::RGB32;
    pb.pitch  = bufferWidth;
    return pb;
  }

  void blit() {
  }

  void update(unsigned row) {
    uint32_t *source = buffer + row * 160 * 8;
    uint8_t  *buffer = new(zeromemory) uint8_t[5760];

    for(unsigned y = row * 8; y < row * 8 + 8; y++) {
      for(unsigned x = 0; x < 160; x++) {
        unsigned pixel = *source++ / 0x555555;
        pixel ^= 3;

        unsigned tile = ((y / 8) * 20) + (x / 8);
        unsigned addr = (tile * 16) + ((y & 7) * 2);

        buffer[addr + 0] |= ((pixel & 1) >> 0) << (7 - (x & 7));
        buffer[addr + 1] |= ((pixel & 2) >> 1) << (7 - (x & 7));
      }
    }

    memcpy(sgb.gameboy->vram, buffer, 5760);
    delete[] buffer;
  }

  ~GambatteVideo() {
    if(buffer) delete[] buffer;
  }
};

class GambatteInput : public Gambatte::InputStateGetter {
public:
  Gambatte::InputState is;
  const Gambatte::InputState& operator()() {
    unsigned joypad = sgb.gameboy->joypadid();
    unsigned data   = sgb.mmio.joypad[joypad];

    //TODO: fix SGB detection
    joypad = 1;
    data = sgb.mmio.joypad[0];

    is.joypadId     = 0x0f - joypad;
    is.startButton  = !(data & 0x80);
    is.selectButton = !(data & 0x40);
    is.bButton      = !(data & 0x20);
    is.aButton      = !(data & 0x10);
    is.dpadDown     = !(data & 0x08);
    is.dpadUp       = !(data & 0x04);
    is.dpadLeft     = !(data & 0x02);
    is.dpadRight    = !(data & 0x01);
    return is;
  }
};

class GambatteMemory : public Gambatte::MemoryInterface {
public:
  Gambatte::MemoryBuffer loadRomData() {
    Gambatte::MemoryBuffer mb;
    mb.data = (void*)memory::dmgrom.handle();
    mb.size = memory::dmgrom.size();
    return mb;
  }

  Gambatte::MemoryBuffer loadRamData() {
    Gambatte::MemoryBuffer mb;
    mb.data = (void*)memory::dmgram.handle();
    mb.size = memory::dmgram.size();
    return mb;
  }

  Gambatte::MemoryBuffer loadRtcData() {
    Gambatte::MemoryBuffer mb;
    mb.data = (void*)memory::dmgrtc.handle();
    mb.size = memory::dmgram.size();
    return mb;
  }

  Gambatte::MemoryBuffer saveRamData(unsigned size) {
    memory::dmgram.reset();
    memory::dmgram.map(new uint8_t[size], size);

    Gambatte::MemoryBuffer mb;
    mb.data = (void*)memory::dmgram.handle();
    mb.size = memory::dmgram.size();
    return mb;
  }

  Gambatte::MemoryBuffer saveRtcData() {
    memory::dmgrtc.reset();
    memory::dmgrtc.map(new uint8_t[4], 4);

    Gambatte::MemoryBuffer mb;
    mb.data = (void*)memory::dmgrtc.handle();
    mb.size = memory::dmgrtc.size();
    return mb;
  }

  void joypWrite(bool p15, bool p14) {
    sgb.gameboy->write(p15, p14);
  }
};

class GameboyGambatte : public Gameboy {
public:
  Gambatte::GB gambatte;
  GambatteInput gambatte_input;
  GambatteVideo gambatte_video;
  GambatteMemory gambatte_memory;
  uint32_t audio_buffer[65536];

  unsigned run() {
    unsigned samples = gambatte.runFor(audio_buffer, 16);
    for(unsigned i = 0; i < samples; i++) {
      system.audio.cop_sample(audio_buffer[i] >> 0, audio_buffer[i] >> 16);
    }
    return (samples << 3) + (samples << 1);  //1 sample = 10 S-CPU clock cycles
  }

  unsigned lyCounter() {
    return gambatte.lyCounter();
  }

  void updateVideo(unsigned row) {
    gambatte.updateVideo();
    gambatte_video.update(row);
  }

  void unload() {
    gambatte.save();
  }

  void power() {
    Gameboy::power();
    gambatte.load(true);
    system.audio.set_cop_frequency(60 * 35112);
  }

  void reset() {
    Gameboy::reset();
    gambatte.reset();
    system.audio.set_cop_frequency(60 * 35112);
  }

  GameboyGambatte() {
    gambatte.setVideoBlitter(&gambatte_video);
    gambatte.setInputStateGetter(&gambatte_input);
    gambatte.setMemoryInterface(&gambatte_memory);
  }
};

};
