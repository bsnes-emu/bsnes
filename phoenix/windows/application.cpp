namespace phoenix {

static bool Application_keyboardProc(HWND, UINT, WPARAM, LPARAM);
static void Application_processDialogMessage(MSG&);
static LRESULT CALLBACK Application_windowProc(HWND, UINT, WPARAM, LPARAM);

void pApplication::run() {
  MSG msg;
  if(Application::main) {
    while(applicationState.quit == false) {
      Application::main();
      processEvents();
    }
  } else {
    MSG msg;
    while(GetMessage(&msg, 0, 0, 0)) {
      Application_processDialogMessage(msg);
    }
  }
}

bool pApplication::pendingEvents() {
  MSG msg;
  return PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE);
}

void pApplication::processEvents() {
  while(pendingEvents()) {
    MSG msg;
    if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      Application_processDialogMessage(msg);
    }
  }
}

void Application_processDialogMessage(MSG& msg) {
  if(msg.message == WM_KEYDOWN || msg.message == WM_KEYUP
  || msg.message == WM_SYSKEYDOWN || msg.message == WM_SYSKEYUP) {
    if(Application_keyboardProc(msg.hwnd, msg.message, msg.wParam, msg.lParam)) {
      DispatchMessage(&msg);
      return;
    }
  }

  if(!IsDialogMessage(GetForegroundWindow(), &msg)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

void pApplication::quit() {
  PostQuitMessage(0);
}

void pApplication::initialize() {
  CoInitialize(0);
  InitCommonControls();

  WNDCLASS wc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(2));
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = Application_windowProc;
  wc.lpszClassName = L"phoenix_window";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
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
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
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

  settings = new Settings;
  pKeyboard::initialize();
}

static bool Application_keyboardProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  if(msg != WM_KEYDOWN && msg != WM_SYSKEYDOWN && msg != WM_KEYUP && msg != WM_SYSKEYUP) return false;

  GUITHREADINFO info;
  memset(&info, 0, sizeof(GUITHREADINFO));
  info.cbSize = sizeof(GUITHREADINFO);
  GetGUIThreadInfo(GetCurrentThreadId(), &info);
  Object* object = (Object*)GetWindowLongPtr(info.hwndFocus, GWLP_USERDATA);
  if(object == nullptr) return false;

  if(dynamic_cast<Window*>(object)) {
    Window& window = (Window&)*object;
    if(pWindow::modal.size() > 0 && !pWindow::modal.find(&window.p)) return false;
    Keyboard::Keycode keysym = Keysym(wparam, lparam);
    if(keysym != Keyboard::Keycode::None) {
      if((msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN) && window.onKeyPress) window.onKeyPress(keysym);
      if((msg == WM_KEYUP || msg == WM_SYSKEYUP) && window.onKeyRelease) window.onKeyRelease(keysym);
    }
    return false;
  }

  if(msg == WM_KEYDOWN) {
    if(dynamic_cast<ListView*>(object)) {
      ListView& listView = (ListView&)*object;
      if(wparam == VK_RETURN) {
        if(listView.selected()) return true;  //returning true generates LVN_ITEMACTIVATE message
      }
    } else if(dynamic_cast<LineEdit*>(object)) {
      LineEdit& lineEdit = (LineEdit&)*object;
      if(wparam == VK_RETURN) {
        if(lineEdit.onActivate) lineEdit.onActivate();
      }
    } else if(dynamic_cast<TextEdit*>(object)) {
      TextEdit& textEdit = (TextEdit&)*object;
      if(wparam == 'A' && GetKeyState(VK_CONTROL) < 0) {
        //Ctrl+A = select all text
        //note: this is not a standard accelerator on Windows
        Edit_SetSel(textEdit.p.hwnd, 0, ~0);
        return true;
      } else if(wparam == 'V' && GetKeyState(VK_CONTROL) < 0) {
        //Ctrl+V = paste text
        //note: this formats Unix (LF) and OS9 (CR) line-endings to Windows (CR+LF) line-endings
        //this is necessary as the EDIT control only supports Windows line-endings
        OpenClipboard(hwnd);
        HANDLE handle = GetClipboardData(CF_UNICODETEXT);
        if(handle) {
          wchar_t* text = (wchar_t*)GlobalLock(handle);
          if(text) {
            string data = (const char*)utf8_t(text);
            data.replace("\r\n", "\n");
            data.replace("\r", "\n");
            data.replace("\n", "\r\n");
            GlobalUnlock(handle);
            utf16_t output(data);
            HGLOBAL resource = GlobalAlloc(GMEM_MOVEABLE, (wcslen(output) + 1) * sizeof(wchar_t));
            if(resource) {
              wchar_t* write = (wchar_t*)GlobalLock(resource);
              if(write) {
                wcscpy(write, output);
                GlobalUnlock(write);
                if(SetClipboardData(CF_UNICODETEXT, resource) == FALSE) {
                  GlobalFree(resource);
                }
              }
            }
          }
        }
        CloseClipboard();
        return false;
      }
    }
  }

  return false;
}

/*case WM_GETMINMAXINFO: {
    MINMAXINFO* mmi = (MINMAXINFO*)lparam;
    mmi->ptMinTrackSize.x = 256 + window.p.frameMargin().width;
    mmi->ptMinTrackSize.y = 256 + window.p.frameMargin().height;
    return TRUE;
    break;
  }*/

static LRESULT CALLBACK Application_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  Object* object = (Object*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(object == nullptr) return DefWindowProc(hwnd, msg, wparam, lparam);
  Window& window = dynamic_cast<Window*>(object) ? (Window&)*object : *((Widget*)object)->Sizable::state.window;

  bool process = true;
  if(!pWindow::modal.empty() && !pWindow::modal.find(&window.p)) process = false;
  if(applicationState.quit) process = false;
  if(process == false) return DefWindowProc(hwnd, msg, wparam, lparam);

  switch(msg) {
  case WM_CLOSE: window.p.onClose(); return TRUE;
  case WM_MOVE: window.p.onMove(); break;
  case WM_SIZE: window.p.onSize(); break;
  case WM_DROPFILES: window.p.onDrop(wparam); return FALSE;
  case WM_ERASEBKGND: if(window.p.onEraseBackground()) return true; break;
  case WM_ENTERMENULOOP: case WM_ENTERSIZEMOVE: window.p.onModalBegin(); return FALSE;
  case WM_EXITMENULOOP: case WM_EXITSIZEMOVE: window.p.onModalEnd(); return FALSE;
  }

  return Shared_windowProc(DefWindowProc, hwnd, msg, wparam, lparam);
}

}
