class STLoaderWindow : public ui::Window {
public:
  ui::Label   lbase;
  ui::Editbox tbase;
  ui::Button  bbase, cbase;

  ui::Label   lslotA;
  ui::Editbox tslotA;
  ui::Button  bslotA, cslotA;

  ui::Label   lslotB;
  ui::Editbox tslotB;
  ui::Button  bslotB, cslotB;

  ui::Button  load, cancel;

  bool message(uint id, uintptr_t param = 0);
  void setup();
} window_stloader;
