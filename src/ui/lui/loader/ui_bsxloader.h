class BSXLoaderWindow : public ui::Window {
public:
  enum Mode {
    ModeBSX, //BS-X cartridge
    ModeBSC, //BS-X slotted cartridge
  };

  Mode mode;

  ui::Label   lbase;
  ui::Editbox tbase;
  ui::Button  bbase, cbase;

  ui::Label   lslot;
  ui::Editbox tslot;
  ui::Button  bslot, cslot;

  ui::Button  load, cancel;

  bool message(uint id, uintptr_t param = 0);
  void setup();

  BSXLoaderWindow();
} window_bsxloader;
