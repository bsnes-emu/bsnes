#if defined(Hiro_RadioLabel)

@interface CocoaRadioLabel : NSButton {
@public
  hiro::mRadioLabel* radioLabel;
}
-(id) initWith:(hiro::mRadioLabel&)radioLabel;
-(IBAction) activate:(id)sender;
@end

namespace hiro {

struct pRadioLabel : pWidget {
  Declare(RadioLabel, Widget)

  auto minimumSize() const -> Size override;
  auto setChecked() -> void;
  auto setGeometry(Geometry geometry) -> void override;
  auto setGroup(sGroup group) -> void;
  auto setText(const string& text) -> void;

  CocoaRadioLabel* cocoaRadioLabel = nullptr;
};

}

#endif
