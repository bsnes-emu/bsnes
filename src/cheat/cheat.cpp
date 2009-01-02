#include <../base.hpp>

Cheat cheat;

Cheat::cheat_t& Cheat::cheat_t::operator=(const Cheat::cheat_t& source) {
  enabled = source.enabled;
  addr    = source.addr;
  data    = source.data;
  code    = source.code;
  desc    = source.desc;
  return *this;
}

//used to sort cheat code list by description
bool Cheat::cheat_t::operator<(const Cheat::cheat_t& source) {
  return strcmp(desc, source.desc) < 0;
}

/*****
 * string <> binary code translation routines
 * decode() "7e1234:56" ->  0x7e123456
 * encode()  0x7e123456 -> "7e1234:56"
 *****/

bool Cheat::decode(const char *str, unsigned &addr, uint8 &data, type_t &type) {
  string t = str;
  strlower(t);

  #define ischr(n) ((n >= '0' && n <= '9') || (n >= 'a' && n <= 'f'))

  if(strlen(t) == 8 || (strlen(t) == 9 && t[6] == ':')) {
    //strip ':'
    if(strlen(t) == 9 && t[6] == ':') t = string() << substr(t, 0, 6) << substr(t, 7);
    //validate input
    for(unsigned i = 0; i < 8; i++) if(!ischr(t[i])) return false;

    type = ProActionReplay;
    unsigned r = strhex((const char*)t);
    addr = r >> 8;
    data = r & 0xff;
    return true;
  } else if(strlen(t) == 9 && t[4] == '-') {
    //strip '-'
    t = string() << substr(t, 0, 4) << substr(t, 5);
    //validate input
    for(unsigned i = 0; i < 8; i++) if(!ischr(t[i])) return false;

    type = GameGenie;
    strtr(t, "df4709156bc8a23e", "0123456789abcdef");
    unsigned r = strhex((const char*)t);
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
  } else {
    return false;
  }
}

bool Cheat::encode(string &str, unsigned addr, uint8 data, type_t type) {
  char t[16];

  if(type == ProActionReplay) {
    sprintf(t, "%.6x:%.2x", addr, data);
    str = t;
    return true;
  } else if(type == GameGenie) {
    unsigned r = addr;
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
    sprintf(t, "%.2x%.2x-%.4x", data, addr >> 16, addr & 0xffff);
    strtr(t, "0123456789abcdef", "df4709156bc8a23e");
    str = t;
    return true;
  } else {
    return false;
  }
}

/*****
 * address lookup table manipulation and mirroring
 * mirror_address() 0x000000 -> 0x7e0000
 * set() enable specified address, mirror accordingly
 * clear() disable specified address, mirror accordingly
 *****/

unsigned Cheat::mirror_address(unsigned addr) const {
  if((addr & 0x40e000) != 0x0000) return addr;
  //8k WRAM mirror
  //$[00-3f|80-bf]:[0000-1fff] -> $7e:[0000-1fff]
  return (0x7e0000 + (addr & 0x1fff));
}

void Cheat::set(unsigned addr) {
  addr = mirror_address(addr);

  mask[addr >> 3] |= 1 << (addr & 7);
  if((addr & 0xffe000) == 0x7e0000) {
    //mirror $7e:[0000-1fff] to $[00-3f|80-bf]:[0000-1fff]
    unsigned mirror;
    for(unsigned x = 0; x <= 0x3f; x++) {
      mirror = ((0x00 + x) << 16) + (addr & 0x1fff);
      mask[mirror >> 3] |= 1 << (mirror & 7);
      mirror = ((0x80 + x) << 16) + (addr & 0x1fff);
      mask[mirror >> 3] |= 1 << (mirror & 7);
    }
  }
}

