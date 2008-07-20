#include "hiro.h"

#include <nall/algorithm.hpp>
using nall::min;
using nall::max;

namespace libhiro {

LRESULT CALLBACK phiro_wndproc(HWND, UINT, WPARAM, LPARAM);

#include "utf.cpp"
#include "keymap.cpp"
#include "widget.cpp"
  #include "window.cpp"
  #include "menucontrol.cpp"
    #include "menugroup.cpp"
    #include "menuitem.cpp"
    #include "menucheckitem.cpp"
    #include "menuradioitem.cpp"
    #include "menuseparator.cpp"
  #include "formcontrol.cpp"
    #include "frame.cpp"
    #include "canvas.cpp"
    #include "label.cpp"
    #include "button.cpp"
    #include "checkbox.cpp"
    #include "radiobox.cpp"
    #include "editbox.cpp"
    #include "listbox.cpp"
    #include "combobox.cpp"
    #include "progressbar.cpp"
    #include "slider.cpp"

void pHiro::init() {
  memset(&osversioninfo, 0, sizeof(OSVERSIONINFO));
  osversioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&osversioninfo);

  WNDCLASS wc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = phiro_wndproc;
  wc.lpszClassName = L"hiro_window";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  InitCommonControls();
  default_hwnd = CreateWindow(L"hiro_window", L"", WS_POPUP, 0, 0, 640, 480, 0, 0, GetModuleHandle(0), 0);
  default_font = create_font("Tahoma", 8);
  black_brush = CreateSolidBrush(RGB(0, 0, 0));
}

void pHiro::term() {
  DeleteObject(black_brush);
}

bool pHiro::run() {
  MSG msg;
  if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
    //TODO: IsDialogMessage() does not clear keyboard buffer, but is required for tab key to work ...
    //if(!IsDialogMessage(GetParent(msg.hwnd) ? GetParent(msg.hwnd) : msg.hwnd, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    //}
  }
  return pending();
}

bool pHiro::pending() {
  MSG msg;
  return PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE);
}

bool pHiro::folder_select(Window *focus, char *filename, const char *path) {
  wchar_t wfilename[_MAX_PATH] = L"";
  strcpy(filename, "");
  BROWSEINFO bi;
  bi.hwndOwner = focus ? focus->p.hwnd : 0;
  bi.pidlRoot = NULL;
  bi.pszDisplayName = wfilename;
  bi.lpszTitle = L"";
  bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;
  bi.lpfn = NULL;
  bi.lParam = 0;
  bi.iImage = 0;
  bool result = false;
  LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
  if(pidl) {
    if(SHGetPathFromIDList(pidl, wfilename)) {
      result = true;
      IMalloc *imalloc = 0;
      if(SUCCEEDED(SHGetMalloc(&imalloc))) {
        imalloc->Free(pidl);
        imalloc->Release();
      }
    }
  }
  strcpy(filename, utf8(wfilename));
  return result;
}

bool pHiro::file_open(Window *focus, char *filename, const char *path, const char *filter) {
  string dir, f;
  strcpy(dir, path ? path : "");
  replace(dir, "/", "\\");

  lstring type, part;
  strcpy(f, "");
  split(type, "\n", filter);
  for(int i = 0; i < count(type); i++) {
    split(part, "\t", type[i]);
    if(count(part) != 2) continue;

    strcat(f, part[0]);
    strcat(f, " (");
    strcat(f, part[1]);
    strcat(f, ")\t");
    replace(part[1], ",", ";");
    strcat(f, part[1]);
    strcat(f, "\t");
  }

  utf16 wfilter(f);
  utf16 wdir(dir);
  wchar_t wfilename[_MAX_PATH] = L"";

  wchar_t *p = wfilter;
  while(*p != L'\0') {
    if(*p == L'\t') *p = L'\0';
    p++;
  }

  OPENFILENAME ofn;
  strcpy(filename, "");
  memset(&ofn, 0, sizeof(ofn));
  ofn.lStructSize     = sizeof(ofn);
  ofn.hwndOwner       = focus ? focus->p.hwnd : 0;
  ofn.lpstrFilter     = wfilter;
  ofn.lpstrInitialDir = wdir;
  ofn.lpstrFile       = wfilename;
  ofn.nMaxFile        = MAX_PATH;
  ofn.Flags           = OFN_EXPLORER | OFN_FILEMUSTEXIST;
  ofn.lpstrDefExt     = L"";

  bool result = GetOpenFileName(&ofn);
  strcpy(filename, utf8(wfilename));
  return result;
}

