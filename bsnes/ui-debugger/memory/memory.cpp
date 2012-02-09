#include "../base.hpp"
MemoryEditor *memoryEditor = nullptr;

MemoryEditor::MemoryEditor() {
  setTitle("Memory Editor");
  setGeometry({640, 64, 485, 255});

  gotoLabel.setText("Goto:");
  gotoAddress.setFont(application->monospaceFont);
  source.append("CPU-Bus", "APU-Bus", "VRAM", "OAM", "CGRAM");
  autoRefresh.setText("Auto");
  updateButton.setText("Update");
  editor.setFont(application->monospaceFont);
  editor.setColumns(16);
  editor.setRows(16);

  layout.setMargin(5);
  layout.append(controlLayout, {~0, 0}, 5);
    controlLayout.append(gotoLabel, {0, 0}, 5);
    controlLayout.append(gotoAddress, {100, 0}, 5);
    controlLayout.append(source, {0, 0}, 5);
    controlLayout.append(spacer, {~0, 0});
    controlLayout.append(autoRefresh, {0, 0}, 5);
    controlLayout.append(updateButton, {80, 0});
  layout.append(editor, {~0, ~0});
  append(layout);

  gotoAddress.onChange = gotoAddress.onActivate = [&] {
    editor.setOffset(hex(gotoAddress.text()));
    editor.update();
  };

  updateButton.onActivate = { &MemoryEditor::update, this };

  source.onChange = { &MemoryEditor::selectSource, this };
  editor.onRead = { &MemoryEditor::read, this };
  editor.onWrite = { &MemoryEditor::write, this };

  selectSource();
}

uint8_t MemoryEditor::read(unsigned addr) {
  if(SNES::cartridge.loaded() == false) return 0x00;
  if(source.selection() == 0) {
    return SNES::bus.read(addr & 0xffffff);
  }
  return 0x00;
}

void MemoryEditor::write(unsigned addr, uint8_t data) {
  if(SNES::cartridge.loaded() == false) return;
  if(source.selection() == 0) {
    SNES::cartridge.rom.write_protect(false);
    SNES::bus.write(addr & 0xffffff, data);
    SNES::cartridge.rom.write_protect(true);
    return;
  }
}

void MemoryEditor::selectSource() {
  editor.setOffset(0);
  switch(source.selection()) {
  case 0: editor.setLength(16 * 1024 * 1024); break;
  case 1: editor.setLength(64 * 1024); break;
  case 2: editor.setLength(64 * 1024); break;
  case 3: editor.setLength(544); break;
  case 4: editor.setLength(512); break;
  }
  update();
}

void MemoryEditor::update() {
  editor.update();
}
