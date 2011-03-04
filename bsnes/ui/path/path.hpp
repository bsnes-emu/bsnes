struct Path : public configuration {
  string user;
  string base;

  string sfc;
  string bs;
  string st;
  string gb;

  string filter;
  string shader;

  string satellaviewBios;
  string sufamiTurboBios;
  string superGameBoyBios;

  string srm;
  string rtc;

  string bsa;
  string bst;
  string cht;
  string log;

  string home(const string &filename);
  string load(const string &path);
  void save(const string &path, const string &value);
  string load(SNES::Cartridge::Slot slot, const string &type, const string &suffix = "");

  void load();
  void save();

  Path();
};

extern Path path;
