namespace GameBoy {

struct ID {
  enum : uint {
    System,
    GameBoy,
    SuperGameBoy,
    GameBoyColor,
  };

  struct Port { enum : uint {
    Hardware,
  };};

  struct Device { enum : uint {
    Controls,
  };};
};

struct Interface : Emulator::Interface {
  Interface();

  auto manifest() -> string override;
  auto title() -> string override;

  auto videoSize() -> VideoSize override;
  auto videoSize(uint width, uint height, bool arc) -> VideoSize override;
  auto videoFrequency() -> double override;

  auto audioFrequency() -> double override;

  auto loaded() -> bool override;
  auto sha256() -> string override;

  auto save() -> void override;
  auto unload() -> void override;

  auto power() -> void override;
  auto run() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheatSet(const string_vector&) -> void override;

  auto cap(const string& name) -> bool override;
  auto get(const string& name) -> any override;
  auto set(const string& name, const any& value) -> bool override;
};

struct GameBoyInterface : Interface {
  using Emulator::Interface::load;

  GameBoyInterface();

  auto videoColors() -> uint32 override;
  auto videoColor(uint32 color) -> uint64 override;

  auto load(uint id) -> bool override;
};

struct GameBoyColorInterface : Interface {
  using Emulator::Interface::load;

  GameBoyColorInterface();

  auto videoColors() -> uint32 override;
  auto videoColor(uint32 color) -> uint64 override;

  auto load(uint id) -> bool override;
};

struct SuperGameBoyInterface {
  virtual auto audioSample(const double* samples, uint channels) -> void = 0;
  virtual auto inputPoll(uint port, uint device, uint id) -> int16 = 0;

  virtual auto lcdScanline() -> void = 0;
  virtual auto lcdOutput(uint2 color) -> void = 0;
  virtual auto joypWrite(bool p15, bool p14) -> void = 0;
};

struct Settings {
  bool blurEmulation = true;
  bool colorEmulation = true;
};

extern SuperGameBoyInterface* superGameBoy;
extern Settings settings;

}
