#include "input.moc"
InputSettingsWindow *inputSettingsWindow;

InputSettingsWindow::InputSettingsWindow() : activeObject(0) {
  layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(Style::WidgetSpacing);
  setLayout(layout);

  title = new QLabel("Input Configuration Editor");
  title->setProperty("class", "title");
  layout->addWidget(title);

  comboLayout = new QHBoxLayout;
  layout->addLayout(comboLayout);

  port = new QComboBox;
  port->addItem("Controller Port 1");
  port->addItem("Controller Port 2");
  port->addItem("User Interface");
  comboLayout->addWidget(port);

  device = new QComboBox;
  comboLayout->addWidget(device);

  selectionWidget = new QWidget;
  selectionWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout->addWidget(selectionWidget);

  selectionLayout = new QVBoxLayout;
  selectionLayout->setMargin(0);
  selectionLayout->setSpacing(Style::WidgetSpacing);
  selectionWidget->setLayout(selectionLayout);

  list = new QTreeWidget;
  list->setColumnCount(3);
  list->setHeaderLabels(QStringList() << "Hidden" << "Name" << "Assignment");
  list->setAllColumnsShowFocus(true);
  list->setRootIsDecorated(false);
  list->hideColumn(0);  //used for default sorting
  selectionLayout->addWidget(list);

  selectionControlLayout = new QHBoxLayout;
  selectionLayout->addLayout(selectionControlLayout);

  assignButton = new QPushButton("Assign");
  selectionControlLayout->addWidget(assignButton);

  assignAllButton = new QPushButton("Assign All");
  selectionControlLayout->addWidget(assignAllButton);

  unassignButton = new QPushButton("Unassign");
  selectionControlLayout->addWidget(unassignButton);

  unassignAllButton = new QPushButton("Unassign All");
  selectionControlLayout->addWidget(unassignAllButton);

  assignmentWidget = new QWidget;
  assignmentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  assignmentWidget->hide();
  layout->addWidget(assignmentWidget);

  assignmentLayout = new QVBoxLayout;
  assignmentLayout->setMargin(0);
  assignmentLayout->setSpacing(Style::WidgetSpacing);
  assignmentWidget->setLayout(assignmentLayout);

  assignmentLabel = new QLabel;
  assignmentLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  assignmentLabel->setAlignment(Qt::AlignTop);
  assignmentLabel->setFocusPolicy(Qt::StrongFocus);
  assignmentLabel->setFrameShape(QFrame::Panel);
  assignmentLabel->setFrameShadow(QFrame::Sunken);
  assignmentLayout->addWidget(assignmentLabel);

  assignmentControlLayout = new QHBoxLayout;
  assignmentLayout->addLayout(assignmentControlLayout);

  xaxisButton = new QPushButton("X-axis");
  assignmentControlLayout->addWidget(xaxisButton);

  yaxisButton = new QPushButton("Y-axis");
  assignmentControlLayout->addWidget(yaxisButton);

  spacer = new QWidget;
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  assignmentControlLayout->addWidget(spacer);

  helpButton = new QPushButton("Help");
  assignmentControlLayout->addWidget(helpButton);

  cancelButton = new QPushButton("Cancel Assignment");
  assignmentControlLayout->addWidget(cancelButton);

  connect(port, SIGNAL(currentIndexChanged(int)), this, SLOT(portChanged()));
  connect(device, SIGNAL(currentIndexChanged(int)), this, SLOT(reloadList()));

  connect(list, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(assign()));
  connect(list, SIGNAL(itemSelectionChanged()), this, SLOT(listChanged()));
  connect(assignButton, SIGNAL(released()), this, SLOT(assign()));
  connect(assignAllButton, SIGNAL(released()), this, SLOT(assignAll()));
  connect(unassignButton, SIGNAL(released()), this, SLOT(unassign()));
  connect(unassignAllButton, SIGNAL(released()), this, SLOT(unassignAll()));

  connect(xaxisButton, SIGNAL(released()), this, SLOT(assignXaxis()));
  connect(yaxisButton, SIGNAL(released()), this, SLOT(assignYaxis()));
  connect(helpButton, SIGNAL(released()), this, SLOT(showHelp()));
  connect(cancelButton, SIGNAL(released()), this, SLOT(cancelAssignment()));

  portChanged();
}

void InputSettingsWindow::syncUi() {
  QList<QTreeWidgetItem*> itemList = list->selectedItems();
  assignButton->setEnabled(itemList.count() == 1);
  assignAllButton->setEnabled(port->currentIndex() < 2);  //only allow for controllers, not GUI hotkeys
  unassignButton->setEnabled(itemList.count() == 1);
}

//when port combobox item is changed, device list needs to be repopulated
void InputSettingsWindow::portChanged() {
  disconnect(device, SIGNAL(currentIndexChanged(int)), this, SLOT(reloadList()));
  device->clear();

  unsigned index = port->currentIndex();
  for(unsigned i = 0; i < mapper().size(); i++) {
    if(mapper()[i]->category == index) {
      device->addItem(mapper()[i]->label);
    }
  }

  reloadList();
  connect(device, SIGNAL(currentIndexChanged(int)), this, SLOT(reloadList()));
}

