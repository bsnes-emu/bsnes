auto APU::Channel5::run() -> void {
  if(r.speed <= 5 && s.clock++ < 1536) return;  //2000hz
  if(r.speed == 6 && s.clock++ < 2048) return;  //1500hz
  if(r.speed == 7 && s.clock++ < 3072) return;  //1000hz
  s.clock = 0;

  uint11 output = s.data;
  switch(r.scale) {
  case 0: output <<= 3 - r.volume; break;
  case 1: output <<= 3 - r.volume; output |= -0x100 << (3 - r.volume); break;
  case 2: output <<= 3 - r.volume; break;
  case 3: output <<= r.volume; break;
  }

  o.left = r.leftEnable ? output : (uint11)0;
  o.right = r.rightEnable ? output : (uint11)0;
}
