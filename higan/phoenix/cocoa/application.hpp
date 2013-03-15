@interface CocoaDelegate : NSObject <NSApplicationDelegate> {
}
-(NSApplicationTerminateReply) applicationShouldTerminate :(NSApplication*)sender;
-(void) run :(NSTimer*)timer;
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
