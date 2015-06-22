#include <xinput.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "shared/rawinput.cpp"
#include "keyboard/rawinput.cpp"
#include "mouse/rawinput.cpp"
#include "joypad/xinput.cpp"
#include "joypad/directinput.cpp"

struct InputWindows : Input {
  InputKeyboardRawInput rawinputKeyboard;
  InputMouseRawInput rawinputMouse;
  InputJoypadXInput xinput;
  InputJoypadDirectInput directinput;
  InputWindows() : rawinputKeyboard(*this), rawinputMouse(*this), xinput(*this), directinput(*this) {}
  ~InputWindows() { term(); }

  LPDIRECTINPUT8 directinputContext = nullptr;

  struct Settings {
    uintptr_t handle = 0;
  } settings;

  auto cap(const string& name) -> bool {
    if(name == Input::Handle) return true;
    if(name == Input::KeyboardSupport) return true;
    if(name == Input::MouseSupport) return true;
    if(name == Input::JoypadSupport) return true;
    if(name == Input::JoypadRumbleSupport) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Input::Handle) return (uintptr_t)settings.handle;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Input::Handle && value.is<uintptr_t>()) {
      settings.handle = value.get<uintptr_t>();
      return true;
    }
    return false;
  }

  auto acquire() -> bool {
    return rawinputMouse.acquire();
  }

  auto release() -> bool {
    return rawinputMouse.release();
  }

  auto acquired() -> bool {
    return rawinputMouse.acquired();
  }

  auto poll() -> vector<shared_pointer<HID::Device>> {
    vector<shared_pointer<HID::Device>> devices;
    rawinputKeyboard.poll(devices);
    rawinputMouse.poll(devices);
    xinput.poll(devices);
    directinput.poll(devices);
    return devices;
  }

  auto rumble(uint64_t id, bool enable) -> bool {
    if(xinput.rumble(id, enable)) return true;
    if(directinput.rumble(id, enable)) return true;
    return false;
  }

  auto init() -> bool {
    if(rawinput.initialized == false) {
      rawinput.initialized = true;
      rawinput.mutex = CreateMutex(NULL, FALSE, NULL);
      CreateThread(NULL, 0, RawInputThreadProc, 0, 0, NULL);

      do {
        Sleep(1);
        WaitForSingleObject(rawinput.mutex, INFINITE);
        ReleaseMutex(rawinput.mutex);
      } while(rawinput.ready == false);
    }

    DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directinputContext, 0);
    if(directinputContext == nullptr) return false;

    if(rawinputKeyboard.init() == false) return false;
    if(rawinputMouse.init(settings.handle) == false) return false;
    bool xinputAvailable = xinput.init();
    if(directinput.init(settings.handle, directinputContext, xinputAvailable) == false) return false;
    return true;
  }

  auto term() -> void {
    rawinputKeyboard.term();
    rawinputMouse.term();
    xinput.term();
    directinput.term();

    if(directinputContext) { directinputContext->Release(); directinputContext = nullptr; }
  }
};
