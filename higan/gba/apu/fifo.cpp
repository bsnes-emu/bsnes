auto APU::FIFO::read() -> void {
  if(size == 0) return;
  size--;
  output = sample[rdoffset++];
}

auto APU::FIFO::write(int8 byte) -> void {
  if(size == 32) rdoffset++;
  else size++;
  sample[wroffset++] = byte;
}

auto APU::FIFO::reset() -> void {
  for(auto& byte : sample) byte = 0;
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
