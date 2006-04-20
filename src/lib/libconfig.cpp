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
  data = _data;

  switch(type) {
  case TRUE_FALSE:
  case ENABLED_DISABLED:
  case ON_OFF:
  case YES_NO:
    data &= 1;
    break;
  case HEX8:
    data &= 0xff;
    break;
  case HEX16:
    data &= 0xffff;
    break;
  case HEX24:
    data &= 0xffffff;
    break;
  }
}

char *Setting::sget() {
  return strptr(char_data);
}

void Setting::sset(const char *_data) {
  strcpy(char_data, _data);
  strunquote(char_data);
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

Setting::Setting(Config *_parent, char *_name, char *_desc, char *_data) {
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

  strcpy(char_data, _data);
  strcpy(char_def,  _data);

  type = STR;
}

void Config::add(Setting *setting) {
  list[list_count++] = setting;
}

uint Config::string_to_uint(uint type, char *input) {
  if(strmatch(input, "true") ||
     strmatch(input, "enabled") ||
     strmatch(input, "on") ||
     strmatch(input, "yes")
  ) {
    return (uint)true;
  }

  if(strmatch(input, "false") ||
     strmatch(input, "disabled") ||
     strmatch(input, "off") ||
     strmatch(input, "no")
  ) {
    return (uint)false;
  }

  if(strbegin(input, "0x") || strbegin(input, "-0x")) {
    return sstrhex(input + 2);
  }

  return sstrdec(input);
}

char *Config::uint_to_string(uint type, uint input) {
static char output[512];
  switch(type) {
  case Setting::TRUE_FALSE:
    sprintf(output, "%s", (input & 1) ? "true" : "false");
    break;
  case Setting::ENABLED_DISABLED:
    sprintf(output, "%s", (input & 1) ? "enabled" : "disabled");
    break;
  case Setting::ON_OFF:
    sprintf(output, "%s", (input & 1) ? "on" : "off");
    break;
  case Setting::YES_NO:
    sprintf(output, "%s", (input & 1) ? "yes" : "no");
    break;
  case Setting::DEC:   sprintf(output, "%d",      input);            break;
  case Setting::HEX:   sprintf(output, "0x%x",    input);            break;
  case Setting::HEX8:  sprintf(output, "0x%0.2x", input & 0xff);     break;
  case Setting::HEX16: sprintf(output, "0x%0.4x", input & 0xffff);   break;
  case Setting::HEX24: sprintf(output, "0x%0.6x", input & 0xffffff); break;
  case Setting::HEX32: sprintf(output, "0x%0.8x", input);            break;
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

  for(int i = 0; i < count(line); i++) {
    if(strlen(line[i]) == 0)continue;
    if(*strptr(line[i]) == '#')continue;

    qsplit(part, "=", line[i]);
    for(int l = 0; l < list_count; l++) {
      if(strmatch(list[l]->name, part[0])) {
        if(list[l]->type != Setting::STR) {
          list[l]->set(string_to_uint(list[l]->type, strptr(part[1])));
        } else {
          list[l]->sset(strptr(part[1]));
        }
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

  for(int i = 0; i < list_count; i++) {
    strcpy(data, list[i]->desc);
    replace(data, "\r\n", "\n");
    split(line, "\n", data);
    for(int l = 0; l < count(line); l++) {
      fprintf(fp, "# %s\r\n", strptr(line[l]));
    }
    if(list[i]->type != Setting::STR) {
      fprintf(fp, "# (default = %s)\r\n", uint_to_string(list[i]->type, list[i]->def));
      fprintf(fp, "%s = %s\r\n\r\n", list[i]->name, uint_to_string(list[i]->type, list[i]->data));
    } else {
      fprintf(fp, "# (default = \"%s\")\r\n", strptr(list[i]->char_def));
      fprintf(fp, "%s = \"%s\"\r\n\r\n", list[i]->name, strptr(list[i]->char_data));
    }
  }

  return true;
}
bool Config::save(substring &fn) { return save(strptr(fn)); }

Config::Config() {
  list_count = 0;
}
