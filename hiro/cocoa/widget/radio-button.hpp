#if defined(Hiro_RadioButton)

@interface CocoaRadioButton : NSButton {
@public
  hiro::mRadioButton* radioButton;
}
-(id) initWith:(hiro::mRadioButton&)radioButton;
-(IBAction) activate:(id)sender;
@end

namespace hiro {

struct pRadioButton : pWidget {
  Declare(RadioButton, Widget)

  auto minimumSize() const -> Size override;
  auto setBordered(bool bordered) -> void;
  auto setChecked() -> void;
  auto setGeometry(Geometry geometry) -> void override;
  auto setGroup(sGroup group) -> void;
  auto setIcon(const image& icon) -> void;
  auto setOrientation(Orientation orientation) -> void;
  auto setText(const string& text) -> void;

  CocoaRadioButton* cocoaRadioButton = nullptr;
};

}

#endif
