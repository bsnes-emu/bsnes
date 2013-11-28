@interface CocoaWindow : NSWindow <NSWindowDelegate> {
@public
  phoenix::Window* window;
  NSMenu* menuBar;
  NSMenu* rootMenu;
  NSTextField* statusBar;
}
-(id) initWith:(phoenix::Window&)window;
-(BOOL) canBecomeKeyWindow;
-(BOOL) canBecomeMainWindow;
-(void) windowDidBecomeMain:(NSNotification*)notification;
-(void) windowDidMove:(NSNotification*)notification;
-(void) windowDidResize:(NSNotification*)notification;
-(BOOL) windowShouldClose:(id)sender;
-(NSDragOperation) draggingEntered:(id<NSDraggingInfo>)sender;
-(BOOL) performDragOperation:(id<NSDraggingInfo>)sender;
-(NSMenu*) menuBar;
-(void) menuAbout;
-(void) menuPreferences;
-(void) menuQuit;
-(NSTextField*) statusBar;
@end

namespace phoenix {

struct pWindow : public pObject {
  Window& window;
  CocoaWindow* cocoaWindow = nullptr;

  static Window& none();

  void append(Layout& layout);
  void append(Menu& menu);
  void append(Widget& widget);
  bool focused();
  Geometry frameMargin();
  Geometry geometry();
  void remove(Layout& layout);
  void remove(Menu& menu);
  void remove(Widget& widget);
  void setBackgroundColor(Color color);
  void setDroppable(bool droppable);
  void setFocused();
  void setFullScreen(bool fullScreen);
  void setGeometry(Geometry geometry);
  void setMenuFont(string font);
  void setMenuVisible(bool visible);
  void setModal(bool modal);
  void setResizable(bool resizable);
  void setStatusFont(string font);
  void setStatusText(string text);
  void setStatusVisible(bool visible);
  void setTitle(string text);
  void setVisible(bool visible);
  void setWidgetFont(string font);

  pWindow(Window& window) : pObject(window), window(window) {}
  void constructor();
  void destructor();
  void moveEvent();
  void sizeEvent();
  unsigned statusBarHeight();
  void statusBarReposition();
};

}
