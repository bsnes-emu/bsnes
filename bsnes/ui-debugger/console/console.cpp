#include "../base.hpp"
ConsoleWindow *consoleWindow = nullptr;

#include "about.cpp"

ConsoleWindow::ConsoleWindow() {
  setTitle({"Console - Laevateinn v", Version});
  setGeometry({64, 650, 640, 400});
  setMenuVisible();

  menuEmulation.setText("&Emulation");
    menuEmulationReloadCartridge.setText("Reload Cartridge");
    menuEmulationPowerCycle.setText("Power Cycle");
    menuEmulationReset.setText("Reset");
    menuEmulationSynchronizeAudio.setText("Synchronize Audio");
    menuEmulationSynchronizeAudio.setChecked();
    menuEmulationMuteAudio.setText("Mute Audio");
    menuEmulation.append(menuEmulationReloadCartridge, menuEmulationPowerCycle, menuEmulationReset,
      menuEmulationSeparator, menuEmulationSynchronizeAudio, menuEmulationMuteAudio);
  append(menuEmulation);

  menuDebug.setText("&Debug");
    menuDebugCPU.setText("CPU");
    menuDebugCPU.setChecked(debugger->debug.cpu);
    menuDebugSMP.setText("SMP");
    menuDebugSMP.setChecked(debugger->debug.smp);
    menuDebugSMP.setEnabled(false);
    menuDebug.append(menuDebugCPU, menuDebugSMP);
  append(menuDebug);

  menuTracer.setText("&Tracer");
    menuTracerEnable.setText("Enable");
    menuTracerMask.setText("Mask");
    menuTracerMask.setEnabled(false);
    menuTracerMaskReset.setText("Reset Mask");
    menuTracerMaskReset.setEnabled(false);
    menuTracer.append(menuTracerEnable, menuTracerMask, menuTracerMaskReset);
  append(menuTracer);

  menuWindows.setText("&Windows");
    menuWindowsVideoWindow.setText("Video");
    menuWindowsCPUDebugger.setText("CPU Debugger");
    menuWindowsMemoryEditor.setText("Memory Editor");
    menuWindowsBreakpointEditor.setText("Breakpoint Editor");
    menuWindows.append(menuWindowsVideoWindow, menuWindowsCPUDebugger, menuWindowsMemoryEditor,
      menuWindowsBreakpointEditor);
  append(menuWindows);

  menuHelp.setText("&Help");
    menuHelpAbout.setText("About ...");
    menuHelp.append(menuHelpAbout);
  append(menuHelp);

  layout.setMargin(5);
  runButton.setText("Run");
  stepButton.setText("Step");
  clearButton.setText("Clear");
  console.setFont(application->monospaceFont);

  layout.append(commandLayout, {~0, 0}, 5);
    commandLayout.append(runButton, {80, ~0}, 5);
    commandLayout.append(stepButton, {80, ~0}, 5);
    commandLayout.append(spacer, {~0, 0});
    commandLayout.append(clearButton, {80, ~0});
  layout.append(console, {~0, ~0});
  append(layout);

  onClose = [] { application->quit = true; };

  menuEmulationReloadCartridge.onActivate = [&] {
    interface->loadCartridge(interface->fileName);
  };

  menuEmulationPowerCycle.onActivate = [&] {
    SNES::system.power();
    print("System power cycled\n");
  };

  menuEmulationReset.onActivate = [&] {
    SNES::system.reset();
    print("System reset\n");
  };

  menuEmulationSynchronizeAudio.onToggle = [&] {
    audio.set(Audio::Synchronize, menuEmulationSynchronizeAudio.checked());
  };

  menuDebugCPU.onToggle = [&] { debugger->debug.cpu = menuDebugCPU.checked(); };
  menuDebugSMP.onToggle = [&] { debugger->debug.smp = menuDebugSMP.checked(); };

  menuTracerEnable.onToggle = [&] { debugger->tracerEnable(menuTracerEnable.checked()); };

  menuWindowsVideoWindow.onActivate = [&] {
    videoWindow->setVisible();
    videoWindow->setFocused();
  };

  menuWindowsCPUDebugger.onActivate = [&] {
    cpuDebugger->setVisible();
    cpuDebugger->setFocused();
  };

  menuWindowsMemoryEditor.onActivate = [&] {
    memoryEditor->update();
    memoryEditor->setVisible();
    memoryEditor->setFocused();
  };

  menuWindowsBreakpointEditor.onActivate = [&] {
    breakpointEditor->setVisible();
    breakpointEditor->setFocused();
  };

  menuHelpAbout.onActivate = [&] { aboutWindow->show(); };

  runButton.onActivate = [&] {
    if(debugger->paused) debugger->resume();
    else debugger->suspend();
  };

  stepButton.onActivate = [&] {
    debugger->flags.step = true;
    debugger->resume();
  };

  clearButton.onActivate = [&] {
    console.setText("");
  };
}

void ConsoleWindow::print(const string &text) {
  string output = console.text();
  output.append(text);
  console.setText(output);
  console.setCursorPosition(~0);
}
