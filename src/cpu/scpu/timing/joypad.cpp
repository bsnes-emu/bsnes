void sCPU::run_auto_joypad_poll() {
uint16 joy1 = 0, joy2 = 0;
  for(int i = 0; i < 16; i++) {
    joy1 |= (uint16)snes.port_read(0) ? (0x8000 >> i) : 0;
    joy2 |= (uint16)snes.port_read(1) ? (0x8000 >> i) : 0;
  }

  status.joy1l = joy1;
  status.joy1h = joy1 >> 8;

  status.joy2l = joy2;
  status.joy2h = joy2 >> 8;

  status.joy3l = 0x00;
  status.joy3h = 0x00;

  status.joy4l = 0x00;
  status.joy4h = 0x00;
}
