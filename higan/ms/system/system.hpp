struct System {
  auto loaded() const -> bool { return false; }
  auto colorburst() const -> double { return 0.0; }

  auto run() -> void;

  auto load() -> bool;
  auto save() -> void;
  auto unload() -> void;

  auto power() -> void;
  auto reset() -> void;
};

extern System system;
