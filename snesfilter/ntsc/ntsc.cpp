#include "snes_ntsc/snes_ntsc.h"
#include "snes_ntsc/snes_ntsc.c"

#include "ntsc.moc.hpp"
#include "ntsc.moc"

void NTSCFilter::bind(configuration &config) {
  config.attach(hue         =  0.0, "snesfilter.ntsc.hue");
  config.attach(saturation  =  0.0, "snesfilter.ntsc.saturation");
  config.attach(contrast    =  0.0, "snesfilter.ntsc.contrast");
  config.attach(brightness  =  0.0, "snesfilter.ntsc.brightness");
  config.attach(sharpness   =  0.0, "snesfilter.ntsc.sharpness");
  config.attach(gamma       =  0.0, "snesfilter.ntsc.gamma");
  config.attach(resolution  =  0.0, "snesfilter.ntsc.resolution");
  config.attach(artifacts   =  0.0, "snesfilter.ntsc.artifacts");
  config.attach(fringing    =  0.0, "snesfilter.ntsc.fringing");
  config.attach(bleed       =  0.0, "snesfilter.ntsc.bleed");
  config.attach(mergeFields = true, "snesfilter.ntsc.mergeFields");
}

void NTSCFilter::size(unsigned &outwidth, unsigned &outheight, unsigned width, unsigned height) {
  outwidth  = SNES_NTSC_OUT_WIDTH(256);
  outheight = height;
}

void NTSCFilter::render(
  uint32_t *output, unsigned outpitch, const uint16_t *input, unsigned pitch,
  const unsigned *line, unsigned width, unsigned height
) {
  if(!ntsc) return;

  width = SNES_NTSC_OUT_WIDTH(256);
  burst ^= burst_toggle;

  pitch >>= 1;
  outpitch >>= 2;

  unsigned line_burst = burst;
  for(unsigned y = 0; y < height;) {
    const uint16_t *in = input + y * pitch;
    uint32_t *out = output + y * outpitch;

    //render as many lines in one snes_ntsc_blit as possible:
    //do this by determining for how many lines the width stays the same
    unsigned rheight = 1;
    unsigned rwidth  = line[y];
    while(y + rheight < height && rwidth == line[y + rheight]) rheight++;

    if(rwidth == 256) {
      snes_ntsc_blit      (ntsc, in, pitch, line_burst, rwidth, rheight, out, outpitch << 2);
    } else {
      snes_ntsc_blit_hires(ntsc, in, pitch, line_burst, rwidth, rheight, out, outpitch << 2);
    }

    line_burst = (line_burst + rheight) % 3;
    y += rheight;
  }
}

