MemoryEditor memoryEditor;

void MemoryEditor::create() {
  Window::create(0, 0, 256, 256, "Memory Editor");
  application.addWindow(this, "Debugger.MemoryEditor", "192,192");

  unsigned x = 5, y = 5;
  editor.create(*this, x, y, 475, 220); x += 475 + 5;
  editor.setFont(application.monospaceFont);
  editor.setColumns(16);
  editor.setRows(16);

  sourceBox.create(*this, x, y, 80, Style::ComboBoxHeight); y += Style::ComboBoxHeight;
  sourceBox.addItem("CPU");
  sourceBox.addItem("APU");
  sourceBox.addItem("VRAM");
  sourceBox.addItem("OAM");
  sourceBox.addItem("CGRAM");

  gotoBox.create(*this, x, y, 80, Style::TextBoxHeight); y += Style::TextBoxHeight;

  refreshButton.create(*this, x, y, 80, Style::ButtonHeight, "Refresh"); y += Style::ButtonHeight;

  setGeometry(0, 0, 570, 230);

  onClose = []() {
    debugger.showMemoryEditor.setChecked(false);
    return true;
  };

  editor.onRead = { &MemoryEditor::read, this };
  editor.onWrite = { &MemoryEditor::write, this };

  sourceBox.onChange = []() {
    switch(memoryEditor.sourceBox.selection()) {
      case 0: memoryEditor.setSource(SNES::Debugger::MemorySource::CPUBus); break;
      case 1: memoryEditor.setSource(SNES::Debugger::MemorySource::APURAM); break;
      case 2: memoryEditor.setSource(SNES::Debugger::MemorySource::VRAM); break;
      case 3: memoryEditor.setSource(SNES::Debugger::MemorySource::OAM); break;
      case 4: memoryEditor.setSource(SNES::Debugger::MemorySource::CGRAM); break;
    }
  };

  gotoBox.onChange = []() {
    unsigned addr = hex(memoryEditor.gotoBox.text());
    memoryEditor.editor.setOffset(addr % memoryEditor.size);
    memoryEditor.editor.update();
  };

  refreshButton.onTick = []() {
    memoryEditor.editor.update();
  };

  setSource(SNES::Debugger::MemorySource::CPUBus);
}

void MemoryEditor::setSource(SNES::Debugger::MemorySource source_) {
  switch(source = source_) {
    case SNES::Debugger::MemorySource::CPUBus: size = 1 << 24; break;
    case SNES::Debugger::MemorySource::APURAM: size = 1 << 16; break;
    case SNES::Debugger::MemorySource::VRAM:   size = 1 << 16; break;
    case SNES::Debugger::MemorySource::OAM:    size = 544; break;
    case SNES::Debugger::MemorySource::CGRAM:  size = 512; break;
  }
  editor.setSize(size);
  editor.setOffset(0);
  editor.update();
}

uint8_t MemoryEditor::read(unsigned addr) {
  if(SNES::cartridge.loaded() == false) return 0x00;
  return SNES::debugger.read(source, addr % size);
}

void MemoryEditor::write(unsigned addr, uint8_t data) {
  if(SNES::cartridge.loaded() == false) return;
  SNES::debugger.write(source, addr % size, data);
}