//when device combobox item is changed, object list needs to be repopulated
void InputSettingsWindow::reloadList() {
  list->clear();
  list->setSortingEnabled(false);

  unsigned portIndex = port->currentIndex();
  unsigned deviceIndex = device->currentIndex();
  unsigned index = 0;
  for(unsigned i = 0; i < mapper().size(); i++) {
    if(mapper()[i]->category == portIndex) {
      if(index != deviceIndex) {
        index++;
        continue;
      }

      InputGroup &group = *(mapper()[i]);
      for(unsigned i = 0; i < group.size(); i++) {
        QTreeWidgetItem *item = new QTreeWidgetItem(list);
        item->setData(0, Qt::UserRole, QVariant(i));
        item->setText(0, string() << (1000000 + i));
        item->setText(1, group[i]->label);
        item->setText(2, group[i]->name);
      }

      activeGroup = &group;
      break;
    }
  }

  list->setSortingEnabled(true);
  list->sortByColumn(0, Qt::AscendingOrder);  //set default sorting on list change, overriding user setting
  list->resizeColumnToContents(1);  //shrink name column
  syncUi();
}

void InputSettingsWindow::listChanged() {
  syncUi();
}

//===================
//assignment routines
//===================

void InputSettingsWindow::beginAssignment() {
  settingsWindow->list->setEnabled(false);
  port->setEnabled(false);
  device->setEnabled(false);
  selectionWidget->hide();
  assignmentWidget->show();
}

void InputSettingsWindow::showHelp() {
  string message;

  message << "<b>Digital Button Assignment:</b><br>";
  message << "To assign, do any one of the following:<br>";
  message << "&bull; press any keyboard key<br>";
  message << "&bull; click any mouse button inside the capture box<br>";
  message << "&bull; move any joypad hat in the desired direction<br>";
  message << "&bull; move any joypad axis in the desired direction<br>";
  message << "&bull; press any joypad button<br><br>";
  message << "For multi-key assignment, hold any combination of these keys down first:<br>";
  message << "&bull; Shift, Control, Alt, Super";
  message << "<br><br>";

  message << "<b>Analog Axis Assignment:</b><br>";
  message << "To assign, do any one of the following:<br>";
  message << "&bull; click the desired mouse axis button<br>";
  message << "&bull; move any joypad axis in either direction";

  audio.clear();
  QMessageBox::information(settingsWindow, "Input Assignment Help", message);
}

void InputSettingsWindow::assignObject(MappedInput *object) {
  //flush any pending events to prevent instantaneous assignment of eg mouse buttons
  activeObject = 0;
  activeMouse = 0;
  mapper().poll();

  activeObject = object;

  string label;
  label << "<b>" << port->currentText().toUtf8().constData() << " :: ";
  label << device->currentText().toUtf8().constData() << " :: ";
  label << activeObject->label << "</b><br>";

  if(dynamic_cast<DigitalInput*>(activeObject)) {
    xaxisButton->setVisible(false);
    yaxisButton->setVisible(false);
    label << "<i>Digital Button Assignment</i>";
  } else if(dynamic_cast<AnalogInput*>(activeObject)) {
    xaxisButton->setVisible(true);
    yaxisButton->setVisible(true);
    label << "<i>Analog Axis Assignment</i>";
  }

  if(dynamic_cast<Controllers::Gamepad*>(activeGroup)
  || dynamic_cast<Controllers::Asciipad*>(activeGroup)) {
    assignmentLabel->setStyleSheet(
      "background-image: url(:/joypad.png);"
      "background-position: bottom center;"
      "background-repeat: none;"
    );
  } else {
    assignmentLabel->setStyleSheet("");
  }

  assignmentLabel->setText(label);
  assignmentLabel->setFocus();
}

void InputSettingsWindow::endAssignment() {
  activeObject = 0;

  if(multiAssign == false || ++multiAssignIndex >= activeGroup->size()) {
    mapper().bind();
    reloadList();

    assignmentWidget->hide();
    selectionWidget->show();
    settingsWindow->list->setEnabled(true);
    port->setEnabled(true);
    device->setEnabled(true);
  } else {
    assignObject((*activeGroup)[multiAssignIndex]);
  }
}

void InputSettingsWindow::assign() {
  if(activeObject) return;
  multiAssign = false;

  QTreeWidgetItem *item = list->currentItem();
  if(!item) return;
  unsigned index = item->data(0, Qt::UserRole).toUInt();

  beginAssignment();
  assignObject((*activeGroup)[index]);
}

void InputSettingsWindow::assignAll() {
  if(activeObject) return;
  multiAssign = true;

  beginAssignment();
  assignObject((*activeGroup)[multiAssignIndex = 0]);
}

void InputSettingsWindow::unassign() {
  QTreeWidgetItem *item = list->currentItem();
  if(!item) return;
  unsigned index = item->data(0, Qt::UserRole).toUInt();
  MappedInput *object = (*activeGroup)[index];
  object->name = "None";
  mapper().bind();
  reloadList();
}

