struct InputKeyboardQuartz {
  Input& input;
  InputKeyboardQuartz(Input& input) : input(input) {}

  shared_pointer<HID::Keyboard> hid{new HID::Keyboard};

  struct Key {
    string name;
    uint id;
  };
  vector<Key> keys;

  auto assign(uint inputID, bool value) -> void {
    auto& group = hid->buttons();
    if(group.input(inputID).value() == value) return;
    input.doChange(hid, HID::Keyboard::GroupID::Button, inputID, group.input(inputID).value(), value);
    group.input(inputID).setValue(value);
  }

  auto poll(vector<shared_pointer<HID::Device>>& devices) -> void {
    uint inputID = 0;
    for(auto& key : keys) {
      bool value = CGEventSourceKeyState(kCGEventSourceStateCombinedSessionState, key.id);
      assign(inputID++, value);
    }
    devices.append(hid);
  }

  auto initialize() -> bool {
    keys.append({"Escape", kVK_Escape});
    keys.append({"F1", kVK_F1});
    keys.append({"F2", kVK_F2});
    keys.append({"F3", kVK_F3});
    keys.append({"F4", kVK_F4});
    keys.append({"F5", kVK_F5});
    keys.append({"F6", kVK_F6});
    keys.append({"F7", kVK_F7});
    keys.append({"F8", kVK_F8});
    keys.append({"F9", kVK_F9});
    keys.append({"F10", kVK_F10});
    keys.append({"F11", kVK_F11});
    keys.append({"F12", kVK_F12});
    keys.append({"F13", kVK_F13});
    keys.append({"F14", kVK_F14});
    keys.append({"F15", kVK_F15});
    keys.append({"F16", kVK_F16});
    keys.append({"F17", kVK_F17});
    keys.append({"F18", kVK_F18});
    keys.append({"F19", kVK_F19});
    keys.append({"F20", kVK_F20});

    keys.append({"Tilde", kVK_ANSI_Grave});
    keys.append({"Num1", kVK_ANSI_1});
    keys.append({"Num2", kVK_ANSI_2});
    keys.append({"Num3", kVK_ANSI_3});
    keys.append({"Num4", kVK_ANSI_4});
    keys.append({"Num5", kVK_ANSI_5});
    keys.append({"Num6", kVK_ANSI_6});
    keys.append({"Num7", kVK_ANSI_7});
    keys.append({"Num8", kVK_ANSI_8});
    keys.append({"Num9", kVK_ANSI_9});
    keys.append({"Num0", kVK_ANSI_0});

    keys.append({"Dash", kVK_ANSI_Minus});
    keys.append({"Equal", kVK_ANSI_Equal});
    keys.append({"Delete", kVK_Delete});

    keys.append({"Erase", kVK_ForwardDelete});
    keys.append({"Home", kVK_Home});
    keys.append({"End", kVK_End});
    keys.append({"PageUp", kVK_PageUp});
    keys.append({"PageDown", kVK_PageDown});

    keys.append({"A", kVK_ANSI_A});
    keys.append({"B", kVK_ANSI_B});
    keys.append({"C", kVK_ANSI_C});
    keys.append({"D", kVK_ANSI_D});
    keys.append({"E", kVK_ANSI_E});
    keys.append({"F", kVK_ANSI_F});
    keys.append({"G", kVK_ANSI_G});
    keys.append({"H", kVK_ANSI_H});
    keys.append({"I", kVK_ANSI_I});
    keys.append({"J", kVK_ANSI_J});
    keys.append({"K", kVK_ANSI_K});
    keys.append({"L", kVK_ANSI_L});
    keys.append({"M", kVK_ANSI_M});
    keys.append({"N", kVK_ANSI_N});
    keys.append({"O", kVK_ANSI_O});
    keys.append({"P", kVK_ANSI_P});
    keys.append({"Q", kVK_ANSI_Q});
    keys.append({"R", kVK_ANSI_R});
    keys.append({"S", kVK_ANSI_S});
    keys.append({"T", kVK_ANSI_T});
    keys.append({"U", kVK_ANSI_U});
    keys.append({"V", kVK_ANSI_V});
    keys.append({"W", kVK_ANSI_W});
    keys.append({"X", kVK_ANSI_X});
    keys.append({"Y", kVK_ANSI_Y});
    keys.append({"Z", kVK_ANSI_Z});

    keys.append({"LeftBracket", kVK_ANSI_LeftBracket});
    keys.append({"RightBracket", kVK_ANSI_RightBracket});
    keys.append({"Backslash", kVK_ANSI_Backslash});
    keys.append({"Semicolon", kVK_ANSI_Semicolon});
    keys.append({"Apostrophe", kVK_ANSI_Quote});
    keys.append({"Comma", kVK_ANSI_Comma});
    keys.append({"Period", kVK_ANSI_Period});
    keys.append({"Slash", kVK_ANSI_Slash});

    keys.append({"Keypad1", kVK_ANSI_Keypad1});
    keys.append({"Keypad2", kVK_ANSI_Keypad2});
    keys.append({"Keypad3", kVK_ANSI_Keypad3});
    keys.append({"Keypad4", kVK_ANSI_Keypad4});
    keys.append({"Keypad5", kVK_ANSI_Keypad5});
    keys.append({"Keypad6", kVK_ANSI_Keypad6});
    keys.append({"Keypad7", kVK_ANSI_Keypad7});
    keys.append({"Keypad8", kVK_ANSI_Keypad8});
    keys.append({"Keypad9", kVK_ANSI_Keypad9});
    keys.append({"Keypad0", kVK_ANSI_Keypad0});

    keys.append({"Clear", kVK_ANSI_KeypadClear});
    keys.append({"Equals", kVK_ANSI_KeypadEquals});
    keys.append({"Divide", kVK_ANSI_KeypadDivide});
    keys.append({"Multiply", kVK_ANSI_KeypadMultiply});
    keys.append({"Subtract", kVK_ANSI_KeypadMinus});
    keys.append({"Add", kVK_ANSI_KeypadPlus});
    keys.append({"Enter", kVK_ANSI_KeypadEnter});
    keys.append({"Decimal", kVK_ANSI_KeypadDecimal});

    keys.append({"Up", kVK_UpArrow});
    keys.append({"Down", kVK_DownArrow});
    keys.append({"Left", kVK_LeftArrow});
    keys.append({"Right", kVK_RightArrow});

    keys.append({"Tab", kVK_Tab});
    keys.append({"Return", kVK_Return});
    keys.append({"Spacebar", kVK_Space});
    keys.append({"Shift", kVK_Shift});
    keys.append({"Control", kVK_Control});
    keys.append({"Option", kVK_Option});
    keys.append({"Command", kVK_Command});

    hid->setID(1);
    for(auto& key : keys) {
      hid->buttons().append(key.name);
    }

    return true;
  }

  auto terminate() -> void {
  }
};
