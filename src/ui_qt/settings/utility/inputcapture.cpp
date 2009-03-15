void InputCaptureWindow::setup() {
  window = new Window;
  window->setObjectName("input-capture-window");
  window->setWindowTitle("Input Capture");

  layout = new QVBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(0);

  hlayout = new QHBoxLayout;
  hlayout->setSpacing(Style::WidgetSpacing); {
    title = new QLabel;
    hlayout->addWidget(title, 0, Qt::AlignTop);

    mouseAxes = new QPushButton("  Assign Mouse Axis  ");
    mouseAxes->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mouseAxisMenu = new QMenu;
      mouseAxisX = mouseAxisMenu->addAction("X-axis");
      mouseAxisY = mouseAxisMenu->addAction("Y-axis");
    mouseAxes->setMenu(mouseAxisMenu);
    hlayout->addWidget(mouseAxes, 0, Qt::AlignTop);

    mouseButtons = new QPushButton("  Assign Mouse Button  ");
    mouseButtons->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mouseButtonMenu = new QMenu;
      mouseButton[0] = mouseButtonMenu->addAction("Button 0 (left)");
      mouseButton[1] = mouseButtonMenu->addAction("Button 1 (middle)");
      mouseButton[2] = mouseButtonMenu->addAction("Button 2 (right)");
      mouseButtonMenu->addSeparator();
      mouseButton[3] = mouseButtonMenu->addAction("Button 3");
      mouseButton[4] = mouseButtonMenu->addAction("Button 4");
      mouseButton[5] = mouseButtonMenu->addAction("Button 5");
      mouseButton[6] = mouseButtonMenu->addAction("Button 6");
      mouseButton[7] = mouseButtonMenu->addAction("Button 7");
    mouseButtons->setMenu(mouseButtonMenu);
    hlayout->addWidget(mouseButtons, 0, Qt::AlignTop);
  }
  layout->addLayout(hlayout);

  imageSpacer = new QWidget;
  imageSpacer->setFixedSize(Style::WidgetSpacing, Style::WidgetSpacing);
  layout->addWidget(imageSpacer);

  imageWidget = new ImageWidget;
  layout->addWidget(imageWidget, 0, Qt::AlignHCenter);

  spacer = new QWidget;
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout->addWidget(spacer);

  window->setLayout(layout);
  connect(mouseAxisX, SIGNAL(triggered()), this, SLOT(assignMouseX()));
  connect(mouseAxisY, SIGNAL(triggered()), this, SLOT(assignMouseY()));
  connect(mouseButton[0], SIGNAL(triggered()), this, SLOT(assignMouse0()));
  connect(mouseButton[1], SIGNAL(triggered()), this, SLOT(assignMouse1()));
  connect(mouseButton[2], SIGNAL(triggered()), this, SLOT(assignMouse2()));
  connect(mouseButton[3], SIGNAL(triggered()), this, SLOT(assignMouse3()));
  connect(mouseButton[4], SIGNAL(triggered()), this, SLOT(assignMouse4()));
  connect(mouseButton[5], SIGNAL(triggered()), this, SLOT(assignMouse5()));
  connect(mouseButton[6], SIGNAL(triggered()), this, SLOT(assignMouse6()));
  connect(mouseButton[7], SIGNAL(triggered()), this, SLOT(assignMouse7()));
}

void InputCaptureWindow::activate(InputObject *object) {
  window->hide();

  utf8 info;
  info << "<b>ID:</b> ";
  if(object->parent) {
    InputDevice *device = dynamic_cast<InputDevice*>(object->parent);
    if(device) info << "Controller port " << (int)(device->port + 1) << " <b>::</b> ";
    else info << "User interface <b>::</b> ";
    info << object->parent->name << " <b>::</b> ";
  }
  info << object->name << "<br>";

  activeObject = object;
  if(activeObject->type == InputObject::Button) {
    mouseAxes->hide();
    mouseButtons->show();

    info << "Press any key or button to assign to this ID.";
  } else /*(activeObject->type == InputObject::Axis)*/ {
    mouseAxes->show();
    mouseButtons->hide();

    info << "Move any axis to assign to this ID.";
  }

  if(dynamic_cast<Joypad*>(activeObject->parent)) {
    imageSpacer->show();
    imageWidget->setFixedSize(480, 210);
    imageWidget->show();
  } else {
    imageSpacer->hide();
    imageWidget->hide();
  }

  title->setText(info);
  window->resize(0, 0);  //shrink window as much as possible (visible widgets will forcefully increase size)
  window->show();
  window->resize(0, 0);

  static bool firstShow = true;
  if(firstShow == true) {
    firstShow = false;
    utility.centerWindow(window);
  }

  application.processEvents();
  window->activateWindow();
  window->raise();
}

void InputCaptureWindow::inputEvent(uint16_t code, bool forceAssign /* = false */) {
  if(!activeObject) return;

  //input polling is global, need to block mouse actions that may be UI interactions.
  //custom controls on window allow mouse assignment instead.
  if(forceAssign == false) {
    InputCode::type_t type = InputCode::type(code);
    int16_t state = inputManager.state(code);

    if(activeObject->type == InputObject::Axis) {
      if(type == InputCode::KeyboardButton
      || type == InputCode::MouseAxis
      || type == InputCode::MouseButton
      || type == InputCode::JoypadButton
      ) return;

      //only capture when axis is positioned at least 50% toward a given direction
      if(abs(state) < 16384) return;
    }

    if(activeObject->type == InputObject::Button) {
      if(type == InputCode::MouseAxis
      || type == InputCode::MouseButton
      || type == InputCode::JoypadAxis
      ) return;

      //only capture on button press, not release
      if(state == false) return;
    }
  }

  if(window->isActiveWindow()) {
    activeObject->id = nall::input_find(code);
    winInputSettings->updateList();
    inputManager.bind();
    activeObject = 0;
    window->hide();
  }
}

void InputCaptureWindow::assignMouseX() { inputEvent(mouse::x, true); }
void InputCaptureWindow::assignMouseY() { inputEvent(mouse::y, true); }
void InputCaptureWindow::assignMouse0() { inputEvent(mouse::button + 0, true); }
void InputCaptureWindow::assignMouse1() { inputEvent(mouse::button + 1, true); }
void InputCaptureWindow::assignMouse2() { inputEvent(mouse::button + 2, true); }
void InputCaptureWindow::assignMouse3() { inputEvent(mouse::button + 3, true); }
void InputCaptureWindow::assignMouse4() { inputEvent(mouse::button + 4, true); }
void InputCaptureWindow::assignMouse5() { inputEvent(mouse::button + 5, true); }
void InputCaptureWindow::assignMouse6() { inputEvent(mouse::button + 6, true); }
void InputCaptureWindow::assignMouse7() { inputEvent(mouse::button + 7, true); }

InputCaptureWindow::InputCaptureWindow() {
  activeObject = 0;
}

void InputCaptureWindow::Window::closeEvent(QCloseEvent*) {
  //window closed by user, cancel key assignment
  winInputCapture->activeObject = 0;
}

void InputCaptureWindow::ImageWidget::paintEvent(QPaintEvent*) {
  //currently, there is only an image available for the joypad.
  //in the future, this routine should determine which type of
  //image to draw via activeObject->parent's derived class type.
  QPainter painter(this);
  QPixmap pixmap(":/joypad.png");
  painter.drawPixmap(0, 0, pixmap);
}
