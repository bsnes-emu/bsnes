HFONT hFont, hMonofont;

#include "ui_window.cpp"
#include "ui_render.cpp"
#include "ui_main.cpp"
#include "ui_console.cpp"
#include "ui_bp.cpp"
#include "ui_memory.cpp"

void CreateFonts() {
HDC hdc;
long height;
  hdc = GetDC(0);
  height = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
  ReleaseDC(0, hdc);
  hFont = CreateFont(height, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Tahoma");

  hdc = GetDC(0);
  height = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
  ReleaseDC(0, hdc);
  hMonofont = CreateFont(height, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Courier New");
}

void CreateWindows() {
  CreateWindowMain();
  CreateWindowConsole();
  CreateWindowBreakpoint();
  CreateWindowMemory();
}

void init_ui0() {
  renderer  = new Render();
  w_main    = new MainWindow();
  w_console = new Console();
  w_bp      = new BreakpointEditor();
  w_memory  = new MemoryEditor();
}

void init_ui1() {
  CreateFonts();
  CreateWindows();
  SetFocus(w_main->hwnd);
  renderer->set_window(w_main->hwnd);
  renderer->to_windowed();
  bsnes->debugger_deactivate();
}
