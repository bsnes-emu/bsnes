#if defined(Hiro_ComboButton)

namespace hiro {

struct pComboButton : pWidget {
  Declare(ComboButton, Widget)

  auto append(sComboButtonItem item) -> void;
  auto minimumSize() const -> Size override;
  auto remove(sComboButtonItem item) -> void;
  auto reset() -> void;
  auto setGeometry(Geometry geometry) -> void override;

  auto onChange() -> void;
};

}

#endif
