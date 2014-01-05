#ifndef RUBY_INPUT_JOYPAD_XINPUT
#define RUBY_INPUT_JOYPAD_XINPUT

//documented functionality
#define oXInputGetState  "XInputGetState"
#define oXInputSetState  "XInputSetState"
typedef DWORD WINAPI (*pXInputGetState)(DWORD dwUserIndex, XINPUT_STATE* pState);
typedef DWORD WINAPI (*pXInputSetState)(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration);

//undocumented functionality
#define oXInputGetStateEx             (LPCSTR)100
#define oXInputWaitForGuideButton     (LPCSTR)101
#define oXInputCancelGuideButtonWait  (LPCSTR)102
#define oXInputPowerOffController     (LPCSTR)103
typedef DWORD WINAPI (*pXInputGetStateEx)(DWORD dwUserIndex, XINPUT_STATE* pState);
typedef DWORD WINAPI (*pXInputWaitForGuideButton)(DWORD dwUserIndex, DWORD dwFlag, void* pUnknown);
typedef DWORD WINAPI (*pXInputCancelGuideButtonWait)(DWORD dwUserIndex);
typedef DWORD WINAPI (*pXInputPowerOffController)(DWORD dwUserIndex);

#define XINPUT_GAMEPAD_GUIDE  0x0400

namespace ruby {

struct InputJoypadXInput {
  HMODULE libxinput = nullptr;
  pXInputGetStateEx XInputGetStateEx = nullptr;
  pXInputSetState XInputSetState = nullptr;

  struct Joypad {
    HID::Joypad hid;
    unsigned id;
  };
  vector<Joypad> joypads;

  void assign(HID::Joypad& hid, unsigned groupID, unsigned inputID, int16_t value) {
    auto& group = hid.group[groupID];
    if(group.input[inputID].value == value) return;
    if(input.onChange) input.onChange(hid, groupID, inputID, group.input[inputID].value, value);
    group.input[inputID].value = value;
  }

