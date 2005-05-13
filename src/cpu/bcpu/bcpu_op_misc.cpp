void bCPU::flags_bit_b() {
  regs.p.n = !!(rd.l & 0x80);
  regs.p.v = !!(rd.l & 0x40);
  regs.p.z = ((rd.l & regs.a.l) == 0);
}

void bCPU::flags_bit_w() {
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.v = !!(rd.w & 0x4000);
  regs.p.z = ((rd.w & regs.a.w) == 0);
}

/************************
 *** 0x89: bit #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+2 ; idh [1]
*/
void bCPU::op_bit_constb() {
  rd.l = op_read(); //2
  flags_bit_b();
}

void bCPU::op_bit_constw() {
  rd.l = op_read(); //2
  rd.h = op_read(); //2a
  flags_bit_w();
}

void bCPU::op_bit_const() { (regs.p.m)?op_bit_constb():op_bit_constw(); }

/**********************
 *** 0x2c: bit addr ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] dbr,aa   ; data low
  [4a] dbr,aa+1 ; data high [1]
*/
void bCPU::op_bit_addrb() {
  aa.l = op_read();                 //2
  aa.h = op_read();                 //3
  rd.l = op_read(OPMODE_DBR, aa.w); //4
  flags_bit_b();
}

void bCPU::op_bit_addrw() {
  aa.l = op_read();                     //2
  aa.h = op_read();                     //3
  rd.l = op_read(OPMODE_DBR, aa.w);     //4
  rd.h = op_read(OPMODE_DBR, aa.w + 1); //4a
  flags_bit_w();
}

void bCPU::op_bit_addr() { (regs.p.m)?op_bit_addrb():op_bit_addrw(); }

/************************
 *** 0x3c: bit addr,x ***
 ************************
cycles:
  [1 ] pbr,pc         ; operand
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [3a] dbr,aah,aal+xl ; io [4]
  [4 ] dbr,aa+x       ; data low
  [4a] dbr,aa+x+1     ; data high [1]
*/
void bCPU::op_bit_addrxb() {
  aa.l = op_read();                            //2
  aa.h = op_read();                            //3
  cpu_c4(aa.w, aa.w + regs.x.w);               //3a
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w); //4
  flags_bit_b();
}

void bCPU::op_bit_addrxw() {
  aa.l = op_read();                                //2
  aa.h = op_read();                                //3
  cpu_c4(aa.w, aa.w + regs.x.w);                   //3a
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);     //4
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1); //4a
  flags_bit_w();
}

void bCPU::op_bit_addrx() { (regs.p.m)?op_bit_addrxb():op_bit_addrxw(); }

/********************
 *** 0x24: bit dp ***
 ********************
cycles:
  [1 ] pbr,pc   ; operand
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; data low
  [3a] 0,d+dp+1 ; data high [1]
*/
void bCPU::op_bit_dpb() {
  dp = op_read();                //2
  cpu_c2();                      //2a
  rd.l = op_read(OPMODE_DP, dp); //3
  flags_bit_b();
}

void bCPU::op_bit_dpw() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  rd.l = op_read(OPMODE_DP, dp);     //3
  rd.h = op_read(OPMODE_DP, dp + 1); //3a
  flags_bit_w();
}

void bCPU::op_bit_dp() { (regs.p.m)?op_bit_dpb():op_bit_dpw(); }

/**********************
 *** 0x34: bit dp,x ***
 **********************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io
  [3 ] pbr,pc+1   ; io
  [4 ] 0,d+dp+x   ; data low
  [4a] 0,d+dp+x+1 ; data high
*/
void bCPU::op_bit_dpxb() {
  dp = op_read();                           //2
  cpu_c2();                                 //2a
  cpu_io();                                 //3
  rd.l = op_read(OPMODE_DP, dp + regs.x.w); //4
  flags_bit_b();
}

void bCPU::op_bit_dpxw() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //4a
  flags_bit_w();
}

void bCPU::op_bit_dpx() { (regs.p.m)?op_bit_dpxb():op_bit_dpxw(); }

void bCPU::flags_cpx_b() {
int32 r = regs.x.l - rd.l;
  regs.p.n = !!(r & 0x80);
  regs.p.z = ((uint8)r == 0);
  regs.p.c = (r >= 0);
}

void bCPU::flags_cpx_w() {
int32 r = regs.x.w - rd.w;
  regs.p.n = !!(r & 0x8000);
  regs.p.z = ((uint16)r == 0);
  regs.p.c = (r >= 0);
}

/************************
 *** 0xe0: cpx #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+2 ; idh [1]
*/
void bCPU::op_cpx_constb() {
  rd.l = op_read(); //2
  flags_cpx_b();
}

void bCPU::op_cpx_constw() {
  rd.l = op_read(); //2
  rd.h = op_read(); //2a
  flags_cpx_w();
}

void bCPU::op_cpx_const() { (regs.p.x)?op_cpx_constb():op_cpx_constw(); }

/**********************
 *** 0xec: cpx addr ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] dbr,aa   ; data low
  [4a] dbr,aa+1 ; data high [1]
*/
void bCPU::op_cpx_addrb() {
  aa.l = op_read();                 //2
  aa.h = op_read();                 //3
  rd.l = op_read(OPMODE_DBR, aa.w); //4
  flags_cpx_b();
}

