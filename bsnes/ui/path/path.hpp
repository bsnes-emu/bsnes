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

  string home(const string &filename);
  string load(const string &path);
  void save(const string &path, const string &value);

  void load();
  void save();

  Path();
};

extern Path path;