QWidget* NTSCFilter::settings() {
  if(!widget) {
    widget = new QWidget;
    widget->setWindowTitle("NTSC Filter Configuration");

    layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignTop);
    widget->setLayout(layout);

    gridLayout = new QGridLayout;
    layout->addLayout(gridLayout);

    basicSettings = new QLabel("<b>Basic settings:</b>");
    gridLayout->addWidget(basicSettings, 0, 0, 1, 3);

    hueLabel = new QLabel("Hue:");
    gridLayout->addWidget(hueLabel, 1, 0);

    hueValue = new QLabel;
    hueValue->setMinimumWidth(hueValue->fontMetrics().width("-100.0"));
    hueValue->setAlignment(Qt::AlignHCenter);
    gridLayout->addWidget(hueValue, 1, 1);

    hueSlider = new QSlider(Qt::Horizontal);
    hueSlider->setMinimum(-100);
    hueSlider->setMaximum(+100);
    gridLayout->addWidget(hueSlider, 1, 2);

    saturationLabel = new QLabel("Saturation:");
    gridLayout->addWidget(saturationLabel, 2, 0);

    saturationValue = new QLabel;
    saturationValue->setAlignment(Qt::AlignHCenter);
    gridLayout->addWidget(saturationValue, 2, 1);

    saturationSlider = new QSlider(Qt::Horizontal);
    saturationSlider->setMinimum(-100);
    saturationSlider->setMaximum(+100);
    gridLayout->addWidget(saturationSlider, 2, 2);

    contrastLabel = new QLabel("Contrast:");
    gridLayout->addWidget(contrastLabel, 3, 0);

    contrastValue = new QLabel;
    contrastValue->setAlignment(Qt::AlignHCenter);
    gridLayout->addWidget(contrastValue, 3, 1);

    contrastSlider = new QSlider(Qt::Horizontal);
    contrastSlider->setMinimum(-100);
    contrastSlider->setMaximum(+100);
    gridLayout->addWidget(contrastSlider, 3, 2);

    brightnessLabel = new QLabel("Brightness:");
    gridLayout->addWidget(brightnessLabel, 4, 0);

    brightnessValue = new QLabel;
    brightnessValue->setAlignment(Qt::AlignHCenter);
    gridLayout->addWidget(brightnessValue, 4, 1);

    brightnessSlider = new QSlider(Qt::Horizontal);
    brightnessSlider->setMinimum(-100);
    brightnessSlider->setMaximum(+100);
    gridLayout->addWidget(brightnessSlider, 4, 2);

    sharpnessLabel = new QLabel("Sharpness:");
    gridLayout->addWidget(sharpnessLabel, 5, 0);

    sharpnessValue = new QLabel;
    sharpnessValue->setAlignment(Qt::AlignHCenter);
    gridLayout->addWidget(sharpnessValue, 5, 1);

    sharpnessSlider = new QSlider(Qt::Horizontal);
    sharpnessSlider->setMinimum(-100);
    sharpnessSlider->setMaximum(+100);
    gridLayout->addWidget(sharpnessSlider, 5, 2);

    advancedSettings = new QLabel("<b>Advanced settings:</b>");
    gridLayout->addWidget(advancedSettings, 6, 0, 1, 3);

    gammaLabel = new QLabel("Gamma:");
    gridLayout->addWidget(gammaLabel, 7, 0);

    gammaValue = new QLabel;
    gammaValue->setAlignment(Qt::AlignHCenter);
    gridLayout->addWidget(gammaValue, 7, 1);

    gammaSlider = new QSlider(Qt::Horizontal);
    gammaSlider->setMinimum(-100);
    gammaSlider->setMaximum(+100);
    gridLayout->addWidget(gammaSlider, 7, 2);

    resolutionLabel = new QLabel("Resolution:");
    gridLayout->addWidget(resolutionLabel, 8, 0);

    resolutionValue = new QLabel;
    resolutionValue->setAlignment(Qt::AlignHCenter);
    gridLayout->addWidget(resolutionValue, 8, 1);

    resolutionSlider = new QSlider(Qt::Horizontal);
    resolutionSlider->setMinimum(-100);
    resolutionSlider->setMaximum(+100);
    gridLayout->addWidget(resolutionSlider, 8, 2);

    artifactsLabel = new QLabel("Artifacts:");
    gridLayout->addWidget(artifactsLabel, 9, 0);

    artifactsValue = new QLabel;
    artifactsValue->setAlignment(Qt::AlignHCenter);
    gridLayout->addWidget(artifactsValue, 9, 1);

    artifactsSlider = new QSlider(Qt::Horizontal);
    artifactsSlider->setMinimum(-100);
    artifactsSlider->setMaximum(+100);
    gridLayout->addWidget(artifactsSlider, 9, 2);

    fringingLabel = new QLabel("Fringing:");
    gridLayout->addWidget(fringingLabel, 10, 0);

    fringingValue = new QLabel;
    fringingValue->setAlignment(Qt::AlignHCenter);
    gridLayout->addWidget(fringingValue, 10, 1);

    fringingSlider = new QSlider(Qt::Horizontal);
    fringingSlider->setMinimum(-100);
    fringingSlider->setMaximum(+100);
    gridLayout->addWidget(fringingSlider, 10, 2);

    bleedLabel = new QLabel("Color bleed:");
    gridLayout->addWidget(bleedLabel, 11, 0);

    bleedValue = new QLabel;
    bleedValue->setAlignment(Qt::AlignHCenter);
    gridLayout->addWidget(bleedValue, 11, 1);

    bleedSlider = new QSlider(Qt::Horizontal);
    bleedSlider->setMinimum(-100);
    bleedSlider->setMaximum(+100);
    gridLayout->addWidget(bleedSlider, 11, 2);

    mergeFieldsBox = new QCheckBox("Merge even and odd fields to reduce flicker");
    gridLayout->addWidget(mergeFieldsBox, 12, 0, 1, 3);

    presets = new QLabel("<b>Presets:</b>");
    gridLayout->addWidget(presets, 13, 0, 1, 3);

    controlLayout = new QHBoxLayout;
    layout->addLayout(controlLayout);

    rfPreset = new QPushButton("RF");
    controlLayout->addWidget(rfPreset);

    compositePreset = new QPushButton("Composite");
    controlLayout->addWidget(compositePreset);

    svideoPreset = new QPushButton("S-Video");
    controlLayout->addWidget(svideoPreset);

    rgbPreset = new QPushButton("RGB");
    controlLayout->addWidget(rgbPreset);

    monoPreset = new QPushButton("Monochrome");
    controlLayout->addWidget(monoPreset);

    spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    spacer->setMinimumWidth(50);
    controlLayout->addWidget(spacer);

    ok = new QPushButton("Ok");
    controlLayout->addWidget(ok);

    blockSignals = true;
    loadSettingsFromConfig();
    syncUiToSettings();
    initialize();

    connect(hueSlider, SIGNAL(valueChanged(int)), this, SLOT(syncSettingsToUi()));
    connect(saturationSlider, SIGNAL(valueChanged(int)), this, SLOT(syncSettingsToUi()));
    connect(contrastSlider, SIGNAL(valueChanged(int)), this, SLOT(syncSettingsToUi()));
    connect(brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(syncSettingsToUi()));
    connect(sharpnessSlider, SIGNAL(valueChanged(int)), this, SLOT(syncSettingsToUi()));
    connect(gammaSlider, SIGNAL(valueChanged(int)), this, SLOT(syncSettingsToUi()));
    connect(resolutionSlider, SIGNAL(valueChanged(int)), this, SLOT(syncSettingsToUi()));
    connect(artifactsSlider, SIGNAL(valueChanged(int)), this, SLOT(syncSettingsToUi()));
    connect(fringingSlider, SIGNAL(valueChanged(int)), this, SLOT(syncSettingsToUi()));
    connect(bleedSlider, SIGNAL(valueChanged(int)), this, SLOT(syncSettingsToUi()));
    connect(mergeFieldsBox, SIGNAL(stateChanged(int)), this, SLOT(syncSettingsToUi()));
    connect(rfPreset, SIGNAL(released()), this, SLOT(setRfPreset()));
    connect(compositePreset, SIGNAL(released()), this, SLOT(setCompositePreset()));
    connect(svideoPreset, SIGNAL(released()), this, SLOT(setSvideoPreset()));
    connect(rgbPreset, SIGNAL(released()), this, SLOT(setRgbPreset()));
    connect(monoPreset, SIGNAL(released()), this, SLOT(setMonoPreset()));
    connect(ok, SIGNAL(released()), widget, SLOT(hide()));

    blockSignals = false;
  }

  return widget;
}

