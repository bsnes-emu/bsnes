#if defined(Hiro_Sizable)
struct mSizable : mObject {
  Declare(Sizable)

  auto collapsible() const -> bool;
  auto doSize() const -> void;
  auto geometry() const -> Geometry;
  virtual auto minimumSize() const -> Size;
  auto onSize(const function<void ()>& callback = {}) -> type&;
  virtual auto setCollapsible(bool collapsible = true) -> type&;
  virtual auto setGeometry(Geometry geometry) -> type&;

//private:
  struct State {
    bool collapsible = false;
    Geometry geometry;
    function<void ()> onSize;
  } state;
};
#endif
