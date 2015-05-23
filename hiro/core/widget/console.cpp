#if defined(Hiro_Console)

auto mConsole::allocate() -> pObject* {
  return new pConsole(*this);
}

//

auto mConsole::backgroundColor() const -> Color {
  return state.backgroundColor;
}

auto mConsole::doActivate(string command) const -> void {
  if(state.onActivate) return state.onActivate(command);
}

auto mConsole::foregroundColor() const -> Color {
  return state.foregroundColor;
}

auto mConsole::onActivate(const function<void (string)>& function) -> type& {
  state.onActivate = function;
  return *this;
}

auto mConsole::print(const string& text) -> type& {
  signal(print, text);
  return *this;
}

auto mConsole::prompt() const -> string {
  return state.prompt;
}

auto mConsole::reset() -> type& {
  signal(reset);
  return *this;
}

auto mConsole::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mConsole::setForegroundColor(Color color) -> type& {
  state.foregroundColor = color;
  signal(setForegroundColor, color);
  return *this;
}

auto mConsole::setPrompt(const string& prompt) -> type& {
  state.prompt = prompt;
  signal(setPrompt, prompt);
  return *this;
}

#endif
