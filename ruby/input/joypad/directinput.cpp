#ifndef RUBY_INPUT_JOYPAD_DIRECTINPUT
#define RUBY_INPUT_JOYPAD_DIRECTINPUT

auto CALLBACK DirectInput_EnumJoypadsCallback(const DIDEVICEINSTANCE* instance, void* p) -> BOOL;
auto CALLBACK DirectInput_EnumJoypadAxesCallback(const DIDEVICEOBJECTINSTANCE* instance, void* p) -> BOOL;
auto CALLBACK DirectInput_EnumJoypadEffectsCallback(const DIDEVICEOBJECTINSTANCE* instance, void* p) -> BOOL;

struct InputJoypadDirectInput {
  Input& input;
  InputJoypadDirectInput(Input& input) : input(input) {}

  struct Joypad {
    shared_pointer<HID::Joypad> hid{new HID::Joypad};

    LPDIRECTINPUTDEVICE8 device = nullptr;
    LPDIRECTINPUTEFFECT effect = nullptr;

    uint32_t pathID = 0;
    uint16_t vendorID = 0;
    uint16_t productID = 0;
    bool isXInputDevice = false;
  };
  vector<Joypad> joypads;

  uintptr_t handle = 0;
  LPDIRECTINPUT8 context = nullptr;
  LPDIRECTINPUTDEVICE8 device = nullptr;
  bool xinputAvailable = false;
  unsigned effects = 0;

  auto assign(shared_pointer<HID::Joypad> hid, unsigned groupID, unsigned inputID, int16_t value) -> void {
    auto& group = hid->group(groupID);
    if(group.input(inputID).value() == value) return;
    input.doChange(hid, groupID, inputID, group.input(inputID).value(), value);
    group.input(inputID).setValue(value);
  }

  auto poll(vector<shared_pointer<HID::Device>>& devices) -> void {
    for(auto& jp : joypads) {
      if(FAILED(jp.device->Poll())) jp.device->Acquire();

      DIJOYSTATE2 state;
      if(FAILED(jp.device->GetDeviceState(sizeof(DIJOYSTATE2), &state))) continue;

      for(unsigned n = 0; n < 4; n++) {
        assign(jp.hid, HID::Joypad::GroupID::Axis, 0, state.lX);
        assign(jp.hid, HID::Joypad::GroupID::Axis, 1, state.lY);
        assign(jp.hid, HID::Joypad::GroupID::Axis, 2, state.lZ);
        assign(jp.hid, HID::Joypad::GroupID::Axis, 3, state.lRx);
        assign(jp.hid, HID::Joypad::GroupID::Axis, 4, state.lRy);
        assign(jp.hid, HID::Joypad::GroupID::Axis, 5, state.lRz);

        unsigned pov = state.rgdwPOV[n];
        int16_t xaxis = 0;
        int16_t yaxis = 0;

        if(pov < 36000) {
          if(pov >= 31500 || pov <=  4500) yaxis = -32768;
          if(pov >=  4500 && pov <= 13500) xaxis = +32767;
          if(pov >= 13500 && pov <= 22500) yaxis = +32767;
          if(pov >= 22500 && pov <= 31500) xaxis = -32768;
        }

        assign(jp.hid, HID::Joypad::GroupID::Hat, n * 2 + 0, xaxis);
        assign(jp.hid, HID::Joypad::GroupID::Hat, n * 2 + 1, yaxis);
      }

      for(unsigned n = 0; n < 128; n++) {
        assign(jp.hid, HID::Joypad::GroupID::Button, n, (bool)state.rgbButtons[n]);
      }

      devices.append(jp.hid);
    }
  }

  auto rumble(uint64_t id, bool enable) -> bool {
    for(auto& jp : joypads) {
      if(jp.hid->id() != id) continue;
      if(jp.effect == nullptr) continue;

      if(enable) jp.effect->Start(1, 0);
      else jp.effect->Stop();
      return true;
    }

    return false;
  }

  auto initialize(uintptr handle, LPDIRECTINPUT8 context, bool xinputAvailable) -> bool {
    if(!handle) return false;
    this->handle = handle;
    this->context = context;
    this->xinputAvailable = xinputAvailable;
    context->EnumDevices(DI8DEVCLASS_GAMECTRL, DirectInput_EnumJoypadsCallback, (void*)this, DIEDFL_ATTACHEDONLY);
    return true;
  }

  auto terminate() -> void {
    for(auto& jp : joypads) {
      jp.device->Unacquire();
      if(jp.effect) jp.effect->Release();
      jp.device->Release();
    }
    joypads.reset();
    context = nullptr;
  }

