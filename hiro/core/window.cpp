#if defined(Hiro_Window)

mWindow::mWindow() {
  mObject::state.visible = false;
}

auto mWindow::allocate() -> pObject* {
  return new pWindow(*this);
}

auto mWindow::destruct() -> void {
  if(auto& menuBar = state.menuBar) menuBar->destruct();
  if(auto& sizable = state.sizable) sizable->destruct();
  if(auto& statusBar = state.statusBar) statusBar->destruct();
  mObject::destruct();
}

//

auto mWindow::append(sMenuBar menuBar) -> type& {
  if(auto& menuBar = state.menuBar) remove(menuBar);
  menuBar->setParent(this, 0);
  state.menuBar = menuBar;
  signal(append, menuBar);
  return *this;
}

auto mWindow::append(sSizable sizable) -> type& {
  if(auto& sizable = state.sizable) remove(sizable);
  state.sizable = sizable;
  sizable->setParent(this, 0);
  signal(append, sizable);
  return *this;
}

auto mWindow::append(sStatusBar statusBar) -> type& {
  if(auto& statusBar = state.statusBar) remove(statusBar);
  statusBar->setParent(this, 0);
  state.statusBar = statusBar;
  signal(append, statusBar);
  return *this;
}

auto mWindow::backgroundColor() const -> Color {
  return state.backgroundColor;
}

auto mWindow::dismissable() const -> bool {
  return state.dismissable;
}

auto mWindow::doClose() const -> void {
  if(state.onClose) return state.onClose();
}

auto mWindow::doDrop(vector<string> names) const -> void {
  if(state.onDrop) return state.onDrop(names);
}

auto mWindow::doKeyPress(signed key) const -> void {
  if(state.onKeyPress) return state.onKeyPress(key);
}

auto mWindow::doKeyRelease(signed key) const -> void {
  if(state.onKeyRelease) return state.onKeyRelease(key);
}

auto mWindow::doMove() const -> void {
  if(state.onMove) return state.onMove();
}

auto mWindow::doSize() const -> void {
  if(state.onSize) return state.onSize();
}

auto mWindow::droppable() const -> bool {
  return state.droppable;
}

auto mWindow::frameGeometry() const -> Geometry {
  Geometry margin = signal(frameMargin);
  return {
    state.geometry.x() - margin.x(), state.geometry.y() - margin.y(),
    state.geometry.width() + margin.width(), state.geometry.height() + margin.height()
  };
}

auto mWindow::fullScreen() const -> bool {
  return state.fullScreen;
}

auto mWindow::geometry() const -> Geometry {
  return state.geometry;
}

auto mWindow::maximized() const -> bool {
  return state.maximized;
}

auto mWindow::maximumSize() const -> Size {
  return state.maximumSize;
}

auto mWindow::menuBar() const -> MenuBar {
  return state.menuBar;
}

auto mWindow::minimized() const -> bool {
  return state.minimized;
}

auto mWindow::minimumSize() const -> Size {
  return state.minimumSize;
}

auto mWindow::modal() const -> bool {
  return state.modal;
}

auto mWindow::monitor() const -> uint {
  return signal(monitor);
}

auto mWindow::onClose(const function<void ()>& callback) -> type& {
  state.onClose = callback;
  return *this;
}

auto mWindow::onDrop(const function<void (vector<string>)>& callback) -> type& {
  state.onDrop = callback;
  return *this;
}

auto mWindow::onKeyPress(const function<void (signed)>& callback) -> type& {
  state.onKeyPress = callback;
  return *this;
}

auto mWindow::onKeyRelease(const function<void (signed)>& callback) -> type& {
  state.onKeyRelease = callback;
  return *this;
}

auto mWindow::onMove(const function<void ()>& callback) -> type& {
  state.onMove = callback;
  return *this;
}

auto mWindow::onSize(const function<void ()>& callback) -> type& {
  state.onSize = callback;
  return *this;
}

auto mWindow::remove(sMenuBar menuBar) -> type& {
  signal(remove, menuBar);
  menuBar->setParent();
  state.menuBar.reset();
  return *this;
}

auto mWindow::remove(sSizable sizable) -> type& {
  signal(remove, sizable);
  sizable->setParent();
  state.sizable.reset();
  return *this;
}

auto mWindow::remove(sStatusBar statusBar) -> type& {
  signal(remove, statusBar);
  statusBar->setParent();
  state.statusBar.reset();
  return *this;
}

auto mWindow::reset() -> type& {
  if(auto& menuBar = state.menuBar) remove(menuBar);
  if(auto& sizable = state.sizable) remove(sizable);
  if(auto& statusBar = state.statusBar) remove(statusBar);
  return *this;
}

auto mWindow::resizable() const -> bool {
  return state.resizable;
}

