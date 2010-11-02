#include "../base.hpp"
#if defined(DEBUGGER)

#include <nall/snes/cpu.hpp>
#include <nall/snes/smp.hpp>

#include "console.cpp"
#include "cpu/debugger.cpp"
#include "smp/debugger.cpp"
#include "tools/breakpoint-editor.cpp"
#include "tools/memory-editor.cpp"
Debugger debugger;

void Debugger::create() {
  console.create();
  cpuDebugger.create();
  smpDebugger.create();
  breakpointEditor.create();
  memoryEditor.create();

  Window::create(0, 0, 256, 256, "Debugger");
  application.addWindow(this, "Debugger", "160,160");

  unsigned x = 5, y = 5;
  enableDebugger.create(*this, x, y, 240, Style::CheckBoxHeight, "Enable debugger"); y += Style::CheckBoxHeight;
  showConsole.create(*this, x, y, 240, Style::CheckBoxHeight, "Console"); y += Style::CheckBoxHeight;
  showCPUDebugger.create(*this, x, y, 240, Style::CheckBoxHeight, "CPU debugger"); y += Style::CheckBoxHeight;
  showSMPDebugger.create(*this, x, y, 240, Style::CheckBoxHeight, "SMP debugger"); y += Style::CheckBoxHeight;
  showBreakpointEditor.create(*this, x, y, 240, Style::CheckBoxHeight, "Breakpoint editor"); y += Style::CheckBoxHeight;
  showMemoryEditor.create(*this, x, y, 240, Style::CheckBoxHeight, "Memory editor"); y += Style::CheckBoxHeight;

  //windows shown by default
  showConsole.setChecked();
  showCPUDebugger.setChecked();
  showSMPDebugger.setChecked();
  showBreakpointEditor.setChecked();

  setGeometry(0, 0, 250, y);

  enableDebugger.onTick = []() {
    debugger.enable(debugger.enableDebugger.checked());
  };

  showConsole.onTick = []() {
    console.setVisible(debugger.showConsole.checked());
  };

  showCPUDebugger.onTick = []() {
    cpuDebugger.setVisible(debugger.showCPUDebugger.checked());
  };

  showSMPDebugger.onTick = []() {
    smpDebugger.setVisible(debugger.showSMPDebugger.checked());
  };

  showBreakpointEditor.onTick = []() {
    breakpointEditor.setVisible(debugger.showBreakpointEditor.checked());
  };

  showMemoryEditor.onTick = []() {
    memoryEditor.setVisible(debugger.showMemoryEditor.checked());
  };

  onClose = []() {
    debugger.enable(false);
    return true;
  };

  synchronize();
}

void Debugger::synchronize() {
  cpuDebugger.synchronize();
  smpDebugger.synchronize();
}

void Debugger::setVisible(bool visible) {
  Window::setVisible(visible);
  console.setVisible(showConsole.checked() & visible);
  cpuDebugger.setVisible(showCPUDebugger.checked() & visible);
  smpDebugger.setVisible(showSMPDebugger.checked() & visible);
  breakpointEditor.setVisible(showBreakpointEditor.checked() & visible);
  memoryEditor.setVisible(showMemoryEditor.checked() & visible);
}

void Debugger::enable(bool state) {
  enableDebugger.setChecked(state);
  SNES::debugger.step_cpu = state;
  SNES::debugger.step_smp = state;
}

void Debugger::run() {
  synchronize();

  if(enableDebugger.checked() == false) {
    SNES::system.run();
    return;
  }

  if(debugMode == DebugMode::None) {
    usleep(20 * 1000);
    return;
  }

  SNES::system.run();

  if(debugMode == DebugMode::WaitForBreakpoint) {
    if(SNES::debugger.break_event == SNES::Debugger::BreakEvent::BreakpointHit) {
      debugMode = DebugMode::None;
      console.eventBreakpoint();
      breakpointEditor.eventBreakpoint();
    }
  }

  if(SNES::debugger.break_event == SNES::Debugger::BreakEvent::CPUStep) {
    if(debugMode == DebugMode::StepIntoCPU) {
      debugMode = DebugMode::None;
      cpuDebugger.eventStepInto();
    }
    console.eventTraceCPU();
  }

  if(SNES::debugger.break_event == SNES::Debugger::BreakEvent::SMPStep) {
    if(debugMode == DebugMode::StepIntoSMP) {
      debugMode = DebugMode::None;
      smpDebugger.eventStepInto();
    }
    console.eventTraceSMP();
  }

  SNES::debugger.break_event = SNES::Debugger::BreakEvent::None;
}

Debugger::Debugger() {
  debugMode = DebugMode::None;
}

#endif
