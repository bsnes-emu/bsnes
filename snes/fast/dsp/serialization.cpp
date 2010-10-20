#ifdef DSP_CPP

void DSP::serialize(serializer &s) {
  Processor::serialize(s);
  s.array(samplebuffer);
}

#endif