bool pHiro::file_save(Window *focus, char *filename, const char *path, const char *filter) {
  string dir, f;
  strcpy(dir, path ? path : "");
  replace(dir, "/", "\\");

  lstring type, part;
  strcpy(f, "");
  split(type, "\n", filter);
  for(int i = 0; i < count(type); i++) {
    split(part, "\t", type[i]);
    if(count(part) != 2) continue;

    strcat(f, part[0]);
    strcat(f, " (");
    strcat(f, part[1]);
    strcat(f, ")\t");
    replace(part[1], ",", ";");
    strcat(f, part[1]);
    strcat(f, "\t");
  }

  utf16 wfilter(f);
  utf16 wdir(dir);
  wchar_t wfilename[_MAX_PATH] = L"";

  wchar_t *p = wfilter;
  while(*p != L'\0') {
    if(*p == L'\t') *p = L'\0';
    p++;
  }

  OPENFILENAME ofn;
  strcpy(filename, "");
  memset(&ofn, 0, sizeof(ofn));
  ofn.lStructSize     = sizeof(ofn);
  ofn.hwndOwner       = focus ? focus->p.hwnd : 0;
  ofn.lpstrFilter     = wfilter;
  ofn.lpstrInitialDir = wdir;
  ofn.lpstrFile       = wfilename;
  ofn.nMaxFile        = MAX_PATH;
  ofn.Flags           = OFN_EXPLORER | OFN_FILEMUSTEXIST;
  ofn.lpstrDefExt     = L"";

  bool result = GetSaveFileName(&ofn);
  strcpy(filename, utf8(wfilename));
  return result;
}

uint pHiro::screen_width() {
  return GetSystemMetrics(SM_CXSCREEN);
}

uint pHiro::screen_height() {
  return GetSystemMetrics(SM_CYSCREEN);
}

void pHiro::enable_screensaver() {
  is_screensaver_enabled = true;
}

void pHiro::disable_screensaver() {
  is_screensaver_enabled = false;
}

pHiro& pHiro::handle() {
  return hiro().p;
}

pHiro::pHiro(Hiro &self_) : self(self_) {
  is_screensaver_enabled = true;
}

pHiro& phiro() {
  return pHiro::handle();
}

/* internal */

HFONT pHiro::create_font(const char *name, uint size) {
  return CreateFont(
    -(size * 96.0 / 72.0 + 0.5), //96 = DPI
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    utf16(name)
  );
}

Widget* pHiro::get_widget(uint instance) {
  Widget *widget = 0;
  for(uint i = 0; i < widget_list.size(); i++) {
    if(widget_list[i]->p.instance != instance) continue;
    widget = widget_list[i];
    break;
  }
  return widget;
}

LRESULT CALLBACK phiro_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  return phiro().wndproc(hwnd, msg, wparam, lparam);
}

LRESULT pHiro::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  pWidget *p = (pWidget*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

  switch(msg) {
    case WM_SYSCOMMAND: {
      switch(wparam) {
        case SC_SCREENSAVE:
        case SC_MONITORPOWER: {
          if(is_screensaver_enabled == false) return FALSE;
          //fallthrough to DefWindowProc()
        } break;
      }
    } break;

    case WM_SIZE: {
      if(!p || p->self.type != Widget::WindowType) break;
      SendMessage(((pWindow*)p)->hstatus, WM_SIZE, 0, 0); //the control itself auto-sizes after receiving WM_SIZE
    } break;

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
      if(p->self.type == Widget::CanvasType) brush = phiro().black_brush;
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
      if(p && p->self.type == Widget::CanvasType) ((pCanvas*)p)->redraw();
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
          if(w.on_tick) w.on_tick(Event(Event::Tick, w.checked(), &w));
        } break;
        case Widget::MenuRadioItemType: {
          MenuRadioItem &w = (MenuRadioItem&)*widget;
          bool checked = w.checked();
          w.check();
          if(!checked && w.on_tick) w.on_tick(Event(Event::Tick, w.checked(), &w));
        } break;
        case Widget::ButtonType: {
          Button &w = (Button&)*widget;
          if(w.on_tick) w.on_tick(Event(Event::Tick, 0, &w));
        } break;
        case Widget::CheckboxType: {
          Checkbox &w = (Checkbox&)*widget;
          w.check(!w.checked()); //invert check state
          if(w.on_tick) w.on_tick(Event(Event::Tick, w.checked(), &w));
        } break;
        case Widget::RadioboxType: {
          Radiobox &w = (Radiobox&)*widget;
          bool checked = w.checked();
          w.check();
          if(!checked && w.on_tick) w.on_tick(Event(Event::Tick, w.checked(), &w));
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

} //namespace libhiro
