#include <windows.h>
#include <commctrl.h>
#include <io.h>
#include <direct.h>
#include <shlobj.h>
#include <nall/platform.hpp>
#include <nall/utf8.hpp>
using namespace nall;

namespace phoenix {

#include "object.cpp"
#include "font.cpp"
#include "menu.cpp"
#include "widget.cpp"
#include "window.cpp"
#include "button.cpp"
#include "canvas.cpp"
#include "checkbox.cpp"
#include "combobox.cpp"
#include "editbox.cpp"
#include "horizontalslider.cpp"
#include "label.cpp"
#include "listbox.cpp"
#include "progressbar.cpp"
#include "radiobox.cpp"
#include "textbox.cpp"
#include "verticalslider.cpp"
#include "viewport.cpp"
#include "messagewindow.cpp"

OS::Data *OS::os = 0;
Window Window::None;

static void OS_keyboardProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK OS_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

void OS::initialize() {
  static bool initialized = false;
  if(initialized == true) return;
  initialized = true;

  InitCommonControls();
  CoInitialize(0);

  os = new OS::Data;
  os->proportionalFont = Font_createFont("Tahoma", 8, false, false);
  os->monospaceFont = Font_createFont("Courier New", 8, false, false);

  WNDCLASS wc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(2));
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = OS_windowProc;
  wc.lpszClassName = L"phoenix_window";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = Canvas_windowProc;
  wc.lpszClassName = L"phoenix_canvas";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = Label_windowProc;
  wc.lpszClassName = L"phoenix_label";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = Viewport_windowProc;
  wc.lpszClassName = L"phoenix_viewport";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);
}

bool OS::pending() {
  MSG msg;
  return PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE);
}

