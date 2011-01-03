#include "base.hpp"
Application application;

#include "interface.cpp"

#include "general/main-window.cpp"

void Application::main(int argc, char **argv) {
  quit = false;

  #if defined(PHOENIX_WINDOWS)
  proportionalFont.create("Tahoma", 8);
  proportionalFontBold.create("Tahoma", 8, Font::Style::Bold);
  monospaceFont.create("Courier New", 8);
  #else
  proportionalFont.create("Sans", 8);
  proportionalFontBold.create("Sans", 8, Font::Style::Bold);
  monospaceFont.create("Liberation Mono", 8);
  #endif

  mainWindow.create();
  mainWindow.setVisible();
  OS::run();

  #if defined(PHOENIX_WINDOWS)
  video.driver("Direct3D");
  #else
  video.driver("OpenGL");
  #endif
  video.set(Video::Handle, (uintptr_t)mainWindow.viewport.handle());
  video.set(Video::Synchronize, true);
  video.set(Video::Filter, (unsigned)0);
  video.init();

  #if defined(PHOENIX_WINDOWS)
  input.driver("RawInput");
  #else
  input.driver("SDL");
  #endif
  input.set(Input::Handle, (uintptr_t)mainWindow.viewport.handle());
  input.init();

  GameBoy::system.init(&interface);

  while(quit == false) {
    OS::run();

    if(GameBoy::cartridge.loaded()) {
      GameBoy::system.run();
    }
  }
}

int main(int argc, char **argv) {
  application.main(argc, argv);
  return 0;
}
