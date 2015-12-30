#if defined(Hiro_Menu)

@interface CocoaMenu : NSMenuItem {
@public
  hiro::mMenu* menu;
  NSMenu* cocoaMenu;
}
-(id) initWith:(hiro::mMenu&)menu;
-(NSMenu*) cocoaMenu;
@end

namespace hiro {

struct pMenu : pAction {
  Declare(Menu, Action)

  auto append(sAction action) -> void;
  auto remove(sAction action) -> void;
  auto setImage(const Image& image) -> void;
  auto setText(const string& text) -> void;

  CocoaMenu* cocoaMenu = nullptr;
};

}

#endif
