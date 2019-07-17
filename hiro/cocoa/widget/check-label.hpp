#if defined(Hiro_CheckLabel)

@interface CocoaCheckLabel : NSButton {
@public
  hiro::mCheckLabel* checkLabel;
}
-(id) initWith:(hiro::mCheckLabel&)checkLabel;
-(IBAction) activate:(id)sender;
@end

namespace hiro {

struct pCheckLabel : pWidget {
  Declare(CheckLabel, Widget)

  auto minimumSize() const -> Size override;
  auto setChecked(bool checked) -> void;
  auto setGeometry(Geometry geometry) -> void;
  auto setText(const string& text) -> void;

  CocoaCheckLabel* cocoaCheckLabel = nullptr;
};

}

#endif
