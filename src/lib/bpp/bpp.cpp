#include "bpp.hpp"

bool BPP::process_macro(Macro &macro, const lstring &arg) {
  lstring linedata;
  linedata.split("\n", macro.data);

  for(unsigned i = 0; i < linedata.size(); i++) {
    string line = linedata[i];

    for(unsigned j = 0; j < macro.args; j++) {
      line.replace(macro.arg[j], arg[j]);
    }

    for(unsigned j = 0; j < global.size(); j++) {
      line.replace(global[j].name, global[j].data);
    }

    output.print(string() << line << "\n");
  }

  return true;
}

bool BPP::process(const char *inputfn) {
  string filedata;
  if(filedata.readfile(inputfn) == false) {
    fprintf(stdout, "error: failed to open input file '%s'\n", inputfn);
    return false;
  }

  filedata.replace("\r", "");
  lstring linedata;
  linedata.split("\n", filedata);

  for(unsigned i = 0; i < linedata.size();) {
    string line = linedata[i];
    trim(line);
    line.replace("\t", " ");

    if(strbegin(line, "@") == false) {
      output.print(string() << linedata[i] << "\n");
      i++;
    } else if(strbegin(line, "@include ")) {
      ltrim(line, "@include ");
      trim(line, "\"");
      if(process(line) == false) return false;
      i++;
    } else if(strbegin(line, "@global ")) {
      ltrim(line, "@global ");
      while(qstrpos(line, "  ") >= 0) line.replace("  ", " ");

      lstring part;
      part.qsplit(" ", line);
      if(part.size() != 2) {
        fprintf(stdout, "error: malformed global\n");
        return false;
      }

      unsigned n = global.size();
      global[n].name = string() << "{" << part[0] << "}";
      global[n].data = trim(part[1], "\"");
      i++;
    } else if(strbegin(line, "@macro ")) {
      ltrim(line, "@macro ");

      int pos = strpos(line, "(");
      if(pos < 0 || !strend(line, ")")) {
        fprintf(stdout, "error: malformed macro\n");
        return false;
      }

      string name = substr(line, 0, pos);
      line = substr(line, pos);
      ltrim(line, "(");
      rtrim(line, ")");
      line.qreplace(" ", "");

      unsigned n = macro.size();
      macro[n].name = name;

      if(line == "") {
        macro[n].args = 0;
      } else {
        lstring arg;
        arg.split(",", line);
        if(arg.size() >= 8) {
          fprintf(stdout, "error: too many macro arguments\n");
          return false;
        }

        macro[n].args = arg.size();
        for(unsigned j = 0; j < arg.size(); j++) {
          macro[n].arg[j] = string() << "{" << arg[j] << "}";
        }
      }

      i++;

      while(i < linedata.size()) {
        string line = linedata[i];
        trim(line);
        if(line == "@endmacro") break;
        macro[n].data << linedata[i++] << "\n";
      }

      i++;
    } else {
      ltrim(line, "@");
      int pos = strpos(line, "(");
      if(pos < 0 || !strend(line, ")")) {
        fprintf(stdout, "error: malformed macro invocation\n");
        return false;
      }

      string name = substr(line, 0, pos);
      line = substr(line, pos);
      ltrim(line, "(");
      rtrim(line, ")");
      line.qreplace(" ", "");

      lstring arg;
      if(line != "") arg.qsplit(",", line);
      for(unsigned j = 0; j < arg.size(); j++) {
        trim(arg[j], "\"");
      }

      bool success = false;
      for(unsigned j = 0; j < macro.size(); j++) {
        if(name == macro[j].name && arg.size() == macro[j].args) {
          success = process_macro(macro[j], arg);
          break;
        }
      }

      if(success == false) {
        fprintf(stdout, "error: unable to process macro '%s'\n", (const char*)name);
        return false;
      }

      i++;
    }
  }

  return true;
}

bool BPP::process(const char *outputfn, const char *inputfn) {
  if(file::exists(inputfn) == false) {
    fprintf(stdout, "error: failed to open input file\n");
    return false;
  }

  if(output.open(outputfn, file::mode_write) == false) {
    fprintf(stdout, "error: failed to open output file\n");
    return false;
  }

  bool result = process(inputfn);
  output.close();
  return result;
}

int main(int argc, const char **argv) {
  if(argc != 3) {
    fprintf(stdout, "bpp v0.01\n");
    fprintf(stdout, "author: byuu\n");
    fprintf(stdout, "usage: bpp output input\n");
    return 0;
  }

  BPP bpp;
  bool success = bpp.process(argv[1], argv[2]);
  if(success == false) fprintf(stdout, "error: pre-processing failed\n");
  return 0;
}
