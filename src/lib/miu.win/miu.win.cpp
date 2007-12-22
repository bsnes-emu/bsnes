#include "miu.win.h"
#include "../miu.cpp"

namespace ns_miu {

long __stdcall pmiu_wndproc(HWND, UINT, WPARAM, LPARAM);

#include "miu.win.keymap.cpp"
#include "miu.win.widget.cpp"
  #include "miu.win.window.cpp"
  #include "miu.win.menucontrol.cpp"
    #include "miu.win.menugroup.cpp"
    #include "miu.win.menuitem.cpp"
    #include "miu.win.menucheckitem.cpp"
    #include "miu.win.menuradioitem.cpp"
    #include "miu.win.menuseparator.cpp"
  #include "miu.win.formcontrol.cpp"
    #include "miu.win.frame.cpp"
    #include "miu.win.canvas.cpp"
    #include "miu.win.label.cpp"
    #include "miu.win.button.cpp"
    #include "miu.win.checkbox.cpp"
    #include "miu.win.radiobox.cpp"
    #include "miu.win.editbox.cpp"
    #include "miu.win.listbox.cpp"
    #include "miu.win.combobox.cpp"
    #include "miu.win.progressbar.cpp"
    #include "miu.win.slider.cpp"

void pMiu::init() {
WNDCLASS wc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = pmiu_wndproc;
  wc.lpszClassName = "miu_window";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  InitCommonControls();
  default_hwnd = CreateWindow("miu_window", "", WS_POPUP, 0, 0, 640, 480, 0, 0, GetModuleHandle(0), 0);
  default_font = create_font("Tahoma", 9);
  black_brush = CreateSolidBrush(RGB(0, 0, 0));
}

void pMiu::term() {
  DeleteObject(black_brush);
}

bool pMiu::run() {
MSG msg;
  if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return pending();
}

bool pMiu::pending() {
MSG msg;
  return PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE);
}

bool pMiu::file_load(Window *focus, char *filename, const char *filter, const char *path) {
string dir, f;
  strcpy(dir, path ? path : "");
  replace(dir, "/", "\\");

lstring type, part;
  strcpy(f, "");
  split(type, "|", filter);
  for(int i = 0; i < count(type); i++) {
    split(part, ";", type[i]);
    if(count(part) != 2)continue;

    strcat(f, part[0]);
    strcat(f, " (");
    strcat(f, part[1]);
    strcat(f, ")|");
    replace(part[1], ",", ";");
    strcat(f, part[1]);
    strcat(f, "|");
  }

char *pf = f();
  for(int i = strlen(pf) - 1; i >= 0; i--) {
    if(pf[i] == '|') pf[i] = '\0';
  }

OPENFILENAME ofn;
  strcpy(filename, "");
  memset(&ofn, 0, sizeof(ofn));
  ofn.lStructSize     = sizeof(ofn);
  ofn.hwndOwner       = focus ? focus->p.hwnd : 0;
  ofn.lpstrFilter     = pf;
  ofn.lpstrInitialDir = dir;
  ofn.lpstrFile       = filename;
  ofn.nMaxFile        = MAX_PATH;
  ofn.Flags           = OFN_EXPLORER | OFN_FILEMUSTEXIST;
  ofn.lpstrDefExt     = "";

  return GetOpenFileName(&ofn);
}

bool pMiu::file_save(Window *focus, char *filename, const char *filter, const char *path) {
string dir, f;
  strcpy(dir, path ? path : "");
  replace(dir, "/", "\\");

lstring type, part;
  strcpy(f, "");
  split(type, "|", filter);
  for(int i = 0; i < count(type); i++) {
    split(part, ";", type[i]);
    if(count(part) != 2)continue;

    strcat(f, part[0]);
    strcat(f, " (");
    strcat(f, part[1]);
    strcat(f, ")|");
    replace(part[1], ",", ";");
    strcat(f, part[1]);
    strcat(f, "|");
  }

char *pf = f();
  for(int i = strlen(pf) - 1; i >= 0; i--) {
    if(pf[i] == '|') pf[i] = '\0';
  }

OPENFILENAME ofn;
  strcpy(filename, "");
  memset(&ofn, 0, sizeof(ofn));
  ofn.lStructSize     = sizeof(ofn);
  ofn.hwndOwner       = focus ? focus->p.hwnd : 0;
  ofn.lpstrFilter     = pf;
  ofn.lpstrInitialDir = dir;
  ofn.lpstrFile       = filename;
  ofn.nMaxFile        = MAX_PATH;
  ofn.Flags           = OFN_EXPLORER | OFN_FILEMUSTEXIST;
  ofn.lpstrDefExt     = "";

  return GetSaveFileName(&ofn);
}

uint pMiu::screen_width() {
  return GetSystemMetrics(SM_CXSCREEN);
}

uint pMiu::screen_height() {
  return GetSystemMetrics(SM_CYSCREEN);
}

pMiu& pMiu::handle() {
  return miu().p;
}

pMiu::pMiu(Miu &self_) : self(self_) {
}

pMiu& pmiu() {
  return pMiu::handle();
}

/* internal */

HFONT pMiu::create_font(const char *name, uint size) {
HDC hdc = GetDC(0);
HFONT font = CreateFont(-MulDiv(size, GetDeviceCaps(hdc, LOGPIXELSY), 72),
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, name);
  ReleaseDC(0, hdc);
  return font;
}

