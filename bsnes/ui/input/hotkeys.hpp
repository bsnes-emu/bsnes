struct HotkeysGeneral : Controller {
  DigitalInput stateSave;
  DigitalInput stateLoad;
  DigitalInput stateDecrement;
  DigitalInput stateIncrement;
  DigitalInput fullscreenToggle;
  DigitalInput mouseCaptureToggle;
  DigitalInput pauseToggle;
  DigitalInput fastForward;
  DigitalInput power;
  DigitalInput reset;
  void create(const char *deviceName, const char *configName);
} hotkeysGeneral;

struct Hotkeys : ControllerPort {
} hotkeys;
