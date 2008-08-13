uint16_t pHiro::translate_key(unsigned key) {
  switch(key) {
    case VK_ESCAPE: return keyboard::escape;

    case VK_F1:  return keyboard::f1;
    case VK_F2:  return keyboard::f2;
    case VK_F3:  return keyboard::f3;
    case VK_F4:  return keyboard::f4;
    case VK_F5:  return keyboard::f5;
    case VK_F6:  return keyboard::f6;
    case VK_F7:  return keyboard::f7;
    case VK_F8:  return keyboard::f8;
    case VK_F9:  return keyboard::f9;
    case VK_F10: return keyboard::f10;
    case VK_F11: return keyboard::f11;
    case VK_F12: return keyboard::f12;

    case VK_TAB:    return keyboard::tab;
    case VK_RETURN: return keyboard::return_;
    case VK_SPACE:  return keyboard::spacebar;

    case '0': return keyboard::num_0;
    case '1': return keyboard::num_1;
    case '2': return keyboard::num_2;
    case '3': return keyboard::num_3;
    case '4': return keyboard::num_4;
    case '5': return keyboard::num_5;
    case '6': return keyboard::num_6;
    case '7': return keyboard::num_7;
    case '8': return keyboard::num_8;
    case '9': return keyboard::num_9;

    case VK_INSERT: return keyboard::insert;
    case VK_DELETE: return keyboard::delete_;
    case VK_HOME:   return keyboard::home;
    case VK_END:    return keyboard::end;
    case VK_PRIOR:  return keyboard::page_up;
    case VK_NEXT:   return keyboard::page_down;

    case 'A': return keyboard::a;
    case 'B': return keyboard::b;
    case 'C': return keyboard::c;
    case 'D': return keyboard::d;
    case 'E': return keyboard::e;
    case 'F': return keyboard::f;
    case 'G': return keyboard::g;
    case 'H': return keyboard::h;
    case 'I': return keyboard::i;
    case 'J': return keyboard::j;
    case 'K': return keyboard::k;
    case 'L': return keyboard::l;
    case 'M': return keyboard::m;
    case 'N': return keyboard::n;
    case 'O': return keyboard::o;
    case 'P': return keyboard::p;
    case 'Q': return keyboard::q;
    case 'R': return keyboard::r;
    case 'S': return keyboard::s;
    case 'T': return keyboard::t;
    case 'U': return keyboard::u;
    case 'V': return keyboard::v;
    case 'W': return keyboard::w;
    case 'X': return keyboard::x;
    case 'Y': return keyboard::y;
    case 'Z': return keyboard::z;

    case VK_UP:    return keyboard::up;
    case VK_DOWN:  return keyboard::down;
    case VK_LEFT:  return keyboard::left;
    case VK_RIGHT: return keyboard::right;
  }

  return keyboard::none;
}
