void InputSDL::poll(uint8 device) {
  Input::poll(device);
}

void InputSDL::poll() {
  memcpy(keystate, SDL_GetKeyState(0), SDLK_LAST);
}

void InputSDL::init() {
  #include "sdl_keymap.cpp"
  Input::init();
}

void InputSDL::term() {
  Input::term();
}

InputSDL::InputSDL(void *handle)  {
  window = handle;

//
//SDL hack
//Put SDL output into main GTK+ window, rather than
//creating a new window for SDL output.
//
  if(window) {
  char t[64];
    sprintf(t, "SDL_WINDOWID=%ld", window);
  #if defined(COMPILER_GCC)
    putenv(t);
  #elif defined(COMPILER_VISUALC)
    _putenv(t);
  #endif
  }
}

InputSDL::~InputSDL() {}
