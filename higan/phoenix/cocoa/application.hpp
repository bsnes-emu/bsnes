@interface CocoaDelegate : NSObject <NSApplicationDelegate> {
}
-(NSApplicationTerminateReply) applicationShouldTerminate:(NSApplication*)sender;
-(BOOL) applicationShouldHandleReopen:(NSApplication*)application hasVisibleWindows:(BOOL)flag;
-(void) run:(NSTimer*)timer;
@end

namespace phoenix {

struct pApplication {
  static void run();
  static bool pendingEvents();
  static void processEvents();
  static void quit();

  static void initialize();
};

}
