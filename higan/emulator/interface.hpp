#pragma once

namespace Emulator {

struct Interface {
  //information
  struct Information {
    string manufacturer;
    string name;
    string extension;
    bool resettable = false;
  };
  virtual auto information() -> Information = 0;
  virtual auto manifest() -> string = 0;
  virtual auto title() -> string = 0;

  struct Display {
    struct Type { enum : uint {
      CRT,
      LCD,
    };};
    uint type = 0;
    uint colors = 0;
    uint width = 0;
    uint height = 0;
    uint internalWidth = 0;
    uint internalHeight = 0;
    double aspectCorrection = 0;
    double refreshRate = 0;
  };
  virtual auto display() -> Display = 0;
  virtual auto color(uint32 color) -> uint64 = 0;

  //game interface
  virtual auto loaded() -> bool = 0;
  virtual auto sha256() -> string { return ""; }
  virtual auto load() -> bool = 0;
  virtual auto save() -> void = 0;
  virtual auto unload() -> void = 0;

  //system interface
  struct Port {
    uint id;
    string name;
  };
  virtual auto ports() -> vector<Port> = 0;

  struct Device {
    uint id;
    string name;
  };
  virtual auto devices(uint port) -> vector<Device> = 0;

  struct Input {
    struct Type { enum : uint {
      Hat,
      Button,
      Trigger,
      Control,
      Axis,
      Rumble,
    };};
    uint type;
    string name;
  };
  virtual auto inputs(uint device) -> vector<Input> = 0;

  virtual auto connected(uint port) -> uint { return 0; }
  virtual auto connect(uint port, uint device) -> void {}
  virtual auto power() -> void = 0;
  virtual auto reset() -> void {}
  virtual auto run() -> void = 0;

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
