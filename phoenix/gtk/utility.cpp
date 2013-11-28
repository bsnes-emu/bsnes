namespace phoenix {

static GdkColor CreateColor(uint8_t r, uint8_t g, uint8_t b) {
  GdkColor color;
  color.pixel = (r << 16) | (g << 8) | (b << 0);
  color.red = (r << 8) | (r << 0);
  color.green = (g << 8) | (g << 0);
  color.blue = (b << 8) | (b << 0);
  return color;
}

static GdkPixbuf* CreatePixbuf(const nall::image& image, bool scale = false) {
  nall::image gdkImage = image;
  gdkImage.transform(0, 32, 255u << 24, 255u << 0, 255u << 8, 255u << 16);
  if(scale) gdkImage.scale(15, 15);

  GdkPixbuf* pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, gdkImage.width, gdkImage.height);
  memcpy(gdk_pixbuf_get_pixels(pixbuf), gdkImage.data, gdkImage.width * gdkImage.height * 4);

  return pixbuf;
}

static GtkImage* CreateImage(const nall::image& image, bool scale = false) {
  GdkPixbuf* pixbuf = CreatePixbuf(image, scale);
  GtkImage* gtkImage = (GtkImage*)gtk_image_new_from_pixbuf(pixbuf);
  g_object_unref(pixbuf);
  return gtkImage;
}

static lstring DropPaths(GtkSelectionData* data) {
  gchar** uris = gtk_selection_data_get_uris(data);
  if(uris == nullptr) return {};

  lstring paths;
  for(unsigned n = 0; uris[n] != nullptr; n++) {
    gchar* pathname = g_filename_from_uri(uris[n], nullptr, nullptr);
    if(pathname == nullptr) continue;

    string path = pathname;
    g_free(pathname);
    if(directory::exists(path) && !path.endswith("/")) path.append("/");
    paths.append(path);
  }

  g_strfreev(uris);
  return paths;
}

static Position GetDisplacement(Sizable* sizable) {
  Position position;
  while(sizable->state.parent) {
    Position displacement = sizable->state.parent->p.displacement();
    position.x += displacement.x;
    position.y += displacement.y;
    sizable = sizable->state.parent;
  }
  return position;
}

static Layout* GetParentWidgetLayout(Sizable* sizable) {
  while(sizable) {
    if(sizable->state.parent && dynamic_cast<TabFrame*>(sizable->state.parent)) return (Layout*)sizable;
    sizable = sizable->state.parent;
  }
  return nullptr;
}

static Widget* GetParentWidget(Sizable* sizable) {
  while(sizable) {
    if(sizable->state.parent && dynamic_cast<TabFrame*>(sizable->state.parent)) return (Widget*)sizable->state.parent;
    sizable = sizable->state.parent;
  }
  return nullptr;
}

