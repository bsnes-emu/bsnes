/*
  libconfig : version 0.01 ~byuu
*/

#ifndef __LIBCONFIG
#define __LIBCONFIG

#include "libstring.h"

class config_item {
public:
uint32 *source, def, type;
string *strsource, strdef;
bool is_string;
string name;
  bool changed();
  config_item();
};

class config {
private:
uint32 item_count;
config_item *item[4096];
string data, line, oldline, newline, part;
public:
enum {
  TRUEFALSE = 0,
  YESNO     = 1,
  ONOFF     = 2,
  ENABLED   = 3,
  DEC       = 4,
  HEX       = 5,
  STR       = 6
};
  void add(uint32 *variable, char *name, uint32 def, uint32 type = DEC);
  void add(string *variable, char *name, char *def, uint32 type = STR);
  uint32 find(char *name);
  uint32 get(char *name);
  string &strget(char *name);
  void set(char *name, uint32 value);
  void set(char *name, char *value);

  void load(char *fn);
  void save(char *fn);
  void set_newline(int i);

  config();
  ~config();
};

#endif
