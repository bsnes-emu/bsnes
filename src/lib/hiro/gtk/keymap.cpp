uint16_t pHiro::translate_key(uint key) {
  switch(key) {
    case GDK_Escape: return keyboard::escape;

    case GDK_F1:  return keyboard::f1;
    case GDK_F2:  return keyboard::f2;
    case GDK_F3:  return keyboard::f3;
    case GDK_F4:  return keyboard::f4;
    case GDK_F5:  return keyboard::f5;
    case GDK_F6:  return keyboard::f6;
    case GDK_F7:  return keyboard::f7;
    case GDK_F8:  return keyboard::f8;
    case GDK_F9:  return keyboard::f9;
    case GDK_F10: return keyboard::f10;
    case GDK_F11: return keyboard::f11;
    case GDK_F12: return keyboard::f12;

    case GDK_Print:       return keyboard::print_screen;
    case GDK_Sys_Req:     return keyboard::print_screen;
    case GDK_Scroll_Lock: return keyboard::scroll_lock;
    case GDK_Pause:       return keyboard::pause;
    case GDK_Break:       return keyboard::pause;

    case GDK_grave:      return keyboard::tilde;
    case GDK_asciitilde: return keyboard::tilde;

    case GDK_1: return keyboard::num_1;
    case GDK_2: return keyboard::num_2;
    case GDK_3: return keyboard::num_3;
    case GDK_4: return keyboard::num_4;
    case GDK_5: return keyboard::num_5;
    case GDK_6: return keyboard::num_6;
    case GDK_7: return keyboard::num_7;
    case GDK_8: return keyboard::num_8;
    case GDK_9: return keyboard::num_9;
    case GDK_0: return keyboard::num_0;

    case GDK_exclam:      return keyboard::num_1;
    case GDK_at:          return keyboard::num_2;
    case GDK_numbersign:  return keyboard::num_3;
    case GDK_dollar:      return keyboard::num_4;
    case GDK_percent:     return keyboard::num_5;
    case GDK_asciicircum: return keyboard::num_6;
    case GDK_ampersand:   return keyboard::num_7;
    case GDK_asterisk:    return keyboard::num_8;
    case GDK_parenleft:   return keyboard::num_9;
    case GDK_parenright:  return keyboard::num_0;

    case GDK_minus:      return keyboard::dash;
    case GDK_underscore: return keyboard::dash;
    case GDK_equal:      return keyboard::equal;
    case GDK_plus:       return keyboard::equal;
    case GDK_BackSpace:  return keyboard::backspace;

    case GDK_Insert:    return keyboard::insert;
    case GDK_Delete:    return keyboard::delete_;
    case GDK_Home:      return keyboard::home;
    case GDK_End:       return keyboard::end;
    case GDK_Page_Up:   return keyboard::page_up;
    case GDK_Page_Down: return keyboard::page_down;

    case GDK_a: return keyboard::a;
    case GDK_b: return keyboard::b;
    case GDK_c: return keyboard::c;
    case GDK_d: return keyboard::d;
    case GDK_e: return keyboard::e;
    case GDK_f: return keyboard::f;
    case GDK_g: return keyboard::g;
    case GDK_h: return keyboard::h;
    case GDK_i: return keyboard::i;
    case GDK_j: return keyboard::j;
    case GDK_k: return keyboard::k;
    case GDK_l: return keyboard::l;
    case GDK_m: return keyboard::m;
    case GDK_n: return keyboard::n;
    case GDK_o: return keyboard::o;
    case GDK_p: return keyboard::p;
    case GDK_q: return keyboard::q;
    case GDK_r: return keyboard::r;
    case GDK_s: return keyboard::s;
    case GDK_t: return keyboard::t;
    case GDK_u: return keyboard::u;
    case GDK_v: return keyboard::v;
    case GDK_w: return keyboard::w;
    case GDK_x: return keyboard::x;
    case GDK_y: return keyboard::y;
    case GDK_z: return keyboard::z;

    case GDK_A: return keyboard::a;
    case GDK_B: return keyboard::b;
    case GDK_C: return keyboard::c;
    case GDK_D: return keyboard::d;
    case GDK_E: return keyboard::e;
    case GDK_F: return keyboard::f;
    case GDK_G: return keyboard::g;
    case GDK_H: return keyboard::h;
    case GDK_I: return keyboard::i;
    case GDK_J: return keyboard::j;
    case GDK_K: return keyboard::k;
    case GDK_L: return keyboard::l;
    case GDK_M: return keyboard::m;
    case GDK_N: return keyboard::n;
    case GDK_O: return keyboard::o;
    case GDK_P: return keyboard::p;
    case GDK_Q: return keyboard::q;
    case GDK_R: return keyboard::r;
    case GDK_S: return keyboard::s;
    case GDK_T: return keyboard::t;
    case GDK_U: return keyboard::u;
    case GDK_V: return keyboard::v;
    case GDK_W: return keyboard::w;
    case GDK_X: return keyboard::x;
    case GDK_Y: return keyboard::y;
    case GDK_Z: return keyboard::z;

    case GDK_bracketleft:  return keyboard::lbracket;
    case GDK_bracketright: return keyboard::rbracket;
    case GDK_backslash:    return keyboard::backslash;
    case GDK_semicolon:    return keyboard::semicolon;
    case GDK_apostrophe:   return keyboard::apostrophe;
    case GDK_comma:        return keyboard::comma;
    case GDK_period:       return keyboard::period;
    case GDK_slash:        return keyboard::slash;

    case GDK_braceleft:  return keyboard::lbracket;
    case GDK_braceright: return keyboard::rbracket;
    case GDK_bar:        return keyboard::backslash;
    case GDK_colon:      return keyboard::semicolon;
    case GDK_quotedbl:   return keyboard::apostrophe;
    case GDK_less:       return keyboard::comma;
    case GDK_greater:    return keyboard::period;
    case GDK_question:   return keyboard::slash;

    case GDK_KP_1:       return keyboard::pad_1;
    case GDK_KP_2:       return keyboard::pad_2;
    case GDK_KP_3:       return keyboard::pad_3;
    case GDK_KP_4:       return keyboard::pad_4;
    case GDK_KP_5:       return keyboard::pad_5;
    case GDK_KP_6:       return keyboard::pad_6;
    case GDK_KP_7:       return keyboard::pad_7;
    case GDK_KP_8:       return keyboard::pad_8;
    case GDK_KP_9:       return keyboard::pad_9;
    case GDK_KP_0:       return keyboard::pad_0;
    case GDK_KP_Decimal: return keyboard::point;

    case GDK_KP_End:       return keyboard::pad_1;
    case GDK_KP_Down:      return keyboard::pad_2;
    case GDK_KP_Page_Down: return keyboard::pad_3;
    case GDK_KP_Left:      return keyboard::pad_4;
    case GDK_KP_Begin:     return keyboard::pad_5;
    case GDK_KP_Right:     return keyboard::pad_6;
    case GDK_KP_Home:      return keyboard::pad_7;
    case GDK_KP_Up:        return keyboard::pad_8;
    case GDK_KP_Page_Up:   return keyboard::pad_9;
    case GDK_KP_Insert:    return keyboard::pad_0;
    case GDK_KP_Delete:    return keyboard::point;

    case GDK_KP_Add:      return keyboard::add;
    case GDK_KP_Subtract: return keyboard::subtract;
    case GDK_KP_Multiply: return keyboard::multiply;
    case GDK_KP_Divide:   return keyboard::divide;
    case GDK_KP_Enter:    return keyboard::enter;

    case GDK_Num_Lock:    return keyboard::num_lock;
    case GDK_Caps_Lock:   return keyboard::caps_lock;

    case GDK_Up:          return keyboard::up;
    case GDK_Down:        return keyboard::down;
    case GDK_Left:        return keyboard::left;
    case GDK_Right:       return keyboard::right;

    case GDK_Tab:         return keyboard::tab;
    case GDK_Return:      return keyboard::return_;
    case GDK_space:       return keyboard::spacebar;

    case GDK_Control_L:   return keyboard::lctrl;
    case GDK_Control_R:   return keyboard::rctrl;
    case GDK_Alt_L:       return keyboard::lalt;
    case GDK_Alt_R:       return keyboard::ralt;
    case GDK_Shift_L:     return keyboard::lshift;
    case GDK_Shift_R:     return keyboard::rshift;
    case GDK_Super_L:     return keyboard::lsuper;
    case GDK_Super_R:     return keyboard::rsuper;
    case GDK_Menu:        return keyboard::menu;
  }

  return keyboard::none;
}
