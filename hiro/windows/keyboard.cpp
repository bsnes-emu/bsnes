#if defined(Hiro_Keyboard)

namespace hiro {

vector<uint16_t> pKeyboard::keycodes;

auto pKeyboard::poll() -> vector<bool> {
  vector<bool> result;
  for(auto& code : keycodes) result.append(pressed(code));
  return result;
}

auto pKeyboard::pressed(unsigned code) -> bool {
  uint8_t lo = code >> 0;
  uint8_t hi = code >> 8;
  if(lo && GetAsyncKeyState(lo) & 0x8000) return true;
  if(hi && GetAsyncKeyState(hi) & 0x8000) return true;
  return false;
}

auto pKeyboard::initialize() -> void {
  auto append = [](unsigned lo, unsigned hi = 0) {
    keycodes.append(lo << 0 | hi << 8);
  };

  #define map(name, ...) if(key == name) { append(__VA_ARGS__); continue; }
  for(auto& key : Keyboard::keys) {
    #include <hiro/platform/windows/keyboard.hpp>
  //print("[hiro/windows] warning: unhandled key: ", key, "\n");
    append(0);
  }
  #undef map
}

auto pKeyboard::_translate(unsigned code, unsigned flags) -> signed {
  bool numLock = GetKeyState(VK_NUMLOCK);
  bool capsLock = GetKeyState(VK_CAPITAL);
  bool shifted = (GetAsyncKeyState(VK_LSHIFT) & 0x8000) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000);
  bool pressed = GetAsyncKeyState(code) & 0x8000;
  bool extended = flags & (1 << 24);

  switch(code) {
  case VK_OEM_3: return !shifted ? '`' : '~';
  case '1': return !shifted ? '1' : '!';
  case '2': return !shifted ? '2' : '@';
  case '3': return !shifted ? '3' : '#';
  case '4': return !shifted ? '4' : '$';
  case '5': return !shifted ? '5' : '%';
  case '6': return !shifted ? '6' : '^';
  case '7': return !shifted ? '7' : '&';
  case '8': return !shifted ? '8' : '*';
  case '9': return !shifted ? '9' : '(';
  case '0': return !shifted ? '0' : ')';
  case VK_OEM_MINUS: return !shifted ? '-' : '_';
  case VK_OEM_PLUS: return !shifted ? '=' : '+';
  case VK_BACK: return '\b';

  case VK_TAB: return '\t';
  case VK_RETURN: return '\n';
  case VK_SPACE: return ' ';

  case VK_OEM_4: return !shifted ? '[' : '{';
  case VK_OEM_6: return !shifted ? ']' : '}';
  case VK_OEM_5: return !shifted ? '\\' : '|';
  case VK_OEM_1: return !shifted ? ';' : ':';
  case VK_OEM_7: return !shifted ? '\'' : '\"';
  case VK_OEM_COMMA: return !shifted ? ',' : '<';
  case VK_OEM_PERIOD: return !shifted ? '.' : '>';
  case VK_OEM_2: return !shifted ? '/' : '?';

  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M':
  case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
    if(capsLock) return !shifted ? code : code + 32;
    else return !shifted ? code + 32 : code;

  case VK_DIVIDE: return '/';
  case VK_MULTIPLY: return '*';
  case VK_SUBTRACT: return '-';
  case VK_ADD: return '+';
  case VK_DECIMAL: return '.';

  case VK_NUMPAD1: return numLock ? '1' : 0;
  case VK_NUMPAD2: return numLock ? '2' : 0;
  case VK_NUMPAD3: return numLock ? '3' : 0;
  case VK_NUMPAD4: return numLock ? '4' : 0;
  case VK_NUMPAD5: return numLock ? '5' : 0;
  case VK_NUMPAD6: return numLock ? '6' : 0;
  case VK_NUMPAD7: return numLock ? '7' : 0;
  case VK_NUMPAD8: return numLock ? '8' : 0;
  case VK_NUMPAD9: return numLock ? '9' : 0;
  case VK_NUMPAD0: return numLock ? '0' : 0;
  }

  return 0;
}

}

#endif
