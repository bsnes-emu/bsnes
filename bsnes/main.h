void __winmain(void);

/*
  *1 - how many instructions to execute before saving sram data
       to emu_state.sram_name
*/
emustate emu_state = {
  false,    //rom loaded
  20000000, //sram save tick count *1
  "", ""    //rom name, sram name
};

videostate render = {
  512, 448,                        //display resolution
  256, 224,                        //snes internal resolution
  false,                           //fullscreen
  true,                            //show menu
  0,                               //frame skip
  0,                               //frame count
  { true, true, true },            //bg1 enable
  { true, true, true },            //bg2 enable
  { true, true, true },            //bg3 enable
  { true, true, true },            //bg4 enable
  { true, true, true, true, true } //oam enable
};
