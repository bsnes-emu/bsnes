#include "../base.h"

bool Cheat::decode(char *str, uint32 &addr, uint8 &data, uint8 &type) {
string t, part;
  strcpy(t, str);
  strlower(t);
  if(strlen(t) == 8) {
    type = CT_PRO_ACTION_REPLAY;
  uint32 r = strhex(t);
    addr = r >> 8;
    data = r & 0xff;
    return true;
  } else if(strlen(t) == 9 && strptr(t)[4] == '-') {
    type = CT_GAME_GENIE;
    replace(t, "-", "");
    strtr(t, "df4709156bc8a23e", "0123456789abcdef");
  uint32 r = strhex(t);
  //8421 8421 8421 8421 8421 8421
  //abcd efgh ijkl mnop qrst uvwx
  //ijkl qrst opab cduv wxef ghmn
    addr = (!!(r & 0x002000) << 23) | (!!(r & 0x001000) << 22) |
           (!!(r & 0x000800) << 21) | (!!(r & 0x000400) << 20) |
           (!!(r & 0x000020) << 19) | (!!(r & 0x000010) << 18) |
           (!!(r & 0x000008) << 17) | (!!(r & 0x000004) << 16) |
           (!!(r & 0x800000) << 15) | (!!(r & 0x400000) << 14) |
           (!!(r & 0x200000) << 13) | (!!(r & 0x100000) << 12) |
           (!!(r & 0x000002) << 11) | (!!(r & 0x000001) << 10) |
           (!!(r & 0x008000) <<  9) | (!!(r & 0x004000) <<  8) |
           (!!(r & 0x080000) <<  7) | (!!(r & 0x040000) <<  6) |
           (!!(r & 0x020000) <<  5) | (!!(r & 0x010000) <<  4) |
           (!!(r & 0x000200) <<  3) | (!!(r & 0x000100) <<  2) |
           (!!(r & 0x000080) <<  1) | (!!(r & 0x000040) <<  0);
    data = r >> 24;
    return true;
  }
  return false;
}

bool Cheat::encode(char *str, uint32 addr, uint8 data, uint8 type) {
  if(type == CT_PRO_ACTION_REPLAY) {
    sprintf(str, "%0.6x%0.2x", addr, data);
    return true;
  } else if(type == CT_GAME_GENIE) {
  uint32 r = addr;
    addr = (!!(r & 0x008000) << 23) | (!!(r & 0x004000) << 22) |
           (!!(r & 0x002000) << 21) | (!!(r & 0x001000) << 20) |
           (!!(r & 0x000080) << 19) | (!!(r & 0x000040) << 18) |
           (!!(r & 0x000020) << 17) | (!!(r & 0x000010) << 16) |
           (!!(r & 0x000200) << 15) | (!!(r & 0x000100) << 14) |
           (!!(r & 0x800000) << 13) | (!!(r & 0x400000) << 12) |
           (!!(r & 0x200000) << 11) | (!!(r & 0x100000) << 10) |
           (!!(r & 0x000008) <<  9) | (!!(r & 0x000004) <<  8) |
           (!!(r & 0x000002) <<  7) | (!!(r & 0x000001) <<  6) |
           (!!(r & 0x080000) <<  5) | (!!(r & 0x040000) <<  4) |
           (!!(r & 0x020000) <<  3) | (!!(r & 0x010000) <<  2) |
           (!!(r & 0x000800) <<  1) | (!!(r & 0x000400) <<  0);
    sprintf(str, "%0.2x%0.2x-%0.4x", data, addr >> 16, addr & 0xffff);
    strtr(str, "0123456789abcdef", "df4709156bc8a23e");
    return true;
  }
  return false;
}

//enable cheat system when at least one cheat is enabled,
//disable otherwise to speed up emulation
void Cheat::update_cheat_status() {
  for(int i = 0; i < cheat_count; i++) {
    if(index[i].enabled) {
      cheat_enabled = true;
      return;
    }
  }
  cheat_enabled = false;
}

uint8 Cheat::read(uint32 addr) {
  addr &= 0xffffff;
  for(int i = 0; i < cheat_count; i++) {
    if(index[i].addr == addr) {
      return index[i].data;
    }
  }

//cheat not found? disable and return 0
  clear(addr);
  return 0x00;
}

