#if defined(Hiro_Keyboard)

namespace hiro {

struct pKeyboard {
  static auto poll() -> vector<bool>;
  static auto pressed(unsigned code) -> bool;

  static auto initialize() -> void;

  static auto _translate(unsigned code, unsigned flags) -> signed;

  static vector<uint16_t> keycodes;
};

}

#endif
