uint16 pMiu::translate_key(uint key) {
  switch(key) {
    case GDK_Escape: return keymap::esc;

    case GDK_F1:  return keymap::f1;
    case GDK_F2:  return keymap::f2;
    case GDK_F3:  return keymap::f3;
    case GDK_F4:  return keymap::f4;
    case GDK_F5:  return keymap::f5;
    case GDK_F6:  return keymap::f6;
    case GDK_F7:  return keymap::f7;
    case GDK_F8:  return keymap::f8;
    case GDK_F9:  return keymap::f9;
    case GDK_F10: return keymap::f10;
    case GDK_F11: return keymap::f11;
    case GDK_F12: return keymap::f12;

    case GDK_Print:       return keymap::print_screen;
    case GDK_Sys_Req:     return keymap::sys_req;
    case GDK_Scroll_Lock: return keymap::scroll_lock;
    case GDK_Pause:       return keymap::pause;
    case GDK_Break:       return keymap::brk;

    case GDK_grave:      return keymap::grave;
    case GDK_asciitilde: return keymap::tilde;

    case GDK_1: return keymap::num_1;
    case GDK_2: return keymap::num_2;
    case GDK_3: return keymap::num_3;
    case GDK_4: return keymap::num_4;
    case GDK_5: return keymap::num_5;
    case GDK_6: return keymap::num_6;
    case GDK_7: return keymap::num_7;
    case GDK_8: return keymap::num_8;
    case GDK_9: return keymap::num_9;
    case GDK_0: return keymap::num_0;

    case GDK_exclam:      return keymap::exclamation;
    case GDK_at:          return keymap::at;
    case GDK_numbersign:  return keymap::pound;
    case GDK_dollar:      return keymap::dollar;
    case GDK_percent:     return keymap::percent;
    case GDK_asciicircum: return keymap::power;
    case GDK_ampersand:   return keymap::ampersand;
    case GDK_asterisk:    return keymap::asterisk;
    case GDK_parenleft:   return keymap::lparenthesis;
    case GDK_parenright:  return keymap::rparenthesis;

    case GDK_minus:      return keymap::minus;
    case GDK_underscore: return keymap::underscore;
    case GDK_equal:      return keymap::equal;
    case GDK_plus:       return keymap::plus;
    case GDK_BackSpace:  return keymap::backspace;

    case GDK_Insert:    return keymap::ins;
    case GDK_Delete:    return keymap::del;
    case GDK_Home:      return keymap::home;
    case GDK_End:       return keymap::end;
    case GDK_Page_Up:   return keymap::page_up;
    case GDK_Page_Down: return keymap::page_down;

    case GDK_a: return keymap::a;
    case GDK_b: return keymap::b;
    case GDK_c: return keymap::c;
    case GDK_d: return keymap::d;
    case GDK_e: return keymap::e;
    case GDK_f: return keymap::f;
    case GDK_g: return keymap::g;
    case GDK_h: return keymap::h;
    case GDK_i: return keymap::i;
    case GDK_j: return keymap::j;
    case GDK_k: return keymap::k;
    case GDK_l: return keymap::l;
    case GDK_m: return keymap::m;
    case GDK_n: return keymap::n;
    case GDK_o: return keymap::o;
    case GDK_p: return keymap::p;
    case GDK_q: return keymap::q;
    case GDK_r: return keymap::r;
    case GDK_s: return keymap::s;
    case GDK_t: return keymap::t;
    case GDK_u: return keymap::u;
    case GDK_v: return keymap::v;
    case GDK_w: return keymap::w;
    case GDK_x: return keymap::x;
    case GDK_y: return keymap::y;
    case GDK_z: return keymap::z;

    case GDK_A: return keymap::A;
    case GDK_B: return keymap::B;
    case GDK_C: return keymap::C;
    case GDK_D: return keymap::D;
    case GDK_E: return keymap::E;
    case GDK_F: return keymap::F;
    case GDK_G: return keymap::G;
    case GDK_H: return keymap::H;
    case GDK_I: return keymap::I;
    case GDK_J: return keymap::J;
    case GDK_K: return keymap::K;
    case GDK_L: return keymap::L;
    case GDK_M: return keymap::M;
    case GDK_N: return keymap::N;
    case GDK_O: return keymap::O;
    case GDK_P: return keymap::P;
    case GDK_Q: return keymap::Q;
    case GDK_R: return keymap::R;
    case GDK_S: return keymap::S;
    case GDK_T: return keymap::T;
    case GDK_U: return keymap::U;
    case GDK_V: return keymap::V;
    case GDK_W: return keymap::W;
    case GDK_X: return keymap::X;
    case GDK_Y: return keymap::Y;
    case GDK_Z: return keymap::Z;

    case GDK_bracketleft:  return keymap::lbracket;
    case GDK_bracketright: return keymap::rbracket;
    case GDK_backslash:    return keymap::backslash;
    case GDK_semicolon:    return keymap::semicolon;
    case GDK_apostrophe:   return keymap::apostrophe;
    case GDK_comma:        return keymap::comma;
    case GDK_period:       return keymap::period;
    case GDK_slash:        return keymap::slash;

    case GDK_braceleft:  return keymap::lbrace;
    case GDK_braceright: return keymap::rbrace;
    case GDK_bar:        return keymap::pipe;
    case GDK_colon:      return keymap::colon;
    case GDK_quotedbl:   return keymap::quote;
    case GDK_less:       return keymap::lcaret;
    case GDK_greater:    return keymap::rcaret;
    case GDK_question:   return keymap::question;

    case GDK_KP_1:       return keymap::kp_1;
    case GDK_KP_2:       return keymap::kp_2;
    case GDK_KP_3:       return keymap::kp_3;
    case GDK_KP_4:       return keymap::kp_4;
    case GDK_KP_5:       return keymap::kp_5;
    case GDK_KP_6:       return keymap::kp_6;
    case GDK_KP_7:       return keymap::kp_7;
    case GDK_KP_8:       return keymap::kp_8;
    case GDK_KP_9:       return keymap::kp_9;
    case GDK_KP_0:       return keymap::kp_0;
    case GDK_KP_Decimal: return keymap::kp_decimal;

    case GDK_KP_End:       return keymap::kp_end;
    case GDK_KP_Down:      return keymap::kp_down;
    case GDK_KP_Page_Down: return keymap::kp_page_down;
    case GDK_KP_Left:      return keymap::kp_left;
    case GDK_KP_Begin:     return keymap::kp_center;
    case GDK_KP_Right:     return keymap::kp_right;
    case GDK_KP_Home:      return keymap::kp_home;
    case GDK_KP_Up:        return keymap::kp_up;
    case GDK_KP_Page_Up:   return keymap::kp_page_up;
    case GDK_KP_Insert:    return keymap::kp_insert;
    case GDK_KP_Delete:    return keymap::kp_delete;

    case GDK_KP_Add:      return keymap::kp_plus;
    case GDK_KP_Subtract: return keymap::kp_minus;
    case GDK_KP_Multiply: return keymap::kp_mul;
    case GDK_KP_Divide:   return keymap::kp_div;
    case GDK_KP_Enter:    return keymap::kp_enter;

    case GDK_Num_Lock:    return keymap::num_lock;
    case GDK_Caps_Lock:   return keymap::caps_lock;

    case GDK_Up:          return keymap::up;
    case GDK_Down:        return keymap::down;
    case GDK_Left:        return keymap::left;
    case GDK_Right:       return keymap::right;

    case GDK_Tab:         return keymap::tab;
    case GDK_Return:      return keymap::enter;
    case GDK_space:       return keymap::space;

    case GDK_Control_L:   return keymap::lctrl;
    case GDK_Control_R:   return keymap::rctrl;
    case GDK_Alt_L:       return keymap::lalt;
    case GDK_Alt_R:       return keymap::ralt;
    case GDK_Shift_L:     return keymap::lshift;
    case GDK_Shift_R:     return keymap::rshift;
    case GDK_Super_L:     return keymap::lsuper;
    case GDK_Super_R:     return keymap::rsuper;
    case GDK_Menu:        return keymap::menu;
  }

  return keymap::none;
}
