#if defined(Hiro_Widget)
struct mWidget : mSizable {
  Declare(Widget)

  auto remove() -> type& override;
  auto setToolTip(const string& toolTip = "") -> type&;
  auto toolTip() const -> string;

//private:
//sizeof(mWidget) == 32
  struct State {
    string toolTip;
  } state;
};
#endif