bool Cheat::add(bool enable, char *code, char *desc) {
  if(cheat_count >= CHEAT_LIMIT)return false;

uint32 addr, len;
uint8  data, type;
  if(decode(code, addr, data, type) == false)return false;

  (enable) ? set(addr) : clear(addr);
  index[cheat_count].enabled = enable;
  index[cheat_count].addr = addr;
  index[cheat_count].data = data;
  len = strlen(code);
  len = len > 16 ? 16 : len;
  memcpy(index[cheat_count].code, code, len);
  index[cheat_count].code[len] = 0;
  len = strlen(desc);
  len = len > 128 ? 128 : len;
  memcpy(index[cheat_count].desc, desc, len);
  index[cheat_count].desc[len] = 0;
  cheat_count++;

  update_cheat_status();
  return true;
}

bool Cheat::edit(uint32 n, bool enable, char *code, char *desc) {
  if(n >= cheat_count)return false;

uint32 addr, len;
uint8  data, type;
  if(decode(code, addr, data, type) == false)return false;

  clear(index[n].addr);
  set(addr);
  index[n].enabled = enable;
  index[n].addr = addr;
  index[n].data = data;
  len = strlen(code);
  len = len > 16 ? 16 : len;
  memcpy(index[n].code, code, len);
  index[n].code[len] = 0;
  len = strlen(desc);
  len = len > 128 ? 128 : len;
  memcpy(index[n].desc, desc, len);
  index[n].desc[len] = 0;

  update_cheat_status();
  return true;
}

bool Cheat::remove(uint32 n) {
  if(n >= cheat_count)return false;

  for(int i = n; i < cheat_count; i++) {
    index[i].enabled = index[i + 1].enabled;
    index[i].addr    = index[i + 1].addr;
    index[i].data    = index[i + 1].data;
    strcpy(index[i].desc, index[i + 1].desc);
  }

  cheat_count--;

  update_cheat_status();
  return true;
}

bool Cheat::get(uint32 n, bool &enable, uint32 &addr, uint8 &data, char *code, char *desc) {
  if(n >= cheat_count)return false;
  enable = index[n].enabled;
  addr   = index[n].addr;
  data   = index[n].data;
  strcpy(code, index[n].code);
  strcpy(desc, index[n].desc);
  return true;
}

bool Cheat::enabled(uint32 n) {
  if(n >= cheat_count)return false;
  return index[n].enabled;
}

void Cheat::enable(uint32 n) {
  if(n >= cheat_count)return;
  set(index[n].addr);
  index[n].enabled = true;
  update_cheat_status();
}

void Cheat::disable(uint32 n) {
  if(n >= cheat_count)return;
  clear(index[n].addr);
  index[n].enabled = false;
  update_cheat_status();
}

bool Cheat::load(Reader *rf) {
  if(!rf->ready())return false;

uint8 *raw_data = rf->read();
string data;
  raw_data[rf->size()] = 0;
  strcpy(data, (char*)raw_data);
  SafeFree(raw_data);
  replace(data, "\r\n", "\n");
string line;
  split(line, "\n", data);
  for(int i = 0; i < ::count(line); i++) {
  string part;
  uint8  en = *(strptr(line[i]));
    if(en == '+') {
      strltrim(line[i], "+");
    } else if(en == '-') {
      strltrim(line[i], "-");
    } else {
      continue;
    }
    qreplace(line[i], " ", "");
    qsplit(part, ",", line[i]);
    if(::count(part) != 2)continue;
    strunquote(part[1]);
    add(en == '+', strptr(part[0]), strptr(part[1]));
  }

  return true;
}

bool Cheat::save(Writer *wf) {
  if(!wf->ready())return false;

string data;
char   t[4096];
  strcpy(data, "");
  for(int i = 0; i < cheat_count; i++) {
    sprintf(t, "%c%s, \"%s\"\r\n", index[i].enabled ? '+' : '-', index[i].code, index[i].desc);
    strcat(data, t);
  }

  wf->write((uint8*)strptr(data), strlen(data));
  return true;
}

void Cheat::clear() {
  cheat_enabled = false;
  cheat_count   = 0;
  memset(mask, 0, 0x200000);
  for(int i = 0; i < CHEAT_LIMIT + 1; i++) {
    index[i].enabled = false;
    index[i].addr    = 0x000000;
    index[i].data    = 0x00;
    strcpy(index[i].code, "");
    strcpy(index[i].desc, "");
  }
}

Cheat::Cheat() {
  clear();
}