void InputSettingsWindow::unassignAll() {
  for(unsigned i = 0; i < activeGroup->size(); i++) {
    MappedInput *object = (*activeGroup)[i];
    object->name = "None";
  }
  mapper().bind();
  reloadList();
}

void InputSettingsWindow::cancelAssignment() {
  multiAssign = false;
  endAssignment();
}

void InputSettingsWindow::setAssignment(string name) {
  if(multiAssign == true) {
    //make sure this mapping was not previously used
    for(unsigned i = 0; i < multiAssignIndex; i++) {
      if(name == (*activeGroup)[i]->name) return;
    }
  }

  activeObject->name = name;
  endAssignment();
}

void InputSettingsWindow::assignXaxis() {
  setAssignment(Scancode::encode(mouse(activeMouse).axis(0)));
}

void InputSettingsWindow::assignYaxis() {
  setAssignment(Scancode::encode(mouse(activeMouse).axis(1)));
}

void InputSettingsWindow::inputEvent(uint16_t scancode) {
  if(dynamic_cast<DigitalInput*>(activeObject)) {
    digitalInputEvent(scancode);
  } else if(dynamic_cast<AnalogInput*>(activeObject)) {
    analogInputEvent(scancode);
  }
}

void InputSettingsWindow::digitalInputEvent(uint16_t scancode) {
  if(!isActiveWindow() || isMinimized()) return;

  if(Keyboard::isAnyKey(scancode) && mapper().state(scancode)) {
    for(unsigned i = 0; i < Keyboard::Count; i++) {
      //don't map escape key, as it is reserved by the user interface
      if(scancode == keyboard(i)[Keyboard::Escape]) return;
    }

    setAssignment(string() << mapper().modifierString() << Scancode::encode(scancode));
  } else if(Mouse::isAnyButton(scancode) && mapper().state(scancode)) {
    //ensure button was clicked inside assignment label box
    QRect windowRect = settingsWindow->geometry();
    QRect splitterRect = settingsWindow->splitter->geometry();
    QRect panelRect = settingsWindow->panel->geometry();
    QRect captureRect = assignmentWidget->geometry();
    QRect widgetRect = assignmentLabel->geometry();
    unsigned wx = windowRect.x() + splitterRect.x() + panelRect.x() + captureRect.x();
    unsigned wy = windowRect.y() + splitterRect.y() + panelRect.y() + captureRect.y();
    unsigned px = QCursor::pos().x();
    unsigned py = QCursor::pos().y();

    if(px < wx || px >= wx + widgetRect.size().width()) return;
    if(py < wy || py >= wy + widgetRect.size().height()) return;

    setAssignment(string() << mapper().modifierString() << Scancode::encode(scancode));
  } else if(Joypad::isAnyHat(scancode)) {
    int16_t state = mapper().state(scancode);
    string position;
    if(state == Joypad::HatUp) position = ".Up";
    if(state == Joypad::HatDown) position = ".Down";
    if(state == Joypad::HatLeft) position = ".Left";
    if(state == Joypad::HatRight) position = ".Right";
    if(position == "") return;

    setAssignment(string() << mapper().modifierString() << Scancode::encode(scancode) << position);
  } else if(Joypad::isAnyAxis(scancode) && mapper().distance(scancode) > 64) {
    if(mapper().calibrated == false) {
      MappedInput *temp = activeObject;
      activeObject = 0;
      mapper().calibrate();
      activeObject = temp;
    }

    if(mapper().isTrigger[Joypad::numberDecode(scancode)][Joypad::axisDecode(scancode)] == false) {
      int16_t state = mapper().state(scancode);
      string position;
      if(state < -24576) position = ".Lo";
      if(state > +24576) position = ".Hi";
      if(position == "") return;

      setAssignment(string() << mapper().modifierString() << Scancode::encode(scancode) << position);
    } else {
      int16_t state = mapper().state(scancode);
      if(state >= 0) return;

      setAssignment(string() << mapper().modifierString() << Scancode::encode(scancode) << ".Trigger");
    }
  } else if(Joypad::isAnyButton(scancode) && mapper().state(scancode)) {
    setAssignment(string() << mapper().modifierString() << Scancode::encode(scancode));
  }
}

void InputSettingsWindow::analogInputEvent(uint16_t scancode) {
  if(!isActiveWindow() || isMinimized()) return;

  if(Mouse::isAnyButton(scancode)) {
    activeMouse = Mouse::numberDecode(scancode);
  } else if(Joypad::isAnyAxis(scancode) && mapper().distance(scancode) > 64) {
    if(mapper().calibrated == false) {
      MappedInput *temp = activeObject;
      activeObject = 0;
      mapper().calibrate();
      activeObject = temp;
    }

    if(mapper().isTrigger[Joypad::numberDecode(scancode)][Joypad::axisDecode(scancode)] == false) {
      int16_t state = mapper().state(scancode);
      if(state < -24576 || state > +24576) {
        setAssignment(string() << Scancode::encode(scancode));
      }
    }
  }
}