void bCPU::op_cpx_addrw() {
  aa.l = op_read();                     //2
  aa.h = op_read();                     //3
  rd.l = op_read(OPMODE_DBR, aa.w);     //4
  rd.h = op_read(OPMODE_DBR, aa.w + 1); //4a
  flags_cpx_w();
}

void bCPU::op_cpx_addr() { (regs.p.x)?op_cpx_addrb():op_cpx_addrw(); }

/********************
 *** 0xe4: cpx dp ***
 ********************
cycles:
  [1 ] pbr,pc   ; opercmp
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; data low
  [3a] 0,d+dp+1 ; data high [1]
*/
void bCPU::op_cpx_dpb() {
  dp = op_read();                //2
  cpu_c2();                      //2a
  rd.l = op_read(OPMODE_DP, dp); //3
  flags_cpx_b();
}

void bCPU::op_cpx_dpw() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  rd.l = op_read(OPMODE_DP, dp);     //3
  rd.h = op_read(OPMODE_DP, dp + 1); //3a
  flags_cpx_w();
}

void bCPU::op_cpx_dp() { (regs.p.x)?op_cpx_dpb():op_cpx_dpw(); }

void bCPU::flags_cpy_b() {
int32 r = regs.y.l - rd.l;
  regs.p.n = !!(r & 0x80);
  regs.p.z = ((uint8)r == 0);
  regs.p.c = (r >= 0);
}

void bCPU::flags_cpy_w() {
int32 r = regs.y.w - rd.w;
  regs.p.n = !!(r & 0x8000);
  regs.p.z = ((uint16)r == 0);
  regs.p.c = (r >= 0);
}

/************************
 *** 0xc0: cpy #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+2 ; idh [1]
*/
void bCPU::op_cpy_constb() {
  rd.l = op_read(); //2
  flags_cpy_b();
}

void bCPU::op_cpy_constw() {
  rd.l = op_read(); //2
  rd.h = op_read(); //2a
  flags_cpy_w();
}

void bCPU::op_cpy_const() { (regs.p.x)?op_cpy_constb():op_cpy_constw(); }

/**********************
 *** 0xcc: cpy addr ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] dbr,aa   ; data low
  [4a] dbr,aa+1 ; data high [1]
*/
void bCPU::op_cpy_addrb() {
  aa.l = op_read();                 //2
  aa.h = op_read();                 //3
  rd.l = op_read(OPMODE_DBR, aa.w); //4
  flags_cpy_b();
}

void bCPU::op_cpy_addrw() {
  aa.l = op_read();                     //2
  aa.h = op_read();                     //3
  rd.l = op_read(OPMODE_DBR, aa.w);     //4
  rd.h = op_read(OPMODE_DBR, aa.w + 1); //4a
  flags_cpy_w();
}

void bCPU::op_cpy_addr() { (regs.p.x)?op_cpy_addrb():op_cpy_addrw(); }

/********************
 *** 0xc4: cpy dp ***
 ********************
cycles:
  [1 ] pbr,pc   ; opercmp
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; data low
  [3a] 0,d+dp+1 ; data high [1]
*/
void bCPU::op_cpy_dpb() {
  dp = op_read();                //2
  cpu_c2();                      //2a
  rd.l = op_read(OPMODE_DP, dp); //3
  flags_cpy_b();
}

void bCPU::op_cpy_dpw() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  rd.l = op_read(OPMODE_DP, dp);     //3
  rd.h = op_read(OPMODE_DP, dp + 1); //3a
  flags_cpy_w();
}

void bCPU::op_cpy_dp() { (regs.p.x)?op_cpy_dpb():op_cpy_dpw(); }

void bCPU::flags_ldx_b() {
  regs.p.n = !!(regs.x.l & 0x80);
  regs.p.z = (regs.x.l == 0);
}

void bCPU::flags_ldx_w() {
  regs.p.n = !!(regs.x.w & 0x8000);
  regs.p.z = (regs.x.w == 0);
}

/************************
 *** 0xa2: ldx #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+2 ; idh [1]
*/
void bCPU::op_ldx_constb() {
  regs.x.l = op_read(); //2
  flags_ldx_b();
}

void bCPU::op_ldx_constw() {
  regs.x.l = op_read(); //2
  regs.x.h = op_read(); //2a
  flags_ldx_w();
}

void bCPU::op_ldx_const() { (regs.p.x)?op_ldx_constb():op_ldx_constw(); }

/**********************
 *** 0xae: ldx addr ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] dbr,aa   ; data low
  [4a] dbr,aa+1 ; data high [1]
*/
void bCPU::op_ldx_addrb() {
  aa.l = op_read();                     //2
  aa.h = op_read();                     //3
  regs.x.l = op_read(OPMODE_DBR, aa.w); //4
  flags_ldx_b();
}

void bCPU::op_ldx_addrw() {
  aa.l = op_read();                         //2
  aa.h = op_read();                         //3
  regs.x.l = op_read(OPMODE_DBR, aa.w);     //4
  regs.x.h = op_read(OPMODE_DBR, aa.w + 1); //4a
  flags_ldx_w();
}

void bCPU::op_ldx_addr() { (regs.p.x)?op_ldx_addrb():op_ldx_addrw(); }

