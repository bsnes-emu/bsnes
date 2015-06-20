#ifndef RUBY_INPUT_JOYPAD_SDL
#define RUBY_INPUT_JOYPAD_SDL

struct InputJoypadSDL {
  Input& input;
  InputJoypadSDL(Input& input) : input(input) {}

  struct Joypad {
    shared_pointer<HID::Joypad> hid{new HID::Joypad};

    unsigned id = 0;
    SDL_Joystick* handle = nullptr;
  };
  vector<Joypad> joypads;

  auto assign(shared_pointer<HID::Joypad> hid, unsigned groupID, unsigned inputID, int16_t value) -> void {
    auto& group = hid->group(groupID);
    if(group.input(inputID).value() == value) return;
    input.doChange(hid, groupID, inputID, group.input(inputID).value(), value);
    group.input(inputID).setValue(value);
  }

  auto poll(vector<shared_pointer<HID::Device>>& devices) -> void {
    SDL_JoystickUpdate();

    for(auto& jp : joypads) {
      for(auto n : range(jp.hid->axes())) {
        assign(jp.hid, HID::Joypad::GroupID::Axis, n, (int16_t)SDL_JoystickGetAxis(jp.handle, n));
      }

      for(signed n = 0; n < (signed)jp.hid->hats().size() - 1; n += 2) {
        uint8_t state = SDL_JoystickGetHat(jp.handle, n >> 1);
        assign(jp.hid, HID::Joypad::GroupID::Hat, n + 0, state & SDL_HAT_LEFT ? -32768 : state & SDL_HAT_RIGHT ? +32767 : 0);
        assign(jp.hid, HID::Joypad::GroupID::Hat, n + 1, state & SDL_HAT_UP ? -32768 : state & SDL_HAT_DOWN ? +32767 : 0);
      }

      for(auto n : range(jp.hid->buttons())) {
        assign(jp.hid, HID::Joypad::GroupID::Button, n, (bool)SDL_JoystickGetButton(jp.handle, n));
      }

      devices.append(jp.hid);
    }
  }

  auto init() -> bool {
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_IGNORE);

    for(auto id : range(SDL_NumJoysticks())) {
      Joypad jp;
      jp.id = id;
      jp.handle = SDL_JoystickOpen(id);

      unsigned axes = SDL_JoystickNumAxes(jp.handle);
      unsigned hats = SDL_JoystickNumHats(jp.handle) * 2;
      unsigned buttons = 32;  //there is no SDL_JoystickNumButtons()

      jp.hid->setID(3 + jp.id);
      for(auto n : range(axes)) jp.hid->axes().append(n);
      for(auto n : range(hats)) jp.hid->hats().append(n);
      for(auto n : range(buttons)) jp.hid->buttons().append(n);
      jp.hid->setRumble(false);

      joypads.append(jp);
    }

    return true;
  }

  auto term() -> void {
    for(auto& jp : joypads) {
      SDL_JoystickClose(jp.handle);
    }
    joypads.reset();
    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
  }
};

#endif
