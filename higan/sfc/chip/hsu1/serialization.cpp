#ifdef HSU1_CPP

void HSU1::serialize(serializer &s) {
  s.integer(txbusy);
  s.integer(rxbusy);
  s.integer(txlatch);

  unsigned size;

  size = txbuffer.size();
  s.integer(size);
  txbuffer.resize(size);
  s.array(txbuffer.data(), txbuffer.size());

  size = rxbuffer.size();
  s.integer(size);
  rxbuffer.resize(size);
  s.array(rxbuffer.data(), rxbuffer.size());
}

#endif
