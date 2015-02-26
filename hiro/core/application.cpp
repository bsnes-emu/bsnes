auto Application::doMain() -> void {
  if(state.onMain) return state.onMain();
}

auto Application::font() -> string {
  return state.font;
}

auto Application::name() -> string {
  return state.name;
}

auto Application::onMain(const nall::function<void ()>& function) -> void {
  state.onMain = function;
}

auto Application::run() -> void {
  return pApplication::run();
}

auto Application::pendingEvents() -> bool {
  return pApplication::pendingEvents();
}

auto Application::processEvents() -> void {
  return pApplication::processEvents();
}

auto Application::quit() -> void {
  state.quit = true;
  return pApplication::quit();
}

auto Application::setFont(const string& font) -> void {
  state.font = font;
}

auto Application::setName(const string& name) -> void {
  state.name = name;
}

//Windows
//=======

auto Application::Windows::doModalChange(bool modal) -> void {
  if(state.windows.onModalChange) return state.windows.onModalChange(modal);
}

auto Application::Windows::onModalChange(const function<void (bool)>& function) -> void {
  state.windows.onModalChange = function;
}

//Cocoa
//=====

auto Application::Cocoa::doAbout() -> void {
  if(state.cocoa.onAbout) return state.cocoa.onAbout();
}

auto Application::Cocoa::doActivate() -> void {
  if(state.cocoa.onActivate) return state.cocoa.onActivate();
}

auto Application::Cocoa::doPreferences() -> void {
  if(state.cocoa.onPreferences) return state.cocoa.onPreferences();
}

auto Application::Cocoa::doQuit() -> void {
  if(state.cocoa.onQuit) return state.cocoa.onQuit();
}

auto Application::Cocoa::onAbout(const function<void ()>& function) -> void {
  state.cocoa.onAbout = function;
}

auto Application::Cocoa::onActivate(const function<void ()>& function) -> void {
  state.cocoa.onActivate = function;
}

auto Application::Cocoa::onPreferences(const function<void ()>& function) -> void {
  state.cocoa.onPreferences = function;
}

auto Application::Cocoa::onQuit(const function<void ()>& function) -> void {
  state.cocoa.onQuit = function;
}

//Internal
//========

auto Application::initialize() -> void {
  static bool initialized = false;
  if(initialized == false) {
    initialized = true;
    return pApplication::initialize();
  }
}
