void MemoryEditor::setup() {
  window = new QWidget;
  window->setObjectName("memory-editor");
  window->setWindowTitle("Memory Editor");

  layout = new QVBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  window->setLayout(layout);

  controls = new QHBoxLayout;
  controls->setSpacing(Style::WidgetSpacing);
  layout->addLayout(controls);

  spacer = new QWidget;
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  controls->addWidget(spacer);

  addr = new QLineEdit;
  addr->setFixedWidth(80);
  controls->addWidget(addr);

  source = new QComboBox;
  source->addItem("S-CPU bus");
  source->addItem("S-APU bus");
  source->addItem("S-PPU VRAM");
  source->addItem("S-PPU OAM");
  source->addItem("S-PPU CGRAM");
  controls->addWidget(source);

  refresh = new QPushButton("Refresh");
  controls->addWidget(refresh);

  autoUpdate = new QCheckBox("Auto update");
  autoUpdate->setEnabled(false);
  controls->addWidget(autoUpdate);

  editor = new QTextEdit;
  editor->setFont(QFont(Style::Monospace));
  layout->addWidget(editor);

  window->setMinimumSize(425, 325);
  window->resize(0, 0);
  connect(refresh, SIGNAL(released()), this, SLOT(refreshView()));
}

void MemoryEditor::syncUi() {
  if(SNES::cartridge.loaded() == false) {
    editor->setPlainText("");
    refresh->setEnabled(false);
  } else {
    refresh->setEnabled(true);
  }
}

void MemoryEditor::refreshView() {
  string s;
  unsigned offset = strhex(addr->text().toUtf8().data());
  unsigned mode = source->currentIndex();

  for(unsigned y = 0; y < 16; y++) {
    char t[16];
    sprintf(t, "%.6x ", offset & 0xffffff);
    s << t;
    for(unsigned x = 0; x < 16; x++) {
      uint8 data = SNES::debugger.read((SNES::Debugger::MemorySource)mode, offset++);
      sprintf(t, "%.2x", data);
      s << t;
      if(x != 15) s << " ";
    }
    if(y != 15) s << "\n";
  }

  editor->setPlainText(utf8() << s);
}
