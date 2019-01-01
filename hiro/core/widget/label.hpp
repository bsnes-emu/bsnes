#if defined(Hiro_Label)
struct mLabel : mWidget {
  Declare(Label)

  auto alignment() const -> Alignment;
  auto backgroundColor() const -> Color;
  auto doMousePress(Mouse::Button button) const -> void;
  auto doMouseRelease(Mouse::Button button) const -> void;
  auto foregroundColor() const -> Color;
  auto onMousePress(const function<void (Mouse::Button)>& callback = {}) -> type&;
  auto onMouseRelease(const function<void (Mouse::Button)>& callback = {}) -> type&;
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
    function<void (Mouse::Button)> onMousePress;
    function<void (Mouse::Button)> onMouseRelease;
    string text;
  } state;
};
#endif
