#if defined(Hiro_Monitor)

namespace hiro {

struct pMonitor {
  static auto count() -> uint;
  static auto dpi(uint monitor) -> Position;
  static auto geometry(uint monitor) -> Geometry;
  static auto primary() -> uint;
  static auto workspace(uint monitor) -> Geometry;
};

}

#endif
