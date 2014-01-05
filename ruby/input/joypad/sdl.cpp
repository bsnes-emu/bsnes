#ifndef RUBY_INPUT_JOYPAD_SDL
#define RUBY_INPUT_JOYPAD_SDL

namespace ruby {

struct InputJoypadSDL {
  struct Joypad {
    HID::Joypad hid;

    unsigned id = 0;
    SDL_Joystick* handle = nullptr;
  };
  vector<Joypad> joypads;

  void assign(HID::Joypad& hid, unsigned groupID, unsigned inputID, int16_t value) {
    auto& group = hid.group[groupID];
    if(group.input[inputID].value == value) return;
    if(input.onChange) input.onChange(hid, groupID, inputID, group.input[inputID].value, value);
    group.input[inputID].value = value;
  }

  void poll(vector<HID::Device*>& devices) {
    SDL_JoystickUpdate();

    for(auto& jp : joypads) {
      for(unsigned n = 0; n < jp.hid.axis().input.size(); n++) {
        assign(jp.hid, HID::Joypad::GroupID::Axis, n, (int16_t)SDL_JoystickGetAxis(jp.handle, n));
      }

      for(signed n = 0; n < (signed)jp.hid.hat().input.size() - 1; n += 2) {
        uint8_t state = SDL_JoystickGetHat(jp.handle, n >> 1);
        assign(jp.hid, HID::Joypad::GroupID::Hat, n + 0, state & SDL_HAT_LEFT ? -32768 : state & SDL_HAT_RIGHT ? +32767 : 0);
        assign(jp.hid, HID::Joypad::GroupID::Hat, n + 1, state & SDL_HAT_UP ? -32768 : state & SDL_HAT_DOWN ? +32767 : 0);
      }

      for(unsigned n = 0; n < jp.hid.button().input.size(); n++) {
        assign(jp.hid, HID::Joypad::GroupID::Button, n, (bool)SDL_JoystickGetButton(jp.handle, n));
      }

      devices.append(&jp.hid);
    }
  }

  bool init() {
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_IGNORE);

    unsigned joypadCount = SDL_NumJoysticks();
    for(unsigned id = 0; id < joypadCount; id++) {
      Joypad jp;
      jp.id = id;
      jp.handle = SDL_JoystickOpen(id);

      unsigned axes = SDL_JoystickNumAxes(jp.handle);
      unsigned hats = SDL_JoystickNumHats(jp.handle) * 2;
      unsigned buttons = 32;  //there is no SDL_JoystickNumButtons()

      jp.hid.id = 2 + jp.id;
      for(unsigned n = 0; n < axes; n++) jp.hid.axis().append({n});
      for(unsigned n = 0; n < hats; n++) jp.hid.hat().append({n});
      for(unsigned n = 0; n < buttons; n++) jp.hid.button().append({n});
      jp.hid.rumble = false;

      joypads.append(jp);
    }

    return true;
  }

  void term() {
    for(auto& jp : joypads) {
      SDL_JoystickClose(jp.handle);
    }
    joypads.reset();
    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
  }
};

}

#endif
