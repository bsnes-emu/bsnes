#if defined(Hiro_CheckLabel)

namespace hiro {

struct pCheckLabel : pWidget {
  Declare(CheckLabel, Widget)

  auto minimumSize() const -> Size;
  auto setChecked(bool checked) -> void;
  auto setText(const string& text) -> void;
};

}

#endif
