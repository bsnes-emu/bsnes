#if defined(Hiro_Application)

namespace hiro {

static auto Application_keyboardProc(HWND, UINT, WPARAM, LPARAM) -> bool;
static auto Application_processDialogMessage(MSG&) -> void;
static auto CALLBACK Window_windowProc(HWND, UINT, WPARAM, LPARAM) -> LRESULT;

auto pApplication::modal() -> bool {
  return state().modalCount > 0;
}

auto pApplication::run() -> void {
  while(!Application::state().quit) {
    if(Application::state().onMain) {
      //doMain() is responsible for sleeping the thread where practical
      Application::doMain();
      if(Application::state().quit) break;
    } else {
      //avoid consuming 100% CPU thread usage
      usleep(20 * 1000);
    }
    //called after doMain(), in case doMain() calls Application::quit()
    processEvents();
  }
}

auto pApplication::pendingEvents() -> bool {
  MSG msg;
  return PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE);
}

auto pApplication::processEvents() -> void {
  while(pendingEvents()) {
    MSG msg;
    if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      Application_processDialogMessage(msg);
    }
  }
}

auto Application_processDialogMessage(MSG& msg) -> void {
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

auto pApplication::quit() -> void {
  PostQuitMessage(0);
}

auto pApplication::setScreenSaver(bool screenSaver) -> void {
}

auto pApplication::initialize() -> void {
  CoInitialize(0);
  InitCommonControls();

  WNDCLASS wc{};

  #if defined(Hiro_Window)
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(2));
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = Window_windowProc;
  wc.lpszClassName = L"hiroWindow";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);
  #endif

  #if defined(Hiro_PopupMenu)
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(2));
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = Menu_windowProc;
  wc.lpszClassName = L"hiroPopupMenu";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);
  #endif

  #if defined(Hiro_Widget)
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = ToolTip_windowProc;
  wc.lpszClassName = L"hiroToolTip";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);
  #endif

  #if defined(Hiro_Widget)
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = Default_windowProc;
  wc.lpszClassName = L"hiroWidget";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);
  #endif

  pKeyboard::initialize();
  pWindow::initialize();
}

auto pApplication::state() -> State& {
  static State state;
  return state;
}

static auto Application_keyboardProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> bool {
  if(msg != WM_KEYDOWN && msg != WM_SYSKEYDOWN && msg != WM_KEYUP && msg != WM_SYSKEYUP) return false;

  GUITHREADINFO info{sizeof(GUITHREADINFO)};
  GetGUIThreadInfo(GetCurrentThreadId(), &info);

  auto object = (mObject*)GetWindowLongPtr(info.hwndFocus, GWLP_USERDATA);
  if(!object) return false;

  auto objectWindow = (mObject*)GetWindowLongPtr(GetAncestor(info.hwndFocus, GA_ROOT), GWLP_USERDATA);
  if(!objectWindow) return false;

  if(auto window = dynamic_cast<mWindow*>(objectWindow)) {
    if(auto self = window->self()) {
      if(!self->_modalityDisabled()) {
        if(auto code = pKeyboard::_translate(wparam, lparam)) {
          if(msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN) {
            window->doKeyPress(code);
          }
          if(msg == WM_KEYUP || msg == WM_SYSKEYUP) {
            window->doKeyRelease(code);
          }
        }
        //TODO: does this really need to be hooked here?
        if(msg == WM_KEYDOWN && wparam == VK_ESCAPE && window->state.dismissable) {
          if(auto result = self->windowProc(self->hwnd, WM_CLOSE, wparam, lparam)) return result();
        }
      }
    }
  }

  if(auto window = object->parentWindow(true)) {
    if(auto self = window->self()) {
      if(self->_modalityDisabled()) return false;
    }
  }

  if(msg == WM_KEYDOWN) {
    //TODO: does this really need to be hooked here?
    #if defined(Hiro_Widget)
    if(auto widget = dynamic_cast<mWidget*>(object)) {
      if(auto self = widget->self()) {
        if(auto result = self->windowProc(self->hwnd, msg, wparam, lparam)) {
          return result();
        }
      }
    }
    #endif
  }

  return false;
}

}

#endif
