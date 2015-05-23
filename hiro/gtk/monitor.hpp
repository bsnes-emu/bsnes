#if defined(Hiro_Monitor)

namespace hiro {

struct pMonitor {
  static unsigned count();
  static Geometry geometry(unsigned monitor);
  static unsigned primary();
};

}

#endif
