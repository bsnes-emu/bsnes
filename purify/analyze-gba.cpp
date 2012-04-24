#include <nall/platform.hpp>
#include <nall/file.hpp>
#include <nall/string.hpp>
using namespace nall;

int main() {
  string filedata;
  if(filedata.readfile("database-gameboyadvance.bsv") == false) return 0;
  lstring lines = filedata.split("\n");
  unsigned count;

  file fp;
  if(fp.open("database-gameboyadvance-analysis.txt", file::mode::write) == false) return 0;

  fp.print("Multiple Tags:\n");
  fp.print("--------------\n\n");
  count = 0;
  for(auto &line : lines) {
    if(line.empty()) continue;
    lstring part = line.split("{}");
    if(part(2).position(",")) fp.print(part(3), "\n", part(2), "\n\n"), count++;
  }
  fp.print("Total: ", count, "\n\n");

  fp.print("EEPROM:\n");
  fp.print("-------\n");
  count = 0;
  for(auto &line : lines) {
    if(line.empty()) continue;
    lstring part = line.split("{}");
    if(part(2).position("EEPROM")) fp.print(part(3), "\n", part(2), "\n\n"), count++;
  }
  fp.print("Total: ", count, "\n\n");

  fp.print("No RAM:\n");
  fp.print("-------\n");
  count = 0;
  for(auto &line : lines) {
    if(line.empty()) continue;
    lstring part = line.split("{}");
    if(part(2).empty()) fp.print(part(3), "\n"), count++;
  }
  fp.print("\nTotal: ", count, "\n\n");

  fp.close();
  return 0;
}
