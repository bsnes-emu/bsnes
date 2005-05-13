void bCPU::flags_adc_b() {
int32 r = regs.a.l + rd.l + regs.p.c;
//bcd
  if(regs.p.d) {
    if(((r     ) & 15) > 9)r += 6;
    if(((r >> 4) & 15) > 9)r += 6 << 4;
  }
  regs.p.n = !!(r & 0x80);
  regs.p.v = !!(~(regs.a.l ^ rd.l) & (regs.a.l ^ r) & 0x80);
  regs.p.z = ((byte)r == 0);
  regs.p.c = (r > 0xff);
  regs.a.l = r;
}

void bCPU::flags_adc_w() {
int32 r = regs.a.w + rd.w + regs.p.c;
//bcd
  if(regs.p.d) {
    if(((r      ) & 15) > 9)r += 6;
    if(((r >>  4) & 15) > 9)r += 6 <<  4;
    if(((r >>  8) & 15) > 9)r += 6 <<  8;
    if(((r >> 12) & 15) > 9)r += 6 << 12;
  }
  regs.p.n = !!(r & 0x8000);
  regs.p.v = !!(~(regs.a.w ^ rd.w) & (regs.a.w ^ r) & 0x8000);
  regs.p.z = ((word)r == 0);
  regs.p.c = (r > 0xffff);
  regs.a.w = r;
}

/************************
 *** 0x69: adc #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+2 ; idh [1]
*/
void bCPU::op_adc_constb() {
  rd.l = op_read(); //2
  flags_adc_b();
}

void bCPU::op_adc_constw() {
  rd.l = op_read(); //2
  rd.h = op_read(); //2a
  flags_adc_w();
}

void bCPU::op_adc_const() { (regs.p.m)?op_adc_constb():op_adc_constw(); }

/**********************
 *** 0x6d: adc addr ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] dbr,aa   ; data low
  [4a] dbr,aa+1 ; data high [1]
*/
void bCPU::op_adc_addrb() {
  aa.l = op_read();                 //2
  aa.h = op_read();                 //3
  rd.l = op_read(OPMODE_DBR, aa.w); //4
  flags_adc_b();
}

void bCPU::op_adc_addrw() {
  aa.l = op_read();                     //2
  aa.h = op_read();                     //3
  rd.l = op_read(OPMODE_DBR, aa.w);     //4
  rd.h = op_read(OPMODE_DBR, aa.w + 1); //4a
  flags_adc_w();
}

void bCPU::op_adc_addr() { (regs.p.m)?op_adc_addrb():op_adc_addrw(); }

/************************
 *** 0x7d: adc addr,x ***
 ************************
cycles:
  [1 ] pbr,pc         ; operadc
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [3a] dbr,aah,aal+xl ; io [4]
  [4 ] dbr,aa+x       ; data low
  [4a] dbr,aa+x+1     ; data high [1]
*/
void bCPU::op_adc_addrxb() {
  aa.l = op_read();                            //2
  aa.h = op_read();                            //3
  cpu_c4(aa.w, aa.w + regs.x.w);               //3a
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w); //4
  flags_adc_b();
}

void bCPU::op_adc_addrxw() {
  aa.l = op_read();                                //2
  aa.h = op_read();                                //3
  cpu_c4(aa.w, aa.w + regs.x.w);                   //3a
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);     //4
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1); //4a
  flags_adc_w();
}

void bCPU::op_adc_addrx() { (regs.p.m)?op_adc_addrxb():op_adc_addrxw(); }

/********************
 *** 0x65: adc dp ***
 ********************
cycles:
  [1 ] pbr,pc   ; operadc
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; data low
  [3a] 0,d+dp+1 ; data high [1]
*/
void bCPU::op_adc_dpb() {
  dp = op_read();                //2
  cpu_c2();                      //2a
  rd.l = op_read(OPMODE_DP, dp); //3
  flags_adc_b();
}

void bCPU::op_adc_dpw() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  rd.l = op_read(OPMODE_DP, dp);     //3
  rd.h = op_read(OPMODE_DP, dp + 1); //3a
  flags_adc_w();
}

void bCPU::op_adc_dp() { (regs.p.m)?op_adc_dpb():op_adc_dpw(); }

