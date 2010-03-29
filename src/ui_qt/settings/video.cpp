#include "video.moc"
VideoSettingsWindow *videoSettingsWindow;

VideoSettingsWindow::VideoSettingsWindow() {
  layout = new QVBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  layout->setAlignment(Qt::AlignTop);
  setLayout(layout);

  displayLabel = new QLabel("<b>Display</b>");
  layout->addWidget(displayLabel);

  autoHideFullscreenMenu = new QCheckBox("Auto-hide menus when entering fullscreen mode");
  layout->addWidget(autoHideFullscreenMenu);

  colorLabel = new QLabel("<b>Color Adjustment</b>");
  layout->addWidget(colorLabel);

  colorLayout = new QGridLayout;
  layout->addLayout(colorLayout);

  contrastLabel = new QLabel("Contrast adjust:");
  colorLayout->addWidget(contrastLabel, 0, 0);

  contrastValue = new QLabel;
  contrastValue->setAlignment(Qt::AlignHCenter);
  contrastValue->setMinimumWidth(contrastValue->fontMetrics().width("+100%"));
  colorLayout->addWidget(contrastValue, 0, 1);

  contrastSlider = new QSlider(Qt::Horizontal);
  contrastSlider->setMinimum(-95);
  contrastSlider->setMaximum(+95);
  colorLayout->addWidget(contrastSlider, 0, 2);

  brightnessLabel = new QLabel("Brightness adjust:");
  colorLayout->addWidget(brightnessLabel, 1, 0);

  brightnessValue = new QLabel;
  brightnessValue->setAlignment(Qt::AlignHCenter);
  colorLayout->addWidget(brightnessValue, 1, 1);

  brightnessSlider = new QSlider(Qt::Horizontal);
  brightnessSlider->setMinimum(-95);
  brightnessSlider->setMaximum(+95);
  colorLayout->addWidget(brightnessSlider, 1, 2);

  gammaLabel = new QLabel("Gamma adjust:");
  colorLayout->addWidget(gammaLabel, 2, 0);

  gammaValue = new QLabel;
  gammaValue->setAlignment(Qt::AlignHCenter);
  colorLayout->addWidget(gammaValue, 2, 1);

  gammaSlider = new QSlider(Qt::Horizontal);
  gammaSlider->setMinimum(-95);
  gammaSlider->setMaximum(+95);
  colorLayout->addWidget(gammaSlider, 2, 2);

  scanlineLabel = new QLabel("Scanline adjust:");
  colorLayout->addWidget(scanlineLabel, 3, 0);

  scanlineValue = new QLabel;
  scanlineValue->setAlignment(Qt::AlignHCenter);
  colorLayout->addWidget(scanlineValue, 3, 1);

  scanlineSlider = new QSlider(Qt::Horizontal);
  scanlineSlider->setMinimum(0);
  scanlineSlider->setMaximum(20);
  scanlineSlider->setPageStep(4);
  colorLayout->addWidget(scanlineSlider, 3, 2);

  options = new QHBoxLayout;
  layout->addLayout(options);

  enableGammaRamp = new QCheckBox("Simulate NTSC TV gamma ramp");
  enableGammaRamp->setToolTip("Lower monitor gamma to more accurately match a CRT television");
  options->addWidget(enableGammaRamp);

  cropLabel = new QLabel("<b>Overscan Compensation</b>");
  layout->addWidget(cropLabel);

  cropLayout = new QGridLayout;
  layout->addLayout(cropLayout);

  cropLeftLabel = new QLabel("Left:");
  cropLayout->addWidget(cropLeftLabel, 0, 0);

  cropLeftValue = new QLabel;
  cropLeftValue->setAlignment(Qt::AlignHCenter);
  cropLeftValue->setMinimumWidth(cropLeftValue->fontMetrics().width("+100%"));
  cropLayout->addWidget(cropLeftValue, 0, 1);

  cropLeftSlider = new QSlider(Qt::Horizontal);
  cropLeftSlider->setMinimum(0);
  cropLeftSlider->setMaximum(20);
  cropLayout->addWidget(cropLeftSlider, 0, 2);

  cropTopLabel = new QLabel("Top:");
  cropLayout->addWidget(cropTopLabel, 1, 0);

  cropTopValue = new QLabel;
  cropTopValue->setAlignment(Qt::AlignHCenter);
  cropLayout->addWidget(cropTopValue, 1, 1);

  cropTopSlider = new QSlider(Qt::Horizontal);
  cropTopSlider->setMinimum(0);
  cropTopSlider->setMaximum(20);
  cropLayout->addWidget(cropTopSlider, 1, 2);

  cropRightLabel = new QLabel("Right:");
  cropLayout->addWidget(cropRightLabel, 2, 0);

  cropRightValue = new QLabel;
  cropRightValue->setAlignment(Qt::AlignHCenter);
  cropLayout->addWidget(cropRightValue, 2, 1);

  cropRightSlider = new QSlider(Qt::Horizontal);
  cropRightSlider->setMinimum(0);
  cropRightSlider->setMaximum(20);
  cropLayout->addWidget(cropRightSlider, 2, 2);

  cropBottomLabel = new QLabel("Bottom:");
  cropLayout->addWidget(cropBottomLabel, 3, 0);

  cropBottomValue = new QLabel;
  cropBottomValue->setAlignment(Qt::AlignHCenter);
  cropLayout->addWidget(cropBottomValue, 3, 1);

  cropBottomSlider = new QSlider(Qt::Horizontal);
  cropBottomSlider->setMinimum(0);
  cropBottomSlider->setMaximum(20);
  cropLayout->addWidget(cropBottomSlider, 3, 2);

  pixelShaderLabel = new QLabel("<b>Pixel Shader</b>");
  layout->addWidget(pixelShaderLabel);

  pixelShaderLayout = new QGridLayout;
  layout->addLayout(pixelShaderLayout);

  fragmentShaderLabel = new QLabel("Fragment:");
  pixelShaderLayout->addWidget(fragmentShaderLabel, 0, 0);

  fragmentShaderValue = new QLineEdit;
  pixelShaderLayout->addWidget(fragmentShaderValue, 0, 1);

  fragmentShaderSelect = new QPushButton("Select ...");
  pixelShaderLayout->addWidget(fragmentShaderSelect, 0, 2);

  fragmentShaderDefault = new QPushButton("Default");
  pixelShaderLayout->addWidget(fragmentShaderDefault, 0, 3);

  vertexShaderLabel = new QLabel("Vertex:");
  pixelShaderLayout->addWidget(vertexShaderLabel, 1, 0);

  vertexShaderValue = new QLineEdit;
  pixelShaderLayout->addWidget(vertexShaderValue, 1, 1);

  vertexShaderSelect = new QPushButton("Select ...");
  pixelShaderLayout->addWidget(vertexShaderSelect, 1, 2);

  vertexShaderDefault = new QPushButton("Default");
  pixelShaderLayout->addWidget(vertexShaderDefault, 1, 3);

  connect(autoHideFullscreenMenu, SIGNAL(stateChanged(int)), this, SLOT(autoHideFullscreenMenuToggle()));
  connect(contrastSlider, SIGNAL(valueChanged(int)), this, SLOT(contrastAdjust(int)));
  connect(brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(brightnessAdjust(int)));
  connect(gammaSlider, SIGNAL(valueChanged(int)), this, SLOT(gammaAdjust(int)));
  connect(scanlineSlider, SIGNAL(valueChanged(int)), this, SLOT(scanlineAdjust(int)));
  connect(enableGammaRamp, SIGNAL(stateChanged(int)), this, SLOT(gammaRampToggle(int)));
  connect(cropLeftSlider, SIGNAL(valueChanged(int)), this, SLOT(cropLeftAdjust(int)));
  connect(cropTopSlider, SIGNAL(valueChanged(int)), this, SLOT(cropTopAdjust(int)));
  connect(cropRightSlider, SIGNAL(valueChanged(int)), this, SLOT(cropRightAdjust(int)));
  connect(cropBottomSlider, SIGNAL(valueChanged(int)), this, SLOT(cropBottomAdjust(int)));
  connect(fragmentShaderSelect, SIGNAL(released()), this, SLOT(selectFragmentShader()));
  connect(fragmentShaderDefault, SIGNAL(released()), this, SLOT(defaultFragmentShader()));
  connect(vertexShaderSelect, SIGNAL(released()), this, SLOT(selectVertexShader()));
  connect(vertexShaderDefault, SIGNAL(released()), this, SLOT(defaultVertexShader()));

  syncUi();
}

