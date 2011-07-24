#include <data/bsnes-logo.hpp>

AboutWindow aboutWindow;

void AboutWindow::create() {
  application.addWindow(this, "AboutWindow", "160,160");
  setTitle("About bsnes ...");
  setResizable(false);
  setBackgroundColor(255, 255, 255);

  information.setText({
    "bsnes v", SNES::Info::Version, " ~ Profile: ", SNES::Info::Profile,
    " ~ Author: byuu ~ Website: http://byuu.org/"
  });

  layout.setMargin(5);
  layout.append(canvas, 720, 180);
  informationLayout.append(spacer, ~0, 0);
  informationLayout.append(information, 0, 0);
  layout.append(informationLayout);
  append(layout);
  setGeometry({ 0, 0, layout.minimumGeometry().width, layout.minimumGeometry().height });
}

void AboutWindow::show() {
  logo.decode(bsnesLogoData, sizeof bsnesLogoData);
  setVisible();
  uint32_t *buffer = canvas.buffer();
  for(unsigned y = 0; y < 180; y++) {
    uint8_t *p = logo.data + 0x36 + (180 - 1 - y) * 720 * 3;
    for(unsigned x = 0; x < 720; x++) {
      *buffer++ = (p[0] << 0) | (p[1] << 8) | (p[2] << 16);
      p += 3;
    }
  }
  canvas.update();
}
