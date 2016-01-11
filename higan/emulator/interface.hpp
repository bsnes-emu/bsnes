#pragma once

namespace Emulator {

struct Interface {
  struct Information {
    string name;
    uint width;
    uint height;
    bool overscan;
    double aspectRatio;
    bool resettable;
    struct Capability {
      bool states;
      bool cheats;
    } capability;
  } information;

  struct Media {
    uint id;
    string name;
    string type;
    bool bootable;  //false for cartridge slots (eg Sufami Turbo cartridges)
  };
  vector<Media> media;

  struct Device {
    uint id;
    uint portmask;
    string name;
    struct Input {
      uint id;
      uint type;  //0 = digital, 1 = analog (relative), 2 = rumble
      string name;
      uintptr guid;  //user data field
    };
    vector<Input> input;
    vector<uint> order;
  };

  struct Port {
    uint id;
    string name;
    vector<Device> device;
  };
  vector<Port> port;

  struct Bind {
    virtual auto loadRequest(uint, string, string, bool) -> void {}
    virtual auto loadRequest(uint, string, bool) -> void {}
    virtual auto saveRequest(uint, string) -> void {}
    virtual auto videoColor(uint, uint16, uint16, uint16, uint16) -> uint32 { return 0u; }
    virtual auto videoRefresh(const uint32*, const uint32*, uint, uint, uint) -> void {}
    virtual auto audioSample(int16, int16) -> void {}
    virtual auto inputPoll(uint, uint, uint) -> int16 { return 0; }
    virtual auto inputRumble(uint, uint, uint, bool) -> void {}
    virtual auto dipSettings(const Markup::Node&) -> uint { return 0; }
    virtual auto path(uint) -> string { return ""; }
    virtual auto notify(string text) -> void { print(text, "\n"); }
  };
  Bind* bind = nullptr;

  //callback bindings (provided by user interface)
  auto loadRequest(uint id, string name, string type, bool required) -> void { return bind->loadRequest(id, name, type, required); }
  auto loadRequest(uint id, string path, bool required) -> void { return bind->loadRequest(id, path, required); }
  auto saveRequest(uint id, string path) -> void { return bind->saveRequest(id, path); }
  auto videoColor(uint source, uint16 alpha, uint16 red, uint16 green, uint16 blue) -> uint32 { return bind->videoColor(source, alpha, red, green, blue); }
  auto videoRefresh(const uint32* palette, const uint32* data, uint pitch, uint width, uint height) -> void { return bind->videoRefresh(palette, data, pitch, width, height); }
  auto audioSample(int16 lsample, int16 rsample) -> void { return bind->audioSample(lsample, rsample); }
  auto inputPoll(uint port, uint device, uint input) -> int16 { return bind->inputPoll(port, device, input); }
  auto inputRumble(uint port, uint device, uint input, bool enable) -> void { return bind->inputRumble(port, device, input, enable); }
  auto dipSettings(const Markup::Node& node) -> uint { return bind->dipSettings(node); }
  auto path(uint group) -> string { return bind->path(group); }
  template<typename... P> auto notify(P&&... p) -> void { return bind->notify({forward<P>(p)...}); }

  //information
  virtual auto manifest() -> string = 0;
  virtual auto title() -> string = 0;
  virtual auto videoFrequency() -> double = 0;
  virtual auto audioFrequency() -> double = 0;

  //media interface
  virtual auto loaded() -> bool { return false; }
  virtual auto sha256() -> string { return ""; }
  virtual auto group(uint id) -> uint = 0;
  virtual auto load(uint id) -> void {}
  virtual auto save() -> void {}
  virtual auto load(uint id, const stream& memory) -> void {}
  virtual auto save(uint id, const stream& memory) -> void {}
  virtual auto unload() -> void {}

  //system interface
  virtual auto connect(uint port, uint device) -> void {}
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
  enum class PaletteMode : uint { Literal, Channel, Standard, Emulation };
  virtual auto paletteUpdate(PaletteMode mode) -> void {}
};

}