/************************
 *** 0xbe: ldx addr,y ***
 ************************
cycles:
  [1 ] pbr,pc         ; operand
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [3a] dbr,aah,aal+xl ; io [4]
  [4 ] dbr,aa+x       ; data low
  [4a] dbr,aa+x+1     ; data high [1]
*/
void bCPU::op_ldx_addryb() {
  aa.l = op_read();                                //2
  aa.h = op_read();                                //3
  cpu_c4(aa.w, aa.w + regs.y.w);                   //3a
  regs.x.l = op_read(OPMODE_DBR, aa.w + regs.y.w); //4
  flags_ldx_b();
}

void bCPU::op_ldx_addryw() {
  aa.l = op_read();                                    //2
  aa.h = op_read();                                    //3
  cpu_c4(aa.w, aa.w + regs.y.w);                       //3a
  regs.x.l = op_read(OPMODE_DBR, aa.w + regs.y.w);     //4
  regs.x.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1); //4a
  flags_ldx_w();
}

void bCPU::op_ldx_addry() { (regs.p.x)?op_ldx_addryb():op_ldx_addryw(); }

/********************
 *** 0xa6: ldx dp ***
 ********************
cycles:
  [1 ] pbr,pc   ; operand
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; data low
  [3a] 0,d+dp+1 ; data high [1]
*/
void bCPU::op_ldx_dpb() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  regs.x.l = op_read(OPMODE_DP, dp); //3
  flags_ldx_b();
}

void bCPU::op_ldx_dpw() {
  dp = op_read();                        //2
  cpu_c2();                              //2a
  regs.x.l = op_read(OPMODE_DP, dp);     //3
  regs.x.h = op_read(OPMODE_DP, dp + 1); //3a
  flags_ldx_w();
}

void bCPU::op_ldx_dp() { (regs.p.x)?op_ldx_dpb():op_ldx_dpw(); }

/**********************
 *** 0xb6: ldx dp,y ***
 **********************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io
  [3 ] pbr,pc+1   ; io
  [4 ] 0,d+dp+x   ; data low
  [4a] 0,d+dp+x+1 ; data high
*/
void bCPU::op_ldx_dpyb() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  regs.x.l = op_read(OPMODE_DP, dp + regs.y.w); //4
  flags_ldx_b();
}

void bCPU::op_ldx_dpyw() {
  dp = op_read();                                   //2
  cpu_c2();                                         //2a
  cpu_io();                                         //3
  regs.x.l = op_read(OPMODE_DP, dp + regs.y.w);     //4
  regs.x.h = op_read(OPMODE_DP, dp + regs.y.w + 1); //4a
  flags_ldx_w();
}

void bCPU::op_ldx_dpy() { (regs.p.x)?op_ldx_dpyb():op_ldx_dpyw(); }

void bCPU::flags_ldy_b() {
  regs.p.n = !!(regs.y.l & 0x80);
  regs.p.z = (regs.y.l == 0);
}

void bCPU::flags_ldy_w() {
  regs.p.n = !!(regs.y.w & 0x8000);
  regs.p.z = (regs.y.w == 0);
}

/************************
 *** 0xa0: ldy #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+2 ; idh [1]
*/
void bCPU::op_ldy_constb() {
  regs.y.l = op_read(); //2
  flags_ldy_b();
}

void bCPU::op_ldy_constw() {
  regs.y.l = op_read(); //2
  regs.y.h = op_read(); //2a
  flags_ldy_w();
}

void bCPU::op_ldy_const() { (regs.p.x)?op_ldy_constb():op_ldy_constw(); }

/**********************
 *** 0xac: ldy addr ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] dbr,aa   ; data low
  [4a] dbr,aa+1 ; data high [1]
*/
void bCPU::op_ldy_addrb() {
  aa.l = op_read();                     //2
  aa.h = op_read();                     //3
  regs.y.l = op_read(OPMODE_DBR, aa.w); //4
  flags_ldy_b();
}

void bCPU::op_ldy_addrw() {
  aa.l = op_read();                         //2
  aa.h = op_read();                         //3
  regs.y.l = op_read(OPMODE_DBR, aa.w);     //4
  regs.y.h = op_read(OPMODE_DBR, aa.w + 1); //4a
  flags_ldy_w();
}

void bCPU::op_ldy_addr() { (regs.p.x)?op_ldy_addrb():op_ldy_addrw(); }

/************************
 *** 0xbc: ldy addr,x ***
 ************************
cycles:
  [1 ] pbr,pc         ; operand
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [3a] dbr,aah,aal+xl ; io [4]
  [4 ] dbr,aa+x       ; data low
  [4a] dbr,aa+x+1     ; data high [1]
*/
void bCPU::op_ldy_addrxb() {
  aa.l = op_read();                                //2
  aa.h = op_read();                                //3
  cpu_c4(aa.w, aa.w + regs.x.w);                   //3a
  regs.y.l = op_read(OPMODE_DBR, aa.w + regs.x.w); //4
  flags_ldy_b();
}

void bCPU::op_ldy_addrxw() {
  aa.l = op_read();                                    //2
  aa.h = op_read();                                    //3
  cpu_c4(aa.w, aa.w + regs.x.w);                       //3a
  regs.y.l = op_read(OPMODE_DBR, aa.w + regs.x.w);     //4
  regs.y.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1); //4a
  flags_ldy_w();
}

