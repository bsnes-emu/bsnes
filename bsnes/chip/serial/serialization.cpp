#ifdef SERIAL_CPP

void Serial::serialize(serializer &s) {
  Processor::serialize(s);
  s.integer((bool&)latch);
}

#endif
