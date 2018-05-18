#include "../bsnes.hpp"
#include "about.cpp"
unique_pointer<AboutWindow> aboutWindow;
unique_pointer<Presentation> presentation;

Presentation::Presentation() {
  presentation = this;

  fileMenu.setText("File");
  quit.setText("Quit").onActivate([&] { program->quit(); });

  settingsMenu.setText("Settings");

  toolsMenu.setText("Tools");

  helpMenu.setText("Help");
  about.setText("About ...").onActivate([&] {
    aboutWindow->setCentered(*this).setVisible().setFocused();
  });

  onClose([&] {
    program->quit();
  });

  setTitle({"bsnes v", Emulator::Version});
  setBackgroundColor({0, 0, 0});
  setSize({640, 480});
  setCentered();

  #if defined(PLATFORM_MACOS)
  Application::Cocoa::onAbout([&] { about.doActivate(); });
  Application::Cocoa::onActivate([&] { setFocused(); });
  Application::Cocoa::onQuit([&] { doClose(); });
  #endif
}
