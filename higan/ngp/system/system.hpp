struct System {
  enum class Model : uint { NeoGeoPocket, NeoGeoPocketColor };

  auto loaded() const -> bool { return information.loaded; }
  auto model() const -> Model { return information.model; }

  struct Information {
    string manifest;
    bool loaded = false;
    Model model = Model::NeoGeoPocket;
  } information;
};

extern System system;

auto Model::NeoGeoPocket() -> bool { return system.model() == System::Model::NeoGeoPocket; }
auto Model::NeoGeoPocketColor() -> bool { return system.model() == System::Model::NeoGeoPocketColor; }
