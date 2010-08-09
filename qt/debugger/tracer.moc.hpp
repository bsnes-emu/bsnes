class Tracer : public QObject {
  Q_OBJECT

public:
  void stepCpu();
  void stepSmp();

  Tracer();
  ~Tracer();

public slots:
  void setCpuTraceState(int);
  void setSmpTraceState(int);
  void setTraceMaskState(int);

private:
  file tracefile;
  bool traceCpu;
  bool traceSmp;
  bool traceMask;

  uint8_t *traceMaskCPU;
  uint8_t *traceMaskSMP;
};

extern Tracer *tracer;
