#if defined(Hiro_Label)

@interface CocoaLabel : NSTextField {
@public
  hiro::mLabel* label;
}
-(id) initWith:(hiro::mLabel&)label;
@end

namespace hiro {

struct pLabel : pWidget {
  Declare(Label, Widget)

  auto minimumSize() const -> Size override;
  auto setAlignment(Alignment alignment) -> void;
  auto setGeometry(Geometry geometry) -> void override;
  auto setText(const string& text) -> void;

  CocoaLabel* cocoaLabel = nullptr;
};

}

#endif
