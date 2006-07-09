/*
  libconfig : version 0.08 ~byuu (07/02/06)
*/

#ifndef __LIBCONFIG
#define __LIBCONFIG

#include "libstring.h"

class Config;

//operator= is the /only/ overloaded operator that isn't inherited by derived classes.
//similarly, base constructor/destructors with arguments are not inherited.
//the inclusion of the overloaded virtual function 'toggle' is to allow toggle to call
//the overloaded set() function, if it exists. for some reason, Setting::toggle() calls
//Setting::set() no matter what, even if the derived class defines set()...
//the below macro is a quick way to take care of all of these issues.
//usage example:
//  class T : public Setting { public: SettingOperators(T); } t;
//  t = 0; // -> t.set(0);
#define SettingOperators(__name) \
  template<typename T> inline __name &operator=(const T x) { set(T(x)); return *this; } \
  void toggle() { data ^= 1; set(data); } \
  __name(Config *_parent, char *_name, char *_desc, uint  _data, uint _type) : \
  Setting(_parent, _name, _desc, _data, _type) {} \
  __name(Config *_parent, char *_name, char *_desc, char *_data) : \
  Setting(_parent, _name, _desc, _data) {}

class Setting {
  friend class Config;

protected:
uint data, type, def;

public:
enum {
  BOOL             =  0,
  BOOLEAN          =  0,
  TRUE_FALSE       =  0,
  ENABLED_DISABLED =  1,
  ON_OFF           =  2,
  YES_NO           =  3,
  DEC              =  4,
  HEX              =  5,
  HEX8             =  6,
  HEX16            =  7,
  HEX24            =  8,
  HEX32            =  9,
  STR              = 10,
};
char *name, *desc;
substring char_data, char_def;
  virtual void  toggle();
  virtual uint  get();
  virtual void  set(uint _data);

  virtual char *sget();
  virtual void  sset(const char *_data);

  Setting(Config *_parent, char *_name, char *_desc, uint  _data, uint _type);
  Setting(Config *_parent, char *_name, char *_desc, char *_data);

  template<typename T> inline operator T() { return T(get()); }
  template<typename T> inline Setting &operator=(const T x) { set(T(x)); return *this; }
  template<typename T> inline bool operator==(const T x) { return T(get()) == x; }
  template<typename T> inline bool operator!=(const T x) { return T(get()) != x; }
  template<typename T> inline bool operator>=(const T x) { return T(get()) >= x; }
  template<typename T> inline bool operator> (const T x) { return T(get()) >  x; }
  template<typename T> inline bool operator<=(const T x) { return T(get()) <= x; }
  template<typename T> inline bool operator< (const T x) { return T(get()) <  x; }
};

class Config {
protected:
vector<Setting*> list;
uint list_count;

string data, line, part, subpart;

uint  string_to_uint(uint type, char *input);
char *uint_to_string(uint type, uint  input);

public:
  void add(Setting *setting);
  bool load(char *fn);
  bool load(substring &fn);
  bool save(char *fn);
  bool save(substring &fn);
  Config();
};

#endif
