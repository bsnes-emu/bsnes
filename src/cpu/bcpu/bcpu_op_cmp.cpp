inline void bCPU::flags_cmp_b() {
int32 r = regs.a.l - rd.l;
  regs.p.n = !!(r & 0x80);
  regs.p.z = ((uint8)r == 0);
  regs.p.c = (r >= 0);
}

inline void bCPU::flags_cmp_w() {
int32 r = regs.a.w - rd.w;
  regs.p.n = !!(r & 0x8000);
  regs.p.z = ((uint16)r == 0);
  regs.p.c = (r >= 0);
}

/************************
 *** 0xc9: cmp #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+2 ; idh [1]
*/
void bCPU::op_cmp_constb() {
  rd.l = op_read(); //2
  flags_cmp_b();
}

void bCPU::op_cmp_constw() {
  rd.l = op_read(); //2
  rd.h = op_read(); //2a
  flags_cmp_w();
}

/**********************
 *** 0xcd: cmp addr ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] dbr,aa   ; data low
  [4a] dbr,aa+1 ; data high [1]
*/
void bCPU::op_cmp_addrb() {
  aa.l = op_read();                 //2
  aa.h = op_read();                 //3
  rd.l = op_read(OPMODE_DBR, aa.w); //4
  flags_cmp_b();
}

void bCPU::op_cmp_addrw() {
  aa.l = op_read();                     //2
  aa.h = op_read();                     //3
  rd.l = op_read(OPMODE_DBR, aa.w);     //4
  rd.h = op_read(OPMODE_DBR, aa.w + 1); //4a
  flags_cmp_w();
}

/************************
 *** 0xdd: cmp addr,x ***
 ************************
cycles:
  [1 ] pbr,pc         ; opercmp
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [3a] dbr,aah,aal+xl ; io [4]
  [4 ] dbr,aa+x       ; data low
  [4a] dbr,aa+x+1     ; data high [1]
*/
void bCPU::op_cmp_addrxb() {
  aa.l = op_read();                            //2
  aa.h = op_read();                            //3
  cpu_c4(aa.w, aa.w + regs.x.w);               //3a
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w); //4
  flags_cmp_b();
}

void bCPU::op_cmp_addrxw() {
  aa.l = op_read();                                //2
  aa.h = op_read();                                //3
  cpu_c4(aa.w, aa.w + regs.x.w);                   //3a
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);     //4
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1); //4a
  flags_cmp_w();
}

/********************
 *** 0xc5: cmp dp ***
 ********************
cycles:
  [1 ] pbr,pc   ; opercmp
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; data low
  [3a] 0,d+dp+1 ; data high [1]
*/
void bCPU::op_cmp_dpb() {
  dp = op_read();                //2
  cpu_c2();                      //2a
  rd.l = op_read(OPMODE_DP, dp); //3
  flags_cmp_b();
}

void bCPU::op_cmp_dpw() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  rd.l = op_read(OPMODE_DP, dp);     //3
  rd.h = op_read(OPMODE_DP, dp + 1); //3a
  flags_cmp_w();
}

/**********************
 *** 0xd2: cmp (dp) ***
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
void bCPU::op_cmp_idpb() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  aa.l = op_read(OPMODE_DP, dp);     //3
  aa.h = op_read(OPMODE_DP, dp + 1); //4
  rd.l = op_read(OPMODE_DBR, aa.w);  //5
  flags_cmp_b();
}

void bCPU::op_cmp_idpw() {
  dp = op_read();                       //2
  cpu_c2();                             //2a
  aa.l = op_read(OPMODE_DP, dp);        //3
  aa.h = op_read(OPMODE_DP, dp + 1);    //4
  rd.l = op_read(OPMODE_DBR, aa.w);     //5
  rd.h = op_read(OPMODE_DBR, aa.w + 1); //5
  flags_cmp_w();
}

/**********************
 *** 0xc7: cmp [dp] ***
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
void bCPU::op_cmp_ildpb() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  aa.l = op_read(OPMODE_DP, dp);     //3
  aa.h = op_read(OPMODE_DP, dp + 1); //4
  aa.b = op_read(OPMODE_DP, dp + 2); //5
  rd.l = op_read(OPMODE_LONG, aa.d); //6
  flags_cmp_b();
}

void bCPU::op_cmp_ildpw() {
  dp = op_read();                        //2
  cpu_c2();                              //2a
  aa.l = op_read(OPMODE_DP, dp);         //3
  aa.h = op_read(OPMODE_DP, dp + 1);     //4
  aa.b = op_read(OPMODE_DP, dp + 2);     //5
  rd.l = op_read(OPMODE_LONG, aa.d);     //6
  rd.h = op_read(OPMODE_LONG, aa.d + 1); //6a
  flags_cmp_w();
}

/**********************
 *** 0xcf: cmp long ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] pbr,pc+3 ; aab
  [5 ] aab,aa   ; data low
  [5a] aab,aa+1 ; data high
*/
void bCPU::op_cmp_longb() {
  aa.l = op_read();                  //2
  aa.h = op_read();                  //3
  aa.b = op_read();                  //4
  rd.l = op_read(OPMODE_LONG, aa.d); //5
  flags_cmp_b();
}

void bCPU::op_cmp_longw() {
  aa.l = op_read();                      //2
  aa.h = op_read();                      //3
  aa.b = op_read();                      //4
  rd.l = op_read(OPMODE_LONG, aa.d);     //5
  rd.h = op_read(OPMODE_LONG, aa.d + 1); //5a
  flags_cmp_w();
}

