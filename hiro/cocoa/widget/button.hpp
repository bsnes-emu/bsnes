#if defined(Hiro_Button)

@interface CocoaButton : NSButton {
@public
  hiro::mButton* button;
}
-(id) initWith:(hiro::mButton&)button;
-(IBAction) activate:(id)sender;
@end

namespace hiro {

struct pButton : pWidget {
  Declare(Button, Widget)

  auto minimumSize() const -> Size override;
  auto setBordered(bool bordered) -> void;
  auto setGeometry(Geometry geometry) -> void override;
  auto setIcon(const image& icon) -> void;
  auto setOrientation(Orientation orientation) -> void;
  auto setText(const string& text) -> void;

  CocoaButton* cocoaButton = nullptr;
};

}

#endif
