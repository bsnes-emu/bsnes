#include "libbase.h"
#include "libconfig.h"

void Setting::toggle() {
  data ^= 1;
  set(data);
}

uint Setting::get() {
  return data;
}

void Setting::set(uint _data) {
printf("%s %d\n", name, _data);
  data = _data;

  if(type != DEC && type != HEX) {
  //data is a boolean type
    data &= 1;
  }
}

Setting::Setting(Config *_parent, char *_name, char *_desc, uint _data, uint _type) {
int s;
  if(_parent) {
    _parent->add(this);
  }

  s = strlen(_name);
  name = (char*)malloc(s + 1);
  strcpy(name, _name);

  if(_desc) {
    s = strlen(_desc);
    desc = (char*)malloc(s + 1);
    strcpy(desc, _desc);
  } else {
    desc = (char*)malloc(1);
    *desc = 0;
  }

  data = _data;
  def  = _data;
  type = _type;
}

void Config::add(Setting *setting) {
  list[list_count++] = setting;
}

uint Config::string_to_uint(uint type, char *input) {
  if(!strcmp(input, "true") ||
     !strcmp(input, "enabled") ||
     !strcmp(input, "on") ||
     !strcmp(input, "yes")
  ) {
    return (uint)true;
  }

  if(!strcmp(input, "false") ||
     !strcmp(input, "disabled") ||
     !strcmp(input, "off") ||
     !strcmp(input, "no")
  ) {
    return (uint)false;
  }

  if(!strbegin(input, "0x")) {
    return strhex(input + 2);
  }

  return strdec(input);
}

char *Config::uint_to_string(uint type, uint input) {
static char output[512];
  switch(type) {
  case Setting::TRUE_FALSE:
    sprintf(output, "%s", (input & 1)?"true":"false");
    break;
  case Setting::ENABLED_DISABLED:
    sprintf(output, "%s", (input & 1)?"enabled":"disabled");
    break;
  case Setting::ON_OFF:
    sprintf(output, "%s", (input & 1)?"on":"off");
    break;
  case Setting::YES_NO:
    sprintf(output, "%s", (input & 1)?"yes":"no");
    break;
  case Setting::DEC:
    sprintf(output, "%d", input);
    break;
  case Setting::HEX:
    sprintf(output, "0x%x", input);
    break;
  }

  return output;
}

bool Config::load(char *fn) {
FILE *fp;
  fp = fopen(fn, "rb");
  if(!fp)return false;

//load the config file into memory
  fseek(fp, 0, SEEK_END);
int fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);
char *buffer = (char*)malloc(fsize + 1);
  fread(buffer, 1, fsize, fp);
  fclose(fp);
  *(buffer + fsize) = 0;
  strcpy(data, buffer);
  free(buffer);

//split the file into lines
  replace(data, "\r\n", "\n");
  qreplace(data, "\t", "");
  qreplace(data, " ", "");
  split(line, "\n", data);

  for(int i=0;i<count(line);i++) {
    if(strlen(line[i]) == 0)continue;
    if(*strptr(line[i]) == '#')continue;

    split(part, "=", line[i]);
    for(int l=0;l<list_count;l++) {
      if(!strcmp(list[l]->name, part[0])) {
        list[l]->set(string_to_uint(list[l]->type, strptr(part[1])));
      }
    }
  }

  return true;
}
bool Config::load(substring &fn) { return load(strptr(fn)); }

bool Config::save(char *fn) {
FILE *fp;
  fp = fopen(fn, "wb");
  if(!fp)return false;

  for(int i=0;i<list_count;i++) {
    strcpy(data, list[i]->desc);
    replace(data, "\r\n", "\n");
    split(line, "\n", data);
    for(int l=0;l<count(line);l++) {
      fprintf(fp, "# %s\r\n", strptr(line[l]));
    }
    fprintf(fp, "# (default = %s)\r\n", uint_to_string(list[i]->type, list[i]->def));
    fprintf(fp, "%s = %s\r\n\r\n", list[i]->name, uint_to_string(list[i]->type, list[i]->data));
  }

  return true;
}
bool Config::save(substring &fn) { return save(strptr(fn)); }

Config::Config() {
  list_count = 0;
}
