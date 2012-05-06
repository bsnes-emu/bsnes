#ifndef EMULATOR_INTERFACE_HPP
#define EMULATOR_INTERFACE_HPP

namespace Emulator {

struct Interface {
  struct Information {
    string name;
    unsigned width;
    unsigned height;
    bool overscan;
    double aspectRatio;
    unsigned frequency;
    bool resettable;
  } information;

  struct Media {
    unsigned id;
    string name;
    string type;
    string path;
    string extension;
  };

  vector<Media> firmware;
  vector<Media> media;

  struct Memory {
    unsigned id;
    string name;
  };
  vector<Memory> memory;

  struct Device {
    unsigned id;
    unsigned portmask;
    string name;
    struct Input {
      unsigned id;
      unsigned type;  //0 = digital, 1 = analog
      string name;
      unsigned guid;
    };
    vector<Input> input;
    vector<unsigned> order;
  };
  vector<Device> device;

  struct Port {
    unsigned id;
    string name;
    vector<Device> device;
  };
  vector<Port> port;

  struct Callback {
    function<uint32_t (unsigned, uint16_t, uint16_t, uint16_t)> videoColor;
    function<void (const uint32_t*, unsigned, unsigned, unsigned)> videoRefresh;
    function<void (int16_t, int16_t)> audioSample;
    function<int16_t (unsigned, unsigned, unsigned)> inputPoll;
    function<void (Media)> mediaRequest;
    function<unsigned (const XML::Node&)> dipSettings;
    function<string (unsigned)> path;
  } callback;

  //callback bindings (provided by user interface)
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

  virtual void mediaRequest(Media media) {
    if(callback.mediaRequest) return callback.mediaRequest(media);
  }

  virtual unsigned dipSettings(const XML::Node &node) {
    if(callback.dipSettings) return callback.dipSettings(node);
    return 0u;
  }

  virtual string path(unsigned group) {
    if(callback.path) return callback.path(group);
    return "";
  }

  //information
  virtual double videoFrequency() = 0;
  virtual double audioFrequency() = 0;

  //media interface
  virtual bool loaded() { return false; }
  virtual string sha256() { return ""; }
  virtual unsigned group(unsigned id) { return 0u; }
  virtual void load(unsigned id, const stream &memory, const string &markup = "") {}
  virtual void save(unsigned id, const stream &memory) {}
  virtual void unload() {}

  //system interface
  virtual void connect(unsigned port, unsigned device) {}
  virtual void power() {}
  virtual void reset() {}
  virtual void run() {}

  //state functions
  virtual serializer serialize() = 0;
  virtual bool unserialize(serializer&) = 0;

  //cheat functions
  virtual void cheatSet(const lstring& = lstring{}) {}

  //utility functions
  virtual void updatePalette() {}
};

}

#endif
