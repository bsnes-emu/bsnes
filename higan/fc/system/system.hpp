struct System {
  auto run() -> void;
  auto runtosave() -> void;
  auto runthreadtosave() -> void;

  auto load() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto init() -> void;
  auto term() -> void;

  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

  auto serialize(serializer&) -> void;
  auto serialize_all(serializer&) -> void;
  auto serialize_init() -> void;

  struct Information {
    string manifest;
  } information;

  uint serialize_size;
};

extern System system;
