#include "libbase.h"
#include "libconfig.h"

bool config_item::changed() {
  return (is_string == true) ? (*strsource != strdef) : (*source != def);
}

config_item::config_item() {
  is_string = false;
  source    = 0;
  strsource = 0;
  def       = 0;
  strdef    = "";
  type      = 0;
  name      = "";
}

void config::add(uint32 *variable, char *name, uint32 def, uint32 type) {
int n;
  if(item_count >= 4096)return;
  n = item_count;

  item[n] = new config_item();

  item[n]->is_string = false;
  item[n]->name      = name;
  item[n]->source    = variable;
  item[n]->def       = def;
  item[n]->type      = type;
  *item[n]->source   = item[n]->def;

  item_count++;
}

void config::add(string *variable, char *name, char *def, uint32 type) {
int n;
  if(item_count >= 4096)return;
  n = item_count;

  item[n] = new config_item();
  item[n]->is_string  = true;
  item[n]->name       = name;
  item[n]->strsource  = variable;
  item[n]->strdef     = def;
  *item[n]->strsource = item[n]->strdef;

  item_count++;
}

uint32 config::find(char *name) {
  for(int i=0;i<item_count;i++) {
    if(item[i]->name == name) {
      return i;
    }
  }
  return null;
}

uint32 config::get(char *name) {
int i = find(name);
  if(i == null)return 0;
  return *item[i]->source;
}

string &config::strget(char *name) {
int i = find(name);
  if(i == null) {
  static string not_found;
    not_found = "";
    return not_found;
  }
  return *item[i]->strsource;
}

void config::set(char *name, uint32 value) {
int i = find(name);
  if(i == null)return;
  *item[i]->source = value;
}

void config::set(char *name, char *value) {
int i = find(name);
  if(i == null)return;
  *item[i]->strsource = value;
}

void config::load(char *fn) {
FILE *fp;
char *buffer;
int i, fsize;
uint32 l;
  fp = fopen(fn, "rb");
/* file doesn't exist yet, do nothing */
  if(!fp)return;

  fseek(fp, 0, SEEK_END);
  fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

/* blank file, do nothing */
  if(fsize == 0) {
    fclose(fp);
    return;
  }

  buffer = (char*)malloc(fsize + 1);
  fread(buffer, 1, fsize, fp);
  fclose(fp);
  buffer[fsize] = 0;

  data = buffer;
  free(buffer);
  replace(data, "\r\n", "\n");
  qreplace(data, "\t", " ");

  split(line, "\n", data);

  for(i=0;i<count(line);i++) {
    qreplace(line[i], " ", "");

  /* remove comment, if it exists */
    if(strqpos(line[i], "#") != null) {
      strset(line[i], strqpos(line[i], "#"), 0);
    }

  /* ignore blank lines */
    if(strlen(line[i]) == 0)continue;

    qsplit(part, "=", line[i]);

    l = find(*part[0]);
    if(l != null) {
    /* if the config item name is valid... update item value */
      if(item[l]->is_string == true) {
        strunquote(part[1]);
        *item[l]->strsource = part[1];
      } else {
        if(part[1] == "true" || part[1] == "yes" || part[1] == "on" || part[1] == "enabled") {
          *item[l]->source = 1;
        } else if(part[1] == "false" || part[1] == "no" || part[1] == "off" || part[1] == "disabled") {
          *item[l]->source = 0;
        } else if(item[l]->type == HEX) {
          *item[l]->source = strhex(part[1]);
        } else { /* fall back on DEC */
          *item[l]->source = strdec(part[1]);
        }
      }
    }
  }
}

/* create a text string from config item[i] to be output via config->save() */
void config::set_newline(int i) {
char t[16];
  if(item[i]->is_string == true) {
    newline  = item[i]->name;
    newline += " = \"";
    newline += *item[i]->strsource;
    newline += "\"";
  } else {
    newline  = item[i]->name;
    newline += " = ";
    switch(item[i]->type) {
    case TRUEFALSE:
      newline += (*item[i]->source)?"true":"false";
      break;
    case YESNO:
      newline += (*item[i]->source)?"yes":"no";
      break;
    case ONOFF:
      newline += (*item[i]->source)?"on":"off";
      break;
    case ENABLED:
      newline += (*item[i]->source)?"enabled":"disabled";
      break;
    case HEX:
      sprintf(t, "%x", *item[i]->source);
      newline += t;
      break;
    case DEC:
    default:
      sprintf(t, "%d", *item[i]->source);
      newline += t;
      break;
    }
  }
}

void config::save(char *fn) {
FILE *fp;
int i, fsize;
uint32 l;
char *buffer;
uint8 set[4096];
bool blank = false;
  fp = fopen(fn, "rb");
  if(!fp) {
    blank = true;
  } else {
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if(fsize == 0) {
      blank = true;
    } else {
      buffer = (char*)malloc(fsize + 1);
      fread(buffer, 1, fsize, fp);
      fclose(fp);
      buffer[fsize] = 0;

      data = buffer;
      free(buffer);
    }
  }

  fp = fopen(fn, "wb");
/* no write access? */
  if(!fp)return;

/* list of config items. if the item is set in the
   existing config file, then don't test it to see
   if it needs to be written again later on */
  memset(set, 0, item_count);

  if(blank == false) {
    replace(data, "\r\n", "\n");
    qreplace(data, "\t", " ");

    split(line,    "\n", data);
    split(oldline, "\n", data);

    for(i=0;i<count(line);i++) {
      qreplace(line[i], " ", "");

      if(strqpos(line[i], "#") != null) {
        strset(line[i], strqpos(line[i], "#"), 0);
      }

    /* this line is empty, restore the old line and continue */
      if(strlen(line[i]) == 0) {
        line[i] = oldline[i];
        continue;
      }

      qsplit(part, "=", line[i]);

      l = find(*part[0]);
      if(l == null) {
      /* invalid item name, restore the old line and continue */
        line[i] = oldline[i];
        continue;
      }

      set[l] = 1;
      set_newline(l);

    /* copy the user comment from the old config file, if it exists */
      if(strqpos(oldline[i], "#") != null) {
        newline += " ";
        newline += *oldline[i] + strqpos(oldline[i], "#");
      }

      line[i] = newline;
    }

  /* write out the old config file + changes first */
    for(i=0;i<count(line);) {
      fprintf(fp, "%s", *line[i]);

    /* write a line feed on all lines but the last.
       keeps the file from growing everytime it is saved */
      if(++i < count(line))fprintf(fp, "\r\n");
    }
  }

int lines_written;
  for(i=lines_written=0;i<item_count;i++) {
  /* if the item was written to the file above... */
    if(set[i] == 1)continue;
  /* ...or if it is still set to the default value,
     then don't output it to the file here */
    if(item[i]->changed() == false)continue;

    set_newline(i);
  /* prevent a newline from appearing at the top of the file
     when the config file is created for the first time */
    if(lines_written == 0 && blank == false)fprintf(fp, "\r\n");
    fprintf(fp, "%s\r\n", *newline);
    lines_written++;
  }

  fclose(fp);
}

config::config() {
  item_count = 0;
}

config::~config() {
  for(int i=0;i<item_count;i++) {
    if(item[i])delete(item[i]);
  }
}
