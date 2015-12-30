#if defined(Hiro_ComboButton)

@interface CocoaComboButton : NSPopUpButton {
@public
  hiro::mComboButton* comboButton;
}
-(id) initWith:(hiro::mComboButton&)comboButton;
-(IBAction) activate:(id)sender;
@end

namespace hiro {

struct pComboButton : pWidget {
  Declare(ComboButton, Widget)

  auto append(sComboButtonItem item) -> void;
  auto minimumSize() const -> Size override;
  auto remove(sComboButtonItem item) -> void;
  auto reset() -> void;
  auto setGeometry(Geometry geometry) -> void override;

  auto _updateSelected(signed selected) -> void;

  CocoaComboButton* cocoaComboButton = nullptr;
};

}

#endif
