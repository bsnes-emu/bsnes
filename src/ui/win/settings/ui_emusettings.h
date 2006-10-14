class EmuSettingsWindow : public Window {
public:
Label    PortSelectionLabel;
Label    Port1Label;
Combobox Port1;
Label    Port2Label;
Combobox Port2;
Label    Separator1;
Label    Header;
Checkbox HDMAEnable;
Checkbox OffsetEnable;
Label    Separator2;
Label    PPULayerSettings;
Checkbox BG1P0, BG1P1;
Checkbox BG2P0, BG2P1;
Checkbox BG3P0, BG3P1;
Checkbox BG4P0, BG4P1;
Checkbox OAMP0, OAMP1, OAMP2, OAMP3;
  bool Event(EventInfo &info);
  void Setup();
} wEmuSettings;
