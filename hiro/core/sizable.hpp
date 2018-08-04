#if defined(Hiro_Sizable)
struct mSizable : mObject {
  Declare(Sizable)

  auto geometry() const -> Geometry;
  virtual auto minimumSize() const -> Size;
  virtual auto setGeometry(Geometry geometry) -> type&;

//private:
//sizeof(mSizable) == 16
  struct State {
    Geometry geometry;
  } state;
};
#endif
