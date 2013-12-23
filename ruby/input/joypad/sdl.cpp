#ifndef RUBY_INPUT_JOYPAD_SDL
#define RUBY_INPUT_JOYPAD_SDL

namespace ruby {

struct InputJoypadSDL {
  struct Joystick {
    unsigned id = 0;
    SDL_Joystick* handle = nullptr;
  };
  vector<Joystick> joysticks;

  bool poll(int16_t* table) {
    SDL_JoystickUpdate();
    for(auto& js : joysticks) {
      unsigned axes = min((unsigned)Joypad::Axes, SDL_JoystickNumAxes(js.handle));
      for(unsigned axis = 0; axis < axes; axis++) {
        table[joypad(js.id).axis(axis)] = (int16_t)SDL_JoystickGetAxis(js.handle, axis);
      }

      unsigned hats = min((unsigned)Joypad::Hats, SDL_JoystickNumHats(js.handle));
      for(unsigned hat = 0; hat < hats; hat++) {
        uint8_t state = SDL_JoystickGetHat(js.handle, hat);
        int16_t value = 0;
        if(state & SDL_HAT_UP   ) value |= Joypad::HatUp;
        if(state & SDL_HAT_DOWN ) value |= Joypad::HatDown;
        if(state & SDL_HAT_LEFT ) value |= Joypad::HatLeft;
        if(state & SDL_HAT_RIGHT) value |= Joypad::HatRight;
        table[joypad(js.id).hat(hat)] = value;
      }

      //there is no SDL_JoystickNumButtons function
      for(unsigned button = 0; button < Joypad::Buttons; button++) {
        table[joypad(js.id).button(button)] = (bool)SDL_JoystickGetButton(js.handle, button);
      }
    }

    return true;
  }

  bool init() {
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_IGNORE);

    unsigned joystickCount = SDL_NumJoysticks();
    for(unsigned id = 0; id < joystickCount; id++) {
      Joystick joystick;
      joystick.id = id;
      joystick.handle = SDL_JoystickOpen(id);
    }

    return true;
  }

  void term() {
    for(auto& js : joysticks) {
      SDL_JoystickClose(js.handle);
    }
    joysticks.reset();
    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
  }
};

}

#endif
