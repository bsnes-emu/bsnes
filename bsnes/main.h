void __winmain(void);

/*
  *1 - how many instructions to execute before saving sram data
       to emu_state.sram_name
*/
emustate emu_state = {
  "", "", //rom name, sram name
  1000000 //sram save tick count *1
};

debugstate debugger = {
  DEBUGMODE_DISABLED, //default debug mode
  false,              //trace enabled
  0x7e0000,           //debugger memory start pos
  true,               //debugger disassemble op
  true,               //debugger refresh mem
  true,               //debugger refresh breakpoints
  0                   //tracelog file pointer
};

videostate render = {
  512, 448,                        //resolution
  256, 224,                        //snes internal resolution
  false,                           //fullscreen
  true,                            //show menu
  1,                               //frame skip
  0,                               //frame count
  { true, true, true },            //bg1 enable
  { true, true, true },            //bg2 enable
  { true, true, true },            //bg3 enable
  { true, true, true },            //bg4 enable
  { true, true, true, true, true } //oam enable
};
