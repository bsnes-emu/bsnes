#if defined(Hiro_Keyboard)

namespace hiro {

auto pKeyboard::poll() -> vector<bool> {
  if(Application::state().quit) return {};

  vector<bool> result;
  char state[256];
  #if defined(DISPLAY_XORG)
  XQueryKeymap(pApplication::display, state);
  #endif
  for(auto& code : settings.keycodes) {
    result.append(_pressed(state, code));
  }
  return result;
}

auto pKeyboard::pressed(unsigned code) -> bool {
  char state[256];
  #if defined(DISPLAY_XORG)
  XQueryKeymap(pApplication::display, state);
  #endif
  return _pressed(state, code);
}

auto pKeyboard::_pressed(const char* state, uint16_t code) -> bool {
  uint8_t lo = code >> 0;
  uint8_t hi = code >> 8;

  #if defined(DISPLAY_WINDOWS)
  if(lo && GetAsyncKeyState(lo) & 0x8000) return true;
  if(hi && GetAsyncKeyState(hi) & 0x8000) return true;
  #endif

  #if defined(DISPLAY_XORG)
  if(lo && state[lo >> 3] & (1 << (lo & 7))) return true;
  if(hi && state[hi >> 3] & (1 << (hi & 7))) return true;
  #endif

  return false;
}

auto pKeyboard::_translate(unsigned code) -> signed {
  switch(code) {
  case GDK_KEY_Escape: return 0;
  case GDK_KEY_F1: return 0;
  case GDK_KEY_F2: return 0;
  case GDK_KEY_F3: return 0;
  case GDK_KEY_F4: return 0;
  case GDK_KEY_F5: return 0;
  case GDK_KEY_F6: return 0;
  case GDK_KEY_F7: return 0;
  case GDK_KEY_F8: return 0;
  case GDK_KEY_F9: return 0;
  case GDK_KEY_F10: return 0;
  case GDK_KEY_F11: return 0;
  case GDK_KEY_F12: return 0;

  case GDK_KEY_Print: return 0;
  case GDK_KEY_Scroll_Lock: return 0;
  case GDK_KEY_Pause: return 0;

  case GDK_KEY_Insert: return 0;
  case GDK_KEY_Delete: return 0;
  case GDK_KEY_Home: return 0;
  case GDK_KEY_End: return 0;
  case GDK_KEY_Prior: return 0;
  case GDK_KEY_Next: return 0;

  case GDK_KEY_Up: return 0;
  case GDK_KEY_Down: return 0;
  case GDK_KEY_Left: return 0;
  case GDK_KEY_Right: return 0;

  case GDK_KEY_grave: return '`';
  case GDK_KEY_1: return '1';
  case GDK_KEY_2: return '2';
  case GDK_KEY_3: return '3';
  case GDK_KEY_4: return '4';
  case GDK_KEY_5: return '5';
  case GDK_KEY_6: return '6';
  case GDK_KEY_7: return '7';
  case GDK_KEY_8: return '8';
  case GDK_KEY_9: return '9';
  case GDK_KEY_0: return '0';
  case GDK_KEY_minus: return '-';
  case GDK_KEY_equal: return '=';
  case GDK_KEY_BackSpace: return '\b';

  case GDK_KEY_asciitilde: return '~';
  case GDK_KEY_exclam: return '!';
  case GDK_KEY_at: return '@';
  case GDK_KEY_numbersign: return '#';
  case GDK_KEY_dollar: return '$';
  case GDK_KEY_percent: return '%';
  case GDK_KEY_asciicircum: return '^';
  case GDK_KEY_ampersand: return '&';
  case GDK_KEY_asterisk: return '*';
  case GDK_KEY_parenleft: return '(';
  case GDK_KEY_parenright: return ')';
  case GDK_KEY_underscore: return '_';
  case GDK_KEY_plus: return '+';

  case GDK_KEY_Tab: return '\t';
  case GDK_KEY_Caps_Lock: return 0;
  case GDK_KEY_Return: return '\n';
  case GDK_KEY_Shift_L: return 0;
  case GDK_KEY_Shift_R: return 0;
  case GDK_KEY_Control_L: return 0;
  case GDK_KEY_Control_R: return 0;
  case GDK_KEY_Alt_L: return 0;
  case GDK_KEY_Alt_R: return 0;
  case GDK_KEY_Super_L: return 0;
  case GDK_KEY_Super_R: return 0;
  case GDK_KEY_Menu: return 0;
  case GDK_KEY_space: return ' ';

  case GDK_KEY_bracketleft: return '[';
  case GDK_KEY_bracketright: return ']';
  case GDK_KEY_backslash: return '\\';
  case GDK_KEY_semicolon: return ';';
  case GDK_KEY_apostrophe: return '\'';
  case GDK_KEY_comma: return ',';
  case GDK_KEY_period: return '.';
  case GDK_KEY_slash: return '/';

  case GDK_KEY_braceleft: return '{';
  case GDK_KEY_braceright: return '}';
  case GDK_KEY_bar: return '|';
  case GDK_KEY_colon: return ':';
  case GDK_KEY_quotedbl: return '\"';
  case GDK_KEY_less: return '<';
  case GDK_KEY_greater: return '>';
  case GDK_KEY_question: return '?';

  case GDK_KEY_A: return 'A';
  case GDK_KEY_B: return 'B';
  case GDK_KEY_C: return 'C';
  case GDK_KEY_D: return 'D';
  case GDK_KEY_E: return 'E';
  case GDK_KEY_F: return 'F';
  case GDK_KEY_G: return 'G';
  case GDK_KEY_H: return 'H';
  case GDK_KEY_I: return 'I';
  case GDK_KEY_J: return 'J';
  case GDK_KEY_K: return 'K';
  case GDK_KEY_L: return 'L';
  case GDK_KEY_M: return 'M';
  case GDK_KEY_N: return 'N';
  case GDK_KEY_O: return 'O';
  case GDK_KEY_P: return 'P';
  case GDK_KEY_Q: return 'Q';
  case GDK_KEY_R: return 'R';
  case GDK_KEY_S: return 'S';
  case GDK_KEY_T: return 'T';
  case GDK_KEY_U: return 'U';
  case GDK_KEY_V: return 'V';
  case GDK_KEY_W: return 'W';
  case GDK_KEY_X: return 'X';
  case GDK_KEY_Y: return 'Y';
  case GDK_KEY_Z: return 'Z';

  case GDK_KEY_a: return 'a';
  case GDK_KEY_b: return 'b';
  case GDK_KEY_c: return 'c';
  case GDK_KEY_d: return 'd';
  case GDK_KEY_e: return 'e';
  case GDK_KEY_f: return 'f';
  case GDK_KEY_g: return 'g';
  case GDK_KEY_h: return 'h';
  case GDK_KEY_i: return 'i';
  case GDK_KEY_j: return 'j';
  case GDK_KEY_k: return 'k';
  case GDK_KEY_l: return 'l';
  case GDK_KEY_m: return 'm';
  case GDK_KEY_n: return 'n';
  case GDK_KEY_o: return 'o';
  case GDK_KEY_p: return 'p';
  case GDK_KEY_q: return 'q';
  case GDK_KEY_r: return 'r';
  case GDK_KEY_s: return 's';
  case GDK_KEY_t: return 't';
  case GDK_KEY_u: return 'u';
  case GDK_KEY_v: return 'v';
  case GDK_KEY_w: return 'w';
  case GDK_KEY_x: return 'x';
  case GDK_KEY_y: return 'y';
  case GDK_KEY_z: return 'z';

  case GDK_KEY_Num_Lock: return 0;
  case GDK_KEY_KP_Divide: return '/';
  case GDK_KEY_KP_Multiply: return '*';
  case GDK_KEY_KP_Subtract: return '-';
  case GDK_KEY_KP_Add: return '+';
  case GDK_KEY_KP_Enter: return '\n';
  case GDK_KEY_KP_Decimal: return '.';

  case GDK_KEY_KP_1: return '1';
  case GDK_KEY_KP_2: return '2';
  case GDK_KEY_KP_3: return '3';
  case GDK_KEY_KP_4: return '4';
  case GDK_KEY_KP_5: return '5';
  case GDK_KEY_KP_6: return '6';
  case GDK_KEY_KP_7: return '7';
  case GDK_KEY_KP_8: return '8';
  case GDK_KEY_KP_9: return '9';
  case GDK_KEY_KP_0: return '0';

  case GDK_KEY_KP_Home: return 0;
  case GDK_KEY_KP_End: return 0;
  case GDK_KEY_KP_Page_Up: return 0;
  case GDK_KEY_KP_Page_Down: return 0;
  case GDK_KEY_KP_Up: return 0;
  case GDK_KEY_KP_Down: return 0;
  case GDK_KEY_KP_Left: return 0;
  case GDK_KEY_KP_Right: return 0;
  case GDK_KEY_KP_Begin: return 0;
  case GDK_KEY_KP_Insert: return 0;
  case GDK_KEY_KP_Delete: return 0;
  }

  return 0;
}

auto pKeyboard::initialize() -> void {
  auto append = [](unsigned lo, unsigned hi = 0) {
    #if defined(DISPLAY_XORG)
    lo = lo ? (uint8_t)XKeysymToKeycode(pApplication::display, lo) : 0;
    hi = hi ? (uint8_t)XKeysymToKeycode(pApplication::display, hi) : 0;
    #endif
    settings.keycodes.append(lo | (hi << 8));
  };

  #define map(name, ...) if(key == name) { append(__VA_ARGS__); continue; }
  for(auto& key : Keyboard::keys) {
    #if defined(DISPLAY_WINDOWS)
      #include <hiro/platform/windows/keyboard.hpp>
    #endif

    #if defined(DISPLAY_XORG)
      #include <hiro/platform/xorg/keyboard.hpp>
    #endif

  //print("[hiro/gtk] warning: unhandled key: ", key, "\n");
    append(0);
  }
  #undef map
}

}

#endif
