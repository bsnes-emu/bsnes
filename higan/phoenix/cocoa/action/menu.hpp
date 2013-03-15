@interface CocoaMenu : NSMenuItem {
@public
  phoenix::Menu *menu;
  NSMenu *cocoaMenu;
}
-(id) initWith :(phoenix::Menu&)menu;
-(NSMenu*) cocoaMenu;
@end

namespace phoenix {

struct pMenu : public pAction {
  Menu &menu;
  CocoaMenu *cocoaMenu;

  void append(Action &action);
  void remove(Action &action);
  void setImage(const image &image);
  void setText(const string &text);

  pMenu(Menu &menu) : pAction(menu), menu(menu) {}
  void constructor();
  void destructor();
};

}