void bCPU::op_ldy_addrx() { (regs.p.x)?op_ldy_addrxb():op_ldy_addrxw(); }

/********************
 *** 0xa4: ldy dp ***
 ********************
cycles:
  [1 ] pbr,pc   ; operand
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; data low
  [3a] 0,d+dp+1 ; data high [1]
*/
void bCPU::op_ldy_dpb() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  regs.y.l = op_read(OPMODE_DP, dp); //3
  flags_ldy_b();
}

void bCPU::op_ldy_dpw() {
  dp = op_read();                        //2
  cpu_c2();                              //2a
  regs.y.l = op_read(OPMODE_DP, dp);     //3
  regs.y.h = op_read(OPMODE_DP, dp + 1); //3a
  flags_ldy_w();
}

void bCPU::op_ldy_dp() { (regs.p.x)?op_ldy_dpb():op_ldy_dpw(); }

/**********************
 *** 0xb4: ldy dp,x ***
 **********************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io
  [3 ] pbr,pc+1   ; io
  [4 ] 0,d+dp+x   ; data low
  [4a] 0,d+dp+x+1 ; data high
*/
void bCPU::op_ldy_dpxb() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  regs.y.l = op_read(OPMODE_DP, dp + regs.x.w); //4
  flags_ldy_b();
}

void bCPU::op_ldy_dpxw() {
  dp = op_read();                                   //2
  cpu_c2();                                         //2a
  cpu_io();                                         //3
  regs.y.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  regs.y.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //4a
  flags_ldy_w();
}

void bCPU::op_ldy_dpx() { (regs.p.x)?op_ldy_dpxb():op_ldy_dpxw(); }

/**********************
 *** 0x8e: stx addr ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] dbr,aa   ; data low
  [4a] dbr,aa+1 ; data high [1]
*/
void bCPU::op_stx_addrb() {
  aa.l = op_read();                     //2
  aa.h = op_read();                     //3
  op_write(OPMODE_DBR, aa.w, regs.x.l); //4
}

void bCPU::op_stx_addrw() {
  aa.l = op_read();                         //2
  aa.h = op_read();                         //3
  op_write(OPMODE_DBR, aa.w,     regs.x.l); //4
  op_write(OPMODE_DBR, aa.w + 1, regs.x.h); //4a
}

void bCPU::op_stx_addr() { (regs.p.x)?op_stx_addrb():op_stx_addrw(); }

/********************
 *** 0x86: stx dp ***
 ********************
cycles:
  [1 ] pbr,pc   ; operand
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; data low
  [3a] 0,d+dp+1 ; data high [1]
*/
void bCPU::op_stx_dpb() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  op_write(OPMODE_DP, dp, regs.x.l); //3
}

void bCPU::op_stx_dpw() {
  dp = op_read();                        //2
  cpu_c2();                              //2a
  op_write(OPMODE_DP, dp,     regs.x.l); //3
  op_write(OPMODE_DP, dp + 1, regs.x.h); //3a
}

void bCPU::op_stx_dp() { (regs.p.x)?op_stx_dpb():op_stx_dpw(); }

/**********************
 *** 0x96: stx dp,y ***
 **********************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io
  [3 ] pbr,pc+1   ; io
  [4 ] 0,d+dp+x   ; data low
  [4a] 0,d+dp+x+1 ; data high
*/
void bCPU::op_stx_dpyb() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  op_write(OPMODE_DP, dp + regs.y.w, regs.x.l); //4
}

void bCPU::op_stx_dpyw() {
  dp = op_read();                                   //2
  cpu_c2();                                         //2a
  cpu_io();                                         //3
  op_write(OPMODE_DP, dp + regs.y.w,     regs.x.l); //4
  op_write(OPMODE_DP, dp + regs.y.w + 1, regs.x.h); //4a
}

void bCPU::op_stx_dpy() { (regs.p.x)?op_stx_dpyb():op_stx_dpyw(); }

/**********************
 *** 0x8c: sty addr ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] dbr,aa   ; data low
  [4a] dbr,aa+1 ; data high [1]
*/
void bCPU::op_sty_addrb() {
  aa.l = op_read();                     //2
  aa.h = op_read();                     //3
  op_write(OPMODE_DBR, aa.w, regs.y.l); //4
}

void bCPU::op_sty_addrw() {
  aa.l = op_read();                         //2
  aa.h = op_read();                         //3
  op_write(OPMODE_DBR, aa.w,     regs.y.l); //4
  op_write(OPMODE_DBR, aa.w + 1, regs.y.h); //4a
}

void bCPU::op_sty_addr() { (regs.p.x)?op_sty_addrb():op_sty_addrw(); }

/********************
 *** 0x84: sty dp ***
 ********************
cycles:
  [1 ] pbr,pc   ; operand
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; data low
  [3a] 0,d+dp+1 ; data high [1]
*/
void bCPU::op_sty_dpb() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  op_write(OPMODE_DP, dp, regs.y.l); //3
}

void bCPU::op_sty_dpw() {
  dp = op_read();                        //2
  cpu_c2();                              //2a
  op_write(OPMODE_DP, dp,     regs.y.l); //3
  op_write(OPMODE_DP, dp + 1, regs.y.h); //3a
}

void bCPU::op_sty_dp() { (regs.p.x)?op_sty_dpb():op_sty_dpw(); }

