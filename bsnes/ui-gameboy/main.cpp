#include "base.hpp"
nall::DSP dspaudio;
Application application;

#include "interface.cpp"

#include "general/main-window.cpp"

void Application::main(int argc, char **argv) {
  quit = false;

  #if defined(PLATFORM_WIN)
  proportionalFont.setFamily("Tahoma");
  proportionalFont.setSize(8);

  proportionalFontBold.setFamily("Tahoma");
  proportionalFontBold.setSize(8);
  proportionalFontBold.setBold();

  monospaceFont.setFamily("Lucida Console");
  monospaceFont.setSize(8);
  #else
  proportionalFont.setFamily("Sans");
  proportionalFont.setSize(8);

  proportionalFontBold.setFamily("Sans");
  proportionalFontBold.setSize(8);
  proportionalFontBold.setBold();

  monospaceFont.setFamily("Liberation Mono");
  monospaceFont.setSize(8);
  #endif

  mainWindow.create();
  mainWindow.setVisible();
  OS::processEvents();

  #if defined(PLATFORM_WIN)
  video.driver("Direct3D");
  #else
  video.driver("OpenGL");
  #endif
  video.set(Video::Handle, (uintptr_t)mainWindow.viewport.handle());
  video.set(Video::Synchronize, false);
  video.set(Video::Filter, (unsigned)0);
  video.init();

  #if defined(PLATFORM_WIN)
  audio.driver("XAudio2");
  #else
  audio.driver("ALSA");
  #endif
  audio.set(Audio::Handle, (uintptr_t)mainWindow.viewport.handle());
  audio.set(Audio::Synchronize, true);
  audio.set(Audio::Latency, 80u);
  audio.set(Audio::Frequency, 44100u);
  audio.init();

  dspaudio.setPrecision(16);
  dspaudio.setVolume(1.0);
  dspaudio.setBalance(0.0);
  dspaudio.setFrequency(4194304.0);
  dspaudio.setResampler(DSP::Resampler::Average);
  dspaudio.setResamplerFrequency(44100.0);

  #if defined(PLATFORM_WIN)
  input.driver("RawInput");
  #else
  input.driver("SDL");
  #endif
  input.set(Input::Handle, (uintptr_t)mainWindow.viewport.handle());
  input.init();

  GameBoy::system.init(&interface);

  while(quit == false) {
    OS::processEvents();

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
