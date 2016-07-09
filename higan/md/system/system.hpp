struct System {
  auto loaded() const { return information.manifest; }

  auto run() -> void;

  auto load() -> bool;
  auto save() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  struct Information {
    bool loaded = false;
    string manifest;
  } information;
};

extern System system;
