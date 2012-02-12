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
  if(cpuDebugger->autoRefresh.checked()) cpuDebugger->updateDisassembly();
  if(memoryEditor->autoRefresh.checked()) memoryEditor->update();
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
  consoleWindow->runButton.setText("Run");
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
    if(file::exists({ interface->pathName, "debug/trace-", decimal<3, '0'>(n), ".log" }) == false) break;
  } while(++n <= 999);

  string filename = { interface->pathName, "debug/trace-", decimal<3, '0'>(n), ".log" };
  if(fpTracer.open(filename, file::mode::write) == false) return;
  print("Tracing to ", filename, "\n");
}

Debugger::Debugger() {
  paused = true;

  flags.step = false;
  flags.cpu.stepInto = false;

  debug.cpu = true;
  debug.smp = false;

  cpuUsage.allocate(16 * 1024 * 1024);
  apuUsage.allocate(64 * 1024);

  SNES::cpu.debugger.op_exec = { &Debugger::cpu_op_exec, this };
  SNES::cpu.debugger.op_read = { &Debugger::cpu_op_read, this };
  SNES::cpu.debugger.op_write = { &Debugger::cpu_op_write, this };

  SNES::smp.debugger.op_exec = { &Debugger::smp_op_exec, this };
  SNES::smp.debugger.op_read = { &Debugger::smp_op_read, this };
  SNES::smp.debugger.op_write = { &Debugger::smp_op_write, this };
}
