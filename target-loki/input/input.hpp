struct AbstractInput {
  void bind();
  int16_t poll();

  string name;
  string mapping;

  HID::Device* device = nullptr;
  unsigned group = 0;
  unsigned input = 0;
};

struct InputManager : Configuration::Document {
  InputManager();
  void load();
  void unload();

  void bind();
  void poll();

  vector<HID::Device*> devices;
  vector<AbstractInput*> inputMap;
};

extern InputManager* inputManager;
