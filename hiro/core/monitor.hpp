#if defined(Hiro_Monitor)
struct Monitor {
  Monitor() = delete;

  static auto count() -> uint;
  static auto dpi(maybe<uint> monitor = nothing) -> Position;
  static auto geometry(maybe<uint> monitor = nothing) -> Geometry;
  static auto primary() -> uint;
  static auto workspace(maybe<uint> monitor = nothing) -> Geometry;
};

//DPI scale X
inline auto sx(float x) -> float {
  //round DPI scalar to increments of 0.5 (eg 1.0, 1.5, 2.0, ...)
  static auto scale = round(Monitor::dpi().x() / 96.0 * 2.0) / 2.0;
  return x * scale;
}

//DPI scale y
inline auto sy(float y) -> float {
  static auto scale = round(Monitor::dpi().y() / 96.0 * 2.0) / 2.0;
  return y * scale;
}

inline auto operator"" _sx(unsigned long long x) -> float { return sx(x); }
inline auto operator"" _sy(unsigned long long y) -> float { return sy(y); }
#endif
