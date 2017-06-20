#pragma once

namespace Emulator {

struct Platform {
  struct Load {
    Load() : _pathID(nothing) {}
    Load(uint pathID, string option = "") : _pathID(pathID), _option(option) {}

    explicit operator bool() const { return (bool)_pathID; }
    auto pathID() const -> uint { return _pathID(); }
    auto option() const -> string { return _option; }

  private:
    maybe<uint> _pathID;
    string _option;
  };

  virtual auto path(uint id) -> string { return ""; }
  virtual auto open(uint id, string name, vfs::file::mode mode, bool required = false) -> vfs::shared::file { return {}; }
  virtual auto load(uint id, string name, string type, string_vector options = {}) -> Load { return {}; }
  virtual auto videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void {}
  virtual auto audioSample(const double* samples, uint channels) -> void {}
  virtual auto inputPoll(uint port, uint device, uint input) -> int16 { return 0; }
  virtual auto inputRumble(uint port, uint device, uint input, bool enable) -> void {}
  virtual auto dipSettings(Markup::Node node) -> uint { return 0; }
  virtual auto notify(string text) -> void { print(text, "\n"); }
};

extern Platform* platform;

}
