joypad_state joypad1;

/*
  *1 - The joypad contains a small bit shifter that has 16 bits.
    Reading from 4016 reads one bit from this buffer, then moves
    the buffer left one, and adds a '1' to the rightmost bit.
    Writing a one to $4016 will fill the buffer with the current
    joypad button states, and lock the bit shifter at position
    zero. All reads will be the first buffer state, or 'B'.
    A zero must be written back to $4016 to unlock the buffer,
    so that reads will increment the bit shifting position.
*/
byte mmio_r4016(void) {
byte r = 0;
  if(ppu.joypad_strobe_value == 1) { //*1
    r |= joypad1.b;
  } else {
    if     (joypad1.read_pos ==  0)r |= joypad1.b;
    else if(joypad1.read_pos ==  1)r |= joypad1.y;
    else if(joypad1.read_pos ==  2)r |= joypad1.select;
    else if(joypad1.read_pos ==  3)r |= joypad1.start;
    else if(joypad1.read_pos ==  4)r |= joypad1.up;
    else if(joypad1.read_pos ==  5)r |= joypad1.down;
    else if(joypad1.read_pos ==  6)r |= joypad1.left;
    else if(joypad1.read_pos ==  7)r |= joypad1.right;
    else if(joypad1.read_pos ==  8)r |= joypad1.a;
    else if(joypad1.read_pos ==  9)r |= joypad1.x;
    else if(joypad1.read_pos == 10)r |= joypad1.l;
    else if(joypad1.read_pos == 11)r |= joypad1.r;
    else if(joypad1.read_pos == 16)r |= 1; //joypad connected bit (1=yes, 0=no)
    else                           r |= 1; //after 16th read, all subsequent reads return 1
    if(++joypad1.read_pos > 17)joypad1.read_pos = 17;
  }
  return r;
}

byte mmio_r4218(void) {
byte r;
  if(ppu.auto_joypad_read == false)               return 0x00; //cannot read joypad if auto joypad read not enabled
  if(ppu.vline_pos >= 225 && ppu.vline_pos <= 227)return 0x00; //cannot read joypad while SNES is polling the joypad data
  r = joypad1.a << 7 |
      joypad1.x << 6 |
      joypad1.l << 5 |
      joypad1.r << 4;
  return r;
}

byte mmio_r4219(void) {
byte r;
  if(ppu.auto_joypad_read == false)               return 0x00; //cannot read joypad if auto joypad read not enabled
  if(ppu.vline_pos >= 225 && ppu.vline_pos <= 227)return 0x00; //cannot read joypad while SNES is polling the joypad data
  r = joypad1.b      << 7 |
      joypad1.y      << 6 |
      joypad1.select << 5 |
      joypad1.start  << 4 |
      joypad1.up     << 3 |
      joypad1.down   << 2 |
      joypad1.left   << 1 |
      joypad1.right;
  return r;
}

void mmio_w4016(byte value) {
  ppu.joypad_strobe_value = value;
  if(value == 1)UpdateJoypad();
  if(value == 0)joypad1.read_pos = 0;
}
