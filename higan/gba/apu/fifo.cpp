auto APU::FIFO::sample() -> void {
  output = active;
}

auto APU::FIFO::read() -> void {
  if(size == 0) return;
  size--;
  active = samples[rdoffset++];
}

auto APU::FIFO::write(int8 byte) -> void {
  if(size == 32) rdoffset++;
  else size++;
  samples[wroffset++] = byte;
}

auto APU::FIFO::reset() -> void {
  for(auto& byte : samples) byte = 0;
  active = 0;
  output = 0;

  rdoffset = 0;
  wroffset = 0;
  size = 0;
}

auto APU::FIFO::power() -> void {
  reset();

  lenable = 0;
  renable = 0;
  timer = 0;
}