  void poll(vector<HID::Device*>& devices) {
    for(auto& jp : joypads) {
      XINPUT_STATE state;
      if(XInputGetStateEx(jp.id, &state) != ERROR_SUCCESS) continue;

      //flip vertical axes so that -32768 = up, +32767 = down
      uint16_t axisLY = 32768 + state.Gamepad.sThumbLY;
      uint16_t axisRY = 32768 + state.Gamepad.sThumbRY;
      assign(jp.hid, HID::Joypad::GroupID::Axis, 0, (int16_t)state.Gamepad.sThumbLX);
      assign(jp.hid, HID::Joypad::GroupID::Axis, 1, (int16_t)(~axisLY - 32768));
      assign(jp.hid, HID::Joypad::GroupID::Axis, 2, (int16_t)state.Gamepad.sThumbRX);
      assign(jp.hid, HID::Joypad::GroupID::Axis, 3, (int16_t)(~axisRY - 32768));

      int16_t hatX = 0;
      int16_t hatY = 0;
      if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP   ) hatY = -32768;
      if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN ) hatY = +32767;
      if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT ) hatX = -32768;
      if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) hatX = +32767;

      assign(jp.hid, HID::Joypad::GroupID::Hat, 0, hatX);
      assign(jp.hid, HID::Joypad::GroupID::Hat, 1, hatY);

      //scale trigger ranges for up to down from (0 to 255) to (-32768 to +32767)
      uint16_t triggerL = state.Gamepad.bLeftTrigger;
      uint16_t triggerR = state.Gamepad.bRightTrigger;
      triggerL = triggerL << 8 | triggerL << 0;
      triggerR = triggerR << 8 | triggerR << 0;

      assign(jp.hid, HID::Joypad::GroupID::Trigger, 0, (int16_t)(triggerL - 32768));
      assign(jp.hid, HID::Joypad::GroupID::Trigger, 1, (int16_t)(triggerR - 32768));

      assign(jp.hid, HID::Joypad::GroupID::Button,  0, (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_A));
      assign(jp.hid, HID::Joypad::GroupID::Button,  1, (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_B));
      assign(jp.hid, HID::Joypad::GroupID::Button,  2, (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_X));
      assign(jp.hid, HID::Joypad::GroupID::Button,  3, (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_Y));
      assign(jp.hid, HID::Joypad::GroupID::Button,  4, (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK));
      assign(jp.hid, HID::Joypad::GroupID::Button,  5, (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_START));
      assign(jp.hid, HID::Joypad::GroupID::Button,  6, (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER));
      assign(jp.hid, HID::Joypad::GroupID::Button,  7, (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER));
      assign(jp.hid, HID::Joypad::GroupID::Button,  8, (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB));
      assign(jp.hid, HID::Joypad::GroupID::Button,  9, (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB));
      assign(jp.hid, HID::Joypad::GroupID::Button, 10, (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_GUIDE));

      devices.append(&jp.hid);
    }
  }

  bool rumble(uint64_t id, bool enable) {
    for(auto& jp : joypads) {
      if(jp.hid.id != id) continue;

      XINPUT_VIBRATION vibration;
      memset(&vibration, 0, sizeof(XINPUT_VIBRATION));
      vibration.wLeftMotorSpeed  = enable ? 65535 : 0;  //low-frequency motor  (0 = off, 65535 = max)
      vibration.wRightMotorSpeed = enable ? 65535 : 0;  //high-frequency motor (0 = off, 65535 = max)
      XInputSetState(jp.id, &vibration);
      return true;
    }

    return false;
  }

  bool init() {
    if(!libxinput) libxinput = LoadLibraryA("xinput1_3.dll");
    if(!libxinput) return false;

    //XInputGetStateEx is an undocumented function; but is required to get the state of the guide button
    //if for some reason it is not available, fall back on XInputGetState, which takes the same parameters
    XInputGetStateEx = (pXInputGetStateEx)GetProcAddress(libxinput, oXInputGetStateEx);
    XInputSetState = (pXInputSetState)GetProcAddress(libxinput, oXInputSetState);
    if(!XInputGetStateEx) XInputGetStateEx = (pXInputGetStateEx)GetProcAddress(libxinput, oXInputGetState);
    if(!XInputGetStateEx || !XInputSetState) return term(), false;

    //XInput supports a maximum of four controllers
    //add all four to devices list now. If they are not connected, they will not show up in poll() results
    for(unsigned id = 0; id < 4; id++) {
      Joypad jp;
      jp.id = id;
      jp.hid.id = (uint64_t)(1 + id) << 32 | 0x045e << 16 | 0x028e << 0;  //Xbox 360 Player# + VendorID + ProductID

      jp.hid.axis().append({"LeftThumbX"});
      jp.hid.axis().append({"LeftThumbY"});
      jp.hid.axis().append({"RightThumbX"});
      jp.hid.axis().append({"RightThumbY"});

      jp.hid.hat().append({"HatX"});
      jp.hid.hat().append({"HatY"});

      jp.hid.trigger().append({"LeftTrigger"});
      jp.hid.trigger().append({"RightTrigger"});

      jp.hid.button().append({"A"});
      jp.hid.button().append({"B"});
      jp.hid.button().append({"X"});
      jp.hid.button().append({"Y"});
      jp.hid.button().append({"Back"});
      jp.hid.button().append({"Start"});
      jp.hid.button().append({"LeftShoulder"});
      jp.hid.button().append({"RightShoulder"});
      jp.hid.button().append({"LeftThumb"});
      jp.hid.button().append({"RightThumb"});
      jp.hid.button().append({"Guide"});

      joypads.append(jp);
    }

    return true;
  }

  void term() {
    if(!libxinput) return;

    FreeLibrary(libxinput);
    libxinput = nullptr;
  }
};

}

#endif
