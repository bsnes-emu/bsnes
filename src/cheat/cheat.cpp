#include "../base.h"

/*****
 * string <> binary code translation routines
 * decode() "7e1234:56" ->  0x7e123456
 * encode()  0x7e123456 -> "7e1234:56"
 *****/

bool Cheat::decode(char *str, uint32 &addr, uint8 &data, uint8 &type) {
string t, part;
  strcpy(t, str);
  strlower(t);
  if(strlen(t) == 8 || (strlen(t) == 9 && strptr(t)[6] == ':')) {
    type = CT_PRO_ACTION_REPLAY;
    replace(t, ":", "");
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
    sprintf(str, "%0.6x:%0.2x", addr, data);
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

/*****
 * address lookup table manipulation and mirroring
 * mirror_address() 0x000000 -> 0x7e0000
 * set() enable specified address, mirror accordingly
 * clear() disable specified address, mirror accordingly
 *****/

uint Cheat::mirror_address(uint addr) {
  if((addr & 0x40e000) != 0x0000)return addr;
//8k WRAM mirror
//$[00-3f|80-bf]:[0000-1fff] -> $7e:[0000-1fff]
  return (0x7e0000 + (addr & 0x1fff));
}

void Cheat::set(uint32 addr) {
  addr = mirror_address(addr);

  mask[addr >> 3] |= 1 << (addr & 7);
  if((addr & 0xffe000) == 0x7e0000) {
  //mirror $7e:[0000-1fff] to $[00-3f|80-bf]:[0000-1fff]
  uint mirror;
    for(int x = 0; x <= 0x3f; x++) {
      mirror = ((0x00 + x) << 16) + (addr & 0x1fff);
      mask[mirror >> 3] |= 1 << (mirror & 7);
      mirror = ((0x80 + x) << 16) + (addr & 0x1fff);
      mask[mirror >> 3] |= 1 << (mirror & 7);
    }
  }
}

void Cheat::clear(uint32 addr) {
  addr = mirror_address(addr);

//is there more than one cheat code using the same address
//(and likely a different override value) that is enabled?
//if so, do not clear code lookup table entry for this address.
uint8 r;
  if(read(addr, r) == true)return;

  mask[addr >> 3] &= ~(1 << (addr & 7));
  if((addr & 0xffe000) == 0x7e0000) {
  //mirror $7e:[0000-1fff] to $[00-3f|80-bf]:[0000-1fff]
  uint mirror;
    for(int x = 0; x <= 0x3f; x++) {
      mirror = ((0x00 + x) << 16) + (addr & 0x1fff);
      mask[mirror >> 3] &= ~(1 << (mirror & 7));
      mirror = ((0x80 + x) << 16) + (addr & 0x1fff);
      mask[mirror >> 3] &= ~(1 << (mirror & 7));
    }
  }
}

/*****
 * read() is used by MemBus::read() if Cheat::enabled(addr)
 * returns true to look up cheat code.
 * returns true if cheat code was found, false if it was not.
 * when true, cheat code substitution value is stored in data.
 *****/

bool Cheat::read(uint32 addr, uint8 &data) {
  addr = mirror_address(addr);
  for(int i = 0; i < cheat_count; i++) {
    if(enabled(i) == false)continue;
    if(addr == mirror_address(index[i].addr)) {
      data = index[i].data;
      return true;
    }
  }
//code not found, or code is disabled
  return false;
}

/*****
 * update_cheat_status() will scan to see if any codes are
 * enabled. if any are, make sure the cheat system is on.
 * otherwise, turn cheat system off to speed up emulation.
 *****/
void Cheat::update_cheat_status() {
  for(int i = 0; i < cheat_count; i++) {
    if(index[i].enabled) {
      cheat_enabled = true;
      return;
    }
  }
  cheat_enabled = false;
}

/*****
 * cheat list manipulation routines
 *****/

bool Cheat::add(bool enable, char *code, char *desc) {
  if(cheat_count >= CHEAT_LIMIT)return false;

uint32 addr, len;
uint8  data, type;
  if(decode(code, addr, data, type) == false)return false;

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
  (enable) ? set(addr) : clear(addr);

  update_cheat_status();
  return true;
}

bool Cheat::edit(uint32 n, bool enable, char *code, char *desc) {
  if(n >= cheat_count)return false;

uint32 addr, len;
uint8  data, type;
  if(decode(code, addr, data, type) == false)return false;

//disable current code and clear from code lookup table
  index[n].enabled = false;
  clear(index[n].addr);

//update code and enable in code lookup table
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
  set(addr);

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

/*****
 * code status modifier routines
 *****/

bool Cheat::enabled(uint32 n) {
  if(n >= cheat_count)return false;
  return index[n].enabled;
}

void Cheat::enable(uint32 n) {
  if(n >= cheat_count)return;
  index[n].enabled = true;
  set(index[n].addr);
  update_cheat_status();
}

void Cheat::disable(uint32 n) {
  if(n >= cheat_count)return;
  index[n].enabled = false;
  clear(index[n].addr);
  update_cheat_status();
}

/*****
 * cheat file manipulation routines
 *****/

bool Cheat::load(Reader &rf) {
  if(!rf.ready())return false;

uint8 *raw_data = rf.read();
string data;
  raw_data[rf.size()] = 0;
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

bool Cheat::save(Writer &wf) {
  if(!wf.ready())return false;

string data;
char   t[4096];
  strcpy(data, "");
  for(int i = 0; i < cheat_count; i++) {
    sprintf(t, "%c%s, \"%s\"\r\n", index[i].enabled ? '+' : '-', index[i].code, index[i].desc);
    strcat(data, t);
  }

  wf.write((uint8*)strptr(data), strlen(data));
  return true;
}

/*****
 * initialization routines
 *****/

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
