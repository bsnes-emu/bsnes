#if defined(Hiro_Desktop)
struct Desktop {
  Desktop() = delete;

  static auto size() -> Size;
  static auto workspace() -> Geometry;
};
#endif
