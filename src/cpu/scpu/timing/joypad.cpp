void sCPU::run_manual_joypad_poll() {
  snes->poll_input(SNES::DEV_JOYPAD1);
  snes->poll_input(SNES::DEV_JOYPAD2);

  status.joypad1_bits  = uint8(snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_B))      <<  0;
  status.joypad1_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_Y))      <<  1;
  status.joypad1_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_SELECT)) <<  2;
  status.joypad1_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_START))  <<  3;
  status.joypad1_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_UP))     <<  4;
  status.joypad1_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_DOWN))   <<  5;
  status.joypad1_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_LEFT))   <<  6;
  status.joypad1_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_RIGHT))  <<  7;
  status.joypad1_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_A))      <<  8;
  status.joypad1_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_X))      <<  9;
  status.joypad1_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_L))      << 10;
  status.joypad1_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_R))      << 11;

  status.joypad2_bits  = uint8(snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_B))      <<  0;
  status.joypad2_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_Y))      <<  1;
  status.joypad2_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_SELECT)) <<  2;
  status.joypad2_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_START))  <<  3;
  status.joypad2_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_UP))     <<  4;
  status.joypad2_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_DOWN))   <<  5;
  status.joypad2_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_LEFT))   <<  6;
  status.joypad2_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_RIGHT))  <<  7;
  status.joypad2_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_A))      <<  8;
  status.joypad2_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_X))      <<  9;
  status.joypad2_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_L))      << 10;
  status.joypad2_bits |= uint8(snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_R))      << 11;
}

/*****
 * The joypad contains a small bit shifter that has 16 bits.
 * Reading from 4016 reads one bit from this buffer, then moves
 * the buffer left one, and adds a '1' to the rightmost bit.
 * Writing a one to $4016 will fill the buffer with the current
 * joypad button states, and lock the bit shifter at position
 * zero. All reads will be the first buffer state, or 'B'.
 * A zero must be written back to $4016 to unlock the buffer,
 * so that reads will increment the bit shifting position.
 *****/

void sCPU::run_auto_joypad_poll() {
  run_manual_joypad_poll();

uint16 joy1 = 0x0000, joy2 = 0x0000;
  for(int i = 0; i < 16; i++) {
    joy1 |= (!!(status.joypad1_bits & (0x8000 >> i))) << i;
    joy2 |= (!!(status.joypad2_bits & (0x8000 >> i))) << i;
  }

  status.joy1l = joy1;
  status.joy1h = joy1 >> 8;

  status.joy2l = joy2;
  status.joy2h = joy2 >> 8;

  status.joy3l = 0x00;
  status.joy3h = 0x00;

  status.joy4l = 0x00;
  status.joy4h = 0x00;

  status.joypad1_bits >>= 16;
  status.joypad2_bits >>= 16;

  status.joypad1_bits  |= 0xffff;
  status.joypad2_bits  |= 0xffff;
}
