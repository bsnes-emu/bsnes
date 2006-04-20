class RasterSettingsWindow : public Window {
public:
Label   Header;
Editbox ScanlineDesc;
Label   PScanlineLabel;
Slider  PScanlineSlider;
Label   IScanlineLabel;
Slider  IScanlineSlider;

  bool Event(EventInfo &info);

  void Setup();
} wRasterSettings;
