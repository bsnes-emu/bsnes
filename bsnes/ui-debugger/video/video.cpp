#include "../base.hpp"
VideoWindow *videoWindow = nullptr;

VideoWindow::VideoWindow() {
  setTitle("Video");
//setResizable(false);
  setGeometry({64, 64, 512, 480});
  setStatusFont(application->proportionalFontBold);
  setStatusVisible();

  canvas.setSize({512, 480});
  layout.append(canvas, {~0, ~0});
  append(layout);

  image logo(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);
  logo.loadPNG(laevateinnLogo, sizeof laevateinnLogo);
  logo.alphaBlend(0x000000);

  unsigned cx = (512 - logo.width) / 2, cy = (480 - logo.height) / 2;
  for(unsigned y = 0; y < logo.height; y++) {
    uint32_t *dp = canvas.data() + (y + cy) * 512 + cx;
    const uint32_t *sp = (const uint32_t*)logo.data + y * logo.width;
    for(unsigned x = 0; x < logo.width; x++) {
      *dp++ = *sp++;
    }
  }

  canvas.update();

  canvas.onMouseLeave = [&] {
    setStatusText("");
  };

  canvas.onMouseMove = [&](Position position) {
    uint32_t color = canvas.data()[position.y * 512 + position.x];
    unsigned r = (color >> 19) & 31, g = (color >> 11) & 31, b = (color >> 3) & 31;

    setStatusText({
      decimal(position.x / 2), ".", decimal((position.x & 1) * 5), ", ",
      decimal(position.y / 2), ".", decimal((position.y & 1) * 5), ", ",
      "0x", hex<4>((b << 10) + (g << 5) + (r << 0))
    });
  };
}