void Cheat::clear(unsigned addr) {
  addr = mirror_address(addr);

  //if there is more than one cheat code using the same address,
  //(eg with a different override value) then do not clear code
  //lookup table entry.
  uint8 r;
  if(read(addr, r) == true) return;

  mask[addr >> 3] &= ~(1 << (addr & 7));
  if((addr & 0xffe000) == 0x7e0000) {
    //mirror $7e:[0000-1fff] to $[00-3f|80-bf]:[0000-1fff]
    unsigned mirror;
    for(unsigned x = 0; x <= 0x3f; x++) {
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

bool Cheat::read(unsigned addr, uint8 &data) const {
  addr = mirror_address(addr);
  for(unsigned i = 0; i < code.size(); i++) {
    if(enabled(i) == false) continue;
    if(addr == mirror_address(code[i].addr)) {
      data = code[i].data;
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
  for(unsigned i = 0; i < code.size(); i++) {
    if(code[i].enabled) {
      cheat_system_enabled = true;
      return;
    }
  }
  cheat_system_enabled = false;
}

/*****
 * cheat list manipulation routines
 *****/

bool Cheat::add(bool enable, const char *code_, const char *desc_) {
  unsigned addr;
  uint8    data;
  type_t   type;
  if(decode(code_, addr, data, type) == false) return false;

  unsigned n = code.size();
  code[n].enabled = enable;
  code[n].addr = addr;
  code[n].data = data;
  code[n].code = code_;
  code[n].desc = desc_;
  (enable) ? set(addr) : clear(addr);

  update_cheat_status();
  return true;
}

bool Cheat::edit(unsigned n, bool enable, const char *code_, const char *desc_) {
  unsigned addr;
  uint8    data;
  type_t   type;
  if(decode(code_, addr, data, type) == false) return false;

  //disable current code and clear from code lookup table
  code[n].enabled = false;
  clear(code[n].addr);

  //update code and enable in code lookup table
  code[n].enabled = enable;
  code[n].addr = addr;
  code[n].data = data;
  code[n].code = code_;
  code[n].desc = desc_;
  set(addr);

  update_cheat_status();
  return true;
}

bool Cheat::remove(unsigned n) {
  unsigned size = code.size();
  if(n >= size) return false; //also verifies size cannot be < 1

  for(unsigned i = n; i < size - 1; i++) code[i] = code[i + 1];
  code.resize(size - 1);

  update_cheat_status();
  return true;
}

bool Cheat::get(unsigned n, cheat_t &cheat) const {
  if(n >= code.size()) return false;

  cheat = code[n];
  return true;
}

/*****
 * code status modifier routines
 *****/

bool Cheat::enabled(unsigned n) const {
  return (n < code.size()) ? code[n].enabled : false;
}

void Cheat::enable(unsigned n) {
  if(n >= code.size()) return;

  code[n].enabled = true;
  set(code[n].addr);
  update_cheat_status();
}

void Cheat::disable(unsigned n) {
  if(n >= code.size()) return;

  code[n].enabled = false;
  clear(code[n].addr);
  update_cheat_status();
}

/*****
 * cheat file manipulation routines
 *****/

/* file format: */
/* nnnn-nnnn = status, "description" \r\n */
/* ... */

bool Cheat::load(const char *fn) {
  string data;
  if(!fread(data, fn)) return false;
  replace(data, "\r\n", "\n");
  qreplace(data, "=", ",");
  qreplace(data, " ", "");

  lstring line;
  split(line, "\n", data);
  for(unsigned i = 0; i < ::count(line); i++) {
    lstring part;
    split(part, ",", line[i]);
    if(::count(part) != 3) continue;
    trim(part[2], "\"");
    add(part[1] == "enabled", part[0], part[2]);
  }

  return true;
}

bool Cheat::save(const char *fn) const {
  file fp;
  if(!fp.open(fn, file::mode_write)) return false;
  for(unsigned i = 0; i < code.size(); i++) {
    fp.print(string()
      << code[i].code << " = "
      << (code[i].enabled ? "enabled" : "disabled") << ", "
      << "\"" << code[i].desc << "\""
      << "\r\n");
  }
  fp.close();
  return true;
}

void Cheat::sort() {
  if(code.size() <= 1) return;  //nothing to sort?
  cheat_t *buffer = new cheat_t[code.size()];
  for(unsigned i = 0; i < code.size(); i++) buffer[i] = code[i];
  nall::sort(buffer, code.size());
  for(unsigned i = 0; i < code.size(); i++) code[i] = buffer[i];
  delete[] buffer;
}

void Cheat::clear() {
  cheat_system_enabled = false;
  memset(mask, 0, 0x200000);
  code.reset();
}

Cheat::Cheat() {
  clear();
}
