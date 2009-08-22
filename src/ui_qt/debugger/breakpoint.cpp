BreakpointItem::BreakpointItem(unsigned id_) : id(id_) {
  layout = new QHBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(Style::WidgetSpacing);
  setLayout(layout);

  enabled = new QCheckBox;
  layout->addWidget(enabled);

  addr = new QLineEdit;
  addr->setMinimumWidth(80);
  layout->addWidget(addr);

  data = new QLineEdit;
  data->setMinimumWidth(40);
  layout->addWidget(data);

  mode = new QComboBox;
  mode->addItem("Exec");
  mode->addItem("Read");
  mode->addItem("Write");
  layout->addWidget(mode);

  source = new QComboBox;
  source->addItem("S-CPU bus");
  source->addItem("S-SMP bus");
  source->addItem("S-PPU VRAM");
  source->addItem("S-PPU OAM");
  source->addItem("S-PPU CGRAM");
  layout->addWidget(source);

  connect(enabled, SIGNAL(stateChanged(int)), this, SLOT(toggle()));
}

void BreakpointItem::toggle() {
  bool state = enabled->isChecked();

  if(state) {
    SNES::debugger.breakpoint[id].enabled = true;
    SNES::debugger.breakpoint[id].addr = strhex(addr->text().toUtf8().data()) & 0xffffff;
    SNES::debugger.breakpoint[id].data = strhex(data->text().toUtf8().data()) & 0xff;
    if(data->text().length() == 0) SNES::debugger.breakpoint[id].data = -1;
    SNES::debugger.breakpoint[id].mode = (SNES::Debugger::Breakpoint::Mode)mode->currentIndex();
    SNES::debugger.breakpoint[id].source = (SNES::Debugger::Breakpoint::Source)source->currentIndex();
    SNES::debugger.breakpoint[id].counter = 0;

    addr->setEnabled(false);
    data->setEnabled(false);
    mode->setEnabled(false);
    source->setEnabled(false);
  } else {
    SNES::debugger.breakpoint[id].enabled = false;

    addr->setEnabled(true);
    data->setEnabled(true);
    mode->setEnabled(true);
    source->setEnabled(true);
  }
}

void BreakpointEditor::setup() {
  window = new QWidget;
  window->setObjectName("breakpoint-editor");
  window->setWindowTitle("Breakpoint Editor");

  layout = new QVBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  window->setLayout(layout);

  for(unsigned n = 0; n < SNES::Debugger::Breakpoints; n++) {
    breakpoint[n] = new BreakpointItem(n);
    layout->addWidget(breakpoint[n]);
  }

  window->resize(0, 0);
}
