#if defined(Hiro_MenuItem)

@interface CocoaMenuItem : NSMenuItem {
@public
  hiro::mMenuItem* menuItem;
}
-(id) initWith:(hiro::mMenuItem&)menuItem;
-(void) activate;
@end

namespace hiro {

struct pMenuItem : pAction {
  Declare(MenuItem, Action)

  auto setImage(const Image& image) -> void;
  auto setText(const string& text) -> void;

  CocoaMenuItem* cocoaMenuItem = nullptr;
};

}

#endif
