#define CLASS_NAME "sCPU"
#include "../../../lib/opgen_s.cpp"

int main() {
//fph = fopen("op.h", "wb");
//fpt = fopen("optable.cpp", "wb");

  generate("op_read.cpp",  "op_read.b");
  generate("op_write.cpp", "op_write.b");
  generate("op_rmw.cpp",   "op_rmw.b");
  generate("op_pc.cpp",    "op_pc.b");
  generate("op_misc.cpp",  "op_misc.b");

//fclose(fph);
//fclose(fpt);

  return 0;
}
