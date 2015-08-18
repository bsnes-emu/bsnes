#if defined(Hiro_Window)

namespace hiro {

auto pWindow::construct() -> void {
  qtWindow = new QtWindow(*this);
  qtWindow->setWindowTitle(" ");

  //if program was given a name, try and set the window taskbar icon to a matching pixmap image
  if(auto name = Application::state.name) {
    if(file::exists({userpath(), ".local/share/icons/", name, ".png"})) {
      qtWindow->setWindowIcon(QIcon(string{userpath(), ".local/share/icons/", name, ".png"}));
    } else if(file::exists({"/usr/local/share/pixmaps/", name, ".png"})) {
      qtWindow->setWindowIcon(QIcon(string{"/usr/local/share/pixmaps/", name, ".png"}));
    } else if(file::exists({"/usr/share/pixmaps/", name, ".png"})) {
      qtWindow->setWindowIcon(QIcon(string{"/usr/share/pixmaps/", name, ".png"}));
    }
  }

  qtLayout = new QVBoxLayout(qtWindow);
  qtLayout->setMargin(0);
  qtLayout->setSpacing(0);
  qtWindow->setLayout(qtLayout);

  qtMenuBar = new QMenuBar(qtWindow);
  qtMenuBar->setVisible(false);
  qtLayout->addWidget(qtMenuBar);

  qtContainer = new QWidget(qtWindow);
  qtContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  qtContainer->setVisible(true);
  qtLayout->addWidget(qtContainer);

  qtStatusBar = new QStatusBar(qtWindow);
  qtStatusBar->setSizeGripEnabled(true);
  qtStatusBar->setVisible(false);
  qtLayout->addWidget(qtStatusBar);

  setBackgroundColor(state().backgroundColor);
  setDroppable(state().droppable);
  setGeometry(state().geometry);
  setResizable(state().resizable);
  setTitle(state().title);
}

auto pWindow::destruct() -> void {
  delete qtStatusBar;
  delete qtContainer;
  delete qtMenuBar;
  delete qtLayout;
  delete qtWindow;
}

auto pWindow::append(sLayout layout) -> void {
}

auto pWindow::append(sMenuBar menuBar) -> void {
}

auto pWindow::append(sStatusBar statusBar) -> void {
}

auto pWindow::focused() const -> bool {
  return qtWindow->isActiveWindow() && !qtWindow->isMinimized();
}

auto pWindow::frameMargin() const -> Geometry {
  if(state().fullScreen) return {
    0, _menuHeight(),
    0, _menuHeight() + _statusHeight()
  };
  return {
    settings->geometry.frameX,
    settings->geometry.frameY + _menuHeight(),
    settings->geometry.frameWidth,
    settings->geometry.frameHeight + _menuHeight() + _statusHeight()
  };
}

auto pWindow::remove(sLayout layout) -> void {
}

auto pWindow::remove(sMenuBar menuBar) -> void {
  //QMenuBar::removeMenu() does not exist
  //qtMenu->clear();
  //for(auto& menu : window.state.menu) append(menu);
}

auto pWindow::remove(sStatusBar statusBar) -> void {
}

/*
  //orphan() destroys and recreates widgets (to disassociate them from their parent);
  //attempting to create widget again after QApplication::quit() crashes libQtGui
  if(qtApplication) widget.p.orphan();
}
*/

auto pWindow::setBackgroundColor(Color color) -> void {
  if(color) {
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(color.red(), color.green(), color.blue() /*, color.alpha() */));
    qtContainer->setPalette(palette);
    qtContainer->setAutoFillBackground(true);
    //translucency results are very unpleasant without a compositor; so disable for now
    //qtWindow->setAttribute(Qt::WA_TranslucentBackground, color.alpha() != 255);
  }
}

auto pWindow::setDroppable(bool droppable) -> void {
  qtWindow->setAcceptDrops(droppable);
}

auto pWindow::setEnabled(bool enabled) -> void {
}

auto pWindow::setFocused() -> void {
  qtWindow->raise();
  qtWindow->activateWindow();
}

auto pWindow::setFullScreen(bool fullScreen) -> void {
  if(!fullScreen) {
    setResizable(state().resizable);
    qtWindow->showNormal();
    qtWindow->adjustSize();
  } else {
    qtLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    qtContainer->setFixedSize(Desktop::size().width() - frameMargin().width(), Desktop::size().height() - frameMargin().height());
    qtWindow->showFullScreen();
  }
}

auto pWindow::setGeometry(Geometry geometry) -> void {
  lock();
  Application::processEvents();
  QApplication::syncX();

  setResizable(state().resizable);
  qtWindow->move(geometry.x() - frameMargin().x(), geometry.y() - frameMargin().y());
  //qtWindow->adjustSize() fails if larger than 2/3rds screen size
  qtWindow->resize(qtWindow->sizeHint());
  if(state().resizable) {
    //required to allow shrinking window from default size
    qtWindow->setMinimumSize(1, 1);
    qtContainer->setMinimumSize(1, 1);
  }

//  for(auto& layout : window.state.layout) {
//    geometry.x = geometry.y = 0;
//    layout.setGeometry(geometry);
//  }
  unlock();
}

