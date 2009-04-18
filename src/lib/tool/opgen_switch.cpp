#include <nall/string.hpp>
using namespace nall;

FILE *fp;

string data, output_op;
lstring line, part, subpart;

struct OpList {
  string name;
  lstring arg;
} op_list[64];

int32_t op_count, line_num;

void clear_op_list() {
  op_count = 0;
  for(unsigned i = 0; i < 64; i++) {
    op_list[i].name = "";
    for(unsigned l = 0; l < 8; l++) {
      op_list[i].arg[l] = "";
    }
  }
}

void gen_begin() {
  int i = line_num;
  clear_op_list();

  while(true) {
    int z = op_count++;
    string temp = line[i];
    rtrim(temp, "),");
    rtrim(temp, ") {");
    subpart.split("(", temp);
    op_list[z].name = subpart[0];
    part.split(", ", subpart[1]);
    for(unsigned l = 0; l < part.size(); l++) {
      op_list[z].arg[l] = part[l];
    }
    if(strend(line[i], " {") == true) break;
    i++;
  }

  output_op = "//$$\r\ncase $0: {\r\n";
  line_num = i + 1;
}

void update_line(int i) {
  line[i].replace("end;", "break;");
}

void gen_op() {
  int i = line_num, n, c;
  char t[4096];
  while(true) {
    if(!strcmp(line[i], "}"))break;

    //remove cycle number
    n = strunsigned(line[i]);
    sprintf(t, "%d:", n);
    ltrim(line[i], t);
    //sprintf(t, "//%d:\r\n", n);
    //strcat(output_op, t);

    update_line(i);
    if(strcmp(line[i], "")) {
      output_op << "  ";
      output_op << line[i];
      output_op << "\r\n";
    }

    i++;
    while(true) {
      if(line[i][1] == ':' || line[i][2] == ':' || line[i] == "}") break;

      update_line(i);
      output_op << line[i];
      output_op << "\r\n";

      i++;
    }
  }

  output_op << "} break;";
  line_num = i + 1;
}

void gen_end() {
  string t;
  for(unsigned i = 0; i < op_count; i++) {
    t = output_op;
    t.replace("$$", op_list[i].name);
    t.replace("$0", op_list[i].arg[0]);
    t.replace("$1", op_list[i].arg[1]);
    t.replace("$2", op_list[i].arg[2]);
    t.replace("$3", op_list[i].arg[3]);
    t.replace("$4", op_list[i].arg[4]);
    t.replace("$5", op_list[i].arg[5]);
    t.replace("$6", op_list[i].arg[6]);
    t.replace("$7", op_list[i].arg[7]);
    fprintf(fp, "%s\r\n\r\n", (const char*)t);
  }
}

void generate(const char *dest, const char *src) {
  data.readfile(src);
  data.replace("\r\n", "\n");
  line.split("\n", data);

  fp = fopen(dest, "wb");
  string header = CLASS_NAME;
  fprintf(fp, "#ifdef %s_CPP\n\n", (const char*)strupper(header));  //inclusion guard

  line_num = 0;
  while(line_num < line.size()) {
    while(line_num < line.size() && !strcmp(line[line_num], "")) line_num++;
    if(line_num >= line.size()) break;

    gen_begin();
    gen_op();
    gen_end();
  }

  fprintf(fp, "#endif\n");
  fclose(fp);
}
