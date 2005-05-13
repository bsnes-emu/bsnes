void bCPU::flags_and_b() {
  regs.p.n = !!(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

void bCPU::flags_and_w() {
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

/************************
 *** 0x29: and #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+2 ; idh [1]
*/
void bCPU::op_and_constb() {
  regs.a.l &= op_read(); //2
  flags_and_b();
}

void bCPU::op_and_constw() {
  regs.a.l &= op_read(); //2
  regs.a.h &= op_read(); //2a
  flags_and_w();
}

void bCPU::op_and_const() { (regs.p.m)?op_and_constb():op_and_constw(); }

/**********************
 *** 0x2d: and addr ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] dbr,aa   ; data low
  [4a] dbr,aa+1 ; data high [1]
*/
void bCPU::op_and_addrb() {
  aa.l = op_read();                      //2
  aa.h = op_read();                      //3
  regs.a.l &= op_read(OPMODE_DBR, aa.w); //4
  flags_and_b();
}

void bCPU::op_and_addrw() {
  aa.l = op_read();                          //2
  aa.h = op_read();                          //3
  regs.a.l &= op_read(OPMODE_DBR, aa.w);     //4
  regs.a.h &= op_read(OPMODE_DBR, aa.w + 1); //4a
  flags_and_w();
}

void bCPU::op_and_addr() { (regs.p.m)?op_and_addrb():op_and_addrw(); }

/************************
 *** 0x3d: and addr,x ***
 ************************
cycles:
  [1 ] pbr,pc         ; operand
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [3a] dbr,aah,aal+xl ; io [4]
  [4 ] dbr,aa+x       ; data low
  [4a] dbr,aa+x+1     ; data high [1]
*/
void bCPU::op_and_addrxb() {
  aa.l = op_read();                                 //2
  aa.h = op_read();                                 //3
  cpu_c4(aa.w, aa.w + regs.x.w);                    //3a
  regs.a.l &= op_read(OPMODE_DBR, aa.w + regs.x.w); //4
  flags_and_b();
}

void bCPU::op_and_addrxw() {
  aa.l = op_read();                                     //2
  aa.h = op_read();                                     //3
  cpu_c4(aa.w, aa.w + regs.x.w);                        //3a
  regs.a.l &= op_read(OPMODE_DBR, aa.w + regs.x.w);     //4
  regs.a.h &= op_read(OPMODE_DBR, aa.w + regs.x.w + 1); //4a
  flags_and_w();
}

void bCPU::op_and_addrx() { (regs.p.m)?op_and_addrxb():op_and_addrxw(); }

/********************
 *** 0x25: and dp ***
 ********************
cycles:
  [1 ] pbr,pc   ; operand
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; data low
  [3a] 0,d+dp+1 ; data high [1]
*/
void bCPU::op_and_dpb() {
  dp = op_read();                     //2
  cpu_c2();                           //2a
  regs.a.l &= op_read(OPMODE_DP, dp); //3
  flags_and_b();
}

void bCPU::op_and_dpw() {
  dp = op_read();                         //2
  cpu_c2();                               //2a
  regs.a.l &= op_read(OPMODE_DP, dp);     //3
  regs.a.h &= op_read(OPMODE_DP, dp + 1); //3a
  flags_and_w();
}

void bCPU::op_and_dp() { (regs.p.m)?op_and_dpb():op_and_dpw(); }

/**********************
 *** 0x32: and (dp) ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; aal
  [4 ] 0,d+dp+1 ; aah
  [5 ] dbr,aa   ; data low
  [5a] dbr,aa+1 ; data high [1]
*/
void bCPU::op_and_idpb() {
  dp = op_read();                        //2
  cpu_c2();                              //2a
  aa.l = op_read(OPMODE_DP, dp);         //3
  aa.h = op_read(OPMODE_DP, dp + 1);     //4
  regs.a.l &= op_read(OPMODE_DBR, aa.w); //5
  flags_and_b();
}

void bCPU::op_and_idpw() {
  dp = op_read();                            //2
  cpu_c2();                                  //2a
  aa.l = op_read(OPMODE_DP, dp);             //3
  aa.h = op_read(OPMODE_DP, dp + 1);         //4
  regs.a.l &= op_read(OPMODE_DBR, aa.w);     //5
  regs.a.h &= op_read(OPMODE_DBR, aa.w + 1); //5
  flags_and_w();
}

void bCPU::op_and_idp() { (regs.p.m)?op_and_idpb():op_and_idpw(); }

/**********************
 *** 0x27: and [dp] ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; aal
  [4 ] 0,d+dp+1 ; aah
  [5 ] 0,d+dp+2 ; aab
  [6 ] aab,aa   ; data low
  [6a] aab,aa+1 ; data high [1]
*/
void bCPU::op_and_ildpb() {
  dp = op_read();                         //2
  cpu_c2();                               //2a
  aa.l = op_read(OPMODE_DP, dp);          //3
  aa.h = op_read(OPMODE_DP, dp + 1);      //4
  aa.b = op_read(OPMODE_DP, dp + 2);      //5
  regs.a.l &= op_read(OPMODE_LONG, aa.d); //6
  flags_and_b();
}

void bCPU::op_and_ildpw() {
  dp = op_read();                             //2
  cpu_c2();                                   //2a
  aa.l = op_read(OPMODE_DP, dp);              //3
  aa.h = op_read(OPMODE_DP, dp + 1);          //4
  aa.b = op_read(OPMODE_DP, dp + 2);          //5
  regs.a.l &= op_read(OPMODE_LONG, aa.d);     //6
  regs.a.h &= op_read(OPMODE_LONG, aa.d + 1); //6a
  flags_and_w();
}

void bCPU::op_and_ildp() { (regs.p.m)?op_and_ildpb():op_and_ildpw(); }

/**********************
 *** 0x2f: and long ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] pbr,pc+3 ; aab
  [5 ] aab,aa   ; data low
  [5a] aab,aa+1 ; data high
*/
void bCPU::op_and_longb() {
  aa.l = op_read();                       //2
  aa.h = op_read();                       //3
  aa.b = op_read();                       //4
  regs.a.l &= op_read(OPMODE_LONG, aa.d); //5
  flags_and_b();
}

void bCPU::op_and_longw() {
  aa.l = op_read();                           //2
  aa.h = op_read();                           //3
  aa.b = op_read();                           //4
  regs.a.l &= op_read(OPMODE_LONG, aa.d);     //5
  regs.a.h &= op_read(OPMODE_LONG, aa.d + 1); //5a
  flags_and_w();
}

void bCPU::op_and_long() { (regs.p.m)?op_and_longb():op_and_longw(); }

/************************
 *** 0x3f: and long,x ***
 ************************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; aal
  [3 ] pbr,pc+2   ; aah
  [4 ] pbr,pc+3   ; aab
  [5 ] aab,aa+x   ; data low
  [5a] aab,aa+x+1 ; data high
*/
void bCPU::op_and_longxb() {
  aa.l = op_read();                                  //2
  aa.h = op_read();                                  //3
  aa.b = op_read();                                  //4
  regs.a.l &= op_read(OPMODE_LONG, aa.d + regs.x.w); //5
  flags_and_b();
}

void bCPU::op_and_longxw() {
  aa.l = op_read();                                      //2
  aa.h = op_read();                                      //3
  aa.b = op_read();                                      //4
  regs.a.l &= op_read(OPMODE_LONG, aa.d + regs.x.w);     //5
  regs.a.h &= op_read(OPMODE_LONG, aa.d + regs.x.w + 1); //5a
  flags_and_w();
}

void bCPU::op_and_longx() { (regs.p.m)?op_and_longxb():op_and_longxw(); }

/************************
 *** 0x39: and addr,y ***
 ************************
cycles:
  [1 ] pbr,pc         ; operand
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [3a] dbr,aah,aal+yl ; io [4]
  [4 ] dbr,aa+y       ; data low
  [4a] dbr,aa+y+1     ; data high [1]
*/
void bCPU::op_and_addryb() {
  aa.l = op_read();                                 //2
  aa.h = op_read();                                 //3
  cpu_c4(aa.w, aa.w + regs.y.w);                    //3a
  regs.a.l &= op_read(OPMODE_DBR, aa.w + regs.y.w); //4
  flags_and_b();
}

void bCPU::op_and_addryw() {
  aa.l = op_read();                                     //2
  aa.h = op_read();                                     //3
  cpu_c4(aa.w, aa.w + regs.y.w);                        //3a
  regs.a.l &= op_read(OPMODE_DBR, aa.w + regs.y.w);     //4
  regs.a.h &= op_read(OPMODE_DBR, aa.w + regs.y.w + 1); //4a
  flags_and_w();
}

void bCPU::op_and_addry() { (regs.p.m)?op_and_addryb():op_and_addryw(); }

/**********************
 *** 0x35: and dp,x ***
 **********************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io
  [3 ] pbr,pc+1   ; io
  [4 ] 0,d+dp+x   ; data low
  [4a] 0,d+dp+x+1 ; data high
*/
void bCPU::op_and_dpxb() {
  dp = op_read();                                //2
  cpu_c2();                                      //2a
  cpu_io();                                      //3
  regs.a.l &= op_read(OPMODE_DP, dp + regs.x.w); //4
  flags_and_b();
}

void bCPU::op_and_dpxw() {
  dp = op_read();                                    //2
  cpu_c2();                                          //2a
  cpu_io();                                          //3
  regs.a.l &= op_read(OPMODE_DP, dp + regs.x.w);     //4
  regs.a.h &= op_read(OPMODE_DP, dp + regs.x.w + 1); //4a
  flags_and_w();
}

void bCPU::op_and_dpx() { (regs.p.m)?op_and_dpxb():op_and_dpxw(); }

/************************
 *** 0x21: and (dp,x) ***
 ************************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io [2]
  [3 ] pbr,pc+1   ; io
  [4 ] 0,d+dp+x   ; aal
  [5 ] 0,d+dp+x+1 ; aah
  [6 ] dbr,aa     ; data low
  [6a] dbr,aa+1   ; data high [1]
*/
void bCPU::op_and_idpxb() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  aa.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //5
  regs.a.l &= op_read(OPMODE_DBR, aa.w);        //6
  flags_and_b();
}

void bCPU::op_and_idpxw() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  aa.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //5
  regs.a.l &= op_read(OPMODE_DBR, aa.w);        //6
  regs.a.h &= op_read(OPMODE_DBR, aa.w + 1);    //6a
  flags_and_w();
}

void bCPU::op_and_idpx() { (regs.p.m)?op_and_idpxb():op_and_idpxw(); }

/************************
 *** 0x31: and (dp),y ***
 ************************
cycles:
  [1 ] pbr,pc         ; opcode
  [2 ] pbr,pc+1       ; dp
  [2a] pbr,pc+1       ; io [2]
  [3 ] 0,d+dp         ; aal
  [4 ] 0,d+dp+1       ; aah
  [4a] dbr,aah,aal+yl ; io [4]
  [5 ] dbr,aa+y       ; data low
  [5a] dbr,aa+y+1     ; data high [1]
*/
void bCPU::op_and_idpyb() {
  dp = op_read();                                   //2
  cpu_c2();                                         //2a
  aa.l = op_read(OPMODE_DP, dp);                    //3
  aa.h = op_read(OPMODE_DP, dp + 1);                //4
  cpu_c4(aa.w, aa.w + regs.y.w);                    //4a
  regs.a.l &= op_read(OPMODE_DBR, aa.w + regs.y.w); //5
  flags_and_b();
}

void bCPU::op_and_idpyw() {
  dp = op_read();                                       //2
  cpu_c2();                                             //2a
  aa.l = op_read(OPMODE_DP, dp);                        //3
  aa.h = op_read(OPMODE_DP, dp + 1);                    //4
  cpu_c4(aa.w, aa.w + regs.y.w);                        //4a
  regs.a.l &= op_read(OPMODE_DBR, aa.w + regs.y.w);     //5
  regs.a.h &= op_read(OPMODE_DBR, aa.w + regs.y.w + 1); //5a
  flags_and_w();
}

void bCPU::op_and_idpy() { (regs.p.m)?op_and_idpyb():op_and_idpyw(); }

/************************
 *** 0x37: and [dp],y ***
 ************************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io [2]
  [3 ] 0,d+dp     ; aal
  [4 ] 0,d+dp+1   ; aah
  [5 ] 0,d+dp+2   ; aab
  [6 ] aab,aa+y   ; data low
  [6a] aab,aa+y+1 ; data high [1]
*/
void bCPU::op_and_ildpyb() {
  dp = op_read();                                    //2
  cpu_c2();                                          //2a
  aa.l = op_read(OPMODE_DP, dp);                     //3
  aa.h = op_read(OPMODE_DP, dp + 1);                 //4
  aa.b = op_read(OPMODE_DP, dp + 2);                 //5
  regs.a.l &= op_read(OPMODE_LONG, aa.d + regs.y.w); //6
  flags_and_b();
}

void bCPU::op_and_ildpyw() {
  dp = op_read();                                        //2
  cpu_c2();                                              //2a
  aa.l = op_read(OPMODE_DP, dp);                         //3
  aa.h = op_read(OPMODE_DP, dp + 1);                     //4
  aa.b = op_read(OPMODE_DP, dp + 2);                     //5
  regs.a.l &= op_read(OPMODE_LONG, aa.d + regs.y.w);     //6
  regs.a.h &= op_read(OPMODE_LONG, aa.d + regs.y.w + 1); //6a
  flags_and_w();
}

void bCPU::op_and_ildpy() { (regs.p.m)?op_and_ildpyb():op_and_ildpyw(); }

/**********************
 *** 0x23: and sr,s ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; sp
  [3 ] pbr,pc+1 ; io
  [4 ] 0,s+sp   ; data low
  [4a] 0,s+sp+1 ; data high [1]
*/
void bCPU::op_and_srb() {
  sp = op_read();                     //2
  cpu_io();                           //3
  regs.a.l &= op_read(OPMODE_SP, sp); //4
  flags_and_b();
}

void bCPU::op_and_srw() {
  sp = op_read();                         //2
  cpu_io();                               //3
  regs.a.l &= op_read(OPMODE_SP, sp);     //4
  regs.a.h &= op_read(OPMODE_SP, sp + 1); //4a
  flags_and_w();
}

void bCPU::op_and_sr() { (regs.p.m)?op_and_srb():op_and_srw(); }

/**************************
 *** 0x33: and (sr,s),y ***
 **************************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; sp
  [3 ] pbr,pc+1   ; io
  [4 ] 0,s+sp     ; aal
  [5 ] 0,s+sp+1   ; aah
  [6 ] 0,s+sp+1   ; io
  [7 ] dbr,aa+y   ; data low
  [7a] dbr,aa+y+1 ; data high [1]
*/
void bCPU::op_and_isryb() {
  sp = op_read();                                   //2
  cpu_io();                                         //3
  aa.l = op_read(OPMODE_SP, sp);                    //4
  aa.h = op_read(OPMODE_SP, sp + 1);                //5
  cpu_io();                                         //6
  regs.a.l &= op_read(OPMODE_DBR, aa.w + regs.y.w); //7
  flags_and_b();
}

void bCPU::op_and_isryw() {
  sp = op_read();                                       //2
  cpu_io();                                             //3
  aa.l = op_read(OPMODE_SP, sp);                        //4
  aa.h = op_read(OPMODE_SP, sp + 1);                    //5
  cpu_io();                                             //6
  regs.a.l &= op_read(OPMODE_DBR, aa.w + regs.y.w);     //7
  regs.a.h &= op_read(OPMODE_DBR, aa.w + regs.y.w + 1); //7a
  flags_and_w();
}

void bCPU::op_and_isry() { (regs.p.m)?op_and_isryb():op_and_isryw(); }
