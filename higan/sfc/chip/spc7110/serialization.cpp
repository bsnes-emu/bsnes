#ifdef SPC7110_CPP

void SPC7110::serialize(serializer &s) {
  s.array(ram.data(), ram.size());

  s.integer(r4801);
  s.integer(r4802);
  s.integer(r4803);
  s.integer(r4804);
  s.integer(r4805);
  s.integer(r4806);
  s.integer(r4807);
  s.integer(r4809);
  s.integer(r480a);
  s.integer(r480b);
  s.integer(r480c);

  s.integer(dcu_pending);
  s.integer(dcu_mode);
  s.integer(dcu_addr);
  s.integer(dcu_offset);
  s.array(dcu_tile);
  decompressor->serialize(s);

  s.integer(r4810);
  s.integer(r4811);
  s.integer(r4812);
  s.integer(r4813);
  s.integer(r4814);
  s.integer(r4815);
  s.integer(r4816);
  s.integer(r4817);
  s.integer(r4818);
  s.integer(r481a);

  s.integer(r4820);
  s.integer(r4821);
  s.integer(r4822);
  s.integer(r4823);
  s.integer(r4824);
  s.integer(r4825);
  s.integer(r4826);
  s.integer(r4827);
  s.integer(r4828);
  s.integer(r4829);
  s.integer(r482a);
  s.integer(r482b);
  s.integer(r482c);
  s.integer(r482d);
  s.integer(r482e);
  s.integer(r482f);

  s.integer(mul_pending);
  s.integer(div_pending);

  s.integer(r4830);
  s.integer(r4831);
  s.integer(r4832);
  s.integer(r4833);
  s.integer(r4834);
}

#endif
