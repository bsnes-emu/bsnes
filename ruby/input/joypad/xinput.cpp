#include <xinput.h>

namespace ruby {

struct InputJoypadXInput {
  HMODULE libxinput = nullptr;
  DWORD WINAPI (*XInputGetState)(DWORD, XINPUT_STATE*) = nullptr;
  DWORD WINAPI (*XInputSetState)(DWORD, XINPUT_VIBRATION*) = nullptr;

  struct Joystick {
    unsigned id;

    int16_t hat = 0;
    int16_t axis[6] = {0};
    bool button[10] = {0};
  };
  vector<Joystick> joysticks;

  bool poll(int16_t* table) {
    if(!XInputGetState) return false;

    for(auto& js : joysticks) {
      XINPUT_STATE state;
      if(XInputGetState(js.id, &state) != ERROR_SUCCESS) continue;

      int16_t hat = 0;
      if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP   ) hat |= Joypad::HatUp;
      if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN ) hat |= Joypad::HatDown;
      if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT ) hat |= Joypad::HatLeft;
      if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) hat |= Joypad::HatRight;

      //scale trigger ranges from (0 to 255) to (-32768 to +32767)
      uint16_t triggerL = state.Gamepad.bLeftTrigger;
      uint16_t triggerR = state.Gamepad.bRightTrigger;
      triggerL = triggerL << 8 | triggerL << 0;
      triggerR = triggerR << 8 | triggerR << 0;

      table[joypad(js.id).axis(0)] = (int16_t)state.Gamepad.sThumbLX;
      table[joypad(js.id).axis(1)] = (int16_t)state.Gamepad.sThumbLY;
      table[joypad(js.id).axis(2)] = (int16_t)state.Gamepad.sThumbRX;
      table[joypad(js.id).axis(3)] = (int16_t)state.Gamepad.sThumbRY;
      table[joypad(js.id).axis(4)] = (int16_t)((~triggerL) - 32768);
      table[joypad(js.id).axis(5)] = (int16_t)((~triggerR) - 32768);
      table[joypad(js.id).hat(0)] = (int16_t)hat;
      table[joypad(js.id).button(0)] = (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_A);
      table[joypad(js.id).button(1)] = (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_B);
      table[joypad(js.id).button(2)] = (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_X);
      table[joypad(js.id).button(3)] = (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_Y);
      table[joypad(js.id).button(4)] = (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
      table[joypad(js.id).button(5)] = (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_START);
      table[joypad(js.id).button(6)] = (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
      table[joypad(js.id).button(7)] = (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
      table[joypad(js.id).button(8)] = (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
      table[joypad(js.id).button(9)] = (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
    }

    return true;
  }

  void rumble(unsigned id, bool enable) {
    if(!XInputSetState) return;
    if(id >= joysticks.size()) return;

    XINPUT_VIBRATION vibration;
    memset(&vibration, 0, sizeof(XINPUT_VIBRATION));
    vibration.wLeftMotorSpeed  = enable ? 65535 : 0;  //low-frequency motor  (0 = off, 65535 = max)
    vibration.wRightMotorSpeed = enable ? 65535 : 0;  //high-frequency motor (0 = off, 65535 = max)
    XInputSetState(joysticks(id).id, &vibration);
  }

  bool init() {
    if(!libxinput) libxinput = LoadLibraryA("xinput1_3.dll");
    if(!libxinput) libxinput = LoadLibraryA("xinput1_2.dll");
    if(!libxinput) libxinput = LoadLibraryA("xinput1_1.dll");
    if(!libxinput) return false;

    XInputGetState = (DWORD WINAPI (*)(DWORD, XINPUT_STATE*))GetProcAddress(libxinput, "XInputGetState");
    XInputSetState = (DWORD WINAPI (*)(DWORD, XINPUT_VIBRATION*))GetProcAddress(libxinput, "XInputSetState");

    //XInput supports a maximum of four controllers
    for(unsigned id = 0; id < 4; id++) {
      XINPUT_STATE state;
      if(XInputGetState(id, &state) == ERROR_SUCCESS) {
        Joystick js;
        js.id = id;
        joysticks.append(js);
      }
    }

    return true;
  }

  void term() {
    if(libxinput) {
      FreeLibrary(libxinput);
      libxinput = nullptr;
    }
  }
};

}
