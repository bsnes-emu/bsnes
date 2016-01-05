#ifndef RUBY_INPUT_KEYBOARD_XLIB
#define RUBY_INPUT_KEYBOARD_XLIB

struct InputKeyboardXlib {
  Input& input;
  InputKeyboardXlib(Input& input) : input(input) {}

  shared_pointer<HID::Keyboard> hid{new HID::Keyboard};

  Display* display = nullptr;

  struct Key {
    string name;
    uint keysym;
    uint keycode;
  };
  vector<Key> keys;

  auto assign(uint inputID, bool value) -> void {
    auto& group = hid->buttons();
    if(group.input(inputID).value() == value) return;
    input.doChange(hid, HID::Keyboard::GroupID::Button, inputID, group.input(inputID).value(), value);
    group.input(inputID).setValue(value);
  }

  auto poll(vector<shared_pointer<HID::Device>>& devices) -> void {
    char state[32];
    XQueryKeymap(display, state);

    uint inputID = 0;
    for(auto& key : keys) {
      bool value = state[key.keycode >> 3] & (1 << (key.keycode & 7));
      assign(inputID++, value);
    }

    devices.append(hid);
  }

  auto init() -> bool {
    display = XOpenDisplay(0);

    keys.append({"Escape", XK_Escape});

    keys.append({"F1", XK_F1});
    keys.append({"F2", XK_F2});
    keys.append({"F3", XK_F3});
    keys.append({"F4", XK_F4});
    keys.append({"F5", XK_F5});
    keys.append({"F6", XK_F6});
    keys.append({"F7", XK_F7});
    keys.append({"F8", XK_F8});
    keys.append({"F9", XK_F9});
    keys.append({"F10", XK_F10});
    keys.append({"F11", XK_F11});
    keys.append({"F12", XK_F12});

    keys.append({"ScrollLock", XK_Scroll_Lock});
    keys.append({"Pause", XK_Pause});

    keys.append({"Tilde", XK_asciitilde});

    keys.append({"Num0", XK_0});
    keys.append({"Num1", XK_1});
    keys.append({"Num2", XK_2});
    keys.append({"Num3", XK_3});
    keys.append({"Num4", XK_4});
    keys.append({"Num5", XK_5});
    keys.append({"Num6", XK_6});
    keys.append({"Num7", XK_7});
    keys.append({"Num8", XK_8});
    keys.append({"Num9", XK_9});

    keys.append({"Dash", XK_minus});
    keys.append({"Equal", XK_equal});
    keys.append({"Backspace", XK_BackSpace});

    keys.append({"Insert", XK_Insert});
    keys.append({"Delete", XK_Delete});
    keys.append({"Home", XK_Home});
    keys.append({"End", XK_End});
    keys.append({"PageUp", XK_Prior});
    keys.append({"PageDown", XK_Next});

    keys.append({"A", XK_A});
    keys.append({"B", XK_B});
    keys.append({"C", XK_C});
    keys.append({"D", XK_D});
    keys.append({"E", XK_E});
    keys.append({"F", XK_F});
    keys.append({"G", XK_G});
    keys.append({"H", XK_H});
    keys.append({"I", XK_I});
    keys.append({"J", XK_J});
    keys.append({"K", XK_K});
    keys.append({"L", XK_L});
    keys.append({"M", XK_M});
    keys.append({"N", XK_N});
    keys.append({"O", XK_O});
    keys.append({"P", XK_P});
    keys.append({"Q", XK_Q});
    keys.append({"R", XK_R});
    keys.append({"S", XK_S});
    keys.append({"T", XK_T});
    keys.append({"U", XK_U});
    keys.append({"V", XK_V});
    keys.append({"W", XK_W});
    keys.append({"X", XK_X});
    keys.append({"Y", XK_Y});
    keys.append({"Z", XK_Z});

    keys.append({"LeftBracket", XK_bracketleft});
    keys.append({"RightBracket", XK_bracketright});
    keys.append({"Backslash", XK_backslash});
    keys.append({"Semicolon", XK_semicolon});
    keys.append({"Apostrophe", XK_apostrophe});
    keys.append({"Comma", XK_comma});
    keys.append({"Period", XK_period});
    keys.append({"Slash", XK_slash});

    keys.append({"Keypad0", XK_KP_0});
    keys.append({"Keypad1", XK_KP_1});
    keys.append({"Keypad2", XK_KP_2});
    keys.append({"Keypad3", XK_KP_3});
    keys.append({"Keypad4", XK_KP_4});
    keys.append({"Keypad5", XK_KP_5});
    keys.append({"Keypad6", XK_KP_6});
    keys.append({"Keypad7", XK_KP_7});
    keys.append({"Keypad8", XK_KP_8});
    keys.append({"Keypad9", XK_KP_9});

    keys.append({"Add", XK_KP_Add});
    keys.append({"Subtract", XK_KP_Subtract});
    keys.append({"Multiply", XK_KP_Multiply});
    keys.append({"Divide", XK_KP_Divide});
    keys.append({"Enter", XK_KP_Enter});

    keys.append({"Up", XK_Up});
    keys.append({"Down", XK_Down});
    keys.append({"Left", XK_Left});
    keys.append({"Right", XK_Right});

    keys.append({"Tab", XK_Tab});
    keys.append({"Return", XK_Return});
    keys.append({"Spacebar", XK_space});

    keys.append({"LeftControl", XK_Control_L});
    keys.append({"RightControl", XK_Control_R});
    keys.append({"LeftAlt", XK_Alt_L});
    keys.append({"RightAlt", XK_Alt_R});
    keys.append({"LeftShift", XK_Shift_L});
    keys.append({"RightShift", XK_Shift_R});
    keys.append({"LeftSuper", XK_Super_L});
    keys.append({"RightSuper", XK_Super_R});
    keys.append({"Menu", XK_Menu});

    hid->setID(1);

    for(auto& key : keys) {
      hid->buttons().append(key.name);
      key.keycode = XKeysymToKeycode(display, key.keysym);
    }

    return true;
  }

  auto term() -> void {
    if(display) {
      XCloseDisplay(display);
      display = nullptr;
    }
  }
};

#endif
