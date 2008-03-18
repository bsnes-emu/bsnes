#ifdef SNES_CPP

bool SNES::Input::port_read(bool port) {
  if(port == 0) {
    switch(input.port0_device) {
      case DeviceNone: return false;

      default: {
        if(input.port0_devicebitpos < input.port0_devicebits) {
          return input.port0_bits[input.port0_devicebitpos++];
        } else {
          return true;
        }
      }
    }
  } else {
    switch(input.port1_device) {
      case DeviceNone: return false;

      default: {
        if(input.port1_devicebitpos < input.port1_devicebits) {
          return input.port1_bits[input.port1_devicebitpos++];
        } else {
          return true;
        }
      }
    }
  }
}

void SNES::Input::port_set_deviceid(bool port, uint deviceid) {
  if(port == 0) {
    switch(deviceid) {
      case DeviceIDNone: {
        input.port0_device = DeviceNone;
      } break;

      case DeviceIDJoypad1:
      case DeviceIDJoypad2: {
        input.port0_device = DeviceJoypad;
        input.port0_devicebits = 16;
      } break;
    }

    memset(input.port0_bits, 0, sizeof(input.port0_bits));
    input.port0_devicebitpos = 0;
    input.port0_deviceid = deviceid;
  } else {
    switch(deviceid) {
      case DeviceIDNone: {
        input.port1_device = DeviceNone;
      } break;

      case DeviceIDJoypad1:
      case DeviceIDJoypad2: {
        input.port1_device = DeviceJoypad;
        input.port1_devicebits = 16;
      } break;
    }

    memset(input.port1_bits, 0, sizeof(input.port1_bits));
    input.port1_devicebitpos = 0;
    input.port1_deviceid = deviceid;
  }
}

void SNES::Input::poll() {
  snesinterface.input_poll();

  bool *p0 = input.port0_bits;
  bool *p1 = input.port1_bits;

  switch(input.port0_device) {
    case DeviceNone: break;

    default: {
      for(int i = 0; i < input.port0_devicebits; i++) {
        *p0++ = snesinterface.input_poll(input.port0_deviceid, i);
      }
    } break;
  }

  switch(input.port1_device) {
    case DeviceNone: break;

    default: {
      for(int i = 0; i < input.port1_devicebits; i++) {
        *p1++ = snesinterface.input_poll(input.port1_deviceid, i);
      }
    } break;
  }

  input.port0_devicebitpos = 0;
  input.port1_devicebitpos = 0;
}

void SNES::Input::init() {
  port_set_deviceid(0, config::snes.controller_port0);
  port_set_deviceid(1, config::snes.controller_port1);
}

#endif //ifdef SNES_CPP
