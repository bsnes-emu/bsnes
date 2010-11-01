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
}

void Debugger::run() {
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

  if(debugMode == DebugMode::StepIntoCPU) {
    if(SNES::debugger.break_event == SNES::Debugger::BreakEvent::CPUStep) {
      debugMode = DebugMode::None;
      console.eventTraceCPU();
      cpuDebugger.eventStepInto();
    }
  }

  if(debugMode == DebugMode::StepIntoSMP) {
    if(SNES::debugger.break_event == SNES::Debugger::BreakEvent::SMPStep) {
      debugMode = DebugMode::None;
      console.eventTraceSMP();
      smpDebugger.eventStepInto();
    }
  }

  SNES::debugger.break_event = SNES::Debugger::BreakEvent::None;
}

Debugger::Debugger() {
  debugMode = DebugMode::None;
}

#endif
