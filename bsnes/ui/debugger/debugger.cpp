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

  Window::create(0, 0, 256, 80, "Debugger");
  application.addWindow(this, "Debugger", "160,160");

  enableDebugger.setText("Enable debugger");
  showConsole.setText("Console");
  showCPUDebugger.setText("CPU debugger");
  showSMPDebugger.setText("SMP debugger");
  showBreakpointEditor.setText("Breakpoint editor");
  showMemoryEditor.setText("Memory editor");

  layout.setMargin(5);
  layout.append(enableDebugger, 0, Style::CheckBoxHeight);
  layout.append(showConsole, 0, Style::CheckBoxHeight);
  layout.append(showCPUDebugger, 0, Style::CheckBoxHeight);
  layout.append(showSMPDebugger, 0, Style::CheckBoxHeight);
  layout.append(showBreakpointEditor, 0, Style::CheckBoxHeight);
  layout.append(showMemoryEditor, 0, Style::CheckBoxHeight);
  setLayout(layout);

/*unsigned x = 5, y = 5;
  layout.append(enableDebugger, x, y, 240, Style::CheckBoxHeight); y += Style::CheckBoxHeight;
  layout.append(showConsole, x, y, 240, Style::CheckBoxHeight); y += Style::CheckBoxHeight;
  layout.append(showCPUDebugger, x, y, 240, Style::CheckBoxHeight); y += Style::CheckBoxHeight;
  layout.append(showSMPDebugger, x, y, 240, Style::CheckBoxHeight); y += Style::CheckBoxHeight;
  layout.append(showBreakpointEditor, x, y, 240, Style::CheckBoxHeight); y += Style::CheckBoxHeight;
  layout.append(showMemoryEditor, x, y, 240, Style::CheckBoxHeight); y += Style::CheckBoxHeight;
  setGeometry(0, 0, 250, y);
  setLayout(layout);*/

  //windows shown by default
  showConsole.setChecked();
  showCPUDebugger.setChecked();
  showSMPDebugger.setChecked();
  showBreakpointEditor.setChecked();

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
  synchronize();
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
      synchronize();
    }
  }

  SNES::debugger.break_event = SNES::Debugger::BreakEvent::None;
}

bool Debugger::step_cpu() {
  if(enableDebugger.checked() == false) return false;
  console.eventTraceCPU();
  if(debugMode == DebugMode::StepIntoCPU) {
    debugMode = DebugMode::None;
    cpuDebugger.eventStepInto();
    synchronize();
    return true;
  }
  return false;
}

bool Debugger::step_smp() {
  if(enableDebugger.checked() == false) return false;
  console.eventTraceSMP();
  if(debugMode == DebugMode::StepIntoSMP) {
    debugMode = DebugMode::None;
    smpDebugger.eventStepInto();
    synchronize();
    return true;
  }
  return false;
}

Debugger::Debugger() {
  debugMode = DebugMode::None;
  SNES::cpu.step_event = { &Debugger::step_cpu, this };
  SNES::smp.step_event = { &Debugger::step_smp, this };
}

#endif
