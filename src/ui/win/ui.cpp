#include "ui_input.cpp"
#include "ui_main.cpp"
#include "ui_about.cpp"
#include "settings/settings.cpp"
#include "debugger/debugger.cpp"

void init_ui() {
HDC  hdc = GetDC(0);
long height;
  height = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
  global::vwf = CreateFont(height, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Tahoma");
  height = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
  global::fwf = CreateFont(height, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Courier New");
  height = -MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
  global::font_about = CreateFont(height, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Verdana");
  height = -MulDiv(14, GetDeviceCaps(hdc, LOGPIXELSY), 72);
  global::font_header = CreateFont(height, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "Verdana");
  height = -MulDiv(10, GetDeviceCaps(hdc, LOGPIXELSY), 72);
  global::font_list = CreateFont(height, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Verdana");
  ReleaseDC(0, hdc);

  wMain.SetBackgroundColor(0, 0, 0);
  wMain.SetIcon(100);
  wMain.Create(0, "bsnes", config::misc.window_style.sget(), 0, 0, 256, 224, BSNES_TITLE);
  wMain.Center();

  wAbout.SetIcon(100);
  wAbout.Create(0, "bsnes_about", "topmost|popup|frame|dragmove", 0, 0, 325, 165, "About bsnes...");
  wAbout.Center();

  init_settings();
  init_debugger();

  if(!stricmp(config::system.video.sget(), "dd")) {
    uiVideo = new VideoDD(wMain.hwnd);
//} else if(!stricmp(config::system.video.sget(), "sdl")) {
//  uiVideo = new VideoSDL((void*)wMain.hwnd);
  } else {
    uiVideo = new VideoD3D(wMain.hwnd);
  }

  if(!stricmp(config::system.audio.sget(), "none")) {
    uiAudio = new Audio();
  } else {
    uiAudio = new AudioDS(wMain.hwnd);
  }

  if(!stricmp(config::system.input.sget(), "none")) {
    uiInput = new Input();
//} else if(!stricmp(config::system.input.sget(), "sdl")) {
//  uiInput = new InputSDL((void*)wMain.hwnd);
  } else {
    uiInput = new InputDI();
  }

  uiVideo->init();
  uiAudio->init();
  uiInput->init();

  wMain.Setup();
  wAbout.Setup();
  setup_settings();
  setup_debugger();

  wMain.Show();
}

void term_ui() {
  wMain.Hide();

  uiVideo->term();
  uiAudio->term();
  uiInput->term();

  SafeDelete(uiVideo);
  SafeDelete(uiAudio);
  SafeDelete(uiInput);
}
