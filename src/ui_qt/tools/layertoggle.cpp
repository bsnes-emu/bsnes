#include "layertoggle.moc"
LayerToggleWindow *layerToggleWindow;

LayerToggleWindow::LayerToggleWindow() {
  layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(Style::WidgetSpacing);
  layout->setAlignment(Qt::AlignTop);
  setLayout(layout);

  title = new QLabel("Layer Toggle Utility");
  title->setProperty("class", "title");
  layout->addWidget(title);

  gridLayout = new QGridLayout;
  gridLayout->setVerticalSpacing(0);
  layout->addLayout(gridLayout);

  bg1Label = new QLabel("BG1:");
  gridLayout->addWidget(bg1Label, 0, 0);

  bg1pri0 = new QCheckBox("Priority 0");
  bg1pri0->setChecked(true);
  gridLayout->addWidget(bg1pri0, 0, 1);

  bg1pri1 = new QCheckBox("Priority 1");
  bg1pri1->setChecked(true);
  gridLayout->addWidget(bg1pri1, 0, 2);

  bg2Label = new QLabel("BG2:");
  gridLayout->addWidget(bg2Label, 1, 0);

  bg2pri0 = new QCheckBox("Priority 0");
  bg2pri0->setChecked(true);
  gridLayout->addWidget(bg2pri0, 1, 1);

  bg2pri1 = new QCheckBox("Priority 1");
  bg2pri1->setChecked(true);
  gridLayout->addWidget(bg2pri1, 1, 2);

  bg3Label = new QLabel("BG3:");
  gridLayout->addWidget(bg3Label, 2, 0);

  bg3pri0 = new QCheckBox("Priority 0");
  bg3pri0->setChecked(true);
  gridLayout->addWidget(bg3pri0, 2, 1);

  bg3pri1 = new QCheckBox("Priority 1");
  bg3pri1->setChecked(true);
  gridLayout->addWidget(bg3pri1, 2, 2);

  bg4Label = new QLabel("BG4:");
  gridLayout->addWidget(bg4Label, 3, 0);

  bg4pri0 = new QCheckBox("Priority 0");
  bg4pri0->setChecked(true);
  gridLayout->addWidget(bg4pri0, 3, 1);

  bg4pri1 = new QCheckBox("Priority 1");
  bg4pri1->setChecked(true);
  gridLayout->addWidget(bg4pri1, 3, 2);

  oamLabel = new QLabel("OAM:");
  gridLayout->addWidget(oamLabel, 4, 0);

  oampri0 = new QCheckBox("Priority 0");
  oampri0->setChecked(true);
  gridLayout->addWidget(oampri0, 4, 1);

  oampri1 = new QCheckBox("Priority 1");
  oampri1->setChecked(true);
  gridLayout->addWidget(oampri1, 4, 2);

  oampri2 = new QCheckBox("Priority 2");
  oampri2->setChecked(true);
  gridLayout->addWidget(oampri2, 4, 3);

  oampri3 = new QCheckBox("Priority 3");
  oampri3->setChecked(true);
  gridLayout->addWidget(oampri3, 4, 4);

  spacer = new QWidget;
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  gridLayout->addWidget(spacer, 4, 5);

  connect(bg1pri0, SIGNAL(stateChanged(int)), this, SLOT(stateChanged()));
  connect(bg1pri1, SIGNAL(stateChanged(int)), this, SLOT(stateChanged()));
  connect(bg2pri0, SIGNAL(stateChanged(int)), this, SLOT(stateChanged()));
  connect(bg2pri1, SIGNAL(stateChanged(int)), this, SLOT(stateChanged()));
  connect(bg3pri0, SIGNAL(stateChanged(int)), this, SLOT(stateChanged()));
  connect(bg3pri1, SIGNAL(stateChanged(int)), this, SLOT(stateChanged()));
  connect(bg4pri0, SIGNAL(stateChanged(int)), this, SLOT(stateChanged()));
  connect(bg4pri1, SIGNAL(stateChanged(int)), this, SLOT(stateChanged()));
  connect(oampri0, SIGNAL(stateChanged(int)), this, SLOT(stateChanged()));
  connect(oampri1, SIGNAL(stateChanged(int)), this, SLOT(stateChanged()));
  connect(oampri2, SIGNAL(stateChanged(int)), this, SLOT(stateChanged()));
  connect(oampri3, SIGNAL(stateChanged(int)), this, SLOT(stateChanged()));
}

void LayerToggleWindow::stateChanged() {
  if(sender() == bg1pri0) SNES::config.ppu.bg1_enabled[0] = bg1pri0->isChecked();
  if(sender() == bg1pri1) SNES::config.ppu.bg1_enabled[1] = bg1pri1->isChecked();
  if(sender() == bg2pri0) SNES::config.ppu.bg2_enabled[0] = bg2pri0->isChecked();
  if(sender() == bg2pri1) SNES::config.ppu.bg2_enabled[1] = bg2pri1->isChecked();
  if(sender() == bg3pri0) SNES::config.ppu.bg3_enabled[0] = bg3pri0->isChecked();
  if(sender() == bg3pri1) SNES::config.ppu.bg3_enabled[1] = bg3pri1->isChecked();
  if(sender() == bg4pri0) SNES::config.ppu.bg4_enabled[0] = bg4pri0->isChecked();
  if(sender() == bg4pri1) SNES::config.ppu.bg4_enabled[1] = bg4pri1->isChecked();
  if(sender() == oampri0) SNES::config.ppu.oam_enabled[0] = oampri0->isChecked();
  if(sender() == oampri1) SNES::config.ppu.oam_enabled[1] = oampri1->isChecked();
  if(sender() == oampri2) SNES::config.ppu.oam_enabled[2] = oampri2->isChecked();
  if(sender() == oampri3) SNES::config.ppu.oam_enabled[3] = oampri3->isChecked();
}
