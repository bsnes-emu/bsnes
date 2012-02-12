#include "../base.hpp"
SMPDebugger *smpDebugger = nullptr;

uint8 SMPDebugger::read(uint16 addr) {
  if((addr & 0xfff0) == 0x00f0) return ~0;  //$00f0-00ff  MMIO
  return SNES::smp.op_busread(addr);
}

void SMPDebugger::write(uint16 addr, uint8 data) {
  if((addr & 0xfff0) == 0x00f0) return;  //$00f0-00ff  MMIO
  return SNES::smp.op_buswrite(addr, data);
}

unsigned SMPDebugger::opcodeLength(uint16 addr) {
  static unsigned lengthTable[256] = {
    0
  };
  return lengthTable[SNES::smp.op_busread(addr)];
}

void SMPDebugger::updateDisassembly() {
  string line[15];

  line[7] = { "> ", SNES::smp.disassemble_opcode(opcodePC) };
  line[7][31] = 0;

  signed addr = opcodePC;
  for(signed o = 6; o >= 0; o--) {
    for(signed b = 1; b <= 3; b++) {
      if(addr - b >= 0 && (debugger->apuUsage.data[addr - b] & Usage::Exec)) {
        addr -= b;
        line[o] = { "  ", SNES::smp.disassemble_opcode(addr) };
        line[o][31] = 0;
        break;
      }
    }
  }

  addr = opcodePC;
  for(signed o = 8; o <= 14; o++) {
    for(signed b = 1; b <= 3; b++) {
      if(addr - b <= 0xffff && (debugger->apuUsage.data[addr + b] & Usage::Exec)) {
        addr += b;
        line[o] = { "  ", SNES::smp.disassemble_opcode(addr) };
        line[o][31] = 0;
        break;
      }
    }
  }

  string output;
  for(auto &n : line) {
    if(n.empty()) output.append("  ...\n");
    else output.append(n, "\n");
  }
  output.rtrim<1>("\n");

  disassembly.setText(output);
  registers.setText({
    "YA:", hex<2>(SNES::smp.regs.y), hex<2>(SNES::smp.regs.a),
    " A:", hex<2>(SNES::smp.regs.a), " X:", hex<2>(SNES::smp.regs.x),
    " Y:", hex<2>(SNES::smp.regs.y), " S:01", hex<2>(SNES::smp.regs.s), " ",
    SNES::smp.regs.p.n ? "N" : "n", SNES::smp.regs.p.v ? "V" : "v",
    SNES::smp.regs.p.p ? "P" : "p", SNES::smp.regs.p.b ? "B" : "b",
    SNES::smp.regs.p.h ? "H" : "h", SNES::smp.regs.p.i ? "I" : "i",
    SNES::smp.regs.p.z ? "Z" : "z", SNES::smp.regs.p.c ? "C" : "c",
  });
}

SMPDebugger::SMPDebugger() {
  opcodePC = 0xffc0;

  setTitle("SMP Debugger");
  setGeometry({800, 800, 350, 255});

  layout.setMargin(5);
  stepInto.setText("Step Into");
  autoUpdate.setText("Auto");
  update.setText("Update");
  disassembly.setFont(application->monospaceFont);
  registers.setFont(application->monospaceFont);
  registers.setText(" ");

  layout.append(controlLayout, {~0, 0}, 5);
    controlLayout.append(stepInto, {80, 0}, 5);
    controlLayout.append(spacer, {~0, 0});
    controlLayout.append(autoUpdate, {0, 0}, 5);
    controlLayout.append(update, {80, 0});
  layout.append(disassembly, {~0, ~0}, 5);
  layout.append(registers, {~0, 0});
  append(layout);

  stepInto.onActivate = [&] {
    debugger->flags.smp.stepInto = true;
    debugger->resume();
  };

  update.onActivate = { &SMPDebugger::updateDisassembly, this };
}
