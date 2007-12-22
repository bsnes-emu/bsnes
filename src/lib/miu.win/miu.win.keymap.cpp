uint16 pMiu::translate_key(uint key) {
  switch(key) {
    case VK_ESCAPE: return keymap::esc;

    case VK_F1:     return keymap::f1;
    case VK_F2:     return keymap::f2;
    case VK_F3:     return keymap::f3;
    case VK_F4:     return keymap::f4;
    case VK_F5:     return keymap::f5;
    case VK_F6:     return keymap::f6;
    case VK_F7:     return keymap::f7;
    case VK_F8:     return keymap::f8;
    case VK_F9:     return keymap::f9;
    case VK_F10:    return keymap::f10;
    case VK_F11:    return keymap::f11;
    case VK_F12:    return keymap::f12;

    case VK_TAB:    return keymap::tab;
    case VK_RETURN: return keymap::enter;
    case VK_SPACE:  return keymap::space;

    case '0':       return keymap::num_0;
    case '1':       return keymap::num_1;
    case '2':       return keymap::num_2;
    case '3':       return keymap::num_3;
    case '4':       return keymap::num_4;
    case '5':       return keymap::num_5;
    case '6':       return keymap::num_6;
    case '7':       return keymap::num_7;
    case '8':       return keymap::num_8;
    case '9':       return keymap::num_9;

    case 'A':       return keymap::a;
    case 'B':       return keymap::b;
    case 'C':       return keymap::c;
    case 'D':       return keymap::d;
    case 'E':       return keymap::e;
    case 'F':       return keymap::f;
    case 'G':       return keymap::g;
    case 'H':       return keymap::h;
    case 'I':       return keymap::i;
    case 'J':       return keymap::j;
    case 'K':       return keymap::k;
    case 'L':       return keymap::l;
    case 'M':       return keymap::m;
    case 'N':       return keymap::n;
    case 'O':       return keymap::o;
    case 'P':       return keymap::p;
    case 'Q':       return keymap::q;
    case 'R':       return keymap::r;
    case 'S':       return keymap::s;
    case 'T':       return keymap::t;
    case 'U':       return keymap::u;
    case 'V':       return keymap::v;
    case 'W':       return keymap::w;
    case 'X':       return keymap::x;
    case 'Y':       return keymap::y;
    case 'Z':       return keymap::z;

    case VK_UP:     return keymap::up;
    case VK_DOWN:   return keymap::down;
    case VK_LEFT:   return keymap::left;
    case VK_RIGHT:  return keymap::right;
  }

  return keymap::none;
}
