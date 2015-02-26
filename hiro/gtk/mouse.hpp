namespace hiro {

struct pMouse {
  static auto position() -> Position;
  static auto pressed(Mouse::Button button) -> bool;
};

}
