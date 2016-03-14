APU::Channel4::Channel4() : Channel(4) {
}

auto APU::Channel4::run() -> void {
  int16 sample = s.data << 8;
  if(r.scale != 3) sample >>= r.volume;
  if(r.scale == 1) sample |= 0x70000 >> r.volume;

  o.left = r.leftEnable ? sample : (int16)0;
  o.right = r.rightEnable ? sample : (int16)0;
}
