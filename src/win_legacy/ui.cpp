HFONT hFont, hMonofont;
HBRUSH hbr_backbrush;

#include "dd_renderer.cpp"
#include "ds_sound.cpp"
#include "uictl_editex.cpp"
#include "ui_window.cpp"
#include "ui_main.cpp"
#include "ui_console.cpp"
#include "ui_bp.cpp"
#include "ui_memory.cpp"
#include "ui_inputconfig.cpp"
#include "ui_about.cpp"

void CreateFonts() {
HDC  hdc    = GetDC(0);
long height = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
  ReleaseDC(0, hdc);
  hFont     = CreateFont(height, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Tahoma");
  hMonofont = CreateFont(height, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Courier New");
}

void CreateWindows() {
  hbr_backbrush = CreateSolidBrush(RGB(32, 32, 32));
  w_main->create();
  w_console->create();
  w_bp->create();
  w_memory->create();
  w_inputconfig->create();
  w_about->create();
}

void init_ui0() {
  LoadLibrary("riched20.dll");

  dd_renderer   = new DDRenderer();
  ds_sound      = new DSSound();
  w_main        = new MainWindow();
  w_console     = new Console();
  w_bp          = new BreakpointEditor();
  w_memory      = new MemoryEditor();
  w_inputconfig = new InputConfig();
  w_about       = new AboutWindow();
}

void init_ui1() {
  CreateFonts();
  CreateWindows();
  SetFocus(w_main->hwnd);
  dd_renderer->set_window(w_main->hwnd);
  dd_renderer->to_windowed();

  w_main->show_menu();
  w_main->set_video_mode(config::video.mode);
  w_main->set_frameskip(0);

  bsnes->debugger_deactivate();
}
