#if defined(Hiro_Monitor)
struct Monitor {
  Monitor() = delete;

  static auto count() -> uint;
  static auto dpi(maybe<uint> monitor = nothing) -> Position;
  static auto geometry(maybe<uint> monitor = nothing) -> Geometry;
  static auto primary() -> uint;
  static auto workspace(maybe<uint> monitor = nothing) -> Geometry;
};
#endif
