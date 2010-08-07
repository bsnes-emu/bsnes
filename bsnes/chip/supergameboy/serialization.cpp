#ifdef SUPERGAMEBOY_CPP

void SuperGameBoy::serialize(serializer &s) {
  Processor::serialize(s);
  s.integer(row);
  if(sgb_serialize) sgb_serialize(s);
}

#endif
