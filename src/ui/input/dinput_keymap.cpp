key.esc          = 0x01;

key.f1           = 0x3b;
key.f2           = 0x3c;
key.f3           = 0x3d;
key.f4           = 0x3e;
key.f5           = 0x3f;
key.f6           = 0x40;
key.f7           = 0x41;
key.f8           = 0x42;
key.f9           = 0x43;
key.f10          = 0x44;
key.f11          = 0x57;
key.f12          = 0x58;

key.print_screen = 0xb7;
key.scroll_lock  = 0x46;
key.pause        = 0xc5;

key.tilde        = 0x29;

key.num_0        = 0x0b;
key.num_1        = 0x02;
key.num_2        = 0x03;
key.num_3        = 0x04;
key.num_4        = 0x05;
key.num_5        = 0x06;
key.num_6        = 0x07;
key.num_7        = 0x08;
key.num_8        = 0x09;
key.num_9        = 0x0a;

key.minus        = 0x0c;
key.plus         = 0x0d;
key.backspace    = 0x0e;

key.ins          = 0xd2;
key.del          = 0xd3;
key.home         = 0xc7;
key.end          = 0xcf;
key.page_up      = 0xc9;
key.page_down    = 0xd1;

key.a            = 0x1e;
key.b            = 0x30;
key.c            = 0x2e;
key.d            = 0x20;
key.e            = 0x12;
key.f            = 0x21;
key.g            = 0x22;
key.h            = 0x23;
key.i            = 0x17;
key.j            = 0x24;
key.k            = 0x25;
key.l            = 0x26;
key.m            = 0x32;
key.n            = 0x31;
key.o            = 0x18;
key.p            = 0x19;
key.q            = 0x10;
key.r            = 0x13;
key.s            = 0x1f;
key.t            = 0x14;
key.u            = 0x16;
key.v            = 0x2f;
key.w            = 0x11;
key.x            = 0x2d;
key.y            = 0x15;
key.z            = 0x2c;

key.lbracket     = 0x1a;
key.rbracket     = 0x1b;
key.pipe         = 0x2b;
key.colon        = 0x27;
key.quote        = 0x28;
key.comma        = 0x33;
key.period       = 0x34;
key.question     = 0x35;

key.numpad_0     = 0x52;
key.numpad_1     = 0x4f;
key.numpad_2     = 0x50;
key.numpad_3     = 0x51;
key.numpad_4     = 0x4b;
key.numpad_5     = 0x4c;
key.numpad_6     = 0x4d;
key.numpad_7     = 0x47;
key.numpad_8     = 0x48;
key.numpad_9     = 0x49;
key.numpad_plus  = 0x4e;
key.numpad_minus = 0x4a;
key.numpad_mul   = 0x37;
key.numpad_div   = 0xb5;
key.numpad_enter = 0x9c;
key.numpad_point = 0x53;

key.numlock      = 0x45;
key.capslock     = 0x3a;

key.up           = 0xc8;
key.down         = 0xd0;
key.left         = 0xcb;
key.right        = 0xcd;

key.tab          = 0x0f;
key.enter        = 0x1c;
key.space        = 0x39;
key.lctrl        = 0x1d;
key.rctrl        = 0x9d;
key.lalt         = 0x38;
key.ralt         = 0xb8;
key.lshift       = 0x2a;
key.rshift       = 0x36;

key.lwin         = 0xdb;
key.rwin         = 0xdc;
key.menu         = 0xdd;

for(int joy = 0; joy < 16; joy++) {
  key.joypad[joy].up    = 0x100 + (joy * 128) + 124;
  key.joypad[joy].down  = 0x100 + (joy * 128) + 125;
  key.joypad[joy].left  = 0x100 + (joy * 128) + 126;
  key.joypad[joy].right = 0x100 + (joy * 128) + 127;
  for(int bn = 0; bn <= 123; bn++) {
    key.joypad[joy].button[bn] = 0x100 + (joy * 128) + bn;
  }
}
