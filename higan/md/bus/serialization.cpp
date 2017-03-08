auto BusCPU::serialize(serializer& s) -> void {
  s.array(ram);
}

auto BusAPU::serialize(serializer& s) -> void {
  s.array(ram);
  s.integer(bank);
}
