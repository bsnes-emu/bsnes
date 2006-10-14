class DebugWindow : public Window {
public:
Button  Run;
Button  Tracer;
Button  Memory;

Editbox Console;
Editbox Command;

struct {
  uint count;
  char line[250][512];
} buffer;

struct {
  bool debug;
  bool cpu;
  bool smp;
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