Widget* pMiu::get_widget(uint instance) {
Widget *widget = 0;
  for(uint i = 0; i < widget_list.size(); i++) {
    if(widget_list[i]->p.instance != instance) continue;
    widget = widget_list[i];
    break;
  }
  return widget;
}

long __stdcall pmiu_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  return pmiu().wndproc(hwnd, msg, wparam, lparam);
}

long pMiu::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
pWidget *p = (pWidget*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

  switch(msg) {
    case WM_CLOSE: {
      if(!p || p->self.type != Widget::WindowType) break;
    Window &w = ((pWindow*)p)->self;
      if(w.on_close) return (bool)w.on_close(Event(Event::Close, 0, &w));
      return TRUE; //true = destroy window
    } break;

    case WM_ENTERMENULOOP: {
      if(!p || p->self.type != Widget::WindowType) break;
    Window &w = ((pWindow*)p)->self;
      if(w.on_block) w.on_block(Event(Event::Block, 0, &w));
    } break;

    case WM_KEYDOWN: {
      if(!p || p->self.type != Widget::WindowType) break;
    Window &w = ((pWindow*)p)->self;
      if(w.on_keydown) w.on_keydown(Event(Event::KeyDown, translate_key(wparam), &w));
    } break;

    case WM_KEYUP: {
      if(!p || p->self.type != Widget::WindowType) break;
    Window &w = ((pWindow*)p)->self;
      if(w.on_keyup) w.on_keyup(Event(Event::KeyUp, translate_key(wparam), &w));
    } break;

    case WM_ERASEBKGND: {
      if(!p) break;
    HBRUSH brush = 0;
      if(p->self.type == Widget::WindowType) brush = ((pWindow*)p)->background;
      if(p->self.type == Widget::CanvasType) brush = pmiu().black_brush;
      if(!brush) break;
    RECT rc;
      GetClientRect(hwnd, &rc);
    PAINTSTRUCT ps;
      BeginPaint(hwnd, &ps);
      FillRect(ps.hdc, &rc, brush);
      EndPaint(hwnd, &ps);
      return TRUE;
    } break;

    case WM_PAINT: {
      if(p && p->self.type == Widget::CanvasType) ((pCanvas*)p)->blit();
    } break;

    case WM_COMMAND: {
    Widget *widget = get_widget(LOWORD(wparam));
      if(!widget) break;

      switch(widget->type) {
        case Widget::MenuItemType: {
        MenuItem &w = (MenuItem&)*widget;
          if(w.on_tick) w.on_tick(Event(Event::Tick, 0, &w));
        } break;
        case Widget::MenuCheckItemType: {
        MenuCheckItem &w = (MenuCheckItem&)*widget;
          w.check(!w.checked()); //invert check state
        } break;
        case Widget::MenuRadioItemType: {
        MenuRadioItem &w = (MenuRadioItem&)*widget;
          w.check();
        } break;
        case Widget::ButtonType: {
        Button &w = (Button&)*widget;
          if(w.on_tick) w.on_tick(Event(Event::Tick, 0, &w));
        } break;
        case Widget::CheckboxType: {
        Checkbox &w = (Checkbox&)*widget;
          w.check(!w.checked()); //invert check state
        } break;
        case Widget::RadioboxType: {
        Radiobox &w = (Radiobox&)*widget;
          w.check();
        } break;
        case Widget::ComboboxType: {
        Combobox &combobox = (Combobox&)*widget;
          if(HIWORD(wparam) == CBN_SELCHANGE) {
            if(combobox.p.combobox_selection == combobox.get_selection()) break;
            if(combobox.on_change) combobox.on_change(Event(Event::Change, combobox.p.combobox_selection = combobox.get_selection(), &combobox));
          }
        } break;
      }
    } break;

    case WM_HSCROLL:
    case WM_VSCROLL: {
    Widget *widget = get_widget(GetDlgCtrlID((HWND)lparam));
      if(!widget) break;

      switch(widget->type) {
        case Widget::SliderType: {
        Slider &slider = (Slider&)*widget;
          if(slider.p.slider_position == slider.get_position()) break;
          if(slider.on_change) slider.on_change(Event(Event::Change, slider.p.slider_position = slider.get_position(), &slider));
        } break;
      }
    } break;

    case WM_NOTIFY: {
    Widget *widget = get_widget(LOWORD(wparam));
      if(!widget) break;

      switch(widget->type) {
        case Widget::ListboxType: {
        Listbox &listbox = (Listbox&)*widget;
          if(((LPNMHDR)lparam)->code == LVN_ITEMCHANGED
            && ((LPNMLISTVIEW)lparam)->uChanged & LVIF_STATE
            && ListView_GetItemState(listbox.p.hwnd, ((LPNMLISTVIEW)lparam)->iItem, LVIS_FOCUSED)
            && ListView_GetItemState(listbox.p.hwnd, ((LPNMLISTVIEW)lparam)->iItem, LVIS_SELECTED)
          ) {
            if(listbox.on_change) listbox.on_change(Event(Event::Change, listbox.get_selection(), &listbox));
          } else if(((LPNMHDR)lparam)->code == LVN_ITEMACTIVATE) {
            if(listbox.on_activate) listbox.on_activate(Event(Event::Activate, listbox.get_selection(), &listbox));
          }
        } break;
      }
    } break;
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

} //namespace ns_miu
