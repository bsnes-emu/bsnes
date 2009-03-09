#include "hiro.hpp"
#include "port.cpp"

namespace libhiro {

LRESULT CALLBACK phiro_wndproc(HWND, UINT, WPARAM, LPARAM);

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
    #if defined(HIRO_WIN_TABSTOP)
    if(!IsDialogMessage(GetParent(msg.hwnd) ? GetParent(msg.hwnd) : msg.hwnd, &msg)) {
    #endif
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    #if defined(HIRO_WIN_TABSTOP)
    }
    #endif
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
  dir = path ? path : "";
  dir.replace("/", "\\");

  lstring type, part;
  type.split("\n", filter);
  for(int i = 0; i < type.size(); i++) {
    part.split("\t", type[i]);
    if(part.size() != 2) continue;

    f.append(part[0]);
    f.append(" (");
    f.append(part[1]);
    f.append(")\t");
    part[1].replace(",", ";");
    f.append(part[1]);
    f.append("\t");
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
  ofn.Flags           = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
  ofn.lpstrDefExt     = L"";

  bool result = GetOpenFileName(&ofn);
  strcpy(filename, utf8(wfilename));
  return result;
}

bool pHiro::file_save(Window *focus, char *filename, const char *path, const char *filter) {
  string dir, f;
  dir = path ? path : "";
  dir.replace("/", "\\");

  lstring type, part;
  type.split("\n", filter);
  for(int i = 0; i < type.size(); i++) {
    part.split("\t", type[i]);
    if(part.size() != 2) continue;

    f.append(part[0]);
    f.append(" (");
    f.append(part[1]);
    f.append(")\t");
    part[1].replace(",", ";");
    f.append(part[1]);
    f.append("\t");
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
  ofn.Flags           = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
  ofn.lpstrDefExt     = L"";

  bool result = GetSaveFileName(&ofn);
  strcpy(filename, utf8(wfilename));
  return result;
}

unsigned pHiro::screen_width() {
  return GetSystemMetrics(SM_CXSCREEN);
}

unsigned pHiro::screen_height() {
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

HFONT pHiro::create_font(const char *name, unsigned size) {
  return CreateFont(
    -(size * 96.0 / 72.0 + 0.5),  //96 = DPI
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    utf16(name)
  );
}

Widget* pHiro::get_widget(unsigned instance) {
  Widget *widget = 0;
  for(unsigned i = 0; i < widget_list.size(); i++) {
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
      if(w.on_close) return (bool)w.on_close(event_t(event_t::Close, 0, &w));
      return TRUE; //true = destroy window
    } break;

    case WM_ENTERMENULOOP: {
      if(!p || p->self.type != Widget::WindowType) break;
      Window &w = ((pWindow*)p)->self;
      if(w.on_block) w.on_block(event_t(event_t::Block, 0, &w));
    } break;

    case WM_KEYDOWN: {
      if(!p || p->self.type != Widget::WindowType) break;
      Window &w = ((pWindow*)p)->self;
      if(w.on_input) w.on_input(event_t(event_t::Input, translate_key(wparam) + (1 << 16), &w));
    } break;

    case WM_KEYUP: {
      if(!p || p->self.type != Widget::WindowType) break;
      Window &w = ((pWindow*)p)->self;
      if(w.on_input) w.on_input(event_t(event_t::Input, translate_key(wparam) + (0 << 16), &w));
    } break;

    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN: {
      if(!p || p->self.type != Widget::CanvasType) break;
      Canvas &canvas = ((pCanvas*)p)->self;
      uintptr_t param = (msg == WM_LBUTTONDOWN ? mouse::button + 0 : mouse::button + 1) + (1 << 16);
      if(canvas.on_input) canvas.on_input(event_t(event_t::Input, param, &canvas));
    } break;

    case WM_LBUTTONUP:
    case WM_RBUTTONUP: {
      if(!p || p->self.type != Widget::CanvasType) break;
      Canvas &canvas = ((pCanvas*)p)->self;
      uintptr_t param = (msg == WM_LBUTTONUP ? mouse::button + 0 : mouse::button + 1) + (0 << 16);
      if(canvas.on_input) canvas.on_input(event_t(event_t::Input, param, &canvas));
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
          if(w.on_tick) w.on_tick(event_t(event_t::Tick, 0, &w));
        } break;

        case Widget::MenuCheckItemType: {
          MenuCheckItem &w = (MenuCheckItem&)*widget;
          w.check(!w.checked()); //invert check state
          if(w.on_tick) w.on_tick(event_t(event_t::Tick, w.checked(), &w));
        } break;

        case Widget::MenuRadioItemType: {
          MenuRadioItem &w = (MenuRadioItem&)*widget;
          bool checked = w.checked();
          w.check();
          if(!checked && w.on_tick) w.on_tick(event_t(event_t::Tick, w.checked(), &w));
        } break;

        case Widget::ButtonType: {
          Button &w = (Button&)*widget;
          if(w.on_tick) w.on_tick(event_t(event_t::Tick, 0, &w));
        } break;

        case Widget::CheckboxType: {
          Checkbox &w = (Checkbox&)*widget;
          w.check(!w.checked()); //invert check state
          if(w.on_tick) w.on_tick(event_t(event_t::Tick, w.checked(), &w));
        } break;

        case Widget::RadioboxType: {
          Radiobox &w = (Radiobox&)*widget;
          bool checked = w.checked();
          w.check();
          if(!checked && w.on_tick) w.on_tick(event_t(event_t::Tick, w.checked(), &w));
        } break;

        case Widget::EditboxType: {
          Editbox &editbox = (Editbox&)*widget;
          if(HIWORD(wparam) == EN_CHANGE) {
            editbox.p.update();  //called to dynamically display vertical scrollbar if needed
            if(editbox.on_change) editbox.on_change(event_t(event_t::Change, 0, &editbox));
          }
        } break;

        case Widget::ComboboxType: {
          Combobox &combobox = (Combobox&)*widget;
          if(HIWORD(wparam) == CBN_SELCHANGE) {
            if(combobox.p.combobox_selection == combobox.get_selection()) break;
            if(combobox.on_change) combobox.on_change(event_t(event_t::Change, combobox.p.combobox_selection = combobox.get_selection(), &combobox));
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
          if(slider.on_change) slider.on_change(event_t(event_t::Change, slider.p.slider_position = slider.get_position(), &slider));
        } break;
      }
    } break;

    case WM_NOTIFY: {
      Widget *widget = get_widget(LOWORD(wparam));
      if(!widget) break;

      switch(widget->type) {
        case Widget::ListboxType: {
          Listbox &listbox = (Listbox&)*widget;
          LPNMHDR nmhdr = (LPNMHDR)lparam;
          LPNMLISTVIEW nmlistview = (LPNMLISTVIEW)lparam;

          if(nmhdr->code == LVN_ITEMCHANGED && (nmlistview->uChanged & LVIF_STATE)) {
            //LVN_ITEMCHANGED is sent whenever an item gains or loses either focus or selection;
            //it does not send a special message to indicate that no items are focused or selected.
            //it will send two messages when a different item gains selection -- the first to remove
            //focus from the old item, the second to set selection to the new item.
            //hiro sends only one message whenever an item changed (eg gained selection),
            //including for deselection of all items. below code adapts win32 model to hiro model.
            //(focused means an item has a dotted outline box around it, but is not highlighted.)
            //(selected means an item is highlighted.)
            if((nmlistview->uOldState & LVIS_FOCUSED) && !(nmlistview->uNewState & LVIS_FOCUSED)) {
              listbox.p.lostfocus = true;
            } else {
              if((!(nmlistview->uOldState & LVIS_SELECTED) && (nmlistview->uNewState & LVIS_SELECTED))
              || (listbox.p.lostfocus == false && listbox.get_selection() == -1)) {
                if(listbox.on_change) listbox.on_change(event_t(event_t::Change, listbox.get_selection(), &listbox));
              }
              listbox.p.lostfocus = false;
            }
          } else if(nmhdr->code == LVN_ITEMACTIVATE) {
            if(listbox.on_activate) listbox.on_activate(event_t(event_t::Activate, listbox.get_selection(), &listbox));
          }
        } break;
      }
    } break;
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

} //namespace libhiro
