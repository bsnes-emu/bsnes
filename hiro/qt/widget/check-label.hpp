#if defined(Hiro_CheckLabel)

namespace hiro {

struct pCheckLabel : pWidget {
  Declare(CheckLabel, Widget)

  auto minimumSize() const -> Size override;
  auto setChecked(bool checked) -> void;
  auto setText(const string& text) -> void;

  auto _setState() -> void;

  QtCheckLabel* qtCheckLabel = nullptr;
};

}

#endif
