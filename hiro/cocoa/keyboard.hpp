#if defined(Hiro_Keyboard)

namespace hiro {

struct pKeyboard {
  static auto poll() -> vector<bool>;
  static auto pressed(uint code) -> bool;
};

}

#endif
