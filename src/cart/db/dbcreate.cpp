#include "../../lib/libbase.h"
#include "../../lib/libvector.h"
#include "../../lib/libstring.h"

#include "../../lib/libstring.cpp"

#include "db.h"

FILE *fp;

uint decode_size(substring &str) {
//hex encoding
  if(strbegin(str, "0x")) {
    strltrim(str, "0x");
    return strhex(str);
  }

//mbit encoding
  if(strend(str, "mbit")) {
    strrtrim(str, "mbit");
    return strdec(str) * 1024 * 1024 / 8;
  }

//kbit encoding
  if(strend(str, "kbit")) {
    strrtrim(str, "kbit");
    return strdec(str) * 1024 / 8;
  }

//decimal encoding
  return strdec(str);
}

void build_block(substring &block) {
string line, hashpart, part;
  split(line, "\n", block);

  if(strbegin(line[0], "[") == false) {
    printf("error: invalid block detected: '%s'\n", strptr(line[0]));
    return;
  }

  strltrim(line[0], "[");
  strrtrim(line[0], "]");
  replace(line[0], "0x", "");
  split(hashpart, ",", line[0]);

db_item dbi;
  dbi.crc32 = 0;
 *dbi.name  = 0;
 *dbi.pcb   = 0;
  dbi.rom   = 0;
  dbi.ram   = 0;

  for(int i = 1; i < count(line); i++) {
  uint pos;
    if(strpos(line[i], ";", pos) == true) {
      strset(line[i], pos, 0);
    }

    if(strmatch(line[i], ""))continue;

    split(part, "=", line[i]);
    strunquote(part[1]);

    if(strmatch(part[0], "name")) {
      strncpy(dbi.name, strptr(part[1]), 128);
      dbi.name[128] = 0;
    }

    if(strmatch(part[0], "pcb")) {
      strncpy(dbi.pcb, strptr(part[1]), 32);
      dbi.pcb[31] = 0;
    }

    if(strmatch(part[0], "rom")) {
      dbi.rom = decode_size(part[1]);
    }

    if(strmatch(part[0], "ram")) {
      dbi.ram = decode_size(part[1]);
    }
  }

  for(int i = 0; i < count(hashpart); i++) {
    dbi.crc32 = strhex(hashpart[i]);
    db_write(fp, dbi);
  }
}

void build_database() {
string data, block;
  if(strfread(data, "cartdb.txt") == false)return;

  fp = fopen("cart.db", "wb");
  fprintf(fp, "SNESDB");

uint blocksize = 4 + 128 + 32 + 4 + 4;
  fputc(blocksize >> 0, fp);
  fputc(blocksize >> 8, fp);

  replace (data, "\r", "");
  qreplace(data, " ",  "");
  qreplace(data, "\t", "");
  split(block, "\n\n", data);

  for(int i = 0; i < count(block); i++) {
    build_block(block[i]);
  }

  fclose(fp);
}

int main() {
  build_database();
  return 0;
}
