#ifdef SERIAL_CPP

void Serial::serialize(serializer &s) {
  Processor::serialize(s);
  s.integer((bool&)data1);
  s.integer((bool&)data2);
}

#endif
