#define CLASS_NAME "sSMP"
#include <tool/opgen_switch.cpp>

int main() {
  generate("op_mov.cpp",  "op_mov.b");
  generate("op_pc.cpp",   "op_pc.b");
  generate("op_read.cpp", "op_read.b");
  generate("op_rmw.cpp",  "op_rmw.b");
  generate("op_misc.cpp", "op_misc.b");

  return 0;
}
