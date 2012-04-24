void pKeyboard::initialize() {
  auto append = [](Keyboard::Scancode scancode, unsigned keysym) {
    settings->keymap.insert(scancode, XKeysymToKeycode(pOS::display, keysym));
  };

  append(Keyboard::Scancode::Escape, XK_Escape);
  append(Keyboard::Scancode::F1, XK_F1);
  append(Keyboard::Scancode::F2, XK_F2);
  append(Keyboard::Scancode::F3, XK_F3);
  append(Keyboard::Scancode::F4, XK_F4);
  append(Keyboard::Scancode::F5, XK_F5);
  append(Keyboard::Scancode::F6, XK_F6);
  append(Keyboard::Scancode::F7, XK_F7);
  append(Keyboard::Scancode::F8, XK_F8);
  append(Keyboard::Scancode::F9, XK_F9);
  append(Keyboard::Scancode::F10, XK_F10);
  append(Keyboard::Scancode::F11, XK_F11);
  append(Keyboard::Scancode::F12, XK_F12);

  append(Keyboard::Scancode::PrintScreen, XK_Print);
  append(Keyboard::Scancode::ScrollLock, XK_Scroll_Lock);
  append(Keyboard::Scancode::Pause, XK_Pause);

  append(Keyboard::Scancode::Insert, XK_Insert);
  append(Keyboard::Scancode::Delete, XK_Delete);
  append(Keyboard::Scancode::Home, XK_Home);
  append(Keyboard::Scancode::End, XK_End);
  append(Keyboard::Scancode::PageUp, XK_Prior);
  append(Keyboard::Scancode::PageDown, XK_Next);

  append(Keyboard::Scancode::Up, XK_Up);
  append(Keyboard::Scancode::Down, XK_Down);
  append(Keyboard::Scancode::Left, XK_Left);
  append(Keyboard::Scancode::Right, XK_Right);

  append(Keyboard::Scancode::Grave, XK_asciitilde);
  append(Keyboard::Scancode::Number1, XK_1);
  append(Keyboard::Scancode::Number2, XK_2);
  append(Keyboard::Scancode::Number3, XK_3);
  append(Keyboard::Scancode::Number4, XK_4);
  append(Keyboard::Scancode::Number5, XK_5);
  append(Keyboard::Scancode::Number6, XK_6);
  append(Keyboard::Scancode::Number7, XK_7);
  append(Keyboard::Scancode::Number8, XK_8);
  append(Keyboard::Scancode::Number9, XK_9);
  append(Keyboard::Scancode::Number0, XK_0);
  append(Keyboard::Scancode::Minus, XK_minus);
  append(Keyboard::Scancode::Equal, XK_equal);
  append(Keyboard::Scancode::Backspace, XK_BackSpace);

  append(Keyboard::Scancode::BracketLeft, XK_bracketleft);
  append(Keyboard::Scancode::BracketRight, XK_bracketright);
  append(Keyboard::Scancode::Backslash, XK_backslash);
  append(Keyboard::Scancode::Semicolon, XK_semicolon);
  append(Keyboard::Scancode::Apostrophe, XK_apostrophe);
  append(Keyboard::Scancode::Comma, XK_comma);
  append(Keyboard::Scancode::Period, XK_period);
  append(Keyboard::Scancode::Slash, XK_slash);

  append(Keyboard::Scancode::Tab, XK_Tab);
  append(Keyboard::Scancode::CapsLock, XK_Caps_Lock);
  append(Keyboard::Scancode::Return, XK_Return);
  append(Keyboard::Scancode::ShiftLeft, XK_Shift_L);
  append(Keyboard::Scancode::ShiftRight, XK_Shift_R);
  append(Keyboard::Scancode::ControlLeft, XK_Control_L);
  append(Keyboard::Scancode::ControlRight, XK_Control_R);
  append(Keyboard::Scancode::SuperLeft, XK_Super_L);
  append(Keyboard::Scancode::SuperRight, XK_Super_R);
  append(Keyboard::Scancode::AltLeft, XK_Alt_L);
  append(Keyboard::Scancode::AltRight, XK_Alt_R);
  append(Keyboard::Scancode::Space, XK_space);
  append(Keyboard::Scancode::Menu, XK_Menu);

  append(Keyboard::Scancode::A, XK_A);
  append(Keyboard::Scancode::B, XK_B);
  append(Keyboard::Scancode::C, XK_C);
  append(Keyboard::Scancode::D, XK_D);
  append(Keyboard::Scancode::E, XK_E);
  append(Keyboard::Scancode::F, XK_F);
  append(Keyboard::Scancode::G, XK_G);
  append(Keyboard::Scancode::H, XK_H);
  append(Keyboard::Scancode::I, XK_I);
  append(Keyboard::Scancode::J, XK_J);
  append(Keyboard::Scancode::K, XK_K);
  append(Keyboard::Scancode::L, XK_L);
  append(Keyboard::Scancode::M, XK_M);
  append(Keyboard::Scancode::N, XK_N);
  append(Keyboard::Scancode::O, XK_O);
  append(Keyboard::Scancode::P, XK_P);
  append(Keyboard::Scancode::Q, XK_Q);
  append(Keyboard::Scancode::R, XK_R);
  append(Keyboard::Scancode::S, XK_S);
  append(Keyboard::Scancode::T, XK_T);
  append(Keyboard::Scancode::U, XK_U);
  append(Keyboard::Scancode::V, XK_V);
  append(Keyboard::Scancode::W, XK_W);
  append(Keyboard::Scancode::X, XK_X);
  append(Keyboard::Scancode::Y, XK_Y);
  append(Keyboard::Scancode::Z, XK_Z);

  append(Keyboard::Scancode::NumLock, XK_Num_Lock);
  append(Keyboard::Scancode::Divide, XK_KP_Divide);
  append(Keyboard::Scancode::Multiply, XK_KP_Multiply);
  append(Keyboard::Scancode::Subtract, XK_KP_Subtract);
  append(Keyboard::Scancode::Add, XK_KP_Add);
  append(Keyboard::Scancode::Enter, XK_KP_Enter);
  append(Keyboard::Scancode::Point, XK_KP_Decimal);

  append(Keyboard::Scancode::Keypad1, XK_KP_1);
  append(Keyboard::Scancode::Keypad2, XK_KP_2);
  append(Keyboard::Scancode::Keypad3, XK_KP_3);
  append(Keyboard::Scancode::Keypad4, XK_KP_4);
  append(Keyboard::Scancode::Keypad5, XK_KP_5);
  append(Keyboard::Scancode::Keypad6, XK_KP_6);
  append(Keyboard::Scancode::Keypad7, XK_KP_7);
  append(Keyboard::Scancode::Keypad8, XK_KP_8);
  append(Keyboard::Scancode::Keypad9, XK_KP_9);
  append(Keyboard::Scancode::Keypad0, XK_KP_0);
}

bool pKeyboard::pressed(Keyboard::Scancode scancode) {
  char state[256];
  XQueryKeymap(pOS::display, state);
  unsigned id = settings->keymap.lhs[scancode];
  return state[id >> 3] & (1 << (id & 7));
}

array<bool> pKeyboard::state() {
  array<bool> output;
  output.resize((unsigned)Keyboard::Scancode::Limit);
  for(auto &n : output) n = false;

  char state[256];
  XQueryKeymap(pOS::display, state);
  for(auto &n : settings->keymap.rhs) {
    if(state[n.name >> 3] & (1 << (n.name & 7))) {
      output[(unsigned)n.data] = true;
    }
  }

  return output;
}
