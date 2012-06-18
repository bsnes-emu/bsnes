static QIcon CreateIcon(const nall::image &image, bool scale = false) {
  nall::image qtBuffer = image;
  qtBuffer.transform(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);
  if(scale) qtBuffer.scale(16, 16, Interpolation::Linear);
  QImage qtImage(qtBuffer.data, qtBuffer.width, qtBuffer.height, QImage::Format_ARGB32);
  return QIcon(QPixmap::fromImage(qtImage));
}

static Keyboard::Keycode Keysym(int keysym) {
  switch(keysym) {
  case XK_Escape: return Keyboard::Keycode::Escape;
  case XK_F1: return Keyboard::Keycode::F1;
  case XK_F2: return Keyboard::Keycode::F2;
  case XK_F3: return Keyboard::Keycode::F3;
  case XK_F4: return Keyboard::Keycode::F4;
  case XK_F5: return Keyboard::Keycode::F5;
  case XK_F6: return Keyboard::Keycode::F6;
  case XK_F7: return Keyboard::Keycode::F7;
  case XK_F8: return Keyboard::Keycode::F8;
  case XK_F9: return Keyboard::Keycode::F9;
  case XK_F10: return Keyboard::Keycode::F10;
  case XK_F11: return Keyboard::Keycode::F11;
  case XK_F12: return Keyboard::Keycode::F12;

  case XK_Print: return Keyboard::Keycode::PrintScreen;
  //Keyboard::Keycode::SysRq
  case XK_Scroll_Lock: return Keyboard::Keycode::ScrollLock;
  case XK_Pause: return Keyboard::Keycode::Pause;
  //Keyboard::Keycode::Break

  case XK_Insert: return Keyboard::Keycode::Insert;
  case XK_Delete: return Keyboard::Keycode::Delete;
  case XK_Home: return Keyboard::Keycode::Home;
  case XK_End: return Keyboard::Keycode::End;
  case XK_Prior: return Keyboard::Keycode::PageUp;
  case XK_Next: return Keyboard::Keycode::PageDown;

  case XK_Up: return Keyboard::Keycode::Up;
  case XK_Down: return Keyboard::Keycode::Down;
  case XK_Left: return Keyboard::Keycode::Left;
  case XK_Right: return Keyboard::Keycode::Right;

  case XK_grave: return Keyboard::Keycode::Grave;
  case XK_1: return Keyboard::Keycode::Number1;
  case XK_2: return Keyboard::Keycode::Number2;
  case XK_3: return Keyboard::Keycode::Number3;
  case XK_4: return Keyboard::Keycode::Number4;
  case XK_5: return Keyboard::Keycode::Number5;
  case XK_6: return Keyboard::Keycode::Number6;
  case XK_7: return Keyboard::Keycode::Number7;
  case XK_8: return Keyboard::Keycode::Number8;
  case XK_9: return Keyboard::Keycode::Number9;
  case XK_0: return Keyboard::Keycode::Number0;
  case XK_minus: return Keyboard::Keycode::Minus;
  case XK_equal: return Keyboard::Keycode::Equal;
  case XK_BackSpace: return Keyboard::Keycode::Backspace;

  case XK_asciitilde: return Keyboard::Keycode::Tilde;
  case XK_exclam: return Keyboard::Keycode::Exclamation;
  case XK_at: return Keyboard::Keycode::At;
  case XK_numbersign: return Keyboard::Keycode::Pound;
  case XK_dollar: return Keyboard::Keycode::Dollar;
  case XK_percent: return Keyboard::Keycode::Percent;
  case XK_asciicircum: return Keyboard::Keycode::Power;
  case XK_ampersand: return Keyboard::Keycode::Ampersand;
  case XK_asterisk: return Keyboard::Keycode::Asterisk;
  case XK_parenleft: return Keyboard::Keycode::ParenthesisLeft;
  case XK_parenright: return Keyboard::Keycode::ParenthesisRight;
  case XK_underscore: return Keyboard::Keycode::Underscore;
  case XK_plus: return Keyboard::Keycode::Plus;

  case XK_bracketleft: return Keyboard::Keycode::BracketLeft;
  case XK_bracketright: return Keyboard::Keycode::BracketRight;
  case XK_backslash: return Keyboard::Keycode::Backslash;
  case XK_semicolon: return Keyboard::Keycode::Semicolon;
  case XK_apostrophe: return Keyboard::Keycode::Apostrophe;
  case XK_comma: return Keyboard::Keycode::Comma;
  case XK_period: return Keyboard::Keycode::Period;
  case XK_slash: return Keyboard::Keycode::Slash;

  case XK_braceleft: return Keyboard::Keycode::BraceLeft;
  case XK_braceright: return Keyboard::Keycode::BraceRight;
  case XK_bar: return Keyboard::Keycode::Pipe;
  case XK_colon: return Keyboard::Keycode::Colon;
  case XK_quotedbl: return Keyboard::Keycode::Quote;
  case XK_less: return Keyboard::Keycode::CaretLeft;
  case XK_greater: return Keyboard::Keycode::CaretRight;
  case XK_question: return Keyboard::Keycode::Question;

  case XK_Tab: return Keyboard::Keycode::Tab;
  case XK_Caps_Lock: return Keyboard::Keycode::CapsLock;
  case XK_Return: return Keyboard::Keycode::Return;
  case XK_Shift_L: return Keyboard::Keycode::ShiftLeft;
  case XK_Shift_R: return Keyboard::Keycode::ShiftRight;
  case XK_Control_L: return Keyboard::Keycode::ControlLeft;
  case XK_Control_R: return Keyboard::Keycode::ControlRight;
  case XK_Super_L: return Keyboard::Keycode::SuperLeft;
  case XK_Super_R: return Keyboard::Keycode::SuperRight;
  case XK_Alt_L: return Keyboard::Keycode::AltLeft;
  case XK_Alt_R: return Keyboard::Keycode::AltRight;
  case XK_space: return Keyboard::Keycode::Space;
  case XK_Menu: return Keyboard::Keycode::Menu;

  case XK_A: return Keyboard::Keycode::A;
  case XK_B: return Keyboard::Keycode::B;
  case XK_C: return Keyboard::Keycode::C;
  case XK_D: return Keyboard::Keycode::D;
  case XK_E: return Keyboard::Keycode::E;
  case XK_F: return Keyboard::Keycode::F;
  case XK_G: return Keyboard::Keycode::G;
  case XK_H: return Keyboard::Keycode::H;
  case XK_I: return Keyboard::Keycode::I;
  case XK_J: return Keyboard::Keycode::J;
  case XK_K: return Keyboard::Keycode::K;
  case XK_L: return Keyboard::Keycode::L;
  case XK_M: return Keyboard::Keycode::M;
  case XK_N: return Keyboard::Keycode::N;
  case XK_O: return Keyboard::Keycode::O;
  case XK_P: return Keyboard::Keycode::P;
  case XK_Q: return Keyboard::Keycode::Q;
  case XK_R: return Keyboard::Keycode::R;
  case XK_S: return Keyboard::Keycode::S;
  case XK_T: return Keyboard::Keycode::T;
  case XK_U: return Keyboard::Keycode::U;
  case XK_V: return Keyboard::Keycode::V;
  case XK_W: return Keyboard::Keycode::W;
  case XK_X: return Keyboard::Keycode::X;
  case XK_Y: return Keyboard::Keycode::Y;
  case XK_Z: return Keyboard::Keycode::Z;

  case XK_a: return Keyboard::Keycode::a;
  case XK_b: return Keyboard::Keycode::b;
  case XK_c: return Keyboard::Keycode::c;
  case XK_d: return Keyboard::Keycode::d;
  case XK_e: return Keyboard::Keycode::e;
  case XK_f: return Keyboard::Keycode::f;
  case XK_g: return Keyboard::Keycode::g;
  case XK_h: return Keyboard::Keycode::h;
  case XK_i: return Keyboard::Keycode::i;
  case XK_j: return Keyboard::Keycode::j;
  case XK_k: return Keyboard::Keycode::k;
  case XK_l: return Keyboard::Keycode::l;
  case XK_m: return Keyboard::Keycode::m;
  case XK_n: return Keyboard::Keycode::n;
  case XK_o: return Keyboard::Keycode::o;
  case XK_p: return Keyboard::Keycode::p;
  case XK_q: return Keyboard::Keycode::q;
  case XK_r: return Keyboard::Keycode::r;
  case XK_s: return Keyboard::Keycode::s;
  case XK_t: return Keyboard::Keycode::t;
  case XK_u: return Keyboard::Keycode::u;
  case XK_v: return Keyboard::Keycode::v;
  case XK_w: return Keyboard::Keycode::w;
  case XK_x: return Keyboard::Keycode::x;
  case XK_y: return Keyboard::Keycode::y;
  case XK_z: return Keyboard::Keycode::z;

  case XK_Num_Lock: return Keyboard::Keycode::NumLock;
  case XK_KP_Divide: return Keyboard::Keycode::Divide;
  case XK_KP_Multiply: return Keyboard::Keycode::Multiply;
  case XK_KP_Subtract: return Keyboard::Keycode::Subtract;
  case XK_KP_Add: return Keyboard::Keycode::Add;
  case XK_KP_Enter: return Keyboard::Keycode::Enter;
  case XK_KP_Decimal: return Keyboard::Keycode::Point;

  case XK_KP_1: return Keyboard::Keycode::Keypad1;
  case XK_KP_2: return Keyboard::Keycode::Keypad2;
  case XK_KP_3: return Keyboard::Keycode::Keypad3;
  case XK_KP_4: return Keyboard::Keycode::Keypad4;
  case XK_KP_5: return Keyboard::Keycode::Keypad5;
  case XK_KP_6: return Keyboard::Keycode::Keypad6;
  case XK_KP_7: return Keyboard::Keycode::Keypad7;
  case XK_KP_8: return Keyboard::Keycode::Keypad8;
  case XK_KP_9: return Keyboard::Keycode::Keypad9;
  case XK_KP_0: return Keyboard::Keycode::Keypad0;

  case XK_KP_Home: return Keyboard::Keycode::KeypadHome;
  case XK_KP_End: return Keyboard::Keycode::KeypadEnd;
  case XK_KP_Page_Up: return Keyboard::Keycode::KeypadPageUp;
  case XK_KP_Page_Down: return Keyboard::Keycode::KeypadPageDown;
  case XK_KP_Up: return Keyboard::Keycode::KeypadUp;
  case XK_KP_Down: return Keyboard::Keycode::KeypadDown;
  case XK_KP_Left: return Keyboard::Keycode::KeypadLeft;
  case XK_KP_Right: return Keyboard::Keycode::KeypadRight;
  case XK_KP_Begin: return Keyboard::Keycode::KeypadCenter;
  case XK_KP_Insert: return Keyboard::Keycode::KeypadInsert;
  case XK_KP_Delete: return Keyboard::Keycode::KeypadDelete;
  }
  return Keyboard::Keycode::None;
}
