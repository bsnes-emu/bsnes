#include "video/ddraw.cpp"
#include "audio/dsound.cpp"
#include "input/dinput.cpp"

#include "ui_window.cpp"
#include "ui_main.cpp"
#include "ui_inputcfg.cpp"
#include "ui_ppucfg.cpp"

void init_ui() {
HDC  hdc    = GetDC(0);
long height = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
  ReleaseDC(0, hdc);
  global::vwf = CreateFont(height, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Tahoma");
  global::fwf = CreateFont(height, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Courier New");

  global::black_brush  = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
  global::window_brush = (HBRUSH)COLOR_WINDOW;

  wMain.init("bsnes", BSNES_TITLE,
    256, 223, global::black_brush,  Window::WF_CENTER);
  wInputCfg.init("bsnes_inputcfg", "bsnes Input Configuration",
    355, 145, global::window_brush, Window::WF_CENTER | Window::WF_TOPMOST);
  wPPUCfg.init("bsnes_ppucfg", "bsnes PPU Configuration",
    250, 165, global::window_brush, Window::WF_CENTER | Window::WF_TOPMOST);

  uiVideo = new VideoDD();
  uiAudio = new AudioDS();
  uiInput = new InputDI();
  uiVideo->init();
  uiAudio->init();
  uiInput->init();

  wMain.setup();
  wInputCfg.setup();
  wPPUCfg.setup();

  wMain.show();
}
