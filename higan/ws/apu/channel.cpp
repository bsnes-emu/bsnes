APU::Channel::Channel(uint id) : id(id) {
}

auto APU::Channel::sample(uint5 index) -> uint4 {
  auto data = iram.read((apu.r.waveBase << 6) + (id << 4) + (index >> 1));
  if(index.bit(0) == 0) return data.bits(0,3);
  if(index.bit(0) == 1) return data.bits(4,7);
}
