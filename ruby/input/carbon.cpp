namespace ruby {

struct pInputCarbon {
  struct Key {
    uint8_t id;
    string name;
  };
  vector<Key> keys;

  struct Keyboard {
    shared_pointer<HID::Keyboard> hid{new HID::Keyboard};
  } kb;

  auto cap(const string& name) -> bool {
    if(name == Input::KeyboardSupport) return true;
    return false;
  }

  auto get(const string& name) -> any {
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    return false;
  }

  auto acquire() -> bool { return false; }
  auto unacquire() -> bool { return false; }
  auto acquired() -> bool { return false; }

  auto assign(shared_pointer<HID::Device> hid, unsigned groupID, unsigned inputID, int16_t value) -> void {
    auto& group = hid->group(groupID);
    if(group.input(inputID).value() == value) return;
    if(input.onChange) input.onChange(hid, groupID, inputID, group.input(inputID).value(), value);
    group.input(inputID).setValue(value);
  }

  auto poll() -> vector<shared_pointer<HID::Device>> {
    vector<shared_pointer<HID::Device>> devices;

    KeyMap keymap;
    GetKeys(keymap);
    auto buffer = (uint8_t*)keymap;

    unsigned inputID = 0;
    for(auto& key : keys) {
      bool value = buffer[key.id >> 3] & (1 << (key.id & 7));
      assign(kb.hid, HID::Keyboard::GroupID::Button, inputID++, value);
    }

    devices.append(kb.hid);
    return devices;
  }

  auto rumble(uint64_t id, bool enable) -> bool {
    return false;
  }

  auto init() -> bool {
    keys.append({0x35, "Escape"});
    keys.append({0x7a, "F1"});
    keys.append({0x78, "F2"});
    keys.append({0x63, "F3"});
    keys.append({0x76, "F4"});
    keys.append({0x60, "F5"});
    keys.append({0x61, "F6"});
    keys.append({0x62, "F7"});
    keys.append({0x64, "F8"});
    keys.append({0x65, "F9"});
    keys.append({0x6d, "F10"});
    keys.append({0x67, "F11"});
  //keys.append({0x??, "F12"});

    keys.append({0x69, "PrintScreen"});
  //keys.append({0x??, "ScrollLock"});
    keys.append({0x71, "Pause"});

    keys.append({0x32, "Tilde"});
    keys.append({0x12, "Num1"});
    keys.append({0x13, "Num2"});
    keys.append({0x14, "Num3"});
    keys.append({0x15, "Num4"});
    keys.append({0x17, "Num5"});
    keys.append({0x16, "Num6"});
    keys.append({0x1a, "Num7"});
    keys.append({0x1c, "Num8"});
    keys.append({0x19, "Num9"});
    keys.append({0x1d, "Num0"});

    keys.append({0x1b, "Dash"});
    keys.append({0x18, "Equal"});
    keys.append({0x33, "Backspace"});

    keys.append({0x72, "Insert"});
    keys.append({0x75, "Delete"});
    keys.append({0x73, "Home"});
    keys.append({0x77, "End"});
    keys.append({0x74, "PageUp"});
    keys.append({0x79, "PageDown"});

    keys.append({0x00, "A"});
    keys.append({0x0b, "B"});
    keys.append({0x08, "C"});
    keys.append({0x02, "D"});
    keys.append({0x0e, "E"});
    keys.append({0x03, "F"});
    keys.append({0x05, "G"});
    keys.append({0x04, "H"});
    keys.append({0x22, "I"});
    keys.append({0x26, "J"});
    keys.append({0x28, "K"});
    keys.append({0x25, "L"});
    keys.append({0x2e, "M"});
    keys.append({0x2d, "N"});
    keys.append({0x1f, "O"});
    keys.append({0x23, "P"});
    keys.append({0x0c, "Q"});
    keys.append({0x0f, "R"});
    keys.append({0x01, "S"});
    keys.append({0x11, "T"});
    keys.append({0x20, "U"});
    keys.append({0x09, "V"});
    keys.append({0x0d, "W"});
    keys.append({0x07, "X"});
    keys.append({0x10, "Y"});
    keys.append({0x06, "Z"});

    keys.append({0x21, "LeftBracket"});
    keys.append({0x1e, "RightBracket"});
    keys.append({0x2a, "Backslash"});
    keys.append({0x29, "Semicolon"});
    keys.append({0x27, "Apostrophe"});
    keys.append({0x2b, "Comma"});
    keys.append({0x2f, "Period"});
    keys.append({0x2c, "Slash"});

    keys.append({0x53, "Keypad1"});
    keys.append({0x54, "Keypad2"});
    keys.append({0x55, "Keypad3"});
    keys.append({0x56, "Keypad4"});
    keys.append({0x57, "Keypad5"});
    keys.append({0x58, "Keypad6"});
    keys.append({0x59, "Keypad7"});
    keys.append({0x5b, "Keypad8"});
    keys.append({0x5c, "Keypad9"});
    keys.append({0x52, "Keypad0"});

  //keys.append({0x??, "Point"});
    keys.append({0x4c, "Enter"});
    keys.append({0x45, "Add"});
    keys.append({0x4e, "Subtract"});
    keys.append({0x43, "Multiply"});
    keys.append({0x4b, "Divide"});

    keys.append({0x47, "NumLock"});
  //keys.append({0x39, "CapsLock"});

    keys.append({0x7e, "Up"});
    keys.append({0x7d, "Down"});
    keys.append({0x7b, "Left"});
    keys.append({0x7c, "Right"});

    keys.append({0x30, "Tab"});
    keys.append({0x24, "Return"});
    keys.append({0x31, "Spacebar"});
  //keys.append({0x??, "Menu"});

    keys.append({0x38, "Shift"});
    keys.append({0x3b, "Control"});
    keys.append({0x3a, "Alt"});
    keys.append({0x37, "Super"});

    kb.hid->setID(1);
    for(auto& key : keys) kb.hid->buttons().append(key.name);

    return true;
  }

  auto term() -> void {
  }
};

DeclareInput(Carbon)

};