auto pWindow::setModal(bool modal) -> void {
  if(modal) {
    //windowModality can only be enabled while window is invisible
    setVisible(false);
    qtWindow->setWindowModality(Qt::ApplicationModal);
    setVisible(true);
    while(!Application::state.quit && state().modal) {
      if(Application::state.onMain) {
        Application::doMain();
      } else {
        usleep(20 * 1000);
      }
      Application::processEvents();
    }
    qtWindow->setWindowModality(Qt::NonModal);
  }
}

auto pWindow::setResizable(bool resizable) -> void {
  if(resizable) {
    qtLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    qtContainer->setMinimumSize(state().geometry.width(), state().geometry.height());
  } else {
    qtLayout->setSizeConstraint(QLayout::SetFixedSize);
    qtContainer->setFixedSize(state().geometry.width(), state().geometry.height());
  }
  qtStatusBar->setSizeGripEnabled(resizable);
}

auto pWindow::setTitle(const string& text) -> void {
  qtWindow->setWindowTitle(QString::fromUtf8(text));
}

auto pWindow::setVisible(bool visible) -> void {
  lock();
  qtWindow->setVisible(visible);
  if(visible) {
    _updateFrameGeometry();
    setGeometry(state().geometry);
  }
  unlock();
}

/*
  if(widget.font().empty() && !window.state.widgetFont.empty()) {
    widget.setFont(window.state.widgetFont);
  }
  if(widget.font().empty()) widget.p.setFont(Font::sans(8));
  if(GetParentWidget(&widget)) {
    widget.p.qtWidget->setParent(GetParentWidget(&widget)->p.container(widget));
  } else {
    widget.p.qtWidget->setParent(qtContainer);
  }
  widget.setVisible(widget.visible());
}
*/

auto pWindow::_append(mWidget& widget) -> void {
  if(auto self = widget.self()) {
    self->qtWidget->setParent(qtContainer);
  }
}

auto pWindow::_menuHeight() const -> signed {
  return qtMenuBar->isVisible() ? settings->geometry.menuHeight : 0;
}

auto pWindow::_statusHeight() const -> signed {
  return qtStatusBar->isVisible() ? settings->geometry.statusHeight : 0;
}

auto pWindow::_updateFrameGeometry() -> void {
  pApplication::syncX();
  QRect border = qtWindow->frameGeometry();
  QRect client = qtWindow->geometry();

  settings->geometry.frameX = client.x() - border.x();
  settings->geometry.frameY = client.y() - border.y();
  settings->geometry.frameWidth = border.width() - client.width();
  settings->geometry.frameHeight = border.height() - client.height();

  if(qtMenuBar->isVisible()) {
    pApplication::syncX();
    settings->geometry.menuHeight = qtMenuBar->height();
  }

  if(qtStatusBar->isVisible()) {
    pApplication::syncX();
    settings->geometry.statusHeight = qtStatusBar->height();
  }

  settings->save();
}

auto QtWindow::closeEvent(QCloseEvent* event) -> void {
  event->ignore();
  if(p.state().onClose) p.self().doClose();
  else p.self().setVisible(false);
  if(p.state().modal && !p.self().visible()) p.self().setModal(false);
}

auto QtWindow::moveEvent(QMoveEvent* event) -> void {
  if(!p.locked() && !p.state().fullScreen && p.qtWindow->isVisible()) {
    p.state().geometry.setX(p.state().geometry.x() + event->pos().x() - event->oldPos().x());
    p.state().geometry.setY(p.state().geometry.y() + event->pos().y() - event->oldPos().y());
  }

  if(!p.locked()) {
    p.self().doMove();
  }
}

auto QtWindow::dragEnterEvent(QDragEnterEvent* event) -> void {
  if(event->mimeData()->hasUrls()) {
    event->acceptProposedAction();
  }
}

auto QtWindow::dropEvent(QDropEvent* event) -> void {
  if(auto paths = DropPaths(event)) p.self().doDrop(paths);
}

auto QtWindow::keyPressEvent(QKeyEvent* event) -> void {
//  Keyboard::Keycode sym = Keysym(event->nativeVirtualKey());
//  if(sym != Keyboard::Keycode::None && self.window.onKeyPress) self.window.onKeyPress(sym);
}

auto QtWindow::keyReleaseEvent(QKeyEvent* event) -> void {
//  Keyboard::Keycode sym = Keysym(event->nativeVirtualKey());
//  if(sym != Keyboard::Keycode::None && self.window.onKeyRelease) self.window.onKeyRelease(sym);
}

auto QtWindow::resizeEvent(QResizeEvent*) -> void {
  if(!p.locked() && !p.state().fullScreen && p.qtWindow->isVisible()) {
    p.state().geometry.setWidth(p.qtContainer->geometry().width());
    p.state().geometry.setHeight(p.qtContainer->geometry().height());
  }

  if(auto& layout = p.state().layout) {
    layout->setGeometry(p.self().geometry().setPosition(0, 0));
  }

  if(!p.locked()) {
    p.self().doSize();
  }
}

auto QtWindow::sizeHint() const -> QSize {
  unsigned width = p.state().geometry.width();
  unsigned height = p.state().geometry.height();
  if(p.qtMenuBar->isVisible()) height += settings->geometry.menuHeight;
  if(p.qtStatusBar->isVisible()) height += settings->geometry.statusHeight;
  return QSize(width, height);
}

}

#endif
