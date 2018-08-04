#if defined(Hiro_Widget)
struct mWidget : mSizable {
  Declare(Widget)

  auto doSize() const -> void;
  auto onSize(const function<void ()>& callback = {}) -> type&;
  auto remove() -> type& override;

//private:
//sizeof(mWidget) == 8
  struct State {
    function<void ()> onSize;
  } state;
};
#endif
