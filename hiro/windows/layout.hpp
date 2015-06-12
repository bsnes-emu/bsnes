#if defined(Hiro_Layout)

namespace hiro {

struct pLayout : pSizable {
  Declare(Layout, Sizable)

  auto setEnabled(bool enabled) -> void override;
  auto setFont(const string& font) -> void override;
  auto setVisible(bool visible) -> void override;
};

}

#endif
