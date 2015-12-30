#if defined(Hiro_Timer)

@interface CocoaTimer : NSObject {
@public
  hiro::mTimer* timer;
  NSTimer* instance;
}
-(id) initWith:(hiro::mTimer&)timer;
-(NSTimer*) instance;
-(void) update;
-(void) run:(NSTimer*)instance;
@end

namespace hiro {

struct pTimer : pObject {
  Declare(Timer, Object)

  auto setEnabled(bool enabled) -> void;
  auto setInterval(uint interval) -> void;

  CocoaTimer* cocoaTimer = nullptr;
};

}

#endif
