#ifndef EMULATOR_INTERFACE_HPP
#define EMULATOR_INTERFACE_HPP

namespace Emulator {

struct Interface {
  struct Information {
    string name;
    unsigned width;
    unsigned height;
    unsigned frequency;
    unsigned ports;
    bool resettable;
  } information;

  struct Firmware {
    string name;
    unsigned id;
  };
  vector<Firmware> firmware;

  struct MediaObject {
    string displayname;
    string name;
    string filter;
    unsigned id;
  };

  struct Media : MediaObject {
    vector<MediaObject> slot;
  };
  vector<Media> media;

  struct Memory {
    string name;
    unsigned id;
    uint8_t *data;
    unsigned size;
  };
  vector<Memory> memory;

  struct Port {
    string name;
    unsigned id;
    struct Device {
      string name;
      unsigned id;
      struct Input {
        string name;
        unsigned id;
        unsigned guid;
      };
      vector<Input> input;
      vector<unsigned> displayinput;
    };
    vector<Device> device;
  };
  vector<Port> port;

  struct Callback {
    function<uint32_t (unsigned, uint16_t, uint16_t, uint16_t)> videoColor;
    function<void (const uint32_t*, unsigned, unsigned, unsigned)> videoRefresh;
    function<void (int16_t, int16_t)> audioSample;
    function<int16_t (unsigned, unsigned, unsigned)> inputPoll;
  } callback;

  //audio/visual bindings (provided by user interface)
  virtual uint32_t videoColor(unsigned source, uint16_t red, uint16_t green, uint16_t blue) {
    if(callback.videoColor) return callback.videoColor(source, red, green, blue);
    return (red >> 8) << 16 | (green >> 8) << 8 | (blue >> 8) << 0;
  }

  virtual void videoRefresh(const uint32_t *data, unsigned pitch, unsigned width, unsigned height) {
    if(callback.videoRefresh) return callback.videoRefresh(data, pitch, width, height);
  }

  virtual void audioSample(int16_t lsample, int16_t rsample) {
    if(callback.audioSample) return callback.audioSample(lsample, rsample);
  }

  virtual int16_t inputPoll(unsigned port, unsigned device, unsigned input) {
    if(callback.inputPoll) return callback.inputPoll(port, device, input);
    return 0;
  }

  //cartridge interface
  virtual bool loaded() { return false; }
  virtual void load(unsigned id, const stream &memory, const string &markup = "") {}
  virtual void unload() {}

  //system interface
  virtual void power() {}
  virtual void reset() {}
  virtual void run() {}

  //utility functions
  virtual void updatePalette() {}
};

}

#endif