/**********************
 *** 0x94: sty dp,x ***
 **********************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io
  [3 ] pbr,pc+1   ; io
  [4 ] 0,d+dp+x   ; data low
  [4a] 0,d+dp+x+1 ; data high
*/
void bCPU::op_sty_dpxb() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  op_write(OPMODE_DP, dp + regs.x.w, regs.y.l); //4
}

void bCPU::op_sty_dpxw() {
  dp = op_read();                                   //2
  cpu_c2();                                         //2a
  cpu_io();                                         //3
  op_write(OPMODE_DP, dp + regs.x.w,     regs.y.l); //4
  op_write(OPMODE_DP, dp + regs.x.w + 1, regs.y.h); //4a
}

void bCPU::op_sty_dpx() { (regs.p.x)?op_sty_dpxb():op_sty_dpxw(); }

/**********************
 *** 0x9c: stz addr ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] dbr,aa   ; data low
  [4a] dbr,aa+1 ; data high [1]
*/
void bCPU::op_stz_addrb() {
  aa.l = op_read();              //2
  aa.h = op_read();              //3
  op_write(OPMODE_DBR, aa.w, 0); //4
}

void bCPU::op_stz_addrw() {
  aa.l = op_read();                  //2
  aa.h = op_read();                  //3
  op_write(OPMODE_DBR, aa.w,     0); //4
  op_write(OPMODE_DBR, aa.w + 1, 0); //4a
}

void bCPU::op_stz_addr() { (regs.p.m)?op_stz_addrb():op_stz_addrw(); }

/************************
 *** 0x9e: stz addr,x ***
 ************************
cycles:
  [1 ] pbr,pc         ; operand
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [3a] dbr,aah,aal+xl ; io [4]
  [4 ] dbr,aa+x       ; data low
  [4a] dbr,aa+x+1     ; data high [1]
*/
void bCPU::op_stz_addrxb() {
  aa.l = op_read();                         //2
  aa.h = op_read();                         //3
  cpu_c4(aa.w, aa.w + regs.x.w);            //3a
  op_write(OPMODE_DBR, aa.w + regs.x.w, 0); //4
}

void bCPU::op_stz_addrxw() {
  aa.l = op_read();                             //2
  aa.h = op_read();                             //3
  cpu_c4(aa.w, aa.w + regs.x.w);                //3a
  op_write(OPMODE_DBR, aa.w + regs.x.w,     0); //4
  op_write(OPMODE_DBR, aa.w + regs.x.w + 1, 0); //4a
}

void bCPU::op_stz_addrx() { (regs.p.m)?op_stz_addrxb():op_stz_addrxw(); }

/********************
 *** 0x64: stz dp ***
 ********************
cycles:
  [1 ] pbr,pc   ; operand
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; data low
  [3a] 0,d+dp+1 ; data high [1]
*/
void bCPU::op_stz_dpb() {
  dp = op_read();             //2
  cpu_c2();                   //2a
  op_write(OPMODE_DP, dp, 0); //3
}

void bCPU::op_stz_dpw() {
  dp = op_read();                 //2
  cpu_c2();                       //2a
  op_write(OPMODE_DP, dp,     0); //3
  op_write(OPMODE_DP, dp + 1, 0); //3a
}

void bCPU::op_stz_dp() { (regs.p.m)?op_stz_dpb():op_stz_dpw(); }

/**********************
 *** 0x74: stz dp,x ***
 **********************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io
  [3 ] pbr,pc+1   ; io
  [4 ] 0,d+dp+x   ; data low
  [4a] 0,d+dp+x+1 ; data high
*/
void bCPU::op_stz_dpxb() {
  dp = op_read();                        //2
  cpu_c2();                              //2a
  cpu_io();                              //3
  op_write(OPMODE_DP, dp + regs.x.w, 0); //4
}

void bCPU::op_stz_dpxw() {
  dp = op_read();                            //2
  cpu_c2();                                  //2a
  cpu_io();                                  //3
  op_write(OPMODE_DP, dp + regs.x.w,     0); //4
  op_write(OPMODE_DP, dp + regs.x.w + 1, 0); //4a
}

void bCPU::op_stz_dpx() { (regs.p.m)?op_stz_dpxb():op_stz_dpxw(); }

