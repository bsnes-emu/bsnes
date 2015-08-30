#if defined(Hiro_Keyboard)

Keyboard::State Keyboard::state;

const vector<string> Keyboard::keys = {
  //physical keyboard buttons
  "Escape", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
  "PrintScreen", "ScrollLock", "Pause",
  "Insert", "Delete", "Home", "End", "PageUp", "PageDown",
  "Up", "Down", "Left", "Right",
  "Grave", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "Dash", "Equal", "Backspace",
  "Tab", "CapsLock", "LeftEnter", "LeftShift", "RightShift",
  "LeftControl", "RightControl", "LeftAlt", "RightAlt", "LeftSuper", "RightSuper", "Menu", "Space",
  "OpenBracket", "CloseBracket", "Backslash", "Semicolon", "Apostrophe", "Comma", "Period", "Slash",
  "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
  "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
  "NumLock", "Divide", "Multiply", "Subtract", "Add", "RightEnter", "Point",
  "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Zero",

  //group aliases
  "Shift",    //"LeftShift"   | "RightShift"
  "Control",  //"LeftControl" | "RightControl"
  "Alt",      //"LeftAlt"     | "RightAlt"
  "Super",    //"LeftSuper"   | "RightSuper"
  "Enter",    //"LeftEnter"   | "RightEnter"
};

auto Keyboard::append(Hotkey hotkey) -> void {
  state.hotkeys.append(hotkey);
}

auto Keyboard::hotkey(unsigned position) -> Hotkey {
  if(position < hotkeyCount()) return state.hotkeys[position];
  return {};
}

auto Keyboard::hotkeyCount() -> unsigned {
  return state.hotkeys.size();
}

auto Keyboard::hotkeys() -> vector<Hotkey> {
  return state.hotkeys;
}

auto Keyboard::poll() -> vector<bool> {
  auto pressed = pKeyboard::poll();

  for(auto& hotkey : state.hotkeys) {
    bool active = hotkey.state->sequence.size() > 0;
    for(auto& key : hotkey.state->keys) {
      if(pressed[key]) continue;
      active = false;
      break;
    }
    if(hotkey.state->active != active) {
      hotkey.state->active = active;
      active ? hotkey.doPress() : hotkey.doRelease();
    }
  }

  return pressed;
}

auto Keyboard::pressed(const string& key) -> bool {
  if(auto code = keys.find(key)) return pKeyboard::pressed(*code);
  return false;
}

auto Keyboard::released(const string& key) -> bool {
  return !pressed(key);
}

auto Keyboard::remove(Hotkey hotkey) -> void {
  if(auto offset = state.hotkeys.find(hotkey)) {
    state.hotkeys.remove(*offset);
  }
}

#endif
