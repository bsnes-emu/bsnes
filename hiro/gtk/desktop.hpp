#if defined(Hiro_Desktop)

namespace hiro {

struct pDesktop {
  static auto size() -> Size;
  static auto workspace() -> Geometry;
};

}

#endif
