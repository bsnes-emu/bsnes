#include "../base.hpp"
BreakpointEditor *breakpointEditor = nullptr;

BreakpointEntry::BreakpointEntry() {
  static unsigned id = 1;
  enable.setText({ "#", id++ });
  addr.setFont(application->monospaceFont);
  data.setFont(application->monospaceFont);
  type.append("Read", "Write", "Exec");
  source.append("CPU", "SMP", "VRAM", "OAM", "CGRAM");

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

  setGeometry({128, 128, layout.minimumGeometry().width, layout.minimumGeometry().height - 5});
  synchronize();

  windowManager->append(this, "BreakpointEditor");
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

  for(auto &bp : breakpointReadCPU) bp.addr = cpuDebugger->mirror(bp.addr);
  for(auto &bp : breakpointWriteCPU) bp.addr = cpuDebugger->mirror(bp.addr);
  for(auto &bp : breakpointExecCPU) bp.addr = cpuDebugger->mirror(bp.addr);

  breakpointReadSMP.reset();
  breakpointWriteSMP.reset();
  breakpointExecSMP.reset();

  for(auto &bp : breakpoint) if(bp.type == Breakpoint::Read && bp.source == Breakpoint::SMP) breakpointReadSMP.append(bp);
  for(auto &bp : breakpoint) if(bp.type == Breakpoint::Write && bp.source == Breakpoint::SMP) breakpointWriteSMP.append(bp);
  for(auto &bp : breakpoint) if(bp.type == Breakpoint::Exec && bp.source == Breakpoint::SMP) breakpointExecSMP.append(bp);

  breakpointReadVRAM.reset();
  breakpointWriteVRAM.reset();

  for(auto &bp : breakpoint) if(bp.type == Breakpoint::Read && bp.source == Breakpoint::VRAM) breakpointReadVRAM.append(bp);
  for(auto &bp : breakpoint) if(bp.type == Breakpoint::Write && bp.source == Breakpoint::VRAM) breakpointWriteVRAM.append(bp);

  breakpointReadOAM.reset();
  breakpointWriteOAM.reset();

  for(auto &bp : breakpoint) if(bp.type == Breakpoint::Read && bp.source == Breakpoint::OAM) breakpointReadOAM.append(bp);
  for(auto &bp : breakpoint) if(bp.type == Breakpoint::Write && bp.source == Breakpoint::OAM) breakpointWriteOAM.append(bp);

  breakpointReadCGRAM.reset();
  breakpointWriteCGRAM.reset();

  for(auto &bp : breakpoint) if(bp.type == Breakpoint::Read && bp.source == Breakpoint::CGRAM) breakpointReadCGRAM.append(bp);
  for(auto &bp : breakpoint) if(bp.type == Breakpoint::Write && bp.source == Breakpoint::CGRAM) breakpointWriteCGRAM.append(bp);
}

//S-CPU
//=====

bool BreakpointEditor::testReadCPU(uint24 addr) {
  addr = cpuDebugger->mirror(addr);
  for(auto &bp : breakpointReadCPU) {
    if(bp.addr == addr) {
      if(bp.compare && bp.data != cpuDebugger->read(addr)) continue;
      debugger->print("Breakpoint #", bp.id, " hit\n");
      return true;
    }
  }
  return false;
}

bool BreakpointEditor::testWriteCPU(uint24 addr, uint8 data) {
  addr = cpuDebugger->mirror(addr);
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
  addr = cpuDebugger->mirror(addr);
  for(auto &bp : breakpointExecCPU) {
    if(bp.addr == addr) {
      debugger->print("Breapoint #", bp.id, " hit\n");
      return true;
    }
  }
  return false;
}

//S-SMP
//=====

bool BreakpointEditor::testReadSMP(uint16 addr) {
  for(auto &bp : breakpointReadSMP) {
    if(bp.addr == addr) {
      if(bp.compare && bp.data != smpDebugger->read(addr)) continue;
      debugger->print("Breakpoint #", bp.id, " hit\n");
      return true;
    }
  }
  return false;
}

bool BreakpointEditor::testWriteSMP(uint16 addr, uint8 data) {
  for(auto &bp : breakpointWriteSMP) {
    if(bp.addr == addr) {
      if(bp.compare && bp.data != data) continue;
      debugger->print("Breakpoint #", bp.id, " hit\n");
      return true;
    }
  }
  return false;
}

bool BreakpointEditor::testExecSMP(uint16 addr) {
  for(auto &bp : breakpointExecSMP) {
    if(bp.addr == addr) {
      debugger->print("Breapoint #", bp.id, " hit\n");
      return true;
    }
  }
  return false;
}

//S-PPU
//=====

bool BreakpointEditor::testReadVRAM(uint16 addr) {
  for(auto &bp : breakpointReadVRAM) {
    if(bp.addr == addr) {
      if(bp.compare && bp.data != SFC::ppu.vram[addr]) continue;
      debugger->print("Breakpoint #", bp.id, " hit\n");
      return true;
    }
  }
  return false;
}

bool BreakpointEditor::testWriteVRAM(uint16 addr, uint8 data) {
  for(auto &bp : breakpointWriteVRAM) {
    if(bp.addr == addr) {
      if(bp.compare && bp.data != data) continue;
      debugger->print("Breakpoint #", bp.id, " hit\n");
      return true;
    }
  }
  return false;
}

bool BreakpointEditor::testReadOAM(uint16 addr) {
  for(auto &bp : breakpointReadOAM) {
    if(bp.addr == addr) {
      if(bp.compare && bp.data != SFC::ppu.oam[addr]) continue;
      debugger->print("Breakpoint #", bp.id, " hit\n");
      return true;
    }
  }
  return false;
}

bool BreakpointEditor::testWriteOAM(uint16 addr, uint8 data) {
  for(auto &bp : breakpointWriteOAM) {
    if(bp.addr == addr) {
      if(bp.compare && bp.data != data) continue;
      debugger->print("Breakpoint #", bp.id, " hit\n");
      return true;
    }
  }
  return false;
}

bool BreakpointEditor::testReadCGRAM(uint16 addr) {
  for(auto &bp : breakpointReadCGRAM) {
    if(bp.addr == addr) {
      if(bp.compare && bp.data != SFC::ppu.cgram[addr]) continue;
      debugger->print("Breakpoint #", bp.id, " hit\n");
      return true;
    }
  }
  return false;
}

bool BreakpointEditor::testWriteCGRAM(uint16 addr, uint8 data) {
  for(auto &bp : breakpointWriteCGRAM) {
    if(bp.addr == addr) {
      if(bp.compare && bp.data != data) continue;
      debugger->print("Breakpoint #", bp.id, " hit\n");
      return true;
    }
  }
  return false;
}
