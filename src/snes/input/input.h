class Input {
public:
  enum Device {
    DeviceNone,
    DeviceJoypad,
  };

  enum {
    DeviceIDNone,
    DeviceIDJoypad1,
    DeviceIDJoypad2,
  };

  enum {
    JoypadB      =  0, JoypadY      =  1,
    JoypadSelect =  2, JoypadStart  =  3,
    JoypadUp     =  4, JoypadDown   =  5,
    JoypadLeft   =  6, JoypadRight  =  7,
    JoypadA      =  8, JoypadX      =  9,
    JoypadL      = 10, JoypadR      = 11,
  };

  struct {
    uint port0_device, port0_devicebits, port0_devicebitpos, port0_deviceid;
    uint port1_device, port1_devicebits, port1_devicebitpos, port1_deviceid;

    bool port0_bits[256];
    bool port1_bits[256];
  } input;

  bool port_read(bool port);
  void port_set_deviceid(bool port, uint deviceid);
  void init();
  void poll();
} input;
