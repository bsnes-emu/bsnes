@interface CocoaLabel : NSTextField {
@public
  phoenix::Label *label;
}
-(id) initWith:(phoenix::Label&)label;
@end

namespace phoenix {

struct pLabel : public pWidget {
  Label &label;
  CocoaLabel *cocoaLabel = nullptr;

  Size minimumSize();
  void setText(const string &text);

  pLabel(Label &label) : pWidget(label), label(label) {}
  void constructor();
  void destructor();
};

}
