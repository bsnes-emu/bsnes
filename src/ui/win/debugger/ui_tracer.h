class TracerSettings : public Window {
public:
Button   TracerEnable;

Groupbox CPUGroup;
Checkbox CPUTraceOp;
Checkbox CPUTraceOpMask;
Checkbox CPUTraceDMA;
Checkbox CPUTraceHDMA;

Groupbox SMPGroup;
Checkbox SMPTraceOp;
Checkbox SMPTraceOpMask;

  bool Event(EventInfo &info);

  void Setup();
} wTracer;
