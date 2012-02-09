#include "../base.hpp"
Debugger *debugger = nullptr;

#include "hook.cpp"

void Debugger::run() {
  if(flags.paused == true) {
    usleep(2000);
    return;
  }

  if(memoryEditor->autoRefresh.checked()) memoryEditor->update();
  SNES::system.run();
}

void Debugger::echo(const string &text) {
  consoleWindow->print(text);
}

void Debugger::resume() {
  flags.paused = false;
  consoleWindow->runButton.setText("Stop");
  consoleWindow->stepButton.setEnabled(false);
}

void Debugger::suspend() {
  flags.paused = true;
  consoleWindow->runButton.setText("Run");
  consoleWindow->stepButton.setEnabled(true);
}

void Debugger::tracerEnable(bool state) {
  if(state == false) {
    print("Tracer disabled\n");
    fpTracer.close();
    return;
  }

  //try not to overwrite existing traces: scan from 001-999.
  //if all files exist, use 000, even if it overwrites another log.
  unsigned n = 1;
  do {
    if(file::exists({ interface->baseName, "-", decimal<3, '0'>(n), ".log" }) == false) break;
  } while(++n <= 999);

  string filename = { interface->baseName, "-", decimal<3, '0'>(n), ".log" };
  if(fpTracer.open(filename, file::mode::write) == false) return;
  print("Tracing to ", filename, "\n");
}

Debugger::Debugger() {
  flags.paused = true;
  flags.step = false;

  flags.debugCPU = true;
  flags.debugSMP = false;

  SNES::cpu.debugger.op_exec = { &Debugger::cpu_op_exec, this };
  SNES::cpu.debugger.op_read = { &Debugger::cpu_op_read, this };
  SNES::cpu.debugger.op_write = { &Debugger::cpu_op_write, this };
}
