#if defined(Hiro_MouseCursor)
struct MouseCursor {
  using type = MouseCursor;

  MouseCursor(const string& name = "");

  explicit operator bool() const;
  auto operator==(const MouseCursor& source) const -> bool;
  auto operator!=(const MouseCursor& source) const -> bool;

  auto name() const -> string;
  auto setName(const string& name = "") -> type&;

  static const string Default;
  static const string Hand;
  static const string HorizontalResize;
  static const string VerticalResize;

//private:
  struct State {
    string name;
  } state;
};
#endif
