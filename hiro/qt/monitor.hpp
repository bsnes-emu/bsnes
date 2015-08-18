#if defined(Hiro_Monitor)

namespace hiro {

struct pMonitor {
  static auto count() -> unsigned;
  static auto geometry(unsigned monitor) -> Geometry;
  static auto primary() -> unsigned;
};

}

#endif
