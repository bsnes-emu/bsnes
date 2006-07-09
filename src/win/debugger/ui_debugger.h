class DebugWindow : public Window {
public:
Button   Breakpoints;
Button   Memory;
Button   RunToFrame;
Button   Run;
Label    Separator;
Editbox  Console;
Editbox  Status;
Groupbox CPUGroup;
Button   CPUStep;
Button   CPUProceed;
Button   CPUSkip;
Editbox  CPUTraceNum;
Button   CPUTrace;
Button   CPUDisable;
Groupbox APUGroup;
Button   APUStep;
Button   APUProceed;
Button   APUSkip;
Editbox  APUTraceNum;
Button   APUTrace;
Button   APUDisable;
Groupbox ConsoleGroup;
Checkbox ConsoleMsgDebug;
Checkbox ConsoleMsgCPU;
Checkbox ConsoleMsgAPU;
Checkbox ConsoleTrace;
Checkbox ConsoleTraceMask;

struct {
  uint count;
  char line[250][128];
} buffer;

struct {
  bool debug;
  bool cpu;
  bool apu;

  FILE *fp;
  bool trace;
  bool trace_mask;
} output;

  bool Event(EventInfo &info);

  void Update();
  void Print(uint source, const char *str);
  void Clear();

  void SetState(uint state);

  void Show();
  void Setup();

  DebugWindow();
  ~DebugWindow();
} wDebug;
