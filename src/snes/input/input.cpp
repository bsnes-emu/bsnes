#ifdef SNES_CPP

uint8 SNES::Input::port_read(bool port) {
  switch(port_device[port]) {
    case DeviceJoypad: {
      if(port_counter0[port] >= 16) return 1;

      unsigned deviceid = port == 0 ? DeviceIDJoypad1 : DeviceIDJoypad2;
      return snesinterface.input_poll(deviceid, port_counter0[port]++);
    } //case DeviceJoypad

    case DeviceMultitap: {
      if(cpu.joylatch()) return 2; //when latch is high -- data2 = 1, data1 = 0

      unsigned deviceidx, deviceid0, deviceid1;
      if(port == 0) {
        if(cpu.pio() & 0x40) {
          deviceidx = port_counter0[port];
          if(deviceidx >= 16) return 3;
          port_counter0[port]++;

          deviceid0 = DeviceIDMultitap1A;
          deviceid1 = DeviceIDMultitap1B;
        } else {
          deviceidx = port_counter1[port];
          if(deviceidx >= 16) return 3;
          port_counter1[port]++;

          deviceid0 = DeviceIDMultitap1C;
          deviceid1 = DeviceIDMultitap1D;
        }
      } else {
        if(cpu.pio() & 0x80) {
          deviceidx = port_counter0[port];
          if(deviceidx >= 16) return 3;
          port_counter0[port]++;

          deviceid0 = DeviceIDMultitap2A;
          deviceid1 = DeviceIDMultitap2B;
        } else {
          deviceidx = port_counter1[port];
          if(deviceidx >= 16) return 3;
          port_counter1[port]++;

          deviceid0 = DeviceIDMultitap2C;
          deviceid1 = DeviceIDMultitap2D;
        }
      }

      return (snesinterface.input_poll(deviceid0, deviceidx) << 0)
           | (snesinterface.input_poll(deviceid1, deviceidx) << 1);
    } //case DeviceMultitap
  }

  //no device connected
  return 0;
}

void SNES::Input::port_set_device(bool port, unsigned device) {
  port_device[port] = device;
  port_counter0[port] = 0;
  port_counter1[port] = 0;
}

void SNES::Input::poll() {
  snesinterface.input_poll();
  port_counter0[0] = 0;
  port_counter1[0] = 0;
  port_counter0[1] = 0;
  port_counter1[1] = 0;
}

void SNES::Input::init() {
  port_set_device(0, config::snes.controller_port1);
  port_set_device(1, config::snes.controller_port2);
}

#endif //ifdef SNES_CPP
