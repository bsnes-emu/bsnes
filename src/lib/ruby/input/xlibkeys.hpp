//shared keycode lookup table + initialization routine:
//#include inside a class interface to use

//Xlib keycodes for each key can vary between platforms, so this header file
//will lookup keycodes from static keysyms, and map them to nall/input.hpp's
//keyboard identifiers.
//
//this allows input capture routine to iterate quickly over all keycodes and
//map their states to ruby's input state table.

uint8_t keycode[256];

bool init_keycodes() {
  Display *display = XOpenDisplay(0);
  memset(&keycode, 0, sizeof keycode);

  #define assign(x, y) keycode[x] = XKeysymToKeycode(display, y)
  assign(keyboard::escape, XK_Escape);

  assign(keyboard::f1, XK_F1);
  assign(keyboard::f2, XK_F2);
  assign(keyboard::f3, XK_F3);
  assign(keyboard::f4, XK_F4);
  assign(keyboard::f5, XK_F5);
  assign(keyboard::f6, XK_F6);
  assign(keyboard::f7, XK_F7);
  assign(keyboard::f8, XK_F8);
  assign(keyboard::f9, XK_F9);
  assign(keyboard::f10, XK_F10);
  assign(keyboard::f11, XK_F11);
  assign(keyboard::f12, XK_F12);

  //assign(keyboard::print_screen, XK_???);
  assign(keyboard::scroll_lock, XK_Scroll_Lock);
  assign(keyboard::pause, XK_Pause);

  assign(keyboard::tilde, XK_asciitilde);

  assign(keyboard::num_0, XK_0);
  assign(keyboard::num_1, XK_1);
  assign(keyboard::num_2, XK_2);
  assign(keyboard::num_3, XK_3);
  assign(keyboard::num_4, XK_4);
  assign(keyboard::num_5, XK_5);
  assign(keyboard::num_6, XK_6);
  assign(keyboard::num_7, XK_7);
  assign(keyboard::num_8, XK_8);
  assign(keyboard::num_9, XK_9);

  assign(keyboard::dash, XK_minus);
  assign(keyboard::equal, XK_equal);
  assign(keyboard::backspace, XK_BackSpace);

  assign(keyboard::insert, XK_Insert);
  assign(keyboard::delete_, XK_Delete);
  assign(keyboard::home, XK_Home);
  assign(keyboard::end, XK_End);
  assign(keyboard::page_up, XK_Prior);
  assign(keyboard::page_down, XK_Next);

  assign(keyboard::a, XK_A);
  assign(keyboard::b, XK_B);
  assign(keyboard::c, XK_C);
  assign(keyboard::d, XK_D);
  assign(keyboard::e, XK_E);
  assign(keyboard::f, XK_F);
  assign(keyboard::g, XK_G);
  assign(keyboard::h, XK_H);
  assign(keyboard::i, XK_I);
  assign(keyboard::j, XK_J);
  assign(keyboard::k, XK_K);
  assign(keyboard::l, XK_L);
  assign(keyboard::m, XK_M);
  assign(keyboard::n, XK_N);
  assign(keyboard::o, XK_O);
  assign(keyboard::p, XK_P);
  assign(keyboard::q, XK_Q);
  assign(keyboard::r, XK_R);
  assign(keyboard::s, XK_S);
  assign(keyboard::t, XK_T);
  assign(keyboard::u, XK_U);
  assign(keyboard::v, XK_V);
  assign(keyboard::w, XK_W);
  assign(keyboard::x, XK_X);
  assign(keyboard::y, XK_Y);
  assign(keyboard::z, XK_Z);

  assign(keyboard::lbracket, XK_bracketleft);
  assign(keyboard::rbracket, XK_bracketright);
  assign(keyboard::backslash, XK_backslash);
  assign(keyboard::semicolon, XK_semicolon);
  assign(keyboard::apostrophe, XK_apostrophe);
  assign(keyboard::comma, XK_comma);
  assign(keyboard::period, XK_period);
  assign(keyboard::slash, XK_slash);

  assign(keyboard::pad_0, XK_KP_0);
  assign(keyboard::pad_1, XK_KP_1);
  assign(keyboard::pad_2, XK_KP_2);
  assign(keyboard::pad_3, XK_KP_3);
  assign(keyboard::pad_4, XK_KP_4);
  assign(keyboard::pad_5, XK_KP_5);
  assign(keyboard::pad_6, XK_KP_6);
  assign(keyboard::pad_7, XK_KP_7);
  assign(keyboard::pad_8, XK_KP_8);
  assign(keyboard::pad_9, XK_KP_9);

  assign(keyboard::add, XK_KP_Add);
  assign(keyboard::subtract, XK_KP_Subtract);
  assign(keyboard::multiply, XK_KP_Multiply);
  assign(keyboard::divide, XK_KP_Divide);
  assign(keyboard::enter, XK_KP_Enter);

  //assign(keyboard::num_lock, XK_???);
  //assign(keyboard::caps_lock, XK_???);

  assign(keyboard::up, XK_Up);
  assign(keyboard::down, XK_Down);
  assign(keyboard::left, XK_Left);
  assign(keyboard::right, XK_Right);

  assign(keyboard::tab, XK_Tab);
  assign(keyboard::return_, XK_Return);
  assign(keyboard::spacebar, XK_space);

  assign(keyboard::lctrl, XK_Control_L);
  assign(keyboard::rctrl, XK_Control_R);
  assign(keyboard::lalt, XK_Alt_L);
  assign(keyboard::ralt, XK_Alt_R);
  assign(keyboard::lshift, XK_Shift_L);
  assign(keyboard::rshift, XK_Shift_R);
  assign(keyboard::lsuper, XK_Super_L);
  assign(keyboard::rsuper, XK_Super_R);
  assign(keyboard::menu, XK_Menu);
  #undef assign

  XCloseDisplay(display);
}
