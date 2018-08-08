#if defined(Hiro_Sizable)
struct mSizable : mObject {
  Declare(Sizable)

  auto doSize() const -> void;
  auto geometry() const -> Geometry;
  virtual auto minimumSize() const -> Size;
  auto onSize(const function<void ()>& callback = {}) -> type&;
  virtual auto setGeometry(Geometry geometry) -> type&;

//private:
//sizeof(mSizable) == 24
  struct State {
    Geometry geometry;
    function<void ()> onSize;
  } state;
};
#endif
