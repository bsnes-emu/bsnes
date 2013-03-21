@interface CocoaRadioItem : NSMenuItem {
@public
  phoenix::RadioItem *radioItem;
}
-(id) initWith:(phoenix::RadioItem&)radioItem;
-(void) activate;
@end

namespace phoenix {

struct pRadioItem : public pAction {
  RadioItem &radioItem;
  CocoaRadioItem *cocoaRadioItem = nullptr;

  bool checked();
  void setChecked();
  void setGroup(const set<RadioItem&> &group);
  void setText(const string &text);

  pRadioItem(RadioItem &radioItem) : pAction(radioItem), radioItem(radioItem) {}
  void constructor();
  void destructor();
};

}