  auto initJoypad(const DIDEVICEINSTANCE* instance) -> bool {
    Joypad jp;
    jp.vendorID = instance->guidProduct.Data1 >> 0;
    jp.productID = instance->guidProduct.Data1 >> 16;
    jp.isXInputDevice = false;
    if(auto device = rawinput.find(jp.vendorID, jp.productID)) {
      jp.isXInputDevice = device().isXInputDevice;
    }

    //Microsoft has intentionally imposed artificial restrictions on XInput devices when used with DirectInput
    //a) the two triggers are merged into a single axis, making uniquely distinguishing them impossible
    //b) rumble support is not exposed
    //thus, it's always preferred to let the XInput driver handle these joypads
    //but if the driver is not available (XInput 1.3 does not ship with stock Windows XP), fall back on DirectInput
    if(jp.isXInputDevice && xinputAvailable) return DIENUM_CONTINUE;

    if(FAILED(context->CreateDevice(instance->guidInstance, &device, 0))) return DIENUM_CONTINUE;
    jp.device = device;
    device->SetDataFormat(&c_dfDIJoystick2);
    device->SetCooperativeLevel((HWND)handle, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

    effects = 0;
    device->EnumObjects(DirectInput_EnumJoypadAxesCallback, (void*)this, DIDFT_ABSAXIS);
    device->EnumObjects(DirectInput_EnumJoypadEffectsCallback, (void*)this, DIDFT_FFACTUATOR);
    jp.hid->setRumble(effects > 0);

    DIPROPGUIDANDPATH property;
    memset(&property, 0, sizeof(DIPROPGUIDANDPATH));
    property.diph.dwSize = sizeof(DIPROPGUIDANDPATH);
    property.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    property.diph.dwObj = 0;
    property.diph.dwHow = DIPH_DEVICE;
    device->GetProperty(DIPROP_GUIDANDPATH, &property.diph);
    string devicePath = (const char*)utf8_t(property.wszPath);
    jp.pathID = Hash::CRC32(devicePath.data(), devicePath.size()).value();
    jp.hid->setID((uint64_t)jp.pathID << 32 | jp.vendorID << 16 | jp.productID << 0);

    if(jp.hid->rumble()) {
      //disable auto-centering spring for rumble support
      DIPROPDWORD property;
      memset(&property, 0, sizeof(DIPROPDWORD));
      property.diph.dwSize = sizeof(DIPROPDWORD);
      property.diph.dwHeaderSize = sizeof(DIPROPHEADER);
      property.diph.dwObj = 0;
      property.diph.dwHow = DIPH_DEVICE;
      property.dwData = false;
      device->SetProperty(DIPROP_AUTOCENTER, &property.diph);

      DWORD dwAxes[2] = {(DWORD)DIJOFS_X, (DWORD)DIJOFS_Y};
      LONG lDirection[2] = {0, 0};
      DICONSTANTFORCE force;
      force.lMagnitude = DI_FFNOMINALMAX;  //full force
      DIEFFECT effect;
      memset(&effect, 0, sizeof(DIEFFECT));
      effect.dwSize = sizeof(DIEFFECT);
      effect.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
      effect.dwDuration = INFINITE;
      effect.dwSamplePeriod = 0;
      effect.dwGain = DI_FFNOMINALMAX;
      effect.dwTriggerButton = DIEB_NOTRIGGER;
      effect.dwTriggerRepeatInterval = 0;
      effect.cAxes = 2;
      effect.rgdwAxes = dwAxes;
      effect.rglDirection = lDirection;
      effect.lpEnvelope = 0;
      effect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
      effect.lpvTypeSpecificParams = &force;
      effect.dwStartDelay = 0;
      device->CreateEffect(GUID_ConstantForce, &effect, &jp.effect, NULL);
    }

    for(unsigned n = 0; n < 6; n++) jp.hid->axes().append(n);
    for(unsigned n = 0; n < 8; n++) jp.hid->hats().append(n);
    for(unsigned n = 0; n < 128; n++) jp.hid->buttons().append(n);
    joypads.append(jp);

    return DIENUM_CONTINUE;
  }

  auto initAxis(const DIDEVICEOBJECTINSTANCE* instance) -> bool {
    DIPROPRANGE range;
    memset(&range, 0, sizeof(DIPROPRANGE));
    range.diph.dwSize = sizeof(DIPROPRANGE);
    range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    range.diph.dwHow = DIPH_BYID;
    range.diph.dwObj = instance->dwType;
    range.lMin = -32768;
    range.lMax = +32767;
    device->SetProperty(DIPROP_RANGE, &range.diph);
    return DIENUM_CONTINUE;
  }

  auto initEffect(const DIDEVICEOBJECTINSTANCE* instance) -> bool {
    effects++;
    return DIENUM_CONTINUE;
  }
};

auto CALLBACK DirectInput_EnumJoypadsCallback(const DIDEVICEINSTANCE* instance, void* p) -> BOOL {
  return ((InputJoypadDirectInput*)p)->initJoypad(instance);
}

auto CALLBACK DirectInput_EnumJoypadAxesCallback(const DIDEVICEOBJECTINSTANCE* instance, void* p) -> BOOL {
  return ((InputJoypadDirectInput*)p)->initAxis(instance);
}

auto CALLBACK DirectInput_EnumJoypadEffectsCallback(const DIDEVICEOBJECTINSTANCE* instance, void* p) -> BOOL {
  return ((InputJoypadDirectInput*)p)->initEffect(instance);
}

#endif
