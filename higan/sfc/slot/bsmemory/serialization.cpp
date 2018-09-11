auto BSMemory::serialize(serializer& s) -> void {
  if(!ROM) s.array(memory.data(), memory.size());
  s.integer(pin.queryable);
  s.integer(pin.flashable);
  s.integer(io.mode);
}
