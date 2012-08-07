void pKeyboard::initialize() {
  auto append = [](Keyboard::Scancode scancode, unsigned keysym) {
    settings->keymap.insert(scancode, keysym);
  };

  append(Keyboard::Scancode::Escape, VK_ESCAPE);
  append(Keyboard::Scancode::F1, VK_F1);
  append(Keyboard::Scancode::F2, VK_F2);
  append(Keyboard::Scancode::F3, VK_F3);
  append(Keyboard::Scancode::F4, VK_F4);
  append(Keyboard::Scancode::F5, VK_F5);
  append(Keyboard::Scancode::F6, VK_F6);
  append(Keyboard::Scancode::F7, VK_F7);
  append(Keyboard::Scancode::F8, VK_F8);
  append(Keyboard::Scancode::F9, VK_F9);
  append(Keyboard::Scancode::F10, VK_F10);
  append(Keyboard::Scancode::F11, VK_F11);
  append(Keyboard::Scancode::F12, VK_F12);

  append(Keyboard::Scancode::PrintScreen, VK_SNAPSHOT);
  append(Keyboard::Scancode::ScrollLock, VK_SCROLL);
  append(Keyboard::Scancode::Pause, VK_PAUSE);

  append(Keyboard::Scancode::Insert, VK_INSERT);
  append(Keyboard::Scancode::Delete, VK_DELETE);
  append(Keyboard::Scancode::Home, VK_HOME);
  append(Keyboard::Scancode::End, VK_END);
  append(Keyboard::Scancode::PageUp, VK_PRIOR);
  append(Keyboard::Scancode::PageDown, VK_NEXT);

  append(Keyboard::Scancode::Up, VK_UP);
  append(Keyboard::Scancode::Down, VK_DOWN);
  append(Keyboard::Scancode::Left, VK_LEFT);
  append(Keyboard::Scancode::Right, VK_RIGHT);

  append(Keyboard::Scancode::Grave, VK_OEM_3);
  append(Keyboard::Scancode::Number1, '1');
  append(Keyboard::Scancode::Number2, '2');
  append(Keyboard::Scancode::Number3, '3');
  append(Keyboard::Scancode::Number4, '4');
  append(Keyboard::Scancode::Number5, '5');
  append(Keyboard::Scancode::Number6, '6');
  append(Keyboard::Scancode::Number7, '7');
  append(Keyboard::Scancode::Number8, '8');
  append(Keyboard::Scancode::Number9, '9');
  append(Keyboard::Scancode::Number0, '0');
  append(Keyboard::Scancode::Minus, VK_OEM_MINUS);
  append(Keyboard::Scancode::Equal, VK_OEM_PLUS);
  append(Keyboard::Scancode::Backspace, VK_BACK);

  append(Keyboard::Scancode::BracketLeft, VK_OEM_4);
  append(Keyboard::Scancode::BracketRight, VK_OEM_6);
  append(Keyboard::Scancode::Backslash, VK_OEM_5);
  append(Keyboard::Scancode::Semicolon, VK_OEM_1);
  append(Keyboard::Scancode::Apostrophe, VK_OEM_7);
  append(Keyboard::Scancode::Comma, VK_OEM_COMMA);
  append(Keyboard::Scancode::Period, VK_OEM_PERIOD);
  append(Keyboard::Scancode::Slash, VK_OEM_2);

  append(Keyboard::Scancode::Tab, VK_TAB);
  append(Keyboard::Scancode::CapsLock, VK_CAPITAL);
  append(Keyboard::Scancode::Return, VK_RETURN);
  append(Keyboard::Scancode::ShiftLeft, VK_LSHIFT);
  append(Keyboard::Scancode::ShiftRight, VK_RSHIFT);
  append(Keyboard::Scancode::ControlLeft, VK_LCONTROL);
  append(Keyboard::Scancode::ControlRight, VK_RCONTROL);
  append(Keyboard::Scancode::SuperLeft, VK_LWIN);
  append(Keyboard::Scancode::SuperRight, VK_RWIN);
  append(Keyboard::Scancode::AltLeft, VK_LMENU);
  append(Keyboard::Scancode::AltRight, VK_RMENU);
  append(Keyboard::Scancode::Space, VK_SPACE);
  append(Keyboard::Scancode::Menu, VK_APPS);

  append(Keyboard::Scancode::A, 'A');
  append(Keyboard::Scancode::B, 'B');
  append(Keyboard::Scancode::C, 'C');
  append(Keyboard::Scancode::D, 'D');
  append(Keyboard::Scancode::E, 'E');
  append(Keyboard::Scancode::F, 'F');
  append(Keyboard::Scancode::G, 'G');
  append(Keyboard::Scancode::H, 'H');
  append(Keyboard::Scancode::I, 'I');
  append(Keyboard::Scancode::J, 'J');
  append(Keyboard::Scancode::K, 'K');
  append(Keyboard::Scancode::L, 'L');
  append(Keyboard::Scancode::M, 'M');
  append(Keyboard::Scancode::N, 'N');
  append(Keyboard::Scancode::O, 'O');
  append(Keyboard::Scancode::P, 'P');
  append(Keyboard::Scancode::Q, 'Q');
  append(Keyboard::Scancode::R, 'R');
  append(Keyboard::Scancode::S, 'S');
  append(Keyboard::Scancode::T, 'T');
  append(Keyboard::Scancode::U, 'U');
  append(Keyboard::Scancode::V, 'V');
  append(Keyboard::Scancode::W, 'W');
  append(Keyboard::Scancode::X, 'X');
  append(Keyboard::Scancode::Y, 'Y');
  append(Keyboard::Scancode::Z, 'Z');

  append(Keyboard::Scancode::NumLock, VK_NUMLOCK);
  append(Keyboard::Scancode::Divide, VK_DIVIDE);
  append(Keyboard::Scancode::Multiply, VK_MULTIPLY);
  append(Keyboard::Scancode::Subtract, VK_SUBTRACT);
  append(Keyboard::Scancode::Add, VK_ADD);
//append(Keyboard::Scancode::Enter, ...);
  append(Keyboard::Scancode::Point, VK_DECIMAL);

  append(Keyboard::Scancode::Keypad1, VK_NUMPAD1);
  append(Keyboard::Scancode::Keypad2, VK_NUMPAD2);
  append(Keyboard::Scancode::Keypad3, VK_NUMPAD3);
  append(Keyboard::Scancode::Keypad4, VK_NUMPAD4);
  append(Keyboard::Scancode::Keypad5, VK_NUMPAD5);
  append(Keyboard::Scancode::Keypad6, VK_NUMPAD6);
  append(Keyboard::Scancode::Keypad7, VK_NUMPAD7);
  append(Keyboard::Scancode::Keypad8, VK_NUMPAD8);
  append(Keyboard::Scancode::Keypad9, VK_NUMPAD9);
  append(Keyboard::Scancode::Keypad0, VK_NUMPAD0);
}

bool pKeyboard::pressed(Keyboard::Scancode scancode) {
  return GetAsyncKeyState(settings->keymap.lhs[scancode]) & 0x8000;
}

vector<bool> pKeyboard::state() {
  vector<bool> output;
  output.resize((unsigned)Keyboard::Scancode::Limit);
  for(auto &n : output) n = false;

  for(auto &n : settings->keymap.rhs) {
    if(GetAsyncKeyState(n.name) & 0x8000) {
      output[(unsigned)n.data] = true;
    }
  }

  return output;
}
