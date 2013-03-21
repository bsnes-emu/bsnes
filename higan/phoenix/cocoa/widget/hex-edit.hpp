@interface CocoaHexEdit : NSScrollView {
@public
  phoenix::HexEdit *hexEdit;
}
-(id) initWith:(phoenix::HexEdit&)hexEdit;
@end

namespace phoenix {

struct pHexEdit : public pWidget {
  HexEdit &hexEdit;
  CocoaHexEdit *cocoaHexEdit = nullptr;

  void setColumns(unsigned columns);
  void setLength(unsigned length);
  void setOffset(unsigned offset);
  void setRows(unsigned rows);
  void update();

  pHexEdit(HexEdit &hexEdit) : pWidget(hexEdit), hexEdit(hexEdit) {}
  void constructor();
  void destructor();
};

}
