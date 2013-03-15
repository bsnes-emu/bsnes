@interface CocoaSeparator : NSMenuItem {
@public
  phoenix::Separator *separator;
}
-(id) initWith :(phoenix::Separator&)separator;
@end

namespace phoenix {

struct pSeparator : public pAction {
  Separator &separator;
  CocoaSeparator *cocoaSeparator;

  pSeparator(Separator &separator) : pAction(separator), separator(separator) {}
  void constructor();
  void destructor();
};

}
