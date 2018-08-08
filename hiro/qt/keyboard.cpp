#if defined(Hiro_Keyboard)

namespace hiro {

auto pKeyboard::poll() -> vector<bool> {
  if(Application::state().quit) return {};

  vector<bool> result;
  char state[256];

  #if defined(DISPLAY_XORG)
  XQueryKeymap(pApplication::state().display, state);
  #endif

  for(auto& code : settings.keycodes) {
    result.append(_pressed(state, code));
  }

  return result;
}

auto pKeyboard::pressed(unsigned code) -> bool {
  char state[256];

  #if defined(DISPLAY_XORG)
  XQueryKeymap(pApplication::state().display, state);
  #endif

  return _pressed(state, code);
}

auto pKeyboard::_pressed(const char* state, uint16_t code) -> bool {
  uint8_t lo = code >> 0;
  uint8_t hi = code >> 8;

  #if defined(DISPLAY_XORG)
  if(lo && state[lo >> 3] & (1 << (lo & 7))) return true;
  if(hi && state[hi >> 3] & (1 << (hi & 7))) return true;
  #endif

  return false;
}

auto pKeyboard::initialize() -> void {
  auto append = [](unsigned lo, unsigned hi = 0) {
    #if defined(DISPLAY_XORG)
    lo = lo ? (uint8_t)XKeysymToKeycode(pApplication::state().display, lo) : 0;
    hi = hi ? (uint8_t)XKeysymToKeycode(pApplication::state().display, hi) : 0;
    #endif
    settings.keycodes.append(lo << 0 | hi << 8);
  };

  #define map(name, ...) if(key == name) { append(__VA_ARGS__); continue; }
  for(auto& key : Keyboard::keys) {
    #if defined(DISPLAY_XORG)
      #include <hiro/platform/xorg/keyboard.hpp>
    #endif

  //print("[hiro/qt] warning: unhandled key: ", key, "\n");
    append(0);
  }
  #undef map
}

}

#endif
