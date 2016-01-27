struct System {
  enum class Revision : uint { WonderSwan, WonderSwanColor, SwanCrystal };

  auto revision() const -> Revision;
  auto monochrome() const -> bool;
  auto color() const -> bool;

  auto init() -> void;
  auto term() -> void;
  auto load(Revision) -> void;
  auto power() -> void;
  auto run() -> void;

  struct Information {
    string manifest;
  } information;

privileged:
  Revision _revision;
};

extern System system;
