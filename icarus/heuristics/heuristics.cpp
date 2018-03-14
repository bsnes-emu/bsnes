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
if(_model)
  output.append("      model: ", _model, "\n");
if(_identity)
  output.append("      identity: ", _identity, "\n");
  return output;
}

auto Oscillator::text() const -> string {
  string output;
  output.append("    oscillator\n");
  output.append("      frequency: ", _frequency, "\n");
  return output;
}

}
