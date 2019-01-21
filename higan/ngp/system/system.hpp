struct System {
  enum class Model : uint { NeoGeoPocket, NeoGeoPocketColor };
  Emulator::Memory::Readable<uint8> bios;

  inline auto loaded() const -> bool { return information.loaded; }
  inline auto model() const -> Model { return information.model; }
  inline auto frequency() const -> double { return 6'144'000; }

  //system.cpp
  auto run() -> void;
  auto runToSave() -> void;
  auto load(Emulator::Interface*, Model) -> bool;
  auto save() -> void;
  auto unload() -> void;
  auto power() -> void;

  //serialization.cpp
  auto serializeInit() -> void;
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;
  auto serializeAll(serializer&) -> void;
  auto serialize(serializer&) -> void;

  struct Information {
    string manifest;
    bool loaded = false;
    Model model = Model::NeoGeoPocket;
    natural serializeSize;
  } information;

private:
  Emulator::Interface* interface = nullptr;
};

extern System system;

auto Model::NeoGeoPocket() -> bool { return system.model() == System::Model::NeoGeoPocket; }
auto Model::NeoGeoPocketColor() -> bool { return system.model() == System::Model::NeoGeoPocketColor; }
