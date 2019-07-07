#if defined(Hiro_TextCursor)
struct TextCursor {
  using type = TextCursor;

  TextCursor(int offset = 0, int length = 0);

  explicit operator bool() const;
  auto operator==(const TextCursor& source) const -> bool;
  auto operator!=(const TextCursor& source) const -> bool;

  auto length() const -> int;
  auto offset() const -> int;
  auto setLength(int length = 0) -> type&;
  auto setOffset(int offset = 0) -> type&;
  auto setTextCursor(int offset = 0, int length = 0) -> type&;

//private:
  struct State {
    int offset;
    int length;
  } state;
};
#endif
