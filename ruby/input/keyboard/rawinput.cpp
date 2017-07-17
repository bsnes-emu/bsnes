#ifndef RUBY_INPUT_KEYBOARD_RAWINPUT
#define RUBY_INPUT_KEYBOARD_RAWINPUT

struct InputKeyboardRawInput {
  Input& input;
  InputKeyboardRawInput(Input& input) : input(input) {}

  struct Key {
    uint16_t code;
    uint16_t flag;
    string name;
    bool value;
  };
  vector<Key> keys;

  struct Keyboard {
    shared_pointer<HID::Keyboard> hid{new HID::Keyboard};
  } kb;

  auto update(RAWINPUT* input) -> void {
    unsigned code = input->data.keyboard.MakeCode;
    unsigned flag = input->data.keyboard.Flags;

    for(auto& key : keys) {
      if(key.code != code) continue;
      key.value = (key.flag == flag);
    }
  }

  auto assign(unsigned inputID, bool value) -> void {
    auto& group = kb.hid->buttons();
    if(group.input(inputID).value() == value) return;
    input.doChange(kb.hid, HID::Keyboard::GroupID::Button, inputID, group.input(inputID).value(), value);
    group.input(inputID).setValue(value);
  }

  auto poll(vector<shared_pointer<HID::Device>>& devices) -> void {
    for(unsigned n = 0; n < keys.size(); n++) assign(n, keys[n].value);
    devices.append(kb.hid);
  }

  auto initialize() -> bool {
    rawinput.updateKeyboard = {&InputKeyboardRawInput::update, this};

    //Pause sends 0x001d,4 + 0x0045,0; NumLock sends only 0x0045,0
    //pressing Pause will falsely trigger NumLock
    //further, pause sends its key release even while button is held down
    //because of this, we cannot map either reliably

    keys.append({0x0001, 0, "Escape"});
    keys.append({0x003b, 0, "F1"});
    keys.append({0x003c, 0, "F2"});
    keys.append({0x003d, 0, "F3"});
    keys.append({0x003e, 0, "F4"});
    keys.append({0x003f, 0, "F5"});
    keys.append({0x0040, 0, "F6"});
    keys.append({0x0041, 0, "F7"});
    keys.append({0x0042, 0, "F8"});
    keys.append({0x0043, 0, "F9"});
    keys.append({0x0044, 0, "F10"});
    keys.append({0x0057, 0, "F11"});
    keys.append({0x0058, 0, "F12"});

    keys.append({0x0037, 2, "PrintScreen"});
    keys.append({0x0046, 0, "ScrollLock"});
  //keys.append({0x001d, 4, "Pause"});
    keys.append({0x0029, 0, "Tilde"});

    keys.append({0x0002, 0, "Num1"});
    keys.append({0x0003, 0, "Num2"});
    keys.append({0x0004, 0, "Num3"});
    keys.append({0x0005, 0, "Num4"});
    keys.append({0x0006, 0, "Num5"});
    keys.append({0x0007, 0, "Num6"});
    keys.append({0x0008, 0, "Num7"});
    keys.append({0x0009, 0, "Num8"});
    keys.append({0x000a, 0, "Num9"});
    keys.append({0x000b, 0, "Num0"});

    keys.append({0x000c, 0, "Dash"});
    keys.append({0x000d, 0, "Equal"});
    keys.append({0x000e, 0, "Backspace"});

    keys.append({0x0052, 2, "Insert"});
    keys.append({0x0053, 2, "Delete"});
    keys.append({0x0047, 2, "Home"});
    keys.append({0x004f, 2, "End"});
    keys.append({0x0049, 2, "PageUp"});
    keys.append({0x0051, 2, "PageDown"});

    keys.append({0x001e, 0, "A"});
    keys.append({0x0030, 0, "B"});
    keys.append({0x002e, 0, "C"});
    keys.append({0x0020, 0, "D"});
    keys.append({0x0012, 0, "E"});
    keys.append({0x0021, 0, "F"});
    keys.append({0x0022, 0, "G"});
    keys.append({0x0023, 0, "H"});
    keys.append({0x0017, 0, "I"});
    keys.append({0x0024, 0, "J"});
    keys.append({0x0025, 0, "K"});
    keys.append({0x0026, 0, "L"});
    keys.append({0x0032, 0, "M"});
    keys.append({0x0031, 0, "N"});
    keys.append({0x0018, 0, "O"});
    keys.append({0x0019, 0, "P"});
    keys.append({0x0010, 0, "Q"});
    keys.append({0x0013, 0, "R"});
    keys.append({0x001f, 0, "S"});
    keys.append({0x0014, 0, "T"});
    keys.append({0x0016, 0, "U"});
    keys.append({0x002f, 0, "V"});
    keys.append({0x0011, 0, "W"});
    keys.append({0x002d, 0, "X"});
    keys.append({0x0015, 0, "Y"});
    keys.append({0x002c, 0, "Z"});

    keys.append({0x001a, 0, "LeftBracket"});
    keys.append({0x001b, 0, "RightBracket"});
    keys.append({0x002b, 0, "Backslash"});
    keys.append({0x0027, 0, "Semicolon"});
    keys.append({0x0028, 0, "Apostrophe"});
    keys.append({0x0033, 0, "Comma"});
    keys.append({0x0034, 0, "Period"});
    keys.append({0x0035, 0, "Slash"});

    keys.append({0x004f, 0, "Keypad1"});
    keys.append({0x0050, 0, "Keypad2"});
    keys.append({0x0051, 0, "Keypad3"});
    keys.append({0x004b, 0, "Keypad4"});
    keys.append({0x004c, 0, "Keypad5"});
    keys.append({0x004d, 0, "Keypad6"});
    keys.append({0x0047, 0, "Keypad7"});
    keys.append({0x0048, 0, "Keypad8"});
    keys.append({0x0049, 0, "Keypad9"});
    keys.append({0x0052, 0, "Keypad0"});

    keys.append({0x0053, 0, "Point"});
    keys.append({0x001c, 2, "Enter"});
    keys.append({0x004e, 0, "Add"});
    keys.append({0x004a, 0, "Subtract"});
    keys.append({0x0037, 0, "Multiply"});
    keys.append({0x0035, 2, "Divide"});

  //keys.append({0x0045, 0, "NumLock"});
    keys.append({0x003a, 0, "CapsLock"});

    keys.append({0x0048, 2, "Up"});
    keys.append({0x0050, 2, "Down"});
    keys.append({0x004b, 2, "Left"});
    keys.append({0x004d, 2, "Right"});

    keys.append({0x000f, 0, "Tab"});
    keys.append({0x001c, 0, "Return"});
    keys.append({0x0039, 0, "Spacebar"});

    keys.append({0x002a, 0, "LeftShift"});
    keys.append({0x0036, 0, "RightShift"});
    keys.append({0x001d, 0, "LeftControl"});
    keys.append({0x001d, 2, "RightControl"});
    keys.append({0x0038, 0, "LeftAlt"});
    keys.append({0x0038, 2, "RightAlt"});
    keys.append({0x005b, 2, "LeftSuper"});
    keys.append({0x005c, 2, "RightSuper"});
    keys.append({0x005d, 2, "Menu"});

    kb.hid->setID(1);
    for(auto& key : keys) kb.hid->buttons().append(key.name);

    return true;
  }

  auto terminate() -> void {
    rawinput.updateKeyboard.reset();
  }
};

#endif
