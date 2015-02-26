@interface CocoaConsole : NSScrollView {
@public
  phoenix::Console* console;
}
-(id) initWith:(phoenix::Console&)console;
@end

namespace phoenix {

struct pConsole : public pWidget {
  Console& console;
  CocoaConsole* cocoaConsole = nullptr;

  void print(string text);
  void reset();
  void setBackgroundColor(Color color);
  void setForegroundColor(Color color);
  void setPrompt(string prompt);

  pConsole(Console& console) : pWidget(console), console(console) {}
  void constructor();
  void destructor();
};

}
