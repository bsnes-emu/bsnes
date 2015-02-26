@interface CocoaTimer : NSObject {
@public
  phoenix::Timer* timer;
  NSTimer* instance;
}
-(id) initWith:(phoenix::Timer&)timer;
-(NSTimer*) instance;
-(void) update;
-(void) run:(NSTimer*)instance;
@end

namespace phoenix {

struct pTimer : public pObject {
  Timer& timer;
  CocoaTimer* cocoaTimer = nullptr;

  void setEnabled(bool enabled);
  void setInterval(unsigned interval);

  pTimer(Timer& timer) : pObject(timer), timer(timer) {}
  void constructor();
  void destructor();
};

}
