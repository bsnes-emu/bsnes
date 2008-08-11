class Input {
public:
  enum Device {
    DeviceNone,
    DeviceJoypad,
    DeviceMultitap,
  };

  enum {
    DeviceIDNone,
    DeviceIDJoypad1,
    DeviceIDJoypad2,
    DeviceIDMultitap1A,
    DeviceIDMultitap1B,
    DeviceIDMultitap1C,
    DeviceIDMultitap1D,
    DeviceIDMultitap2A,
    DeviceIDMultitap2B,
    DeviceIDMultitap2C,
    DeviceIDMultitap2D,
  };

  enum {
    JoypadB      =  0, JoypadY      =  1,
    JoypadSelect =  2, JoypadStart  =  3,
    JoypadUp     =  4, JoypadDown   =  5,
    JoypadLeft   =  6, JoypadRight  =  7,
    JoypadA      =  8, JoypadX      =  9,
    JoypadL      = 10, JoypadR      = 11,
  };

  uint8 port_read(bool port);
  void port_set_device(bool port, unsigned device);
  void init();
  void poll();

private:
  unsigned port_device[2], port_counter0[2], port_counter1[2];
} input;
