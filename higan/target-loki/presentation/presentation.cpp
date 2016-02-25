#include "../loki.hpp"
unique_pointer<Presentation> presentation;

Presentation::Presentation() {
  presentation = this;

  onClose([&] { program->quit(); });

  setTitle({"loki v", Emulator::Version});
  setResizable(false);
  setBackgroundColor({0, 0, 0});
  setSize({512, 480});
  setCentered();
  setVisible();
}

auto Presentation::drawSplashScreen() -> void {
  uint32_t* output;
  uint length;
  if(video->lock(output, length, 512, 480)) {
    for(auto y : range(480)) {
      auto dp = output + y * (length >> 2);
      for(auto x : range(512)) *dp++ = 0xff'00'00'00;
    }
    video->unlock();
    video->refresh();
  }
}
