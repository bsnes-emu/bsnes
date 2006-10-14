void tprintf(char *s, ...);

class Tracer {
private:

FILE *fp;

struct {
  bool   enabled;

  bool   cpuop;
  bool   cpuopmask;
  uint8 *cpuopmasktbl;
  bool   cpudma;

  bool   smpop;
  bool   smpopmask;
  uint8 *smpopmasktbl;
} settings;

public:
  void enable(bool en);
  bool enabled() { return settings.enabled; }

  void cpuop_enable(bool en) { settings.cpuop = en; }
  bool cpuop_enabled() { return settings.cpuop; }

  void cpuopmask_enable(bool en);
  bool cpuopmask_enabled() { return settings.cpuopmask; }

  void cpudma_enable(bool en) { settings.cpudma = en; }
  bool cpudma_enabled() { return settings.cpudma; }

  void smpop_enable(bool en) { settings.smpop = en; }
  bool smpop_enabled() { return settings.smpop; }

  void smpopmask_enable(bool en);
  bool smpopmask_enabled() { return settings.smpopmask; }

  void trace_cpuop();
  void trace_smpop();

  Tracer();
  ~Tracer();

  friend void tprintf(char *s, ...);
};

extern Tracer tracer;
