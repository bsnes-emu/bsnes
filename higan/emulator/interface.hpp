#pragma once

namespace Emulator {

struct Platform {
  virtual auto path(uint id) -> string { return ""; }
  virtual auto open(uint id, string name, vfs::file::mode mode, bool required = false) -> vfs::shared::file { return {}; }
  virtual auto load(uint id, string name, string type) -> maybe<uint> { return nothing; }
  virtual auto videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void {}
  virtual auto audioSample(const double* samples, uint channels) -> void {}
  virtual auto inputPoll(uint port, uint device, uint input) -> int16 { return 0; }
  virtual auto inputRumble(uint port, uint device, uint input, bool enable) -> void {}
  virtual auto dipSettings(Markup::Node node) -> uint { return 0; }
  virtual auto notify(string text) -> void { print(text, "\n"); }
};

struct Interface {
  struct Information {
    string manufacturer;
    string name;
    bool overscan;
    struct Capability {
      bool states;
      bool cheats;
    } capability;
  } information;

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
  virtual auto videoSize() -> VideoSize = 0;
  virtual auto videoSize(uint width, uint height, bool arc) -> VideoSize = 0;
  virtual auto videoFrequency() -> double = 0;
  virtual auto videoColors() -> uint32 = 0;
  virtual auto videoColor(uint32 color) -> uint64 = 0;

  //audio information
  virtual auto audioFrequency() -> double = 0;

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
  virtual auto rtcsync() -> void {}

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

//nall/vfs shorthand constants for open(), load()
struct File {
  static const auto Read = vfs::file::mode::read;
  static const auto Write = vfs::file::mode::write;
  static const auto Optional = false;
  static const auto Required = true;
};

extern Platform* platform;

}
