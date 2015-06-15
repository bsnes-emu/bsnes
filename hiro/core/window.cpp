#if defined(Hiro_Window)

auto mWindow::allocate() -> pObject* {
  return new pWindow(*this);
}

auto mWindow::destruct() -> void {
  if(auto& layout = state.layout) layout->destruct();
  if(auto& menuBar = state.menuBar) menuBar->destruct();
  if(auto& statusBar = state.statusBar) statusBar->destruct();
  mObject::destruct();
}

//

auto mWindow::append(sLayout layout) -> type& {
  if(auto& layout = state.layout) remove(layout);
  state.layout = layout;
  layout->setGeometry(geometry().setPosition(0, 0));
  layout->setParent(this, 0);
  layout->setGeometry(geometry().setPosition(0, 0));
  signal(append, layout);
  return *this;
}

auto mWindow::append(sMenuBar menuBar) -> type& {
  if(auto& menuBar = state.menuBar) remove(menuBar);
  menuBar->setParent(this, 0);
  state.menuBar = menuBar;
  signal(append, menuBar);
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

auto mWindow::doClose() const -> void {
  if(state.onClose) return state.onClose();
}

auto mWindow::doDrop(lstring names) const -> void {
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

auto mWindow::layout() const -> shared_pointer<mLayout> {
  return state.layout;
}

auto mWindow::menuBar() const -> shared_pointer<mMenuBar> {
  return state.menuBar;
}

auto mWindow::modal() const -> bool {
  return state.modal;
}

auto mWindow::onClose(const function<void ()>& callback) -> type& {
  state.onClose = callback;
  return *this;
}

auto mWindow::onDrop(const function<void (lstring)>& callback) -> type& {
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

auto mWindow::remove(sLayout layout) -> type& {
  signal(remove, layout);
  layout->setParent();
  state.layout.reset();
  return *this;
}

auto mWindow::remove(sMenuBar menuBar) -> type& {
  signal(remove, menuBar);
  menuBar->reset();
  menuBar->setParent();
  state.menuBar.reset();
  return *this;
}

auto mWindow::remove(sStatusBar statusBar) -> type& {
  signal(remove, statusBar);
  statusBar->setParent();
  state.statusBar.reset();
  return *this;
}

auto mWindow::reset() -> type& {
  if(auto& layout = state.layout) remove(layout);
  if(auto& menuBar = state.menuBar) remove(menuBar);
  if(auto& statusBar = state.statusBar) remove(statusBar);
  return *this;
}

auto mWindow::resizable() const -> bool {
  return state.resizable;
}

auto mWindow::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mWindow::setCentered(sWindow parent) -> type& {
  Geometry workspace = parent ? parent->frameGeometry() : Desktop::workspace();
  Geometry geometry = frameGeometry();
  signed x = workspace.x();
  signed y = workspace.y();
  if(workspace.width() > geometry.width()) x += (workspace.width() - geometry.width()) / 2;
  if(workspace.height() > geometry.height()) y += (workspace.height() - geometry.height()) / 2;
  return setFrameGeometry({x, y, geometry.width(), geometry.height()});
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
  state.fullScreen = fullScreen;
  signal(setFullScreen, fullScreen);
  return *this;
}

auto mWindow::setGeometry(Geometry geometry) -> type& {
  state.geometry = geometry;
  signal(setGeometry, geometry);
  if(auto& layout = state.layout) {
    layout->setGeometry(geometry.setPosition(0, 0));
  }
  return *this;
}

auto mWindow::setModal(bool modal) -> type& {
  state.modal = modal;
  signal(setModal, modal);
  return *this;
}

auto mWindow::setPlacement(double x, double y) -> type& {
  x = max(0.0, min(1.0, x));
  y = max(0.0, min(1.0, y));
  auto workspace = Desktop::workspace();
  auto geometry = frameGeometry();
  signed left = x * (workspace.width() - geometry.width());
  signed top = y * (workspace.height() - geometry.height());
  setFramePosition({left, top});
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

auto mWindow::statusBar() const -> shared_pointer<mStatusBar> {
  return state.statusBar;
}

auto mWindow::title() const -> string {
  return state.title;
}

#endif
