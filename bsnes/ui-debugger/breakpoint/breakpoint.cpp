#include "../base.hpp"
BreakpointEditor *breakpointEditor = nullptr;

BreakpointEntry::BreakpointEntry() {
  static unsigned id = 1;
  enable.setText({ id++, ". Enable" });
  addr.setFont(application->monospaceFont);
  data.setFont(application->monospaceFont);
  type.append("Read", "Write", "Exec");
  source.append("CPU-Bus", "APU-Bus", "VRAM", "OAM", "CGRAM");

  append(enable, {0, 0}, 5);
  append(addr, {50, 0}, 5);
  append(data, {25, 0}, 5);
  append(type, {0, 0}, 5);
  append(source, {0, 0});

  enable.onToggle = [&] {
    bool flag = !enable.checked();
    addr.setEnabled(flag);
    data.setEnabled(flag);
    type.setEnabled(flag);
    source.setEnabled(flag);
    breakpointEditor->synchronize();
  };
}

BreakpointEditor::BreakpointEditor() {
  setTitle("Breakpoint Editor");

  layout.setMargin(5);
  for(auto &bp : breakpointEntry) layout.append(bp, {0, 0}, 5);
  append(layout);

  setGeometry({800, 600, layout.minimumGeometry().width, layout.minimumGeometry().height - 5});
  synchronize();
}

//enable checkbox toggled on one of the five BreakpointEntry items:
//cache settings to decrease testing overhead whilst debugging
void BreakpointEditor::synchronize() {
  breakpoint.reset();
  unsigned id = 0;
  for(auto &entry : breakpointEntry) {
    id++;
    if(entry.enable.checked() == false) continue;
    Breakpoint bp;
    bp.id = id;
    bp.compare = !entry.data.text().empty();
    bp.addr = hex(entry.addr.text());
    bp.data = hex(entry.data.text());
    bp.type = entry.type.selection();
    bp.source = entry.source.selection();
    breakpoint.append(bp);
  }

  breakpointReadCPU.reset();
  breakpointWriteCPU.reset();
  breakpointExecCPU.reset();

  for(auto &bp : breakpoint) if(bp.type == Breakpoint::Read && bp.source == Breakpoint::CPU) breakpointReadCPU.append(bp);
  for(auto &bp : breakpoint) if(bp.type == Breakpoint::Write && bp.source == Breakpoint::CPU) breakpointWriteCPU.append(bp);
  for(auto &bp : breakpoint) if(bp.type == Breakpoint::Exec && bp.source == Breakpoint::CPU) breakpointExecCPU.append(bp);
}

bool BreakpointEditor::testReadCPU(uint24 addr) {
  for(auto &bp : breakpointReadCPU) {
    if(bp.addr == addr) {
      if(bp.compare && bp.data != SNES::bus.read(addr)) continue;
      debugger->print("Breakpoint #", bp.id, " hit\n");
      return true;
    }
  }
  return false;
}

bool BreakpointEditor::testWriteCPU(uint24 addr, uint8 data) {
  for(auto &bp : breakpointWriteCPU) {
    if(bp.addr == addr) {
      if(bp.compare && bp.data != data) continue;
      debugger->print("Breakpoint #", bp.id, " hit\n");
      return true;
    }
  }
  return false;
}

bool BreakpointEditor::testExecCPU(uint24 addr) {
  for(auto &bp : breakpointExecCPU) {
    if(bp.addr == addr) {
      debugger->print("Breapoint #", bp.id, " hit\n");
      return true;
    }
  }
  return false;
}
