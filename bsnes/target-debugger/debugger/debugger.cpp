#include "../base.hpp"
Debugger *debugger = nullptr;

#include "hook.cpp"
#include "usage.cpp"

void Debugger::run() {
  if(paused == true) {
    usleep(2000);
    return;
  }

  SNES::system.run();
  if(cpuDebugger->autoUpdate.checked()) cpuDebugger->updateDisassembly();
  if(smpDebugger->autoUpdate.checked()) smpDebugger->updateDisassembly();
  if(memoryEditor->autoUpdate.checked()) memoryEditor->updateView();
  if(propertiesViewer->autoUpdate.checked()) propertiesViewer->updateProperties();
  if(vramViewer->autoUpdate.checked()) vramViewer->updateTiles();
}

void Debugger::echo(const string &text) {
  consoleWindow->print(text);
}

void Debugger::resume() {
  if(paused == false) return;
  paused = false;
  consoleWindow->runButton.setText("Stop");
}

void Debugger::suspend() {
  if(paused == true) return;
  paused = true;
  flags.step = false;
  flags.cpu.stepInto = false;
  flags.cpu.nmi = false;
  flags.cpu.irq = false;
  flags.smp.stepInto = false;
  consoleWindow->runButton.setText("Run");
}

Debugger::Debugger() {
  paused = true;

  flags.step = false;
  flags.cpu.stepInto = false;
  flags.cpu.nmi = false;
  flags.cpu.irq = false;
  flags.smp.stepInto = false;

  debug.cpu = true;
  debug.smp = false;

  cpuUsage.allocate(16 * 1024 * 1024);
  apuUsage.allocate(64 * 1024);

  SNES::cpu.debugger.op_exec = { &Debugger::cpu_op_exec, this };
  SNES::cpu.debugger.op_read = { &Debugger::cpu_op_read, this };
  SNES::cpu.debugger.op_write = { &Debugger::cpu_op_write, this };

  SNES::cpu.debugger.op_nmi = { &Debugger::cpu_op_nmi, this };
  SNES::cpu.debugger.op_irq = { &Debugger::cpu_op_irq, this };

  SNES::smp.debugger.op_exec = { &Debugger::smp_op_exec, this };
  SNES::smp.debugger.op_read = { &Debugger::smp_op_read, this };
  SNES::smp.debugger.op_write = { &Debugger::smp_op_write, this };

  SNES::ppu.debugger.vram_read = { &Debugger::ppu_vram_read, this };
  SNES::ppu.debugger.vram_write = { &Debugger::ppu_vram_write, this };

  SNES::ppu.debugger.oam_read = { &Debugger::ppu_oam_read, this };
  SNES::ppu.debugger.oam_write = { &Debugger::ppu_oam_write, this };

  SNES::ppu.debugger.cgram_read = { &Debugger::ppu_cgram_read, this };
  SNES::ppu.debugger.cgram_write = { &Debugger::ppu_cgram_write, this };
}