/**********************
 *** 0x72: adc (dp) ***
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
void bCPU::op_adc_idpb() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  aa.l = op_read(OPMODE_DP, dp);     //3
  aa.h = op_read(OPMODE_DP, dp + 1); //4
  rd.l = op_read(OPMODE_DBR, aa.w);  //5
  flags_adc_b();
}

void bCPU::op_adc_idpw() {
  dp = op_read();                       //2
  cpu_c2();                             //2a
  aa.l = op_read(OPMODE_DP, dp);        //3
  aa.h = op_read(OPMODE_DP, dp + 1);    //4
  rd.l = op_read(OPMODE_DBR, aa.w);     //5
  rd.h = op_read(OPMODE_DBR, aa.w + 1); //5
  flags_adc_w();
}

void bCPU::op_adc_idp() { (regs.p.m)?op_adc_idpb():op_adc_idpw(); }

/**********************
 *** 0x67: adc [dp] ***
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
void bCPU::op_adc_ildpb() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  aa.l = op_read(OPMODE_DP, dp);     //3
  aa.h = op_read(OPMODE_DP, dp + 1); //4
  aa.b = op_read(OPMODE_DP, dp + 2); //5
  rd.l = op_read(OPMODE_LONG, aa.d); //6
  flags_adc_b();
}

void bCPU::op_adc_ildpw() {
  dp = op_read();                        //2
  cpu_c2();                              //2a
  aa.l = op_read(OPMODE_DP, dp);         //3
  aa.h = op_read(OPMODE_DP, dp + 1);     //4
  aa.b = op_read(OPMODE_DP, dp + 2);     //5
  rd.l = op_read(OPMODE_LONG, aa.d);     //6
  rd.h = op_read(OPMODE_LONG, aa.d + 1); //6a
  flags_adc_w();
}

void bCPU::op_adc_ildp() { (regs.p.m)?op_adc_ildpb():op_adc_ildpw(); }

/**********************
 *** 0x6f: adc long ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] pbr,pc+3 ; aab
  [5 ] aab,aa   ; data low
  [5a] aab,aa+1 ; data high
*/
void bCPU::op_adc_longb() {
  aa.l = op_read();                  //2
  aa.h = op_read();                  //3
  aa.b = op_read();                  //4
  rd.l = op_read(OPMODE_LONG, aa.d); //5
  flags_adc_b();
}

void bCPU::op_adc_longw() {
  aa.l = op_read();                      //2
  aa.h = op_read();                      //3
  aa.b = op_read();                      //4
  rd.l = op_read(OPMODE_LONG, aa.d);     //5
  rd.h = op_read(OPMODE_LONG, aa.d + 1); //5a
  flags_adc_w();
}

void bCPU::op_adc_long() { (regs.p.m)?op_adc_longb():op_adc_longw(); }

/************************
 *** 0x7f: adc long,x ***
 ************************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; aal
  [3 ] pbr,pc+2   ; aah
  [4 ] pbr,pc+3   ; aab
  [5 ] aab,aa+x   ; data low
  [5a] aab,aa+x+1 ; data high
*/
void bCPU::op_adc_longxb() {
  aa.l = op_read();                             //2
  aa.h = op_read();                             //3
  aa.b = op_read();                             //4
  rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w); //5
  flags_adc_b();
}

void bCPU::op_adc_longxw() {
  aa.l = op_read();                                 //2
  aa.h = op_read();                                 //3
  aa.b = op_read();                                 //4
  rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w);     //5
  rd.h = op_read(OPMODE_LONG, aa.d + regs.x.w + 1); //5a
  flags_adc_w();
}

void bCPU::op_adc_longx() { (regs.p.m)?op_adc_longxb():op_adc_longxw(); }

/************************
 *** 0x79: adc addr,y ***
 ************************
cycles:
  [1 ] pbr,pc         ; operadc
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [3a] dbr,aah,aal+yl ; io [4]
  [4 ] dbr,aa+y       ; data low
  [4a] dbr,aa+y+1     ; data high [1]
*/
void bCPU::op_adc_addryb() {
  aa.l = op_read();                            //2
  aa.h = op_read();                            //3
  cpu_c4(aa.w, aa.w + regs.y.w);               //3a
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w); //4
  flags_adc_b();
}

void bCPU::op_adc_addryw() {
  aa.l = op_read();                                //2
  aa.h = op_read();                                //3
  cpu_c4(aa.w, aa.w + regs.y.w);                   //3a
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);     //4
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1); //4a
  flags_adc_w();
}

void bCPU::op_adc_addry() { (regs.p.m)?op_adc_addryb():op_adc_addryw(); }

/**********************
 *** 0x75: adc dp,x ***
 **********************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io
  [3 ] pbr,pc+1   ; io
  [4 ] 0,d+dp+x   ; data low
  [4a] 0,d+dp+x+1 ; data high
*/
void bCPU::op_adc_dpxb() {
  dp = op_read();                           //2
  cpu_c2();                                 //2a
  cpu_io();                                 //3
  rd.l = op_read(OPMODE_DP, dp + regs.x.w); //4
  flags_adc_b();
}

