#include "stateselect.moc"
StateSelectWindow *stateSelectWindow;

void StateSelectWindow::setSlot(unsigned slot) {
  state.active = slot;
  hide();
  utility.showMessage(string("Quick state ", slot + 1, " selected."));
}

void StateSelectWindow::keyReleaseEvent(QKeyEvent *event) {
  switch(event->key()) {
    case Qt::Key_1: return setSlot(0);
    case Qt::Key_2: return setSlot(1);
    case Qt::Key_3: return setSlot(2);
    case Qt::Key_4: return setSlot(3);
    case Qt::Key_5: return setSlot(4);
    case Qt::Key_6: return setSlot(5);
    case Qt::Key_7: return setSlot(6);
    case Qt::Key_8: return setSlot(7);
    case Qt::Key_9: return setSlot(8);
    case Qt::Key_0: return setSlot(9);
  }
  return Window::keyReleaseEvent(event);
}

StateSelectWindow::StateSelectWindow() {
  setObjectName("state-select-window");
  setWindowTitle("State Selection");
  setGeometryString(&config().geometry.stateSelectWindow);

  layout = new QGridLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  setLayout(layout);

  slot[0] = new QPushButton("Slot &1");
  slot[1] = new QPushButton("Slot &2");
  slot[2] = new QPushButton("Slot &3");
  slot[3] = new QPushButton("Slot &4");
  slot[4] = new QPushButton("Slot &5");
  slot[5] = new QPushButton("Slot &6");
  slot[6] = new QPushButton("Slot &7");
  slot[7] = new QPushButton("Slot &8");
  slot[8] = new QPushButton("Slot &9");
  slot[9] = new QPushButton("Slot 1&0");
  for(unsigned i = 0; i < 10; i++) {
    layout->addWidget(slot[i], i / 5, i % 5);
    connect(slot[i], SIGNAL(released()), this, SLOT(slotClicked()));
  }
}

void StateSelectWindow::slotClicked() {
  QPushButton *s = (QPushButton*)sender();
  for(unsigned i = 0; i < 10; i++) {
    if(slot[i] == s) return setSlot(i);
  }
}
