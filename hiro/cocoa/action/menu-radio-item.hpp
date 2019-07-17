#if defined(Hiro_MenuRadioItem)

@interface CocoaMenuRadioItem : NSMenuItem {
@public
  hiro::mMenuRadioItem* menuRadioItem;
}
-(id) initWith:(hiro::mMenuRadioItem&)menuRadioItem;
-(void) activate;
@end

namespace hiro {

struct pMenuRadioItem : pAction {
  Declare(MenuRadioItem, Action)

  auto setChecked() -> void;
  auto setGroup(sGroup group) -> void;
  auto setText(const string& text) -> void;

  CocoaMenuRadioItem* cocoaMenuRadioItem = nullptr;
};

}

#endif
