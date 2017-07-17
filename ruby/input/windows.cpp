#include <xinput.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "shared/rawinput.cpp"
#include "keyboard/rawinput.cpp"
#include "mouse/rawinput.cpp"
#include "joypad/xinput.cpp"
#include "joypad/directinput.cpp"

struct InputWindows : Input {
  InputWindows() : _keyboard(*this), _mouse(*this), _joypadXInput(*this), _joypadDirectInput(*this) { initialize(); }
  ~InputWindows() { terminate(); }

  auto ready() -> bool { return _ready; }

  auto context() -> uintptr { return _context; }

  auto setContext(uintptr context) -> bool {
    if(_context == context) return true;
    _context = context;
    return initialize();
  }

  auto acquired() -> bool {
    return _mouse.acquired();
  }

  auto acquire() -> bool {
    return _mouse.acquire();
  }

  auto release() -> bool {
    return _mouse.release();
  }

  auto poll() -> vector<shared_pointer<HID::Device>> {
    vector<shared_pointer<HID::Device>> devices;
    _keyboard.poll(devices);
    _mouse.poll(devices);
    _joypadXInput.poll(devices);
    _joypadDirectInput.poll(devices);
    return devices;
  }

  auto rumble(uint64_t id, bool enable) -> bool {
    if(_joypadXInput.rumble(id, enable)) return true;
    if(_joypadDirectInput.rumble(id, enable)) return true;
    return false;
  }

private:
  auto initialize() -> bool {
    terminate();
    if(!_context) return false;

    if(!rawinput.initialized) {
      rawinput.initialized = true;
      rawinput.mutex = CreateMutex(nullptr, false, nullptr);
      CreateThread(nullptr, 0, RawInputThreadProc, 0, 0, nullptr);

      do {
        Sleep(1);
        WaitForSingleObject(rawinput.mutex, INFINITE);
        ReleaseMutex(rawinput.mutex);
      } while(!rawinput.ready);
    }

    DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_directInputContext, 0);
    if(!_directInputContext) return false;

    if(!_keyboard.initialize()) return false;
    if(!_mouse.initialize(_context)) return false;
    bool xinputAvailable = _joypadXInput.initialize();
    if(!_joypadDirectInput.initialize(_context, _directInputContext, xinputAvailable)) return false;
    return _ready = true;
  }

  auto terminate() -> void {
    _ready = false;

    _keyboard.terminate();
    _mouse.terminate();
    _joypadXInput.terminate();
    _joypadDirectInput.terminate();

    if(_directInputContext) {
      _directInputContext->Release();
      _directInputContext = nullptr;
    }
  }

  bool _ready = false;
  uintptr _context = 0;

  InputKeyboardRawInput _keyboard;
  InputMouseRawInput _mouse;
  InputJoypadXInput _joypadXInput;
  InputJoypadDirectInput _joypadDirectInput;
  LPDIRECTINPUT8 _directInputContext = nullptr;
};
