#pragma once

namespace Emulator {

struct Interface {
  struct Information {
    string manufacturer;
    string name;
    bool overscan;
  } information;

  struct Region {
    string name;
  };
  vector<Region> regions;

  struct Medium {
    uint id;
    string name;
    string type;  //extension
  };
  vector<Medium> media;

  struct Device {
    uint id;
    string name;
    struct Input {
      uint type;  //0 = digital, 1 = analog (relative), 2 = rumble
      string name;
    };
    vector<Input> inputs;
  };

  struct Port {
    uint id;
    string name;
    vector<Device> devices;
  };
  vector<Port> ports;

  //information
  virtual auto manifest() -> string = 0;
  virtual auto title() -> string = 0;

  //video information
  struct VideoSize { uint width, height; };
  virtual auto videoResolution() -> VideoSize = 0;
  virtual auto videoSize(uint width, uint height, bool arc) -> VideoSize = 0;
  virtual auto videoColors() -> uint32 = 0;
  virtual auto videoColor(uint32 color) -> uint64 = 0;

  //media interface
  virtual auto loaded() -> bool { return false; }
  virtual auto sha256() -> string { return ""; }
  virtual auto load(uint id) -> bool { return false; }
  virtual auto save() -> void {}
  virtual auto unload() -> void {}

  //system interface
  virtual auto connect(uint port, uint device) -> void {}
  virtual auto power() -> void {}
  virtual auto run() -> void {}

  //time functions
  virtual auto rtc() -> bool { return false; }
  virtual auto rtcSynchronize() -> void {}

  //state functions
  virtual auto serialize() -> serializer = 0;
  virtual auto unserialize(serializer&) -> bool = 0;

  //cheat functions
  virtual auto cheatSet(const string_vector& = {}) -> void {}

  //settings
  virtual auto cap(const string& name) -> bool { return false; }
  virtual auto get(const string& name) -> any { return {}; }
  virtual auto set(const string& name, const any& value) -> bool { return false; }

  //shared functions
  auto videoColor(uint16 r, uint16 g, uint16 b) -> uint32;
};

}
