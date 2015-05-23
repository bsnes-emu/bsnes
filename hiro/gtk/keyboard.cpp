#if defined(Hiro_Keyboard)

namespace hiro {

auto pKeyboard::poll() -> vector<bool> {
  vector<bool> result;
  char state[256];
  #if defined(PLATFORM_XORG)
  XQueryKeymap(pApplication::display, state);
  #endif
  for(auto& code : settings->keycodes) {
    result.append(_pressed(state, code));
  }
  return result;
}

auto pKeyboard::pressed(unsigned code) -> bool {
  char state[256];
  #if defined(PLATFORM_XORG)
  XQueryKeymap(pApplication::display, state);
  #endif
  return _pressed(state, code);
}

auto pKeyboard::_pressed(const char* state, uint16_t code) -> bool {
  uint8_t lo = code >> 0;
  uint8_t hi = code >> 8;

  #if defined(PLATFORM_WINDOWS)
  if(lo && GetAsyncKeyState(lo) & 0x8000) return true;
  if(hi && GetAsyncKeyState(hi) & 0x8000) return true;
  #endif

  #if defined(PLATFORM_XORG)
  if(lo && state[lo >> 3] & (1 << (lo & 7))) return true;
  if(hi && state[hi >> 3] & (1 << (hi & 7))) return true;
  #endif

  return false;
}

auto pKeyboard::_translate(unsigned code) -> signed {
  switch(code) {
  case GDK_Escape: return 0;
  case GDK_F1: return 0;
  case GDK_F2: return 0;
  case GDK_F3: return 0;
  case GDK_F4: return 0;
  case GDK_F5: return 0;
  case GDK_F6: return 0;
  case GDK_F7: return 0;
  case GDK_F8: return 0;
  case GDK_F9: return 0;
  case GDK_F10: return 0;
  case GDK_F11: return 0;
  case GDK_F12: return 0;

  case GDK_Print: return 0;
  case GDK_Scroll_Lock: return 0;
  case GDK_Pause: return 0;

  case GDK_Insert: return 0;
  case GDK_Delete: return 0;
  case GDK_Home: return 0;
  case GDK_End: return 0;
  case GDK_Prior: return 0;
  case GDK_Next: return 0;

  case GDK_Up: return 0;
  case GDK_Down: return 0;
  case GDK_Left: return 0;
  case GDK_Right: return 0;

  case GDK_grave: return '`';
  case GDK_1: return '1';
  case GDK_2: return '2';
  case GDK_3: return '3';
  case GDK_4: return '4';
  case GDK_5: return '5';
  case GDK_6: return '6';
  case GDK_7: return '7';
  case GDK_8: return '8';
  case GDK_9: return '9';
  case GDK_0: return '0';
  case GDK_minus: return '-';
  case GDK_equal: return '=';
  case GDK_BackSpace: return '\b';

  case GDK_asciitilde: return '~';
  case GDK_exclam: return '!';
  case GDK_at: return '@';
  case GDK_numbersign: return '#';
  case GDK_dollar: return '$';
  case GDK_percent: return '%';
  case GDK_asciicircum: return '^';
  case GDK_ampersand: return '&';
  case GDK_asterisk: return '*';
  case GDK_parenleft: return '(';
  case GDK_parenright: return ')';
  case GDK_underscore: return '_';
  case GDK_plus: return '+';

  case GDK_Tab: return '\t';
  case GDK_Caps_Lock: return 0;
  case GDK_Return: return '\n';
  case GDK_Shift_L: return 0;
  case GDK_Shift_R: return 0;
  case GDK_Control_L: return 0;
  case GDK_Control_R: return 0;
  case GDK_Alt_L: return 0;
  case GDK_Alt_R: return 0;
  case GDK_Super_L: return 0;
  case GDK_Super_R: return 0;
  case GDK_Menu: return 0;
  case GDK_space: return ' ';

  case GDK_bracketleft: return '[';
  case GDK_bracketright: return ']';
  case GDK_backslash: return '\\';
  case GDK_semicolon: return ';';
  case GDK_apostrophe: return '\'';
  case GDK_comma: return ',';
  case GDK_period: return '.';
  case GDK_slash: return '/';

  case GDK_braceleft: return '{';
  case GDK_braceright: return '}';
  case GDK_bar: return '|';
  case GDK_colon: return ':';
  case GDK_quotedbl: return '\"';
  case GDK_less: return '<';
  case GDK_greater: return '>';
  case GDK_question: return '?';

  case GDK_A: return 'A';
  case GDK_B: return 'B';
  case GDK_C: return 'C';
  case GDK_D: return 'D';
  case GDK_E: return 'E';
  case GDK_F: return 'F';
  case GDK_G: return 'G';
  case GDK_H: return 'H';
  case GDK_I: return 'I';
  case GDK_J: return 'J';
  case GDK_K: return 'K';
  case GDK_L: return 'L';
  case GDK_M: return 'M';
  case GDK_N: return 'N';
  case GDK_O: return 'O';
  case GDK_P: return 'P';
  case GDK_Q: return 'Q';
  case GDK_R: return 'R';
  case GDK_S: return 'S';
  case GDK_T: return 'T';
  case GDK_U: return 'U';
  case GDK_V: return 'V';
  case GDK_W: return 'W';
  case GDK_X: return 'X';
  case GDK_Y: return 'Y';
  case GDK_Z: return 'Z';

  case GDK_a: return 'a';
  case GDK_b: return 'b';
  case GDK_c: return 'c';
  case GDK_d: return 'd';
  case GDK_e: return 'e';
  case GDK_f: return 'f';
  case GDK_g: return 'g';
  case GDK_h: return 'h';
  case GDK_i: return 'i';
  case GDK_j: return 'j';
  case GDK_k: return 'k';
  case GDK_l: return 'l';
  case GDK_m: return 'm';
  case GDK_n: return 'n';
  case GDK_o: return 'o';
  case GDK_p: return 'p';
  case GDK_q: return 'q';
  case GDK_r: return 'r';
  case GDK_s: return 's';
  case GDK_t: return 't';
  case GDK_u: return 'u';
  case GDK_v: return 'v';
  case GDK_w: return 'w';
  case GDK_x: return 'x';
  case GDK_y: return 'y';
  case GDK_z: return 'z';

  case GDK_Num_Lock: return 0;
  case GDK_KP_Divide: return '/';
  case GDK_KP_Multiply: return '*';
  case GDK_KP_Subtract: return '-';
  case GDK_KP_Add: return '+';
  case GDK_KP_Enter: return '\n';
  case GDK_KP_Decimal: return '.';

  case GDK_KP_1: return '1';
  case GDK_KP_2: return '2';
  case GDK_KP_3: return '3';
  case GDK_KP_4: return '4';
  case GDK_KP_5: return '5';
  case GDK_KP_6: return '6';
  case GDK_KP_7: return '7';
  case GDK_KP_8: return '8';
  case GDK_KP_9: return '9';
  case GDK_KP_0: return '0';

  case GDK_KP_Home: return 0;
  case GDK_KP_End: return 0;
  case GDK_KP_Page_Up: return 0;
  case GDK_KP_Page_Down: return 0;
  case GDK_KP_Up: return 0;
  case GDK_KP_Down: return 0;
  case GDK_KP_Left: return 0;
  case GDK_KP_Right: return 0;
  case GDK_KP_Begin: return 0;
  case GDK_KP_Insert: return 0;
  case GDK_KP_Delete: return 0;
  }

  return 0;
}

auto pKeyboard::initialize() -> void {
  auto append = [](unsigned lo, unsigned hi = 0) {
    #if defined(PLATFORM_XORG)
    lo = lo ? (uint8_t)XKeysymToKeycode(pApplication::display, lo) : 0;
    hi = hi ? (uint8_t)XKeysymToKeycode(pApplication::display, hi) : 0;
    #endif
    settings->keycodes.append(lo | (hi << 8));
  };

  #define map(name, ...) if(key == name) { append(__VA_ARGS__); continue; }
  for(auto& key : Keyboard::keys) {
    #if defined(PLATFORM_WINDOWS)
      #include <hiro/platform/windows/keyboard.hpp>
    #endif

    #if defined(PLATFORM_XORG)
      #include <hiro/platform/xorg/keyboard.hpp>
    #endif

  //print("[phoenix/gtk] warning: unhandled key: ", key, "\n");
    append(0);
  }
  #undef map
}

}

#endif
