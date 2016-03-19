auto APU::Channel5::run() -> void {
  int11 output = (int8)s.data;
  switch(r.scale) {
  case 0: output <<= 3 - r.volume; break;
  case 1: output <<= 3 - r.volume; output |= -0x100 << (3 - r.volume); break;
  case 2: output <<= 3 - r.volume; break;
  case 3: output <<= 3; break;
  }

  o.left = r.leftEnable ? output : (int11)0;
  o.right = r.rightEnable ? output : (int11)0;
}