void VideoSettingsWindow::synchronizePixelShaderSettings() {
  if(!video.cap(Video::FragmentShader) && !video.cap(Video::VertexShader)) {
    pixelShaderLabel->hide();
  }

  if(!video.cap(Video::FragmentShader)) {
    fragmentShaderLabel->hide();
    fragmentShaderValue->hide();
    fragmentShaderSelect->hide();
    fragmentShaderDefault->hide();
  }

  if(!video.cap(Video::VertexShader)) {
    vertexShaderLabel->hide();
    vertexShaderValue->hide();
    vertexShaderSelect->hide();
    vertexShaderDefault->hide();
  }
}

void VideoSettingsWindow::syncUi() {
  int n;

  autoHideFullscreenMenu->setChecked(config().video.autoHideFullscreenMenu);

  n = config().video.contrastAdjust;
  contrastValue->setText(string() << (n > 0 ? "+" : "") << n << "%");
  contrastSlider->setSliderPosition(n);

  n = config().video.brightnessAdjust;
  brightnessValue->setText(string() << (n > 0 ? "+" : "") << n << "%");
  brightnessSlider->setSliderPosition(n);

  n = config().video.gammaAdjust;
  gammaValue->setText(string() << (n > 0 ? "+" : "") << n << "%");
  gammaSlider->setSliderPosition(n);

  n = config().video.scanlineAdjust;
  scanlineValue->setText(string() << n << "%");
  scanlineSlider->setSliderPosition(n / 5);

  enableGammaRamp->setChecked(config().video.enableGammaRamp);

  n = config().video.cropLeft;
  cropLeftValue->setText(string() << n << "%");
  cropLeftSlider->setSliderPosition(n);

  n = config().video.cropTop;
  cropTopValue->setText(string() << n << "%");
  cropTopSlider->setSliderPosition(n);

  n = config().video.cropRight;
  cropRightValue->setText(string() << n << "%");
  cropRightSlider->setSliderPosition(n);

  n = config().video.cropBottom;
  cropBottomValue->setText(string() << n << "%");
  cropBottomSlider->setSliderPosition(n);

  fragmentShaderValue->setText(config().path.fragmentShader);
  vertexShaderValue->setText(config().path.vertexShader);
}

