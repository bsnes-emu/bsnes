namespace Heuristics {

auto Memory::text() const -> string {
  string output;
  output.append("    memory\n");
  output.append("      type: ", _type, "\n");
  output.append("      size: 0x", hex(_size), "\n");
  output.append("      category: ", _category, "\n");
if(_manufacturer)
  output.append("      manufacturer: ", _manufacturer, "\n");
if(_part)
  output.append("      part: ", _part, "\n");
if(_note)
  output.append("      note: ", _note, "\n");
if(_battery)
  output.append("      battery\n");
  return output;
}

auto Oscillator::text() const -> string {
  string output;
  output.append("    oscillator\n");
  output.append("      frequency: ", _frequency, "\n");
if(_note)
  output.append("      note: ", _note, "\n");
  return output;
}

//deprecated
auto Heuristics::memory(string type, uint size, string name, string metadata) const -> string {
  string output;
  output.append("    memory\n");
  output.append("      type: ", type, "\n");
  output.append("      size: 0x", hex(size), "\n");
  output.append("      name: ", name, "\n");
if(metadata)
  output.append("      metadata: ", metadata, "\n");
  return output;
}

}
