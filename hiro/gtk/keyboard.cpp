namespace hiro {

auto pKeyboard::poll() -> vector<bool> {
  vector<bool> result;
  char state[256];
  XQueryKeymap(pApplication::display, state);
  for(auto& code : settings->keycodes) {
    result.append(_pressed(state, code));
  }
  return result;
}

auto pKeyboard::pressed(unsigned code) -> bool {
  char state[256];
  XQueryKeymap(pApplication::display, state);
  return _pressed(state, code);
}

auto pKeyboard::_pressed(char* state, uint16_t code) -> bool {
  uint8_t lo = code >> 0;
  uint8_t hi = code >> 8;
  if(lo && state[lo >> 3] & (1 << (lo & 7))) return true;
  if(hi && state[hi >> 3] & (1 << (hi & 7))) return true;
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
    lo = lo ? (uint8_t)XKeysymToKeycode(pApplication::display, lo) : 0;
    hi = hi ? (uint8_t)XKeysymToKeycode(pApplication::display, hi) : 0;
    settings->keycodes.append(lo | (hi << 8));
  };

  #define map(name, ...) if(key == name) { append(__VA_ARGS__); continue; }
  for(auto& key : Keyboard::keys) {
    map("Escape", XK_Escape);
    map("F1", XK_F1);
    map("F2", XK_F2);
    map("F3", XK_F3);
    map("F4", XK_F4);
    map("F5", XK_F5);
    map("F6", XK_F6);
    map("F7", XK_F7);
    map("F8", XK_F8);
    map("F9", XK_F9);
    map("F10", XK_F10);
    map("F11", XK_F11);
    map("F12", XK_F12);

    map("PrintScreen", XK_Print);
    map("ScrollLock", XK_Scroll_Lock);
    map("Pause", XK_Pause);

    map("Insert", XK_Insert);
    map("Delete", XK_Delete);
    map("Home", XK_Home);
    map("End", XK_End);
    map("PageUp", XK_Prior);
    map("PageDown", XK_Next);

    map("Up", XK_Up);
    map("Down", XK_Down);
    map("Left", XK_Left);
    map("Right", XK_Right);

    map("Grave", XK_asciitilde);
    map("1", XK_1);
    map("2", XK_2);
    map("3", XK_3);
    map("4", XK_4);
    map("5", XK_5);
    map("6", XK_6);
    map("7", XK_7);
    map("8", XK_8);
    map("9", XK_9);
    map("0", XK_0);
    map("Dash", XK_minus);
    map("Equal", XK_equal);
    map("Backspace", XK_BackSpace);

    map("Tab", XK_Tab);
    map("CapsLock", XK_Caps_Lock);
    map("LeftEnter", XK_Return);
    map("LeftShift", XK_Shift_L);
    map("RightShift", XK_Shift_R);
    map("LeftControl", XK_Control_L);
    map("RightControl", XK_Control_R);
    map("LeftAlt", XK_Alt_L);
    map("RightAlt", XK_Alt_R);
    map("LeftSuper", XK_Super_L);
    map("RightSuper", XK_Super_R);
    map("Menu", XK_Menu);
    map("Space", XK_space);

    map("OpenBracket", XK_bracketleft);
    map("CloseBracket", XK_bracketright);
    map("Backslash", XK_backslash);
    map("Semicolon", XK_semicolon);
    map("Apostrophe", XK_apostrophe);
    map("Comma", XK_comma);
    map("Period", XK_period);
    map("Slash", XK_slash);

    map("A", XK_A);
    map("B", XK_B);
    map("C", XK_C);
    map("D", XK_D);
    map("E", XK_E);
    map("F", XK_F);
    map("G", XK_G);
    map("H", XK_H);
    map("I", XK_I);
    map("J", XK_J);
    map("K", XK_K);
    map("L", XK_L);
    map("M", XK_M);
    map("N", XK_N);
    map("O", XK_O);
    map("P", XK_P);
    map("Q", XK_Q);
    map("R", XK_R);
    map("S", XK_S);
    map("T", XK_T);
    map("U", XK_U);
    map("V", XK_V);
    map("W", XK_W);
    map("X", XK_X);
    map("Y", XK_Y);
    map("Z", XK_Z);

    map("NumLock", XK_Num_Lock);
    map("Divide", XK_KP_Divide);
    map("Multiply", XK_KP_Multiply);
    map("Subtract", XK_KP_Subtract);
    map("Add", XK_KP_Add);
    map("RightEnter", XK_KP_Enter);
    map("Point", XK_KP_Decimal);

    map("One", XK_KP_1);
    map("Two", XK_KP_2);
    map("Three", XK_KP_3);
    map("Four", XK_KP_4);
    map("Five", XK_KP_5);
    map("Six", XK_KP_6);
    map("Seven", XK_KP_7);
    map("Eight", XK_KP_8);
    map("Nine", XK_KP_9);
    map("Zero", XK_KP_0);

    map("Shift", XK_Shift_L, XK_Shift_R);
    map("Control", XK_Control_L, XK_Control_R);
    map("Alt", XK_Alt_L, XK_Alt_R);
    map("Super", XK_Super_L, XK_Super_R);
    map("Enter", XK_Return, XK_KP_Enter);

    print("[phoenix/gtk] error: unhandled key: ", key, "\n");
    append(0);
  }
  #undef map
}

}
