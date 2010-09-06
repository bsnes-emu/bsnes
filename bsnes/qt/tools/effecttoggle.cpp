#include "effecttoggle.moc"
EffectToggleWindow *effectToggleWindow;

EffectToggleWindow::EffectToggleWindow() {
  layout = new QVBoxLayout;
  layout->setAlignment(Qt::AlignTop);
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  setLayout(layout);

  ppuLabel = new QLabel("<b>S-PPU (Video)</b>");
  layout->addWidget(ppuLabel);

  ppuLayout = new QGridLayout;
  ppuLayout->setAlignment(Qt::AlignLeft);
  layout->addLayout(ppuLayout);

  bg1pri0 = new QCheckBox("BG1 Priority 0");
  bg1pri0->setChecked(true);
  ppuLayout->addWidget(bg1pri0, 0, 0);

  bg1pri1 = new QCheckBox("BG1 Priority 1");
  bg1pri1->setChecked(true);
  ppuLayout->addWidget(bg1pri1, 0, 1);

  bg2pri0 = new QCheckBox("BG2 Priority 0");
  bg2pri0->setChecked(true);
  ppuLayout->addWidget(bg2pri0, 1, 0);

  bg2pri1 = new QCheckBox("BG2 Priority 1");
  bg2pri1->setChecked(true);
  ppuLayout->addWidget(bg2pri1, 1, 1);

  bg3pri0 = new QCheckBox("BG3 Priority 0");
  bg3pri0->setChecked(true);
  ppuLayout->addWidget(bg3pri0, 2, 0);

  bg3pri1 = new QCheckBox("BG3 Priority 1");
  bg3pri1->setChecked(true);
  ppuLayout->addWidget(bg3pri1, 2, 1);

  bg4pri0 = new QCheckBox("BG4 Priority 0");
  bg4pri0->setChecked(true);
  ppuLayout->addWidget(bg4pri0, 3, 0);

  bg4pri1 = new QCheckBox("BG4 Priority 1");
  bg4pri1->setChecked(true);
  ppuLayout->addWidget(bg4pri1, 3, 1);

  oampri0 = new QCheckBox("OAM Priority 0");
  oampri0->setChecked(true);
  ppuLayout->addWidget(oampri0, 4, 0);

  oampri1 = new QCheckBox("OAM Priority 1");
  oampri1->setChecked(true);
  ppuLayout->addWidget(oampri1, 4, 1);

  oampri2 = new QCheckBox("OAM Priority 2");
  oampri2->setChecked(true);
  ppuLayout->addWidget(oampri2, 4, 2);

  oampri3 = new QCheckBox("OAM Priority 3");
  oampri3->setChecked(true);
  ppuLayout->addWidget(oampri3, 4, 3);

  dspLabel = new QLabel("<b>S-DSP (Audio)</b>");
  layout->addWidget(dspLabel);

  dspLayout = new QGridLayout;
  dspLayout->setAlignment(Qt::AlignLeft);
  layout->addLayout(dspLayout);

  channel0 = new QCheckBox("Channel 0");
  channel0->setChecked(true);
  dspLayout->addWidget(channel0, 0, 0);

  channel1 = new QCheckBox("Channel 1");
  channel1->setChecked(true);
  dspLayout->addWidget(channel1, 0, 1);

  channel2 = new QCheckBox("Channel 2");
  channel2->setChecked(true);
  dspLayout->addWidget(channel2, 0, 2);

  channel3 = new QCheckBox("Channel 3");
  channel3->setChecked(true);
  dspLayout->addWidget(channel3, 0, 3);

  channel4 = new QCheckBox("Channel 4");
  channel4->setChecked(true);
  dspLayout->addWidget(channel4, 1, 0);

  channel5 = new QCheckBox("Channel 5");
  channel5->setChecked(true);
  dspLayout->addWidget(channel5, 1, 1);

  channel6 = new QCheckBox("Channel 6");
  channel6->setChecked(true);
  dspLayout->addWidget(channel6, 1, 2);

  channel7 = new QCheckBox("Channel 7");
  channel7->setChecked(true);
  dspLayout->addWidget(channel7, 1, 3);

  connect(bg1pri0, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(bg1pri1, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(bg2pri0, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(bg2pri1, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(bg3pri0, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(bg3pri1, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(bg4pri0, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(bg4pri1, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(oampri0, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(oampri1, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(oampri2, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(oampri3, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(channel0, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(channel1, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(channel2, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(channel3, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(channel4, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(channel5, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(channel6, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
  connect(channel7, SIGNAL(stateChanged(int)), this, SLOT(synchronize()));
}

void EffectToggleWindow::synchronize() {
  #if defined(PROFILE_COMPATIBILITY) || defined(PROFILE_PERFORMANCE)
  SNES::ppu.layer_enable(0, 0, bg1pri0->isChecked());
  SNES::ppu.layer_enable(0, 1, bg1pri1->isChecked());
  SNES::ppu.layer_enable(1, 0, bg2pri0->isChecked());
  SNES::ppu.layer_enable(1, 1, bg2pri1->isChecked());
  SNES::ppu.layer_enable(2, 0, bg3pri0->isChecked());
  SNES::ppu.layer_enable(2, 1, bg3pri1->isChecked());
  SNES::ppu.layer_enable(3, 0, bg4pri0->isChecked());
  SNES::ppu.layer_enable(3, 1, bg4pri1->isChecked());
  SNES::ppu.layer_enable(4, 0, oampri0->isChecked());
  SNES::ppu.layer_enable(4, 1, oampri1->isChecked());
  SNES::ppu.layer_enable(4, 2, oampri2->isChecked());
  SNES::ppu.layer_enable(4, 3, oampri3->isChecked());

  SNES::dsp.channel_enable(0, channel0->isChecked());
  SNES::dsp.channel_enable(1, channel1->isChecked());
  SNES::dsp.channel_enable(2, channel2->isChecked());
  SNES::dsp.channel_enable(3, channel3->isChecked());
  SNES::dsp.channel_enable(4, channel4->isChecked());
  SNES::dsp.channel_enable(5, channel5->isChecked());
  SNES::dsp.channel_enable(6, channel6->isChecked());
  SNES::dsp.channel_enable(7, channel7->isChecked());
  #endif
}