/*****************
 *** 0xeb: xba ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
  [3] pbr,pc+1 ; io
*/
void bCPU::op_xba() {
  cpu_io(); //2
  cpu_io(); //3
  regs.a.l ^= regs.a.h;
  regs.a.h ^= regs.a.l;
  regs.a.l ^= regs.a.h;
  regs.p.n = !!(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

/**********************
 *** 0x1c: trb addr ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] dbr,aa   ; data low
  [4a] dbr,aa+1 ; data high [1]
  [5 ] dbr,aa+1 ; io
  [6a] dbr,aa+1 ; data high [1]
  [6 ] dbr,aa   ; data low
*/
void bCPU::op_trb_addrb() {
  aa.l = op_read();                        //2
  aa.h = op_read();                        //3
  rd.l = op_read(OPMODE_DBR, aa.w);        //4
  cpu_io();                                //5
  regs.p.z = ((rd.l & regs.a.l) == 0);
  rd.l &= ~regs.a.l;
  op_write(OPMODE_DBR, aa.w, rd.l);        //6
}

void bCPU::op_trb_addrw() {
  aa.l = op_read();                        //2
  aa.h = op_read();                        //3
  rd.l = op_read(OPMODE_DBR, aa.w);        //4
  rd.h = op_read(OPMODE_DBR, aa.w + 1);    //4a
  cpu_io();                                //5
  regs.p.z = ((rd.w & regs.a.w) == 0);
  rd.w &= ~regs.a.w;
  op_write(OPMODE_DBR, aa.w + 1, rd.h);    //6a
  op_write(OPMODE_DBR, aa.w,     rd.l);    //6
}

void bCPU::op_trb_addr() { (regs.p.m)?op_trb_addrb():op_trb_addrw(); }

/********************
 *** 0x14: trb dp ***
 ********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; data low
  [3a] 0,d+dp+1 ; data high [1]
  [4 ] 0,d+dp+1 ; io
  [5a] 0,d+dp+1 ; data high [1]
  [5 ] 0,d+dp   ; data low
*/
void bCPU::op_trb_dpb() {
  dp = op_read();                          //2
  cpu_c2();                                //2a
  rd.l = op_read(OPMODE_DP, dp);           //3
  cpu_io();                                //4
  regs.p.z = ((rd.l & regs.a.l) == 0);
  rd.l &= ~regs.a.l;
  op_write(OPMODE_DP, dp, rd.l);           //5
}

void bCPU::op_trb_dpw() {
  dp = op_read();                          //2
  cpu_c2();                                //2a
  rd.l = op_read(OPMODE_DP, dp);           //3
  rd.h = op_read(OPMODE_DP, dp + 1);       //3a
  cpu_io();                                //4
  regs.p.z = ((rd.w & regs.a.w) == 0);
  rd.w &= ~regs.a.w;
  op_write(OPMODE_DP, dp + 1, rd.h);       //5a
  op_write(OPMODE_DP, dp,     rd.l);       //5
}

void bCPU::op_trb_dp() { (regs.p.m)?op_trb_dpb():op_trb_dpw(); }

/**********************
 *** 0x0c: tsb addr ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] dbr,aa   ; data low
  [4a] dbr,aa+1 ; data high [1]
  [5 ] dbr,aa+1 ; io
  [6a] dbr,aa+1 ; data high [1]
  [6 ] dbr,aa   ; data low
*/
void bCPU::op_tsb_addrb() {
  aa.l = op_read();                        //2
  aa.h = op_read();                        //3
  rd.l = op_read(OPMODE_DBR, aa.w);        //4
  cpu_io();                                //5
  regs.p.z = ((rd.l & regs.a.l) == 0);
  rd.l |= regs.a.l;
  op_write(OPMODE_DBR, aa.w, rd.l);        //6
}

void bCPU::op_tsb_addrw() {
  aa.l = op_read();                        //2
  aa.h = op_read();                        //3
  rd.l = op_read(OPMODE_DBR, aa.w);        //4
  rd.h = op_read(OPMODE_DBR, aa.w + 1);    //4a
  cpu_io();                                //5
  regs.p.z = ((rd.w & regs.a.w) == 0);
  rd.w |= regs.a.w;
  op_write(OPMODE_DBR, aa.w + 1, rd.h);    //6a
  op_write(OPMODE_DBR, aa.w,     rd.l);    //6
}

void bCPU::op_tsb_addr() { (regs.p.m)?op_tsb_addrb():op_tsb_addrw(); }

/********************
 *** 0x04: tsb dp ***
 ********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; data low
  [3a] 0,d+dp+1 ; data high [1]
  [4 ] 0,d+dp+1 ; io
  [5a] 0,d+dp+1 ; data high [1]
  [5 ] 0,d+dp   ; data low
*/
void bCPU::op_tsb_dpb() {
  dp = op_read();                          //2
  cpu_c2();                                //2a
  rd.l = op_read(OPMODE_DP, dp);           //3
  cpu_io();                                //4
  regs.p.z = ((rd.l & regs.a.l) == 0);
  rd.l |= regs.a.l;
  op_write(OPMODE_DP, dp, rd.l);           //5
}

void bCPU::op_tsb_dpw() {
  dp = op_read();                          //2
  cpu_c2();                                //2a
  rd.l = op_read(OPMODE_DP, dp);           //3
  rd.h = op_read(OPMODE_DP, dp + 1);       //3a
  cpu_io();                                //4
  regs.p.z = ((rd.w & regs.a.w) == 0);
  rd.w |= regs.a.w;
  op_write(OPMODE_DP, dp + 1, rd.h);       //5a
  op_write(OPMODE_DP, dp,     rd.l);       //5
}

void bCPU::op_tsb_dp() { (regs.p.m)?op_tsb_dpb():op_tsb_dpw(); }

/**************************
 *** 0x54: mvn src,dest ***
 **************************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; dba
  [3] pbr,pc+2 ; sba
  [4] sba,x    ; src data
  [5] dba,y    ; dest data
  [6] dba,y    ; io
  [7] dba,y    ; io
*/
void bCPU::op_mvn() {
uint32 src, dest;
uint8  sb, db;
  db = op_read(); //2
  sb = op_read(); //3
  regs.db = db;
  regs.a.w--;
  dest = (db << 16) | regs.y.w;
  src  = (sb << 16) | regs.x.w;
  rd.l = op_read(OPMODE_LONG, src);  //4
  op_write(OPMODE_LONG, dest, rd.l); //5
  if(regs.p.x) {
    regs.x.l++;
    regs.y.l++;
  } else {
    regs.x.w++;
    regs.y.w++;
  }
  if(regs.a.w != 0xffff) {
    regs.pc.w -= 3;
  }
  cpu_io(); //6
  cpu_io(); //7
}

/**************************
 *** 0x44: mvp src,dest ***
 **************************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; dba
  [3] pbr,pc+2 ; sba
  [4] sba,x    ; src data
  [5] dba,y    ; dest data
  [6] dba,y    ; io
  [7] dba,y    ; io
*/
void bCPU::op_mvp() {
uint32 src, dest;
uint8  sb, db;
  db = op_read(); //2
  sb = op_read(); //3
  regs.db = db;
  regs.a.w--;
  dest = (db << 16) | regs.y.w;
  src  = (sb << 16) | regs.x.w;
  rd.l = op_read(OPMODE_LONG, src);  //4
  op_write(OPMODE_LONG, dest, rd.l); //5
  if(regs.p.x) {
    regs.x.l--;
    regs.y.l--;
  } else {
    regs.x.w--;
    regs.y.w--;
  }
  if(regs.a.w != 0xffff) {
    regs.pc.w -= 3;
  }
  cpu_io(); //6
  cpu_io(); //7
}

/************************
 *** 0x00: brk #const ***
 ************************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; signature
* [3] 0,s      ; pbr [3]
  [4] 0,s-1    ; pch
  [5] 0,s-2    ; pcl
  [6] 0,s-3    ; p
  [7] 0,va     ; aavl
  [8] 0,va+1   ; aavh
*/
void bCPU::op_brk() {
uint16 aavw = (regs.e)?0xfffe:0xffe6;
  op_read();                                              //2
  stack_write(regs.pc.b);                                 //3
  stack_write(regs.pc.h);                                 //4
  stack_write(regs.pc.l);                                 //5
  stack_write(regs.p);                                    //6
  rd.l = op_read(OPMODE_LONG, aavw    ); //7
  rd.h = op_read(OPMODE_LONG, aavw + 1); //8
  regs.pc.b = 0x00;
  regs.pc.w = rd.w;
  regs.p.i = 1;
  regs.p.d = 0;
}

/************************
 *** 0x02: cop #const ***
 ************************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; signature
* [3] 0,s      ; pbr [3]
  [4] 0,s-1    ; pch
  [5] 0,s-2    ; pcl
  [6] 0,s-3    ; p
  [7] 0,va     ; aavl
  [8] 0,va+1   ; aavh
*/
void bCPU::op_cop() {
uint16 aavw = (regs.e)?0xfff4:0xffe4;
  op_read();                                              //2
  stack_write(regs.pc.b);                                 //3
  stack_write(regs.pc.h);                                 //4
  stack_write(regs.pc.l);                                 //5
  stack_write(regs.p);                                    //6
  rd.l = op_read(OPMODE_LONG, aavw    ); //7
  rd.h = op_read(OPMODE_LONG, aavw + 1); //8
  regs.pc.b = 0x00;
  regs.pc.w = rd.w;
  regs.p.i = 1;
  regs.p.d = 0;
}

/*****************
 *** 0xdb: stp ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
  [3] pbr,pc+1 ; io
*/
void bCPU::op_stp() {
  cpu_io();
  cpu_io();
  regs.pc.w--;
  cpustate = CPUSTATE_STP;
}

/*****************
 *** 0xcb: wai ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
  [3] pbr,pc+1 ; io
*/
void bCPU::op_wai() {
  cpu_io();
  cpu_io();
  regs.pc.w--;
  cpustate = CPUSTATE_WAI;
}

/*****************
 *** 0xfb: xce ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_xce() {
bool c = regs.p.c;
  cpu_io(); //2
  regs.p.c = regs.e;
  regs.e = c;
  if(regs.e) {
    regs.p.m = 1;
    regs.p.x = 1;
    regs.x.h = 0x00;
    regs.y.h = 0x00;
    regs.s.h = 0x01;
  }
}

/*****************
 *** 0xea: nop ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_nop() {
  cpu_io(); //2
}

/*****************
 *** 0x42: wdm ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_wdm() {
  cpu_io();
  regs.pc.w++;
}

/*****************
 *** 0x18: clc ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_clc() {
  cpu_io();
  regs.p.c = 0;
}

/*****************
 *** 0xd8: cld ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_cld() {
  cpu_io();
  regs.p.d = 0;
}

/*****************
 *** 0x58: cli ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_cli() {
  cpu_io();
  regs.p.i = 0;
}

/*****************
 *** 0xb8: clv ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_clv() {
  cpu_io();
  regs.p.v = 0;
}

/*****************
 *** 0x38: sec ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_sec() {
  cpu_io();
  regs.p.c = 1;
}

/*****************
 *** 0xf8: sed ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_sed() {
  cpu_io();
  regs.p.d = 1;
}

/*****************
 *** 0x78: sei ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_sei() {
  cpu_io();
  regs.p.i = 1;
}

/************************
 *** 0xc2: rep #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+1 ; idl
*/
void bCPU::op_rep() {
  rd.l = op_read(); //2
  cpu_io();         //2a
  regs.p &= ~rd.l;
  if(regs.e) {
    regs.p |= 0x30;
  }
}

/************************
 *** 0xe2: sep #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+1 ; idl
*/
void bCPU::op_sep() {
  rd.l = op_read(); //2
  cpu_io();         //2a
  regs.p |= rd.l;
  if(regs.e) {
    regs.p |= 0x30;
  }
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
}

/*****************
 *** 0xaa: tax ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_taxb() {
  cpu_io(); //2
  regs.x.l = regs.a.l;
  regs.p.n = !!(regs.x.l & 0x80);
  regs.p.z = (regs.x.l == 0);
}

void bCPU::op_taxw() {
  cpu_io(); //2
  regs.x.w = regs.a.w;
  regs.p.n = !!(regs.x.w & 0x8000);
  regs.p.z = (regs.x.w == 0);
}

void bCPU::op_tax() { (regs.p.x)?op_taxb():op_taxw(); }

/*****************
 *** 0xa8: tay ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_tayb() {
  cpu_io(); //2
  regs.y.l = regs.a.l;
  regs.p.n = !!(regs.y.l & 0x80);
  regs.p.z = (regs.y.l == 0);
}

void bCPU::op_tayw() {
  cpu_io(); //2
  regs.y.w = regs.a.w;
  regs.p.n = !!(regs.y.w & 0x8000);
  regs.p.z = (regs.y.w == 0);
}

void bCPU::op_tay() { (regs.p.x)?op_tayb():op_tayw(); }

/*****************
 *** 0x5b: tcd ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_tcd() {
  cpu_io(); //2
  regs.d.w = regs.a.w;
  regs.p.n = !!(regs.d.w & 0x8000);
  regs.p.z = (regs.d.w == 0);
}

/*****************
 *** 0x1b: tcs ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_tcs() {
  cpu_io(); //2
  regs.s.w = regs.a.w;
  if(regs.e) {
    regs.s.h = 0x01;
  }
}

/*****************
 *** 0x7b: tdc ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_tdc() {
  cpu_io(); //2
  regs.a.w = regs.d.w;
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

/*****************
 *** 0x3b: tsc ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_tsc() {
  cpu_io(); //2
  regs.a.w = regs.s.w;
  if(regs.e) {
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
}

/*****************
 *** 0xba: tsx ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_tsxb() {
  cpu_io(); //2
  regs.x.l = regs.s.l;
  regs.p.n = !!(regs.x.l & 0x80);
  regs.p.z = (regs.x.l == 0);
}

void bCPU::op_tsxw() {
  cpu_io(); //2
  regs.x.w = regs.s.w;
  regs.p.n = !!(regs.x.w & 0x8000);
  regs.p.z = (regs.x.w == 0);
}

void bCPU::op_tsx() { (regs.p.x)?op_tsxb():op_tsxw(); }

/*****************
 *** 0x8a: txa ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_txab() {
  cpu_io(); //2
  regs.a.l = regs.x.l;
  regs.p.n = !!(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

void bCPU::op_txaw() {
  cpu_io(); //2
  regs.a.w = regs.x.w;
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

void bCPU::op_txa() { (regs.p.m)?op_txab():op_txaw(); }

/*****************
 *** 0x9a: txs ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_txsb() {
  cpu_io(); //2
  regs.s.l = regs.x.l;
  regs.p.n = !!(regs.s.l & 0x80);
  regs.p.z = (regs.s.l == 0);
}

void bCPU::op_txsw() {
  cpu_io(); //2
  regs.s.w = regs.x.w;
  regs.p.n = !!(regs.s.w & 0x8000);
  regs.p.z = (regs.s.w == 0);
}

void bCPU::op_txs() { (regs.p.x)?op_txsb():op_txsw(); }

/*****************
 *** 0x9b: txy ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_txyb() {
  cpu_io(); //2
  regs.y.l = regs.x.l;
  regs.p.n = !!(regs.y.l & 0x80);
  regs.p.z = (regs.y.l == 0);
}

void bCPU::op_txyw() {
  cpu_io(); //2
  regs.y.w = regs.x.w;
  regs.p.n = !!(regs.y.w & 0x8000);
  regs.p.z = (regs.y.w == 0);
}

void bCPU::op_txy() { (regs.p.x)?op_txyb():op_txyw(); }

/*****************
 *** 0x98: tya ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_tyab() {
  cpu_io(); //2
  regs.a.l = regs.y.l;
  regs.p.n = !!(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

void bCPU::op_tyaw() {
  cpu_io(); //2
  regs.a.w = regs.y.w;
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

void bCPU::op_tya() { (regs.p.m)?op_tyab():op_tyaw(); }

/*****************
 *** 0xbb: tyx ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_tyxb() {
  cpu_io(); //2
  regs.x.l = regs.y.l;
  regs.p.n = !!(regs.x.l & 0x80);
  regs.p.z = (regs.x.l == 0);
}

void bCPU::op_tyxw() {
  cpu_io(); //2
  regs.x.w = regs.y.w;
  regs.p.n = !!(regs.x.w & 0x8000);
  regs.p.z = (regs.x.w == 0);
}

void bCPU::op_tyx() { (regs.p.x)?op_tyxb():op_tyxw(); }