void OS::run() {
  while(pending()) {
    MSG msg;
    if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      if(msg.message == WM_KEYDOWN || msg.message == WM_KEYUP) {
        OS_keyboardProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
      }
      if(!IsDialogMessage(GetParent(msg.hwnd) ? GetParent(msg.hwnd) : msg.hwnd, &msg)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
  }
}

void OS::main() {
  MSG msg;
  while(GetMessage(&msg, 0, 0, 0)) {
    if(msg.message == WM_KEYDOWN || msg.message == WM_KEYUP) {
      OS_keyboardProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
    }
    if(!IsDialogMessage(GetParent(msg.hwnd) ? GetParent(msg.hwnd) : msg.hwnd, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
}

void OS::quit() {
  PostQuitMessage(0);
}

unsigned OS::desktopWidth() {
  return GetSystemMetrics(SM_CXSCREEN);
}

unsigned OS::desktopHeight() {
  return GetSystemMetrics(SM_CYSCREEN);
}

string OS::folderSelect(Window &parent, const string &path) {
  wchar_t wfilename[PATH_MAX + 1] = L"";
  BROWSEINFO bi;
  bi.hwndOwner = &parent != &Window::None ? parent.widget->window : 0;
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
  if(result == false) return "";
  string name = utf8_t(wfilename);
  name.transform("\\", "/");
  if(name.endswith("/") == false) name.append("/");
  return name;
}

string OS::fileOpen(Window &parent, const string &filter, const string &path) {
  string dir = path;
  dir.replace("/", "\\");

  string filterInfo;
  lstring type;
  type.split("\n", filter);
  for(unsigned i = 0; i < type.size(); i++) {
    lstring part;
    part.split("\t", type[i]);
    if(part.size() != 2) continue;
    filterInfo.append(part[0]);
    filterInfo.append(" (");
    filterInfo.append(part[1]);
    filterInfo.append(")\t");
    part[1].replace(",", ";");
    filterInfo.append(part[1]);
    filterInfo.append("\t");
  }

  utf16_t wfilter(filterInfo);
  utf16_t wdir(dir);
  wchar_t wfilename[PATH_MAX] = L"";

  wchar_t *p = wfilter;
  while(*p != L'\0') {
    if(*p == L'\t') *p = L'\0';
    p++;
  }

  OPENFILENAME ofn;
  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = &parent != &Window::None ? parent.widget->window : 0;
  ofn.lpstrFilter = wfilter;
  ofn.lpstrInitialDir = wdir;
  ofn.lpstrFile = wfilename;
  ofn.nMaxFile = PATH_MAX;
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
  ofn.lpstrDefExt = L"";

  bool result = GetOpenFileName(&ofn);
  if(result == false) return "";
  string name = utf8_t(wfilename);
  name.transform("\\", "/");
  return name;
}

string OS::fileSave(Window &parent, const string &filter, const string &path) {
  string dir = path;
  dir.replace("/", "\\");

  string filterInfo;
  lstring type;
  type.split("\n", filter);
  for(unsigned i = 0; i < type.size(); i++) {
    lstring part;
    part.split("\t", type[i]);
    if(part.size() != 2) continue;
    filterInfo.append(part[0]);
    filterInfo.append(" (");
    filterInfo.append(part[1]);
    filterInfo.append(")\t");
    part[1].replace(",", ";");
    filterInfo.append(part[1]);
    filterInfo.append("\t");
  }

  utf16_t wfilter(filterInfo);
  utf16_t wdir(dir);
  wchar_t wfilename[PATH_MAX] = L"";

  wchar_t *p = wfilter;
  while(*p != L'\0') {
    if(*p == L'\t') *p = L'\0';
    p++;
  }

  OPENFILENAME ofn;
  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = &parent != &Window::None ? parent.widget->window : 0;
  ofn.lpstrFilter = wfilter;
  ofn.lpstrInitialDir = wdir;
  ofn.lpstrFile = wfilename;
  ofn.nMaxFile = PATH_MAX;
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
  ofn.lpstrDefExt = L"";

  bool result = GetSaveFileName(&ofn);
  if(result == false) return "";
  string name = utf8_t(wfilename);
  name.transform("\\", "/");
  return name;
}

static void OS_keyboardProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch(msg) {
    case WM_KEYDOWN: {
      GUITHREADINFO info;
      memset(&info, 0, sizeof(GUITHREADINFO));
      info.cbSize = sizeof(GUITHREADINFO);
      GetGUIThreadInfo(GetCurrentThreadId(), &info);
      Object *object_ptr = (Object*)GetWindowLongPtr(info.hwndFocus, GWLP_USERDATA);
      if(object_ptr) {
        if(dynamic_cast<ListBox*>(object_ptr)) {
          ListBox &listBox = (ListBox&)*object_ptr;
          if(wparam == VK_RETURN) {
            if(listBox.onActivate) listBox.onActivate();
          }
        } else if(dynamic_cast<TextBox*>(object_ptr)) {
          TextBox &textBox = (TextBox&)*object_ptr;
          if(wparam == VK_RETURN) {
            if(textBox.onActivate) textBox.onActivate();
          }
        }
      }
    }
  }
}

static LRESULT CALLBACK OS_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  Object *object_ptr = (Object*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(!object_ptr || !dynamic_cast<Window*>(object_ptr)) return DefWindowProc(hwnd, msg, wparam, lparam);
  Window &window = (Window&)*object_ptr;

  switch(msg) {
    case WM_CLOSE: {
      if(window.onClose) {
        if(window.onClose()) window.setVisible(false);
      } else {
        window.setVisible(false);
      }
      return TRUE;
    }

    case WM_ERASEBKGND: {
      if(window.window->brush == 0) break;
      RECT rc;
      GetClientRect(window.widget->window, &rc);
      PAINTSTRUCT ps;
      BeginPaint(window.widget->window, &ps);
      FillRect(ps.hdc, &rc, window.window->brush);
      EndPaint(window.widget->window, &ps);
      return TRUE;
    }

    case WM_CTLCOLORBTN:
    case WM_CTLCOLORSTATIC: {
      Object *object_ptr = (Object*)GetWindowLongPtr((HWND)lparam, GWLP_USERDATA);
      if(object_ptr && window.window->brush) {
        HDC hdc = (HDC)wparam;
        SetBkColor((HDC)wparam, window.window->brushColor);
        return (INT_PTR)window.window->brush;
      }
    }

    case WM_COMMAND: {
      unsigned id = LOWORD(wparam);
      HWND control = GetDlgItem(window.widget->window, id);
      if(control == 0) {
        Object *object_ptr = (Object*)OS::findObject(id);
        if(object_ptr) {
          if(dynamic_cast<MenuItem*>(object_ptr)) {
            MenuItem &menuItem = (MenuItem&)*object_ptr;
            if(menuItem.onTick) menuItem.onTick();
          } else if(dynamic_cast<MenuCheckItem*>(object_ptr)) {
            MenuCheckItem &menuCheckItem = (MenuCheckItem&)*object_ptr;
            menuCheckItem.setChecked(!menuCheckItem.checked());
            if(menuCheckItem.onTick) menuCheckItem.onTick();
          } else if(dynamic_cast<MenuRadioItem*>(object_ptr)) {
            MenuRadioItem &menuRadioItem = (MenuRadioItem&)*object_ptr;
            if(menuRadioItem.checked() == false) {
              menuRadioItem.setChecked();
              if(menuRadioItem.onTick) menuRadioItem.onTick();
            }
          }
        }
      } else {
        Object *object_ptr = (Object*)GetWindowLongPtr(control, GWLP_USERDATA);
        if(object_ptr) {
          if(dynamic_cast<Button*>(object_ptr)) {
            Button &button = (Button&)*object_ptr;
            if(button.onTick) button.onTick();
          } else if(dynamic_cast<CheckBox*>(object_ptr)) {
            CheckBox &checkBox = (CheckBox&)*object_ptr;
            checkBox.setChecked(!checkBox.checked());
            if(checkBox.onTick) checkBox.onTick();
          } else if(dynamic_cast<ComboBox*>(object_ptr)) {
            ComboBox &comboBox = (ComboBox&)*object_ptr;
            if(HIWORD(wparam) == CBN_SELCHANGE) {
              if(comboBox.comboBox->selection != comboBox.selection()) {
                comboBox.comboBox->selection = comboBox.selection();
                if(comboBox.onChange) comboBox.onChange();
              }
            }
          } else if(dynamic_cast<EditBox*>(object_ptr)) {
            EditBox &editBox = (EditBox&)*object_ptr;
            if(HIWORD(wparam) == EN_CHANGE) {
              if(editBox.object->locked == false && editBox.onChange) editBox.onChange();
            }
          } else if(dynamic_cast<RadioBox*>(object_ptr)) {
            RadioBox &radioBox = (RadioBox&)*object_ptr;
            if(radioBox.checked() == false) {
              radioBox.setChecked();
              if(radioBox.onTick) radioBox.onTick();
            }
          } else if(dynamic_cast<TextBox*>(object_ptr)) {
            TextBox &textBox = (TextBox&)*object_ptr;
            if(HIWORD(wparam) == EN_CHANGE) {
              if(textBox.object->locked == false && textBox.onChange) textBox.onChange();
            }
          }
        }
      }
    }

    case WM_NOTIFY: {
      unsigned id = LOWORD(wparam);
      HWND control = GetDlgItem(window.widget->window, id);
      if(control) {
        Object *object_ptr = (Object*)GetWindowLongPtr(control, GWLP_USERDATA);
        if(object_ptr) {
          if(dynamic_cast<ListBox*>(object_ptr)) {
            ListBox &listBox = (ListBox&)*object_ptr;
            LPNMHDR nmhdr = (LPNMHDR)lparam;
            LPNMLISTVIEW nmlistview = (LPNMLISTVIEW)lparam;

            if(nmhdr->code == LVN_ITEMCHANGED && (nmlistview->uChanged & LVIF_STATE)) {
              unsigned imagemask = ((nmlistview->uNewState & LVIS_STATEIMAGEMASK) >> 12) - 1;
              if(imagemask == 0 || imagemask == 1) {
                if(listBox.object->locked == false && listBox.onTick) listBox.onTick(nmlistview->iItem);
              } else if((nmlistview->uOldState & LVIS_FOCUSED) && !(nmlistview->uNewState & LVIS_FOCUSED)) {
                listBox.listBox->lostFocus = true;
              } else {
                if(!(nmlistview->uOldState & LVIS_SELECTED) && (nmlistview->uNewState & LVIS_SELECTED)) {
                  if(listBox.onChange) listBox.onChange();
                } else if(listBox.listBox->lostFocus == false && listBox.selection() == false) {
                  if(listBox.onChange) listBox.onChange();
                }
                listBox.listBox->lostFocus = false;
              }
            } else if(nmhdr->code == LVN_ITEMACTIVATE) {
              if(listBox.onActivate) listBox.onActivate();
            }
          }
        }
      }
    }

    case WM_HSCROLL: {
      unsigned id = LOWORD(wparam);
      HWND control = GetDlgItem(window.widget->window, id);
      if(control) {
        Object *object_ptr = (Object*)GetWindowLongPtr(control, GWLP_USERDATA);
        if(object_ptr) {
          if(dynamic_cast<HorizontalSlider*>(object_ptr)) {
            HorizontalSlider &horizontalSlider = (HorizontalSlider&)*object_ptr;
            if(horizontalSlider.horizontalSlider->position != horizontalSlider.position()) {
              horizontalSlider.horizontalSlider->position = horizontalSlider.position();
              if(horizontalSlider.onChange) horizontalSlider.onChange();
            }
          }
        }
      }
    }

    case WM_VSCROLL: {
      unsigned id = LOWORD(wparam);
      HWND control = GetDlgItem(window.widget->window, id);
      if(control) {
        Object *object_ptr = (Object*)GetWindowLongPtr(control, GWLP_USERDATA);
        if(object_ptr) {
          if(dynamic_cast<VerticalSlider*>(object_ptr)) {
            VerticalSlider &verticalSlider = (VerticalSlider&)*object_ptr;
            if(verticalSlider.verticalSlider->position != verticalSlider.position()) {
              verticalSlider.verticalSlider->position = verticalSlider.position();
              if(verticalSlider.onChange) verticalSlider.onChange();
            }
          }
        }
      }
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

Object* OS::findObject(unsigned id) {
  foreach(object, os->objects) { if(object->object->id == id) return object; }
  return 0;
}

}
