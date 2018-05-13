auto SDD1::serialize(serializer& s) -> void {
  s.integer(r4800);
  s.integer(r4801);
  s.integer(r4804);
  s.integer(r4805);
  s.integer(r4806);
  s.integer(r4807);

  for(auto n : range(8)) {
    s.integer(dma[n].addr);
    s.integer(dma[n].size);
  }
  s.integer(dmaReady);
}