/************************
 *** 0xdf: cmp long,x ***
 ************************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; aal
  [3 ] pbr,pc+2   ; aah
  [4 ] pbr,pc+3   ; aab
  [5 ] aab,aa+x   ; data low
  [5a] aab,aa+x+1 ; data high
*/
void bCPU::op_cmp_longxb() {
  aa.l = op_read();                             //2
  aa.h = op_read();                             //3
  aa.b = op_read();                             //4
  rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w); //5
  flags_cmp_b();
}

void bCPU::op_cmp_longxw() {
  aa.l = op_read();                                 //2
  aa.h = op_read();                                 //3
  aa.b = op_read();                                 //4
  rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w);     //5
  rd.h = op_read(OPMODE_LONG, aa.d + regs.x.w + 1); //5a
  flags_cmp_w();
}

/************************
 *** 0xd9: cmp addr,y ***
 ************************
cycles:
  [1 ] pbr,pc         ; opercmp
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [3a] dbr,aah,aal+yl ; io [4]
  [4 ] dbr,aa+y       ; data low
  [4a] dbr,aa+y+1     ; data high [1]
*/
void bCPU::op_cmp_addryb() {
  aa.l = op_read();                            //2
  aa.h = op_read();                            //3
  cpu_c4(aa.w, aa.w + regs.y.w);               //3a
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w); //4
  flags_cmp_b();
}

void bCPU::op_cmp_addryw() {
  aa.l = op_read();                                //2
  aa.h = op_read();                                //3
  cpu_c4(aa.w, aa.w + regs.y.w);                   //3a
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);     //4
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1); //4a
  flags_cmp_w();
}

/**********************
 *** 0xd5: cmp dp,x ***
 **********************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io
  [3 ] pbr,pc+1   ; io
  [4 ] 0,d+dp+x   ; data low
  [4a] 0,d+dp+x+1 ; data high
*/
void bCPU::op_cmp_dpxb() {
  dp = op_read();                           //2
  cpu_c2();                                 //2a
  cpu_io();                                 //3
  rd.l = op_read(OPMODE_DP, dp + regs.x.w); //4
  flags_cmp_b();
}

void bCPU::op_cmp_dpxw() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //4a
  flags_cmp_w();
}

/************************
 *** 0xc1: cmp (dp,x) ***
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
void bCPU::op_cmp_idpxb() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  aa.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //5
  rd.l = op_read(OPMODE_DBR, aa.w);             //6
  flags_cmp_b();
}

void bCPU::op_cmp_idpxw() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  aa.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //5
  rd.l = op_read(OPMODE_DBR, aa.w);             //6
  rd.h = op_read(OPMODE_DBR, aa.w + 1);         //6a
  flags_cmp_w();
}

/************************
 *** 0xd1: cmp (dp),y ***
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
void bCPU::op_cmp_idpyb() {
  dp = op_read();                              //2
  cpu_c2();                                    //2a
  aa.l = op_read(OPMODE_DP, dp);               //3
  aa.h = op_read(OPMODE_DP, dp + 1);           //4
  cpu_c4(aa.w, aa.w + regs.y.w);               //4a
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w); //5
  flags_cmp_b();
}

void bCPU::op_cmp_idpyw() {
  dp = op_read();                                  //2
  cpu_c2();                                        //2a
  aa.l = op_read(OPMODE_DP, dp);                   //3
  aa.h = op_read(OPMODE_DP, dp + 1);               //4
  cpu_c4(aa.w, aa.w + regs.y.w);                   //4a
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);     //5
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1); //5a
  flags_cmp_w();
}

/************************
 *** 0xd7: cmp [dp],y ***
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
void bCPU::op_cmp_ildpyb() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  aa.l = op_read(OPMODE_DP, dp);                //3
  aa.h = op_read(OPMODE_DP, dp + 1);            //4
  aa.b = op_read(OPMODE_DP, dp + 2);            //5
  rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w); //6
  flags_cmp_b();
}

void bCPU::op_cmp_ildpyw() {
  dp = op_read();                                   //2
  cpu_c2();                                         //2a
  aa.l = op_read(OPMODE_DP, dp);                    //3
  aa.h = op_read(OPMODE_DP, dp + 1);                //4
  aa.b = op_read(OPMODE_DP, dp + 2);                //5
  rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w);     //6
  rd.h = op_read(OPMODE_LONG, aa.d + regs.y.w + 1); //6a
  flags_cmp_w();
}

/**********************
 *** 0xc3: cmp sr,s ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; sp
  [3 ] pbr,pc+1 ; io
  [4 ] 0,s+sp   ; data low
  [4a] 0,s+sp+1 ; data high [1]
*/
void bCPU::op_cmp_srb() {
  sp = op_read();                //2
  cpu_io();                      //3
  rd.l = op_read(OPMODE_SP, sp); //4
  flags_cmp_b();
}

void bCPU::op_cmp_srw() {
  sp = op_read();                    //2
  cpu_io();                          //3
  rd.l = op_read(OPMODE_SP, sp);     //4
  rd.h = op_read(OPMODE_SP, sp + 1); //4a
  flags_cmp_w();
}

/**************************
 *** 0xd3: cmp (sr,s),y ***
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
void bCPU::op_cmp_isryb() {
  sp = op_read();                              //2
  cpu_io();                                    //3
  aa.l = op_read(OPMODE_SP, sp);               //4
  aa.h = op_read(OPMODE_SP, sp + 1);           //5
  cpu_io();                                    //6
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w); //7
  flags_cmp_b();
}

void bCPU::op_cmp_isryw() {
  sp = op_read();                                  //2
  cpu_io();                                        //3
  aa.l = op_read(OPMODE_SP, sp);                   //4
  aa.h = op_read(OPMODE_SP, sp + 1);               //5
  cpu_io();                                        //6
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);     //7
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1); //7a
  flags_cmp_w();
}
