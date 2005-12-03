/*
  libconfig : version 0.05 ~byuu (10/30/05)
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
  inline __name &operator=(const bool   _data) { set((uint)_data); return *this; } \
  inline __name &operator=(const uint   _data) { set(_data); return *this; } \
  inline __name &operator=(const uint8  _data) { set(_data); return *this; } \
  inline __name &operator=(const uint16 _data) { set(_data); return *this; } \
  inline __name &operator=(const uint32 _data) { set(_data); return *this; } \
  inline __name &operator=(const int    _data) { set(_data); return *this; } \
  inline __name &operator=(const int8   _data) { set(_data); return *this; } \
  inline __name &operator=(const int16  _data) { set(_data); return *this; } \
  inline __name &operator=(const int32  _data) { set(_data); return *this; } \
  inline __name &operator=(const float  _data) { set((uint)_data); return *this; } \
  inline __name &operator=(const double _data) { set((uint)_data); return *this; } \
  void toggle() { data ^= 1; set(data); } \
  __name(Config *_parent, char *_name, char *_desc = 0, uint _data = 0, uint _type = Setting::DEC) : \
  Setting(_parent, _name, _desc, _data, _type) {}

class Setting {
  friend class Config;

protected:
uint data, type, def;

public:
enum {
  TRUE_FALSE,
  ENABLED_DISABLED,
  ON_OFF,
  YES_NO,
  BOOL,
  DEC,
  HEX,
  STR
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

  inline operator bool()   { return (bool)get(); }
  inline operator uint()   { return get(); }
  inline operator uint8()  { return get(); }
  inline operator uint16() { return get(); }
  inline operator uint32() { return get(); }
  inline operator int()    { return get(); }
  inline operator int8()   { return get(); }
  inline operator int16()  { return get(); }
  inline operator int32()  { return get(); }
  inline operator float()  { return (float) get(); }
  inline operator double() { return (double)get(); }

  inline Setting &operator=(const bool   _data) { set((uint)_data); return *this; }
  inline Setting &operator=(const uint   _data) { set(_data); return *this; }
  inline Setting &operator=(const uint8  _data) { set(_data); return *this; }
  inline Setting &operator=(const uint16 _data) { set(_data); return *this; }
  inline Setting &operator=(const uint32 _data) { set(_data); return *this; }
  inline Setting &operator=(const int    _data) { set(_data); return *this; }
  inline Setting &operator=(const int8   _data) { set(_data); return *this; }
  inline Setting &operator=(const int16  _data) { set(_data); return *this; }
  inline Setting &operator=(const int32  _data) { set(_data); return *this; }
  inline Setting &operator=(const float  _data) { set((uint)_data); return *this; }
  inline Setting &operator=(const double _data) { set((uint)_data); return *this; }
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
