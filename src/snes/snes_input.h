enum {
  DEV_JOYPAD1 = 0,
  DEV_JOYPAD2 = 1
};

enum {
  JOYPAD_B      =  0, JOYPAD_Y      =  1,
  JOYPAD_SELECT =  2, JOYPAD_START  =  3,
  JOYPAD_UP     =  4, JOYPAD_DOWN   =  5,
  JOYPAD_LEFT   =  6, JOYPAD_RIGHT  =  7,
  JOYPAD_A      =  8, JOYPAD_X      =  9,
  JOYPAD_L      = 10, JOYPAD_R      = 11
};

//The CPU calls poll_input() when the main interface should check the
//status of all joypad buttons and cache the results...
  virtual void poll_input(uint8 type) = 0;

//...and then the CPU calls get_input_status() whenever it needs one
//of the cached button values to be returned for emulation purposes.
  virtual bool get_input_status(uint8 device, uint8 button) = 0;
