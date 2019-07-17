#if defined(Hiro_MenuCheckItem)

@interface CocoaMenuCheckItem : NSMenuItem {
@public
  hiro::mMenuCheckItem* menuCheckItem;
}
-(id) initWith:(hiro::mMenuCheckItem&)checkItem;
-(void) activate;
@end

namespace hiro {

struct pMenuCheckItem : pAction {
  Declare(MenuCheckItem, Action)

  auto setChecked(bool checked) -> void;
  auto setText(const string& text) -> void;

  CocoaMenuCheckItem* cocoaMenuCheckItem = nullptr;
};

}

#endif
