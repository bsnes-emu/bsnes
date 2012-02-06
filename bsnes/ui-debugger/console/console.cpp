#include "../base.hpp"
ConsoleWindow *consoleWindow = nullptr;

ConsoleWindow::ConsoleWindow() {
  setTitle({"Console - bsnes v", Version});
  setGeometry({64, 650, 800, 400});
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

  menuWindows.setText("&Windows");
    menuWindowsVideo.setText("Video");
    menuWindows.append(menuWindowsVideo);
  append(menuWindows);

  layout.setMargin(5);
  runButton.setText("Run");
  stepButton.setText("Step");
  console.setFont(application->monospaceFont);

  layout.append(commandLayout, {~0, 0}, 5);
    commandLayout.append(runButton, {80, ~0}, 5);
    commandLayout.append(stepButton, {80, ~0});
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

  menuWindowsVideo.onActivate = [&] {
    videoWindow->setVisible();
    videoWindow->setFocused();
  };
}

void ConsoleWindow::print(const string &text) {
  string output = console.text();
  output.append(text);
  console.setText(output);
  console.setCursorPosition(~0);
}
