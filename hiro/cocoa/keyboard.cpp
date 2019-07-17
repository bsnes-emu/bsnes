#if defined(Hiro_Keyboard)

namespace hiro {

auto pKeyboard::poll() -> vector<bool> {
  vector<bool> result;
  return result;
}

auto pKeyboard::pressed(uint code) -> bool {
  return false;
}

}

#endif
