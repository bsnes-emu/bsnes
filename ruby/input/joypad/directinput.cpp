#ifndef RUBY_INPUT_JOYPAD_DIRECTINPUT
#define RUBY_INPUT_JOYPAD_DIRECTINPUT

namespace ruby {

BOOL CALLBACK DirectInput_EnumJoypadsCallback(const DIDEVICEINSTANCE* instance, void* p);
BOOL CALLBACK DirectInput_EnumJoypadAxesCallback(const DIDEVICEOBJECTINSTANCE* instance, void* p);
BOOL CALLBACK DirectInput_EnumJoypadEffectsCallback(const DIDEVICEOBJECTINSTANCE* instance, void* p);

struct InputJoypadDirectInput {
  struct Joypad {
    HID::Joypad hid;

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

  void assign(HID::Joypad& hid, unsigned groupID, unsigned inputID, int16_t value) {
    auto& group = hid.group[groupID];
    if(group.input[inputID].value == value) return;
    if(input.onChange) input.onChange(hid, groupID, inputID, group.input[inputID].value, value);
    group.input[inputID].value = value;
  }

  void poll(vector<HID::Device*>& devices) {
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

      devices.append(&jp.hid);
    }
  }

  bool rumble(uint64_t id, bool enable) {
    for(auto& jp : joypads) {
      if(jp.hid.id != id) continue;
      if(jp.effect == nullptr) continue;

      if(enable) jp.effect->Start(1, 0);
      else jp.effect->Stop();
      return true;
    }

    return false;
  }

  bool init(uintptr_t handle, LPDIRECTINPUT8 context, bool xinputAvailable) {
    this->handle = handle;
    this->context = context;
    this->xinputAvailable = xinputAvailable;
    context->EnumDevices(DI8DEVCLASS_GAMECTRL, DirectInput_EnumJoypadsCallback, (void*)this, DIEDFL_ATTACHEDONLY);
    return true;
  }

  void term() {
    for(auto& jp : joypads) {
      jp.device->Unacquire();
      if(jp.effect) jp.effect->Release();
      jp.device->Release();
    }
    joypads.reset();
    context = nullptr;
  }

  bool initJoypad(const DIDEVICEINSTANCE* instance) {
    Joypad jp;
    jp.vendorID = instance->guidProduct.Data1 >> 0;
    jp.productID = instance->guidProduct.Data1 >> 16;
    if(auto device = rawinput.find(jp.vendorID, jp.productID)) {
      jp.pathID = crc32_calculate((const uint8_t*)device().path.data(), device().path.size());
      jp.hid.id = (uint64_t)jp.pathID << 32 | jp.vendorID << 16 | jp.productID << 0;
      jp.isXInputDevice = device().isXInputDevice;
    } else {
      //this should never occur
      return DIENUM_CONTINUE;
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
    jp.hid.rumble = effects > 0;

    if(jp.hid.rumble) {
      //disable auto-centering spring for rumble support
      DIPROPDWORD property;
      memset(&property, 0, sizeof(DIPROPDWORD));
      property.diph.dwSize = sizeof(DIPROPDWORD);
      property.diph.dwHeaderSize = sizeof(DIPROPHEADER);
      property.diph.dwObj = 0;
      property.diph.dwHow = DIPH_DEVICE;
      property.dwData = false;
      device->SetProperty(DIPROP_AUTOCENTER, &property.diph);

      DWORD dwAxes[2] = {DIJOFS_X, DIJOFS_Y};
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

    for(unsigned n = 0; n < 6; n++) jp.hid.axis().append({n});
    for(unsigned n = 0; n < 8; n++) jp.hid.hat().append({n});
    for(unsigned n = 0; n < 128; n++) jp.hid.button().append({n});
    joypads.append(jp);

    return DIENUM_CONTINUE;
  }

  bool initAxis(const DIDEVICEOBJECTINSTANCE* instance) {
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

  bool initEffect(const DIDEVICEOBJECTINSTANCE* instance) {
    effects++;
    return DIENUM_CONTINUE;
  }
};

BOOL CALLBACK DirectInput_EnumJoypadsCallback(const DIDEVICEINSTANCE* instance, void* p) {
  return ((InputJoypadDirectInput*)p)->initJoypad(instance);
}

BOOL CALLBACK DirectInput_EnumJoypadAxesCallback(const DIDEVICEOBJECTINSTANCE* instance, void* p) {
  return ((InputJoypadDirectInput*)p)->initAxis(instance);
}

BOOL CALLBACK DirectInput_EnumJoypadEffectsCallback(const DIDEVICEOBJECTINSTANCE* instance, void* p) {
  return ((InputJoypadDirectInput*)p)->initEffect(instance);
}

}

#endif
