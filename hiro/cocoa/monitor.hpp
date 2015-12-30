#if defined(Hiro_Monitor)

namespace hiro {

struct pMonitor {
  static auto count() -> uint;
  static auto geometry(uint monitor) -> Geometry;
  static auto primary() -> uint;
};

}

#endif
