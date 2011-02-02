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
  video.set(Video::Synchronize, false);
  video.set(Video::Filter, (unsigned)0);
  video.init();

  #if defined(PHOENIX_WINDOWS)
  audio.driver("XAudio2");
  #else
  audio.driver("ALSA");
  #endif
  audio.set(Audio::Handle, (uintptr_t)mainWindow.viewport.handle());
  audio.set(Audio::Synchronize, true);
  audio.set(Audio::Volume, 100U);
  audio.set(Audio::Latency, 80U);
  audio.set(Audio::Frequency, 44100U);
  audio.set(Audio::Resample, true);
  audio.set(Audio::ResampleRatio, (4.0 * 1024.0 * 1024.0) / 44100.0);
  audio.init();

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
      do {
        GameBoy::system.run();
      } while(GameBoy::scheduler.exit_reason() != GameBoy::Scheduler::ExitReason::FrameEvent);
    }
  }
}

int main(int argc, char **argv) {
  application.main(argc, argv);
  return 0;
}
