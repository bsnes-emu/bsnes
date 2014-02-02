#include "../loki.hpp"
Presentation* presentation = nullptr;

Presentation::Presentation() {
  presentation = this;

  setTitle({"loki v", Emulator::Version});
  setWindowGeometry({0, 0, 512, 480});
  setResizable(false);

  layout.append(viewport, {0, 0, 512, 480});
  append(layout);

  onClose = &Application::quit;

  splash.allocate(512, 480);
  splash.verticalGradient(0xff00005f, 0xff000000, 512, 480, 256, 0);
  nall::image floor;
  floor.allocate(512, 480);
  floor.radialGradient(0xffff0000, 0x00000000, 384, 240, 256, 415);
  splash.impose(image::blend::sourceAlpha, 0, 0, floor, 0, 0, floor.width, floor.height);
  nall::image loki(resource::loki, sizeof resource::loki);
  splash.impose(image::blend::sourceAlpha, (512 - loki.width) / 2, (480 - loki.height) / 2, loki, 0, 0, loki.width, loki.height);
}

void Presentation::showSplash() {
  uint32_t* data;
  unsigned pitch;
  if(video.lock(data, pitch, 512, 480)) {
    for(unsigned y = 0; y < 480; y++) {
      memcpy((uint8_t*)data + y * pitch, splash.data + y * splash.pitch, 512 * sizeof(uint32_t));
    }
    video.unlock();
    video.refresh();
  }
}
