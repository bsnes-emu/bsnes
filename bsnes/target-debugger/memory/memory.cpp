#include "../base.hpp"
MemoryEditor *memoryEditor = nullptr;

MemoryEditor::MemoryEditor() {
  setTitle("Memory Editor");
  setGeometry({128, 128, 485, 255});

  gotoLabel.setText("Goto:");
  gotoAddress.setFont(application->monospaceFont);
  source.append("CPU-Bus", "APU-Bus", "VRAM", "OAM", "CGRAM");
  exportMemory.setText("Export");
  autoUpdate.setText("Auto");
  update.setText("Update");
  editor.setFont(application->monospaceFont);
  editor.setColumns(16);
  editor.setRows(16);

  layout.setMargin(5);
  layout.append(controlLayout, {~0, 0}, 5);
    controlLayout.append(gotoLabel, {0, 0}, 5);
    controlLayout.append(gotoAddress, {50, 0}, 5);
    controlLayout.append(source, {0, 0}, 5);
    controlLayout.append(exportMemory, {80, 0}, 5);
    controlLayout.append(spacer, {~0, 0});
    controlLayout.append(autoUpdate, {0, 0}, 5);
    controlLayout.append(update, {80, 0});
  layout.append(editor, {~0, ~0});
  append(layout);

  gotoAddress.onChange = gotoAddress.onActivate = [&] {
    editor.setOffset(hex(gotoAddress.text()));
    editor.update();
  };

  update.onActivate = { &MemoryEditor::updateView, this };

  source.onChange = { &MemoryEditor::selectSource, this };
  exportMemory.onActivate = { &MemoryEditor::exportMemoryToDisk, this };
  editor.onRead = { &MemoryEditor::read, this };
  editor.onWrite = { &MemoryEditor::write, this };

  windowManager->append(this, "MemoryEditor");
}

uint8_t MemoryEditor::read(unsigned addr) {
  if(SNES::cartridge.loaded() == false) return 0x00;
  switch(source.selection()) {
  case 0: return cpuDebugger->read(addr);
  case 1: return smpDebugger->read(addr);
  case 2: return SNES::ppu.vram[addr & 0xffff];
  case 3: return SNES::ppu.oam[addr % 544];
  case 4: return SNES::ppu.cgram[addr & 0x01ff];
  }
  return ~0;
}

void MemoryEditor::write(unsigned addr, uint8_t data) {
  if(SNES::cartridge.loaded() == false) return;
  switch(source.selection()) {
  case 0:
    SNES::cartridge.rom.write_protect(false);
    cpuDebugger->write(addr, data);
    SNES::cartridge.rom.write_protect(true);
    break;
  case 1:
    smpDebugger->write(addr, data);
    break;
  case 2:
    SNES::ppu.vram[addr & 0xffff] = data;
    break;
  case 3:
    SNES::ppu.oam[addr % 544] = data;
    SNES::ppu.sprite.synchronize();  //cache OAM changes internally
    break;
  case 4:
    SNES::ppu.cgram[addr & 0x01ff] = data;
    break;
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
  updateView();
}

void MemoryEditor::exportMemoryToDisk() {
  string filename = { interface->pathName, "debug/memory-" };
  switch(source.selection()) {
  case 0: filename.append("cpu.bin"); break;
  case 1: filename.append("apu.bin"); break;
  case 2: filename.append("vram.bin"); break;
  case 3: filename.append("oam.bin"); break;
  case 4: filename.append("cgram.bin"); break;
  }
  file fp;
  if(fp.open(filename, file::mode::write) == false) return;
  switch(source.selection()) {
  case 0: for(unsigned addr = 0; addr <= 0xffffff; addr++) fp.write(cpuDebugger->read(addr)); break;
  case 1: for(unsigned addr = 0; addr <= 0xffff; addr++) fp.write(smpDebugger->read(addr)); break;
  case 2: for(unsigned addr = 0; addr <= 0xffff; addr++) fp.write(SNES::ppu.vram[addr]); break;
  case 3: for(unsigned addr = 0; addr <= 0x021f; addr++) fp.write(SNES::ppu.oam[addr]); break;
  case 4: for(unsigned addr = 0; addr <= 0x01ff; addr++) fp.write(SNES::ppu.cgram[addr]); break;
  }
  debugger->print("Exported memory to ", filename, "\n");
}

void MemoryEditor::updateView() {
  editor.update();
}