static Keyboard::Keycode Keysym(unsigned keysym) {
  switch(keysym) {
  case GDK_Escape: return Keyboard::Keycode::Escape;
  case GDK_F1: return Keyboard::Keycode::F1;
  case GDK_F2: return Keyboard::Keycode::F2;
  case GDK_F3: return Keyboard::Keycode::F3;
  case GDK_F4: return Keyboard::Keycode::F4;
  case GDK_F5: return Keyboard::Keycode::F5;
  case GDK_F6: return Keyboard::Keycode::F6;
  case GDK_F7: return Keyboard::Keycode::F7;
  case GDK_F8: return Keyboard::Keycode::F8;
  case GDK_F9: return Keyboard::Keycode::F9;
  case GDK_F10: return Keyboard::Keycode::F10;
  case GDK_F11: return Keyboard::Keycode::F11;
  case GDK_F12: return Keyboard::Keycode::F12;

  case GDK_Print: return Keyboard::Keycode::PrintScreen;
  //Keyboard::Keycode::SysRq
  case GDK_Scroll_Lock: return Keyboard::Keycode::ScrollLock;
  case GDK_Pause: return Keyboard::Keycode::Pause;
  //Keyboard::Keycode::Break

  case GDK_Insert: return Keyboard::Keycode::Insert;
  case GDK_Delete: return Keyboard::Keycode::Delete;
  case GDK_Home: return Keyboard::Keycode::Home;
  case GDK_End: return Keyboard::Keycode::End;
  case GDK_Prior: return Keyboard::Keycode::PageUp;
  case GDK_Next: return Keyboard::Keycode::PageDown;

  case GDK_Up: return Keyboard::Keycode::Up;
  case GDK_Down: return Keyboard::Keycode::Down;
  case GDK_Left: return Keyboard::Keycode::Left;
  case GDK_Right: return Keyboard::Keycode::Right;

  case GDK_grave: return Keyboard::Keycode::Grave;
  case GDK_1: return Keyboard::Keycode::Number1;
  case GDK_2: return Keyboard::Keycode::Number2;
  case GDK_3: return Keyboard::Keycode::Number3;
  case GDK_4: return Keyboard::Keycode::Number4;
  case GDK_5: return Keyboard::Keycode::Number5;
  case GDK_6: return Keyboard::Keycode::Number6;
  case GDK_7: return Keyboard::Keycode::Number7;
  case GDK_8: return Keyboard::Keycode::Number8;
  case GDK_9: return Keyboard::Keycode::Number9;
  case GDK_0: return Keyboard::Keycode::Number0;
  case GDK_minus: return Keyboard::Keycode::Minus;
  case GDK_equal: return Keyboard::Keycode::Equal;
  case GDK_BackSpace: return Keyboard::Keycode::Backspace;

  case GDK_asciitilde: return Keyboard::Keycode::Tilde;
  case GDK_exclam: return Keyboard::Keycode::Exclamation;
  case GDK_at: return Keyboard::Keycode::At;
  case GDK_numbersign: return Keyboard::Keycode::Pound;
  case GDK_dollar: return Keyboard::Keycode::Dollar;
  case GDK_percent: return Keyboard::Keycode::Percent;
  case GDK_asciicircum: return Keyboard::Keycode::Power;
  case GDK_ampersand: return Keyboard::Keycode::Ampersand;
  case GDK_asterisk: return Keyboard::Keycode::Asterisk;
  case GDK_parenleft: return Keyboard::Keycode::ParenthesisLeft;
  case GDK_parenright: return Keyboard::Keycode::ParenthesisRight;
  case GDK_underscore: return Keyboard::Keycode::Underscore;
  case GDK_plus: return Keyboard::Keycode::Plus;

  case GDK_bracketleft: return Keyboard::Keycode::BracketLeft;
  case GDK_bracketright: return Keyboard::Keycode::BracketRight;
  case GDK_backslash: return Keyboard::Keycode::Backslash;
  case GDK_semicolon: return Keyboard::Keycode::Semicolon;
  case GDK_apostrophe: return Keyboard::Keycode::Apostrophe;
  case GDK_comma: return Keyboard::Keycode::Comma;
  case GDK_period: return Keyboard::Keycode::Period;
  case GDK_slash: return Keyboard::Keycode::Slash;

  case GDK_braceleft: return Keyboard::Keycode::BraceLeft;
  case GDK_braceright: return Keyboard::Keycode::BraceRight;
  case GDK_bar: return Keyboard::Keycode::Pipe;
  case GDK_colon: return Keyboard::Keycode::Colon;
  case GDK_quotedbl: return Keyboard::Keycode::Quote;
  case GDK_less: return Keyboard::Keycode::CaretLeft;
  case GDK_greater: return Keyboard::Keycode::CaretRight;
  case GDK_question: return Keyboard::Keycode::Question;

  case GDK_Tab: return Keyboard::Keycode::Tab;
  case GDK_Caps_Lock: return Keyboard::Keycode::CapsLock;
  case GDK_Return: return Keyboard::Keycode::Return;
  case GDK_Shift_L: return Keyboard::Keycode::ShiftLeft;
  case GDK_Shift_R: return Keyboard::Keycode::ShiftRight;
  case GDK_Control_L: return Keyboard::Keycode::ControlLeft;
  case GDK_Control_R: return Keyboard::Keycode::ControlRight;
  case GDK_Super_L: return Keyboard::Keycode::SuperLeft;
  case GDK_Super_R: return Keyboard::Keycode::SuperRight;
  case GDK_Alt_L: return Keyboard::Keycode::AltLeft;
  case GDK_Alt_R: return Keyboard::Keycode::AltRight;
  case GDK_space: return Keyboard::Keycode::Space;
  case GDK_Menu: return Keyboard::Keycode::Menu;

  case GDK_A: return Keyboard::Keycode::A;
  case GDK_B: return Keyboard::Keycode::B;
  case GDK_C: return Keyboard::Keycode::C;
  case GDK_D: return Keyboard::Keycode::D;
  case GDK_E: return Keyboard::Keycode::E;
  case GDK_F: return Keyboard::Keycode::F;
  case GDK_G: return Keyboard::Keycode::G;
  case GDK_H: return Keyboard::Keycode::H;
  case GDK_I: return Keyboard::Keycode::I;
  case GDK_J: return Keyboard::Keycode::J;
  case GDK_K: return Keyboard::Keycode::K;
  case GDK_L: return Keyboard::Keycode::L;
  case GDK_M: return Keyboard::Keycode::M;
  case GDK_N: return Keyboard::Keycode::N;
  case GDK_O: return Keyboard::Keycode::O;
  case GDK_P: return Keyboard::Keycode::P;
  case GDK_Q: return Keyboard::Keycode::Q;
  case GDK_R: return Keyboard::Keycode::R;
  case GDK_S: return Keyboard::Keycode::S;
  case GDK_T: return Keyboard::Keycode::T;
  case GDK_U: return Keyboard::Keycode::U;
  case GDK_V: return Keyboard::Keycode::V;
  case GDK_W: return Keyboard::Keycode::W;
  case GDK_X: return Keyboard::Keycode::X;
  case GDK_Y: return Keyboard::Keycode::Y;
  case GDK_Z: return Keyboard::Keycode::Z;

  case GDK_a: return Keyboard::Keycode::a;
  case GDK_b: return Keyboard::Keycode::b;
  case GDK_c: return Keyboard::Keycode::c;
  case GDK_d: return Keyboard::Keycode::d;
  case GDK_e: return Keyboard::Keycode::e;
  case GDK_f: return Keyboard::Keycode::f;
  case GDK_g: return Keyboard::Keycode::g;
  case GDK_h: return Keyboard::Keycode::h;
  case GDK_i: return Keyboard::Keycode::i;
  case GDK_j: return Keyboard::Keycode::j;
  case GDK_k: return Keyboard::Keycode::k;
  case GDK_l: return Keyboard::Keycode::l;
  case GDK_m: return Keyboard::Keycode::m;
  case GDK_n: return Keyboard::Keycode::n;
  case GDK_o: return Keyboard::Keycode::o;
  case GDK_p: return Keyboard::Keycode::p;
  case GDK_q: return Keyboard::Keycode::q;
  case GDK_r: return Keyboard::Keycode::r;
  case GDK_s: return Keyboard::Keycode::s;
  case GDK_t: return Keyboard::Keycode::t;
  case GDK_u: return Keyboard::Keycode::u;
  case GDK_v: return Keyboard::Keycode::v;
  case GDK_w: return Keyboard::Keycode::w;
  case GDK_x: return Keyboard::Keycode::x;
  case GDK_y: return Keyboard::Keycode::y;
  case GDK_z: return Keyboard::Keycode::z;

  case GDK_Num_Lock: return Keyboard::Keycode::NumLock;
  case GDK_KP_Divide: return Keyboard::Keycode::Divide;
  case GDK_KP_Multiply: return Keyboard::Keycode::Multiply;
  case GDK_KP_Subtract: return Keyboard::Keycode::Subtract;
  case GDK_KP_Add: return Keyboard::Keycode::Add;
  case GDK_KP_Enter: return Keyboard::Keycode::Enter;
  case GDK_KP_Decimal: return Keyboard::Keycode::Point;

  case GDK_KP_1: return Keyboard::Keycode::Keypad1;
  case GDK_KP_2: return Keyboard::Keycode::Keypad2;
  case GDK_KP_3: return Keyboard::Keycode::Keypad3;
  case GDK_KP_4: return Keyboard::Keycode::Keypad4;
  case GDK_KP_5: return Keyboard::Keycode::Keypad5;
  case GDK_KP_6: return Keyboard::Keycode::Keypad6;
  case GDK_KP_7: return Keyboard::Keycode::Keypad7;
  case GDK_KP_8: return Keyboard::Keycode::Keypad8;
  case GDK_KP_9: return Keyboard::Keycode::Keypad9;
  case GDK_KP_0: return Keyboard::Keycode::Keypad0;

  case GDK_KP_Home: return Keyboard::Keycode::KeypadHome;
  case GDK_KP_End: return Keyboard::Keycode::KeypadEnd;
  case GDK_KP_Page_Up: return Keyboard::Keycode::KeypadPageUp;
  case GDK_KP_Page_Down: return Keyboard::Keycode::KeypadPageDown;
  case GDK_KP_Up: return Keyboard::Keycode::KeypadUp;
  case GDK_KP_Down: return Keyboard::Keycode::KeypadDown;
  case GDK_KP_Left: return Keyboard::Keycode::KeypadLeft;
  case GDK_KP_Right: return Keyboard::Keycode::KeypadRight;
  case GDK_KP_Begin: return Keyboard::Keycode::KeypadCenter;
  case GDK_KP_Insert: return Keyboard::Keycode::KeypadInsert;
  case GDK_KP_Delete: return Keyboard::Keycode::KeypadDelete;
  }
  return Keyboard::Keycode::None;
}

}
