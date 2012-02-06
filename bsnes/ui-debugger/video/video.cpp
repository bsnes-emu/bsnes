#include "../base.hpp"
VideoWindow *videoWindow = nullptr;

VideoWindow::VideoWindow() {
  setTitle("Video");
  setGeometry({64, 64, 512, 480});
  setResizable(false);
  setStatusFont(application->proportionalFontBold);
  setStatusVisible();

  canvas.setSize({512, 480});
  layout.append(canvas, {~0, ~0});
  append(layout);

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
