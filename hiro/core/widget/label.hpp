#if defined(Hiro_Label)
struct mLabel : mWidget {
  Declare(Label)

  auto alignment() const -> Alignment;
  auto backgroundColor() const -> Color;
  auto foregroundColor() const -> Color;
  auto setAlignment(Alignment alignment = {}) -> type&;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    Alignment alignment;
    Color backgroundColor;
    Color foregroundColor;
    string text;
  } state;
};
#endif