auto mWindow::setAlignment(Alignment alignment) -> type& {
  if(!alignment) alignment = {0.0, 0.0};
  auto workspace = Desktop::workspace();
  auto geometry = frameGeometry();
  int left = alignment.horizontal() * (workspace.width() - geometry.width());
  int top = alignment.vertical() * (workspace.height() - geometry.height());
  setFramePosition({left, top});
  return *this;
}

auto mWindow::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mWindow::setCentered(sWindow parent) -> type& {
  Geometry workspace = Desktop::workspace();
  Geometry parentGeometry = parent ? parent->frameGeometry() : workspace;
  Geometry geometry = frameGeometry();
  //center the window to its parent window ...
  int x = parentGeometry.x() + (parentGeometry.width()  - geometry.width())  / 2;
  int y = parentGeometry.y() + (parentGeometry.height() - geometry.height()) / 2;
  //try and keep the window onscreen ...
  if(x + geometry.width()  > workspace.width())  x = workspace.width()  - geometry.width();
  if(y + geometry.height() > workspace.height()) y = workspace.height() - geometry.height();
  if(x < workspace.x()) x = workspace.x();
  if(y < workspace.y()) y = workspace.y();
  return setFrameGeometry({x, y, geometry.width(), geometry.height()});
}

auto mWindow::setDismissable(bool dismissable) -> type& {
  state.dismissable = dismissable;
  signal(setDismissable, dismissable);
  return *this;
}

auto mWindow::setDroppable(bool droppable) -> type& {
  state.droppable = droppable;
  signal(setDroppable, droppable);
  return *this;
}

auto mWindow::setFrameGeometry(Geometry geometry) -> type& {
  Geometry margin = signal(frameMargin);
  return setGeometry({
    geometry.x() + margin.x(), geometry.y() + margin.y(),
    geometry.width() - margin.width(), geometry.height() - margin.height()
  });
}

auto mWindow::setFramePosition(Position position) -> type& {
  Geometry margin = signal(frameMargin);
  return setGeometry({
    position.x() + margin.x(), position.y() + margin.y(),
    state.geometry.width(), state.geometry.height()
  });
}

auto mWindow::setFrameSize(Size size) -> type& {
  Geometry margin = signal(frameMargin);
  return setGeometry({
    state.geometry.x(), state.geometry.y(),
    size.width() - margin.width(), size.height() - margin.height()
  });
}

auto mWindow::setFullScreen(bool fullScreen) -> type& {
  if(fullScreen != state.fullScreen) {
    state.fullScreen = fullScreen;
    signal(setFullScreen, fullScreen);
  }
  return *this;
}

auto mWindow::setGeometry(Geometry geometry) -> type& {
  state.geometry = geometry;
  signal(setGeometry, geometry);
  if(auto& sizable = state.sizable) sizable->setGeometry(sizable->geometry());
  return *this;
}

auto mWindow::setMaximized(bool maximized) -> type& {
  state.maximized = maximized;
  signal(setMaximized, maximized);
  return *this;
}

auto mWindow::setMaximumSize(Size size) -> type& {
  state.maximumSize = size;
  signal(setMaximumSize, size);
  return *this;
}

auto mWindow::setMinimized(bool minimized) -> type& {
  state.minimized = minimized;
  signal(setMinimized, minimized);
  return *this;
}

auto mWindow::setMinimumSize(Size size) -> type& {
  state.minimumSize = size;
  signal(setMinimumSize, size);
  return *this;
}

auto mWindow::setModal(bool modal) -> type& {
  state.modal = modal;
  if(modal) {
    Application::state().modal++;
  } else {
    Application::state().modal--;
    assert(Application::state().modal >= 0);
  }
  signal(setModal, modal);
  return *this;
}

auto mWindow::setPosition(Position position) -> type& {
  return setGeometry({
    position.x(), position.y(),
    state.geometry.width(), state.geometry.height()
  });
}

auto mWindow::setResizable(bool resizable) -> type& {
  state.resizable = resizable;
  signal(setResizable, resizable);
  return *this;
}

auto mWindow::setSize(Size size) -> type& {
  return setGeometry({
    state.geometry.x(), state.geometry.y(),
    size.width(), size.height()
  });
}

auto mWindow::setTitle(const string& title) -> type& {
  state.title = title;
  signal(setTitle, title);
  return *this;
}

auto mWindow::setVisible(bool visible) -> type& {
  mObject::setVisible(visible);
  if(auto& menuBar = state.menuBar) menuBar->setVisible(menuBar->visible());
  if(auto& sizable = state.sizable) sizable->setVisible(sizable->visible());
  if(auto& statusBar = state.statusBar) statusBar->setVisible(statusBar->visible());
  return *this;
}

auto mWindow::sizable() const -> Sizable {
  return state.sizable;
}

auto mWindow::statusBar() const -> StatusBar {
  return state.statusBar;
}

auto mWindow::title() const -> string {
  return state.title;
}

#endif
