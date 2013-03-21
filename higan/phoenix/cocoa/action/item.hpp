@interface CocoaItem : NSMenuItem {
@public
  phoenix::Item *item;
}
-(id) initWith:(phoenix::Item&)item;
-(void) activate;
@end

namespace phoenix {

struct pItem : public pAction {
  Item &item;
  CocoaItem *cocoaItem = nullptr;

  void setImage(const image &image);
  void setText(const string &text);

  pItem(Item &item) : pAction(item), item(item) {}
  void constructor();
  void destructor();
};

}
