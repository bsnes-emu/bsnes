auto mHotkey::allocate() -> pObject* {
  return new pHotkey(*this);
}

//

auto mHotkey::doPress() const -> void {
  if(state.onPress) return state.onPress();
}

auto mHotkey::doRelease() const -> void {
  if(state.onRelease) return state.onRelease();
}

auto mHotkey::onPress(const function<void ()>& function) -> type& {
  state.onPress = function;
  return *this;
}

auto mHotkey::onRelease(const function<void ()>& function) -> type& {
  state.onRelease = function;
  return *this;
}

auto mHotkey::parent() const -> wObject {
  return state.parent;
}

auto mHotkey::remove() -> type& {
//todo: remove from Keyboard::hotkeys
  return *this;
}

auto mHotkey::sequence() const -> string {
  return state.sequence;
}

auto mHotkey::setParent(sObject parent) -> type& {
  state.parent = parent;
  return *this;
}

auto mHotkey::setSequence(const string& sequence) -> type& {
  state.active = false;
  state.sequence = sequence;
  state.keys.reset();
  for(auto& key : sequence.split("+")) {
    if(auto position = Keyboard::keys.find(key)) {
      state.keys.append(*position);
    }
  }
  return *this;
}