void NTSCFilter::initialize() {
  burst = 0;
  burst_toggle = (setup.merge_fields ? 0 : 1);  //don't toggle burst when fields are merged
  snes_ntsc_init(ntsc, &setup);
}

void NTSCFilter::loadSettingsFromConfig() {
  setup.hue = hue;
  setup.saturation = saturation;
  setup.contrast = contrast;
  setup.brightness = brightness;
  setup.sharpness = sharpness;

  setup.gamma = gamma;
  setup.resolution = resolution;
  setup.artifacts = artifacts;
  setup.fringing = fringing;
  setup.bleed = bleed;

  setup.merge_fields = mergeFields;
}

void NTSCFilter::syncUiToSettings() {
  blockSignals = true;

  hue = setup.hue;
  saturation = setup.saturation;
  contrast = setup.contrast;
  brightness = setup.brightness;
  sharpness = setup.sharpness;

  gamma = setup.gamma;
  resolution = setup.resolution;
  artifacts = setup.artifacts;
  fringing = setup.fringing;
  bleed = setup.bleed;

  mergeFields = setup.merge_fields;

  hueValue->setText(string() << hue);
  hueSlider->setSliderPosition(hue * 100);

  saturationValue->setText(string() << saturation);
  saturationSlider->setSliderPosition(saturation * 100);

  contrastValue->setText(string() << contrast);
  contrastSlider->setSliderPosition(contrast * 100);

  brightnessValue->setText(string() << brightness);
  brightnessSlider->setSliderPosition(brightness * 100);

  sharpnessValue->setText(string() << sharpness);
  sharpnessSlider->setSliderPosition(sharpness * 100);

  gammaValue->setText(string() << gamma);
  gammaSlider->setSliderPosition(gamma * 100);

  resolutionValue->setText(string() << resolution);
  resolutionSlider->setSliderPosition(resolution * 100);

  artifactsValue->setText(string() << artifacts);
  artifactsSlider->setSliderPosition(artifacts * 100);

  fringingValue->setText(string() << fringing);
  fringingSlider->setSliderPosition(fringing * 100);

  bleedValue->setText(string() << bleed);
  bleedSlider->setSliderPosition(bleed * 100);

  mergeFieldsBox->setChecked(mergeFields);

  blockSignals = false;
}

