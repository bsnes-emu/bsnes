void InputCaptureWindow::setup() {
  window = new Window;
  window->setObjectName("input-capture-window");
  window->setWindowTitle("Input Capture");

  layout = new QVBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(0);

  title = new QLabel;
  layout->addWidget(title);

  axisGroup = new QGroupBox("Mouse axes"); {
    axisLayout = new QHBoxLayout;
    axisLayout->setSpacing(Style::WidgetSpacing); {
      mouseAxisX = new QPushButton("X-axis");
      axisLayout->addWidget(mouseAxisX);

      mouseAxisY = new QPushButton("Y-axis");
      axisLayout->addWidget(mouseAxisY);
    }
    axisGroup->setLayout(axisLayout);
  }
  layout->addWidget(axisGroup);

  buttonGroup = new QGroupBox("Mouse buttons"); {
    buttonLayout = new QGridLayout;
    buttonLayout->setSpacing(Style::WidgetSpacing); {
      mouseButton[0] = new QPushButton("Left");
      buttonLayout->addWidget(mouseButton[0], 0, 0);

      mouseButton[1] = new QPushButton("Middle");
      buttonLayout->addWidget(mouseButton[1], 0, 1);

      mouseButton[2] = new QPushButton("Right");
      buttonLayout->addWidget(mouseButton[2], 0, 2);

      mouseButton[3] = new QPushButton("Extra 1");
      buttonLayout->addWidget(mouseButton[3], 1, 0);

      mouseButton[4] = new QPushButton("Extra 2");
      buttonLayout->addWidget(mouseButton[4], 1, 1);

      mouseButton[5] = new QPushButton("Extra 3");
      buttonLayout->addWidget(mouseButton[5], 1, 2);
    }
    buttonGroup->setLayout(buttonLayout);
  }
  layout->addWidget(buttonGroup);

  imageSpacer = new QWidget;
  imageSpacer->setFixedSize(Style::WidgetSpacing, Style::WidgetSpacing);
  layout->addWidget(imageSpacer);

  imageWidget = new InputImage;
  layout->addWidget(imageWidget, 0, Qt::AlignHCenter);

  spacer = new QWidget;
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout->addWidget(spacer);

  window->setLayout(layout);
  connect(mouseAxisX, SIGNAL(released()), this, SLOT(assignMouseX()));
  connect(mouseAxisY, SIGNAL(released()), this, SLOT(assignMouseY()));
  connect(mouseButton[0], SIGNAL(released()), this, SLOT(assignMouse0()));
  connect(mouseButton[1], SIGNAL(released()), this, SLOT(assignMouse1()));
  connect(mouseButton[2], SIGNAL(released()), this, SLOT(assignMouse2()));
  connect(mouseButton[3], SIGNAL(released()), this, SLOT(assignMouse3()));
  connect(mouseButton[4], SIGNAL(released()), this, SLOT(assignMouse4()));
  connect(mouseButton[5], SIGNAL(released()), this, SLOT(assignMouse5()));
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
    axisGroup->hide();
    buttonGroup->show();

    info << "Press any keyboard key / joypad button, or click the desired mouse button below to set assignment.";
  } else /*(activeObject->type == InputObject::Axis)*/ {
    axisGroup->show();
    buttonGroup->hide();

    info << "Move any joypad axis, or click the desired mouse axis below to set assignment.";
  }

  if(dynamic_cast<Joypad*>(activeObject->parent)) {
    imageSpacer->show();
    imageWidget->setFixedSize(600, 300);
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

void InputCaptureWindow::inputEvent(uint16_t code, bool forceAssign) {
  if(!activeObject) return;

  //input polling is global, need to block mouse actions that may be UI interactions.
  //custom controls on window allow mouse assignment instead.
  if(forceAssign == false) {
    InputCode::type_t type = InputCode::type(code);

    if(activeObject->type == InputObject::Button) {
      if(type == InputCode::MouseAxis
      || type == InputCode::MouseButton
      || type == InputCode::JoypadAxis
      ) return;
    } else /*(activeObject->type == InputObject::Axis)*/ {
      if(type == InputCode::KeyboardButton
      || type == InputCode::MouseAxis
      || type == InputCode::MouseButton
      || type == InputCode::JoypadButton
      ) return;
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

InputCaptureWindow::InputCaptureWindow() {
  activeObject = 0;
}

void InputCaptureWindow::Window::closeEvent(QCloseEvent*) {
  //window closed by user, cancel key assignment
  winInputCapture->activeObject = 0;
}

void InputCaptureWindow::InputImage::paintEvent(QPaintEvent*) {
  //currently, there is only an image available for the joypad.
  //in the future, this routine should determine which type of
  //image to draw via activeObject->parent's derived class type.
  QPainter painter(this);
  QPixmap pixmap(":/joypad.png");
  painter.drawPixmap(0, 0, pixmap);
}
