void OBC1::serialize(serializer& s) {
  s.array(ram.data(), ram.size());

  s.integer(status.address);
  s.integer(status.baseptr);
  s.integer(status.shift);
}
