#if defined(Hiro_Cursor)
struct Cursor {
  using type = Cursor;

  Cursor(int offset = 0, int length = 0);

  explicit operator bool() const;
  auto operator==(const Cursor& source) const -> bool;
  auto operator!=(const Cursor& source) const -> bool;

  auto length() const -> int;
  auto offset() const -> int;
  auto setCursor(int offset = 0, int length = 0) -> type&;
  auto setLength(int length = 0) -> type&;
  auto setOffset(int offset = 0) -> type&;

//private:
  struct State {
    int offset;
    int length;
  } state;
};
#endif
