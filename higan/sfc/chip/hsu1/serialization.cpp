#ifdef HSU1_CPP

void HSU1::serialize(serializer &s) {
  s.array(packet);
  s.integer(packetlength);
}

#endif
