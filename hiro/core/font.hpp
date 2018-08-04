#if defined(Hiro_Font)
struct Font {
  using type = Font;

  Font(const string& family = "", float size = 0);

  explicit operator bool() const;
  auto operator==(const Font& source) const -> bool;
  auto operator!=(const Font& source) const -> bool;

  auto bold() const -> bool;
  auto family() const -> string;
  auto italic() const -> bool;
  auto reset() -> type&;
  auto setBold(bool bold = true) -> type&;
  auto setFamily(const string& family = "") -> type&;
  auto setItalic(bool italic = true) -> type&;
  auto setSize(float size = 0) -> type&;
  auto size() const -> float;
  auto size(const string& text) const -> Size;

  static const string Sans;
  static const string Serif;
  static const string Mono;

//private:
//sizeof(Font) == 32
  struct State {
    string family;         //24
    float size = 8.0;      //4
    char bold = false;     //1+
    char italic = false;   //1=4
  } state;
};
#endif
