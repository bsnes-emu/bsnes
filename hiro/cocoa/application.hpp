#if defined(Hiro_Application)

@interface CocoaDelegate : NSObject <NSApplicationDelegate> {
}
-(NSApplicationTerminateReply) applicationShouldTerminate:(NSApplication*)sender;
-(BOOL) applicationShouldHandleReopen:(NSApplication*)application hasVisibleWindows:(BOOL)flag;
-(void) run:(NSTimer*)timer;
-(void) updateInDock:(NSTimer*)timer;
@end

namespace hiro {

struct pApplication {
  static auto exit() -> void;
  static auto modal() -> bool;
  static auto run() -> void;
  static auto pendingEvents() -> bool;
  static auto processEvents() -> void;
  static auto quit() -> void;
  static auto setScreenSaver(bool screenSaver) -> void;

  static auto initialize() -> void;
};

}

#endif
