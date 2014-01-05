#include <xinput.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "shared/rawinput.cpp"
#include "keyboard/rawinput.cpp"
#include "mouse/rawinput.cpp"
#include "joypad/xinput.cpp"
#include "joypad/directinput.cpp"

namespace ruby {

struct pInputWindows {
  InputKeyboardRawInput rawinputKeyboard;
  InputMouseRawInput rawinputMouse;
  InputJoypadXInput xinput;
  InputJoypadDirectInput directinput;

  LPDIRECTINPUT8 directinputContext = nullptr;

  struct Settings {
    uintptr_t handle = 0;
  } settings;

  bool cap(const string& name) {
    if(name == Input::Handle) return true;
    if(name == Input::KeyboardSupport) return true;
    if(name == Input::MouseSupport) return true;
    if(name == Input::JoypadSupport) return true;
    if(name == Input::JoypadRumbleSupport) return true;
    return false;
  }

  any get(const string& name) {
    if(name == Input::Handle) return (uintptr_t)settings.handle;
    return false;
  }

  bool set(const string& name, const any& value) {
    if(name == Input::Handle) {
      settings.handle = any_cast<uintptr_t>(value);
      return true;
    }
    return false;
  }

  bool acquire() {
    return rawinputMouse.acquire();
  }

  bool unacquire() {
    return rawinputMouse.unacquire();
  }

  bool acquired() {
    return rawinputMouse.acquired();
  }

  vector<HID::Device*> poll() {
    vector<HID::Device*> devices;
    rawinputKeyboard.poll(devices);
    rawinputMouse.poll(devices);
    xinput.poll(devices);
    directinput.poll(devices);
    return devices;
  }

  bool rumble(uint64_t id, bool enable) {
    if(xinput.rumble(id, enable)) return true;
    if(directinput.rumble(id, enable)) return true;
    return false;
  }

  bool init() {
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

  void term() {
    rawinputKeyboard.term();
    rawinputMouse.term();
    xinput.term();
    directinput.term();

    if(directinputContext) { directinputContext->Release(); directinputContext = nullptr; }
  }
};

DeclareInput(Windows)

}
