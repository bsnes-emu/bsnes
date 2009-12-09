class Disassembler : public QbWindow {
  Q_OBJECT

public:
  enum Source { CPU, SMP };

  QHBoxLayout *layout;
  QTextEdit *view;
  QVBoxLayout *controlLayout;
  QLabel *sourceLabel;
  QRadioButton *sourceCPU;
  QRadioButton *sourceSMP;

  void refresh(Source, unsigned);
  Disassembler();
};

extern Disassembler *disassembler;
