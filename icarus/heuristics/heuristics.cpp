namespace Heuristics {

auto Heuristics::memory(string type, uint size, string name) const -> string {
  string output;
  output.append("  memory\n");
  output.append("    type: ", type, "\n");
  output.append("    size: 0x", hex(size), "\n");
  output.append("    name: ", name, "\n");
  return output;
}

}
