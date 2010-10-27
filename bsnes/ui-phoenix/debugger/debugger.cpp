#include "../base.hpp"
#if defined(DEBUGGER)

#include <nall/snes/cpu.hpp>

#include "console.cpp"
#include "cpu/debugger.cpp"
Debugger debugger;

void Debugger::create() {
  console.create();
  cpuDebugger.create();

  Window::create(0, 0, 256, 256, "Debugger");
  application.addWindow(this, "Debugger", "160,160");

  unsigned x = 5, y = 5;
  enableDebugger.create(*this, x, y, 390, Style::CheckBoxHeight, "Enable debugger"); y += Style::CheckBoxHeight;
  showMemoryEditor.create(*this, x, y, 390, Style::CheckBoxHeight, "Memory editor"); y += Style::CheckBoxHeight;

  setGeometry(0, 0, 400, y);

  enableDebugger.onTick = []() {
    debugger.enable(debugger.enableDebugger.checked());
  };

  onClose = []() {
    debugger.enable(false);
    return true;
  };
}

void Debugger::setVisible(bool visible) {
  Window::setVisible(visible);
  console.setVisible(visible);
  cpuDebugger.setVisible(visible);
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

  if(debugMode == DebugMode::StepIntoCPU) {
    if(SNES::debugger.break_event == SNES::Debugger::BreakEvent::CPUStep) {
      debugMode = DebugMode::None;
      cpuDebugger.eventStepInto();
      console.eventTraceCPU();
    }
  }

  SNES::debugger.break_event = SNES::Debugger::BreakEvent::None;
}

Debugger::Debugger() {
  debugMode = DebugMode::None;
}

#endif
