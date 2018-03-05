namespace Heuristics {

auto Memory::text() const -> string {
  string output;
  output.append("    memory\n");
  output.append("      type: ", _type, "\n");
if(_battery)
  output.append("        battery\n");
  output.append("      size: 0x", hex(_size), "\n");
  output.append("      category: ", _category, "\n");
if(_manufacturer)
  output.append("      manufacturer: ", _manufacturer, "\n");
if(_part)
  output.append("      part: ", _part, "\n");
if(_note)
  output.append("      note: ", _note, "\n");
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

}
