namespace phoenix {

struct pMonitor {
  static unsigned count();
  static Geometry geometry(unsigned monitor);
  static unsigned primary();
};

}