void VideoSettingsWindow::autoHideFullscreenMenuToggle() {
  config().video.autoHideFullscreenMenu = autoHideFullscreenMenu->isChecked();
}

void VideoSettingsWindow::contrastAdjust(int value) {
  config().video.contrastAdjust = value;
  syncUi();
  utility.updateColorFilter();
}

void VideoSettingsWindow::brightnessAdjust(int value) {
  config().video.brightnessAdjust = value;
  syncUi();
  utility.updateColorFilter();
}

void VideoSettingsWindow::gammaAdjust(int value) {
  config().video.gammaAdjust = value;
  syncUi();
  utility.updateColorFilter();
}

void VideoSettingsWindow::scanlineAdjust(int value) {
  config().video.scanlineAdjust = value * 5;
  syncUi();
  scanlineFilter.setIntensity(value * 5);
}

void VideoSettingsWindow::gammaRampToggle(int state) {
  config().video.enableGammaRamp = (state == Qt::Checked);
  syncUi();
  utility.updateColorFilter();
}

void VideoSettingsWindow::cropLeftAdjust(int state) {
  config().video.cropLeft = state;
  if(config().video.context->multiplier != 8) display.cropLeft = state;
  syncUi();
}

void VideoSettingsWindow::cropTopAdjust(int state) {
  config().video.cropTop = state;
  if(config().video.context->multiplier != 8) display.cropTop = state;
  syncUi();
}

void VideoSettingsWindow::cropRightAdjust(int state) {
  config().video.cropRight = state;
  if(config().video.context->multiplier != 8) display.cropRight = state;
  syncUi();
}

void VideoSettingsWindow::cropBottomAdjust(int state) {
  config().video.cropBottom = state;
  if(config().video.context->multiplier != 8) display.cropBottom = state;
  syncUi();
}

void VideoSettingsWindow::selectFragmentShader() {
  fileBrowser->onChange.reset();
  fileBrowser->onActivate = bind(&VideoSettingsWindow::assignFragmentShader, this);
  fileBrowser->onAccept = bind(&VideoSettingsWindow::assignFragmentShader, this);
  fileBrowser->setWindowTitle("Select Fragment Shader");
  fileBrowser->setPath(config().path.current.shader);
  fileBrowser->setNameFilters("All files (*)");
  fileBrowser->showLoad();
}

void VideoSettingsWindow::selectVertexShader() {
  fileBrowser->onChange.reset();
  fileBrowser->onActivate = bind(&VideoSettingsWindow::assignVertexShader, this);
  fileBrowser->onAccept = bind(&VideoSettingsWindow::assignVertexShader, this);
  fileBrowser->setWindowTitle("Select Vertex Shader");
  fileBrowser->setPath(config().path.current.shader);
  fileBrowser->setNameFilters("All files (*)");
  fileBrowser->showLoad();
}

void VideoSettingsWindow::defaultFragmentShader() { assignFragmentShader(""); }
void VideoSettingsWindow::defaultVertexShader() { assignVertexShader(""); }

void VideoSettingsWindow::assignFragmentShader(const string &filename) {
  if(filename == "" || QDir(filename).exists() == false) {
    config().path.fragmentShader = filename;
    if(filename != "") config().path.current.shader = dir(filename);
    syncUi();
    utility.updatePixelShader();
  }
}

void VideoSettingsWindow::assignVertexShader(const string &filename) {
  if(filename == "" || QDir(filename).exists() == false) {
    config().path.vertexShader = filename;
    if(filename != "") config().path.current.shader = dir(filename);
    syncUi();
    utility.updatePixelShader();
  }
}
