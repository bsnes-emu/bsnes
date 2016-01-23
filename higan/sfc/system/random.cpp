Random random;

auto Random::seed(uint seed) -> void {
  iter = seed;
}

auto Random::operator()(uint result) -> uint {
  if(!settings.random) return result;
  return iter = (iter >> 1) ^ (((iter & 1) - 1) & 0xedb88320);
}

auto Random::serialize(serializer& s) -> void {
  s.integer(iter);
}
