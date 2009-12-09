#include "disassembler.moc"
Disassembler *disassembler;

Disassembler::Disassembler() : QbWindow(config().geometry.disassembler) {
  setObjectName("disassembler");
  setWindowTitle("Disassembler");

  layout = new QHBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  setLayout(layout);

  view = new QTextEdit;
  view->setReadOnly(true);
  view->setFont(QFont(Style::Monospace));
  view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view->setMinimumWidth((23 + 2) * view->fontMetrics().width(' '));
  view->setMinimumHeight((25 + 1) * view->fontMetrics().height());
  layout->addWidget(view);

  controlLayout = new QVBoxLayout;
  controlLayout->setAlignment(Qt::AlignTop);
  controlLayout->setSpacing(0);
  layout->addLayout(controlLayout);

  sourceLabel = new QLabel("Source:");
  controlLayout->addWidget(sourceLabel);

  sourceCPU = new QRadioButton("S-CPU");
  sourceCPU->setChecked(true);
  controlLayout->addWidget(sourceCPU);

  sourceSMP = new QRadioButton("S-SMP");
  controlLayout->addWidget(sourceSMP);
}

void Disassembler::refresh(Source source, unsigned addr) {
  if(source == CPU && !sourceCPU->isChecked()) return;
  if(source == SMP && !sourceSMP->isChecked()) return;

  uint8 *usage;
  unsigned mask;
  if(source == CPU) { usage = SNES::cpu.usage; mask = (1 << 24) - 1; }
  if(source == SMP) { usage = SNES::smp.usage; mask = (1 << 16) - 1; }

  int line[25];
  for(unsigned i = 0; i < 25; i++) line[i] = -1;

  line[12] = addr;

  for(signed index = 11; index >= 0; index--) {
    int base = line[index + 1];
    if(base == -1) break;

    for(unsigned i = 1; i <= 4; i++) {
      if(usage[(base - i) & mask] & 0x20) {
        line[index] = base - i;
        break;
      }
    }
  }

  for(signed index = 13; index <= 24; index++) {
    int base = line[index - 1];
    if(base == -1) break;

    for(unsigned i = 1; i <= 4; i++) {
      if(usage[(base + i) & mask] & 0x20) {
        line[index] = base + i;
        break;
      }
    }
  }

  string output;
  for(unsigned i = 0; i < 25; i++) {
    if(i < 12) output << "<font color='#0000a0'>";
    else if(i == 12) output << "<font color='#00a000'>";
    else output << "<font color='#a00000'>";

    if(line[i] == -1) {
      output << "...";
    } else {
      char t[256];
      if(source == CPU) { SNES::cpu.disassemble_opcode(t, line[i]); t[20] = 0; }
      if(source == SMP) { SNES::smp.disassemble_opcode(t, line[i]); t[23] = 0; }
      string text = rtrim(t);
      text.replace(" ", "&nbsp;");
      output << text;
    }

    output << "</font>";
    if(i != 24) output << "<br>";
  }
  view->setHtml(output);
}