void bCPU::op_adc_dpxw() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //4a
  flags_adc_w();
}

void bCPU::op_adc_dpx() { (regs.p.m)?op_adc_dpxb():op_adc_dpxw(); }

/************************
 *** 0x61: adc (dp,x) ***
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
void bCPU::op_adc_idpxb() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  aa.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //5
  rd.l = op_read(OPMODE_DBR, aa.w);             //6
  flags_adc_b();
}

void bCPU::op_adc_idpxw() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  aa.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //5
  rd.l = op_read(OPMODE_DBR, aa.w);             //6
  rd.h = op_read(OPMODE_DBR, aa.w + 1);         //6a
  flags_adc_w();
}

void bCPU::op_adc_idpx() { (regs.p.m)?op_adc_idpxb():op_adc_idpxw(); }

/************************
 *** 0x71: adc (dp),y ***
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
void bCPU::op_adc_idpyb() {
  dp = op_read();                              //2
  cpu_c2();                                    //2a
  aa.l = op_read(OPMODE_DP, dp);               //3
  aa.h = op_read(OPMODE_DP, dp + 1);           //4
  cpu_c4(aa.w, aa.w + regs.y.w);               //4a
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w); //5
  flags_adc_b();
}

void bCPU::op_adc_idpyw() {
  dp = op_read();                                  //2
  cpu_c2();                                        //2a
  aa.l = op_read(OPMODE_DP, dp);                   //3
  aa.h = op_read(OPMODE_DP, dp + 1);               //4
  cpu_c4(aa.w, aa.w + regs.y.w);                   //4a
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);     //5
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1); //5a
  flags_adc_w();
}

void bCPU::op_adc_idpy() { (regs.p.m)?op_adc_idpyb():op_adc_idpyw(); }

/************************
 *** 0x77: adc [dp],y ***
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
void bCPU::op_adc_ildpyb() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  aa.l = op_read(OPMODE_DP, dp);                //3
  aa.h = op_read(OPMODE_DP, dp + 1);            //4
  aa.b = op_read(OPMODE_DP, dp + 2);            //5
  rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w); //6
  flags_adc_b();
}

void bCPU::op_adc_ildpyw() {
  dp = op_read();                                   //2
  cpu_c2();                                         //2a
  aa.l = op_read(OPMODE_DP, dp);                    //3
  aa.h = op_read(OPMODE_DP, dp + 1);                //4
  aa.b = op_read(OPMODE_DP, dp + 2);                //5
  rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w);     //6
  rd.h = op_read(OPMODE_LONG, aa.d + regs.y.w + 1); //6a
  flags_adc_w();
}

void bCPU::op_adc_ildpy() { (regs.p.m)?op_adc_ildpyb():op_adc_ildpyw(); }

/**********************
 *** 0x63: adc sr,s ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; sp
  [3 ] pbr,pc+1 ; io
  [4 ] 0,s+sp   ; data low
  [4a] 0,s+sp+1 ; data high [1]
*/
void bCPU::op_adc_srb() {
  sp = op_read();                //2
  cpu_io();                      //3
  rd.l = op_read(OPMODE_SP, sp); //4
  flags_adc_b();
}

void bCPU::op_adc_srw() {
  sp = op_read();                    //2
  cpu_io();                          //3
  rd.l = op_read(OPMODE_SP, sp);     //4
  rd.h = op_read(OPMODE_SP, sp + 1); //4a
  flags_adc_w();
}

void bCPU::op_adc_sr() { (regs.p.m)?op_adc_srb():op_adc_srw(); }

/**************************
 *** 0x73: adc (sr,s),y ***
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
void bCPU::op_adc_isryb() {
  sp = op_read();                              //2
  cpu_io();                                    //3
  aa.l = op_read(OPMODE_SP, sp);               //4
  aa.h = op_read(OPMODE_SP, sp + 1);           //5
  cpu_io();                                    //6
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w); //7
  flags_adc_b();
}

void bCPU::op_adc_isryw() {
  sp = op_read();                                  //2
  cpu_io();                                        //3
  aa.l = op_read(OPMODE_SP, sp);                   //4
  aa.h = op_read(OPMODE_SP, sp + 1);               //5
  cpu_io();                                        //6
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);     //7
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1); //7a
  flags_adc_w();
}

void bCPU::op_adc_isry() { (regs.p.m)?op_adc_isryb():op_adc_isryw(); }