void NTSCFilter::syncSettingsToUi() {
  if(blockSignals) return;

  hue = hueSlider->sliderPosition() / 100.0;
  saturation = saturationSlider->sliderPosition() / 100.0;
  contrast = contrastSlider->sliderPosition() / 100.0;
  brightness = brightnessSlider->sliderPosition() / 100.0;
  sharpness = sharpnessSlider->sliderPosition() / 100.0;

  gamma = gammaSlider->sliderPosition() / 100.0;
  resolution = resolutionSlider->sliderPosition() / 100.0;
  artifacts = artifactsSlider->sliderPosition() / 100.0;
  fringing = fringingSlider->sliderPosition() / 100.0;
  bleed = bleedSlider->sliderPosition() / 100.0;

  mergeFields = mergeFieldsBox->isChecked();

  loadSettingsFromConfig();
  syncUiToSettings();
  initialize();
}

void NTSCFilter::setRfPreset() {
  static snes_ntsc_setup_t defaults;
  setup = defaults;
  syncUiToSettings();
  initialize();
}

void NTSCFilter::setCompositePreset() {
  setup = snes_ntsc_composite;
  syncUiToSettings();
  initialize();
}

void NTSCFilter::setSvideoPreset() {
  setup = snes_ntsc_svideo;
  syncUiToSettings();
  initialize();
}

void NTSCFilter::setRgbPreset() {
  setup = snes_ntsc_rgb;
  syncUiToSettings();
  initialize();
}

void NTSCFilter::setMonoPreset() {
  setup = snes_ntsc_monochrome;
  syncUiToSettings();
  initialize();
}

NTSCFilter::NTSCFilter() : widget(0) {
  ntsc = (snes_ntsc_t*)malloc(sizeof *ntsc);
  static snes_ntsc_setup_t defaults;
  setup = defaults;
  initialize();
}

NTSCFilter::~NTSCFilter() {
  if(ntsc) free(ntsc);
}
