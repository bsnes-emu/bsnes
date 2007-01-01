enum {
  DEVICE_NONE = 0,
  DEVICE_JOYPAD,
};

enum {
  DEVICEID_NONE = 0,
  DEVICEID_JOYPAD1,
  DEVICEID_JOYPAD2,
};

enum {
  JOYPAD_B      =  0, JOYPAD_Y      =  1,
  JOYPAD_SELECT =  2, JOYPAD_START  =  3,
  JOYPAD_UP     =  4, JOYPAD_DOWN   =  5,
  JOYPAD_LEFT   =  6, JOYPAD_RIGHT  =  7,
  JOYPAD_A      =  8, JOYPAD_X      =  9,
  JOYPAD_L      = 10, JOYPAD_R      = 11,
};

struct {
  uint port0_device, port0_devicebits, port0_devicebitpos, port0_deviceid;
  uint port1_device, port1_devicebits, port1_devicebitpos, port1_deviceid;

  bool port0_bits[256];
  bool port1_bits[256];
} input;

  bool port_read(bool port);
  void port_set_deviceid(bool port, uint deviceid);
  void input_init();
  void poll_input();
