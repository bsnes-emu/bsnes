struct Program : Emulator::Interface::Bind {
  //program.cpp
  Program(lstring args);
  auto load(string) -> void;
  auto main() -> void;
  auto quit() -> void;

  //interface.cpp
  auto loadRequest(uint id, string name, string type, bool required) -> void override {}
  auto loadRequest(uint id, string path, bool required) -> void override {}
  auto saveRequest(uint id, string path) -> void override {}
  auto videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void override {}
  auto audioSample(int16 lsample, int16 rsample) -> void override {}
  auto inputPoll(uint port, uint device, uint input) -> int16 override { return 0; }
  auto inputRumble(uint port, uint device, uint input, bool enable) -> void override {}
  auto dipSettings(const Markup::Node& node) -> uint override { return 0; }
  auto path(uint group) -> string override { return ""; }
  auto notify(string text) -> void override {}
};

extern unique_pointer<Program> program;
