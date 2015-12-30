#if defined(Hiro_CheckButton)

@interface CocoaCheckButton : NSButton {
@public
  hiro::mCheckButton* checkButton;
}
-(id) initWith:(hiro::mCheckButton&)checkButton;
-(IBAction) activate:(id)sender;
@end

namespace hiro {

struct pCheckButton : pWidget {
  Declare(CheckButton, Widget)

  auto minimumSize() const -> Size override;
  auto setBordered(bool bordered) -> void;
  auto setChecked(bool checked) -> void;
  auto setGeometry(Geometry geometry) -> void override;
  auto setImage(const Image& image) -> void;
  auto setOrientation(Orientation orientation) -> void;
  auto setText(const string& text) -> void;

  CocoaCheckButton* cocoaCheckButton = nullptr;
};

}

#endif
