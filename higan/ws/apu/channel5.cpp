auto APU::Channel5::run() -> void {
  int11 output;
  switch(r.scale) {
  case 0: output = (uint8)s.data << 3 - r.volume; break;
  case 1: output = (uint8)s.data - 0x100 << 3 - r.volume; break;
  case 2: output =  (int8)s.data << 3 - r.volume; break;
  case 3: output = (uint8)s.data << 3; break;
  }

  o.left  = r.leftEnable  ? output : (int11)0;
  o.right = r.rightEnable ? output : (int11)0;
}
