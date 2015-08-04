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
    bool resettable;
    struct Capability {
      bool states;
      bool cheats;
    } capability;
  } information;

  struct Media {
    unsigned id;
    string name;
    string type;
    bool bootable;  //false for cartridge slots (eg Sufami Turbo cartridges)
  };
  vector<Media> media;

  struct Device {
    unsigned id;
    unsigned portmask;
    string name;
    struct Input {
      unsigned id;
      unsigned type;  //0 = digital, 1 = analog (relative), 2 = rumble
      string name;
      uintptr_t guid;  //user data field
    };
    vector<Input> input;
    vector<unsigned> order;
  };

  struct Port {
    unsigned id;
    string name;
    vector<Device> device;
  };
  vector<Port> port;

  struct Bind {
    virtual auto loadRequest(unsigned, string, string, bool) -> void {}
    virtual auto loadRequest(unsigned, string, bool) -> void {}
    virtual auto saveRequest(unsigned, string) -> void {}
    virtual auto videoColor(unsigned, uint16_t, uint16_t, uint16_t, uint16_t) -> uint32_t { return 0u; }
    virtual auto videoRefresh(const uint32_t*, const uint32_t*, unsigned, unsigned, unsigned) -> void {}
    virtual auto audioSample(int16_t, int16_t) -> void {}
    virtual auto inputPoll(unsigned, unsigned, unsigned) -> int16_t { return 0; }
    virtual auto inputRumble(unsigned, unsigned, unsigned, bool) -> void {}
    virtual auto dipSettings(const Markup::Node&) -> unsigned { return 0; }
    virtual auto path(unsigned) -> string { return ""; }
    virtual auto notify(string text) -> void { print(text, "\n"); }
  };
  Bind* bind = nullptr;

  //callback bindings (provided by user interface)
  auto loadRequest(unsigned id, string name, string type, bool required) -> void { return bind->loadRequest(id, name, type, required); }
  auto loadRequest(unsigned id, string path, bool required) -> void { return bind->loadRequest(id, path, required); }
  auto saveRequest(unsigned id, string path) -> void { return bind->saveRequest(id, path); }
  auto videoColor(unsigned source, uint16_t alpha, uint16_t red, uint16_t green, uint16_t blue) -> uint32_t { return bind->videoColor(source, alpha, red, green, blue); }
  auto videoRefresh(const uint32_t* palette, const uint32_t* data, unsigned pitch, unsigned width, unsigned height) -> void { return bind->videoRefresh(palette, data, pitch, width, height); }
  auto audioSample(int16_t lsample, int16_t rsample) -> void { return bind->audioSample(lsample, rsample); }
  auto inputPoll(unsigned port, unsigned device, unsigned input) -> int16_t { return bind->inputPoll(port, device, input); }
  auto inputRumble(unsigned port, unsigned device, unsigned input, bool enable) -> void { return bind->inputRumble(port, device, input, enable); }
  auto dipSettings(const Markup::Node& node) -> unsigned { return bind->dipSettings(node); }
  auto path(unsigned group) -> string { return bind->path(group); }
  template<typename... P> auto notify(P&&... p) -> void { return bind->notify({forward<P>(p)...}); }

  //information
  virtual auto title() -> string = 0;
  virtual auto videoFrequency() -> double = 0;
  virtual auto audioFrequency() -> double = 0;

  //media interface
  virtual auto loaded() -> bool { return false; }
  virtual auto sha256() -> string { return ""; }
  virtual auto group(unsigned id) -> unsigned = 0;
  virtual auto load(unsigned id) -> void {}
  virtual auto save() -> void {}
  virtual auto load(unsigned id, const stream& memory) -> void {}
  virtual auto save(unsigned id, const stream& memory) -> void {}
  virtual auto unload() -> void {}

  //system interface
  virtual auto connect(unsigned port, unsigned device) -> void {}
  virtual auto power() -> void {}
  virtual auto reset() -> void {}
  virtual auto run() -> void {}

  //time functions
  virtual auto rtc() -> bool { return false; }
  virtual auto rtcsync() -> void {}

  //state functions
  virtual auto serialize() -> serializer = 0;
  virtual auto unserialize(serializer&) -> bool = 0;

  //cheat functions
  virtual auto cheatSet(const lstring& = lstring{}) -> void {}

  //utility functions
  enum class PaletteMode : unsigned { Literal, Channel, Standard, Emulation };
  virtual auto paletteUpdate(PaletteMode mode) -> void {}
};

}

#endif
