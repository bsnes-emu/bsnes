/**********************
 *** 0x8d: sta addr ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] dbr,aa   ; data low
  [4a] dbr,aa+1 ; data high [1]
*/
void bCPU::op_sta_addrb() {
  aa.l = op_read();                     //2
  aa.h = op_read();                     //3
  op_write(OPMODE_DBR, aa.w, regs.a.l); //4
}

void bCPU::op_sta_addrw() {
  aa.l = op_read();                         //2
  aa.h = op_read();                         //3
  op_write(OPMODE_DBR, aa.w,     regs.a.l); //4
  op_write(OPMODE_DBR, aa.w + 1, regs.a.h); //4a
}

/************************
 *** 0x9d: sta addr,x ***
 ************************
cycles:
  [1 ] pbr,pc         ; operand
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [3a] dbr,aah,aal+xl ; io [4]
  [4 ] dbr,aa+x       ; data low
  [4a] dbr,aa+x+1     ; data high [1]
*/
void bCPU::op_sta_addrxb() {
  aa.l = op_read();                                //2
  aa.h = op_read();                                //3
  cpu_c4(aa.w, aa.w + regs.x.w);                   //3a
  op_write(OPMODE_DBR, aa.w + regs.x.w, regs.a.l); //4
}

void bCPU::op_sta_addrxw() {
  aa.l = op_read();                                    //2
  aa.h = op_read();                                    //3
  cpu_c4(aa.w, aa.w + regs.x.w);                       //3a
  op_write(OPMODE_DBR, aa.w + regs.x.w,     regs.a.l); //4
  op_write(OPMODE_DBR, aa.w + regs.x.w + 1, regs.a.h); //4a
}

/********************
 *** 0x85: sta dp ***
 ********************
cycles:
  [1 ] pbr,pc   ; operand
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; data low
  [3a] 0,d+dp+1 ; data high [1]
*/
void bCPU::op_sta_dpb() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  op_write(OPMODE_DP, dp, regs.a.l); //3
}

void bCPU::op_sta_dpw() {
  dp = op_read();                        //2
  cpu_c2();                              //2a
  op_write(OPMODE_DP, dp,     regs.a.l); //3
  op_write(OPMODE_DP, dp + 1, regs.a.h); //3a
}

/**********************
 *** 0x92: sta (dp) ***
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
void bCPU::op_sta_idpb() {
  dp = op_read();                       //2
  cpu_c2();                             //2a
  aa.l = op_read(OPMODE_DP, dp);        //3
  aa.h = op_read(OPMODE_DP, dp + 1);    //4
  op_write(OPMODE_DBR, aa.w, regs.a.l); //5
}

void bCPU::op_sta_idpw() {
  dp = op_read();                           //2
  cpu_c2();                                 //2a
  aa.l = op_read(OPMODE_DP, dp);            //3
  aa.h = op_read(OPMODE_DP, dp + 1);        //4
  op_write(OPMODE_DBR, aa.w,     regs.a.l); //5
  op_write(OPMODE_DBR, aa.w + 1, regs.a.h); //5
}

/**********************
 *** 0x87: sta [dp] ***
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
void bCPU::op_sta_ildpb() {
  dp = op_read();                        //2
  cpu_c2();                              //2a
  aa.l = op_read(OPMODE_DP, dp);         //3
  aa.h = op_read(OPMODE_DP, dp + 1);     //4
  aa.b = op_read(OPMODE_DP, dp + 2);     //5
  op_write(OPMODE_LONG, aa.d, regs.a.l); //6
}

void bCPU::op_sta_ildpw() {
  dp = op_read();                            //2
  cpu_c2();                                  //2a
  aa.l = op_read(OPMODE_DP, dp);             //3
  aa.h = op_read(OPMODE_DP, dp + 1);         //4
  aa.b = op_read(OPMODE_DP, dp + 2);         //5
  op_write(OPMODE_LONG, aa.d,     regs.a.l); //6
  op_write(OPMODE_LONG, aa.d + 1, regs.a.h); //6a
}

/**********************
 *** 0x8f: sta long ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] pbr,pc+3 ; aab
  [5 ] aab,aa   ; data low
  [5a] aab,aa+1 ; data high
*/
void bCPU::op_sta_longb() {
  aa.l = op_read();                      //2
  aa.h = op_read();                      //3
  aa.b = op_read();                      //4
  op_write(OPMODE_LONG, aa.d, regs.a.l); //5
}

void bCPU::op_sta_longw() {
  aa.l = op_read();                          //2
  aa.h = op_read();                          //3
  aa.b = op_read();                          //4
  op_write(OPMODE_LONG, aa.d,     regs.a.l); //5
  op_write(OPMODE_LONG, aa.d + 1, regs.a.h); //5a
}

/************************
 *** 0x9f: sta long,x ***
 ************************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; aal
  [3 ] pbr,pc+2   ; aah
  [4 ] pbr,pc+3   ; aab
  [5 ] aab,aa+x   ; data low
  [5a] aab,aa+x+1 ; data high
*/
void bCPU::op_sta_longxb() {
  aa.l = op_read();                                 //2
  aa.h = op_read();                                 //3
  aa.b = op_read();                                 //4
  op_write(OPMODE_LONG, aa.d + regs.x.w, regs.a.l); //5
}

void bCPU::op_sta_longxw() {
  aa.l = op_read();                                     //2
  aa.h = op_read();                                     //3
  aa.b = op_read();                                     //4
  op_write(OPMODE_LONG, aa.d + regs.x.w,     regs.a.l); //5
  op_write(OPMODE_LONG, aa.d + regs.x.w + 1, regs.a.h); //5a
}

/************************
 *** 0x99: sta addr,y ***
 ************************
cycles:
  [1 ] pbr,pc         ; operand
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [3a] dbr,aah,aal+yl ; io [4]
  [4 ] dbr,aa+y       ; data low
  [4a] dbr,aa+y+1     ; data high [1]
*/
void bCPU::op_sta_addryb() {
  aa.l = op_read();                                //2
  aa.h = op_read();                                //3
  cpu_c4(aa.w, aa.w + regs.y.w);                   //3a
  op_write(OPMODE_DBR, aa.w + regs.y.w, regs.a.l); //4
}

void bCPU::op_sta_addryw() {
  aa.l = op_read();                                    //2
  aa.h = op_read();                                    //3
  cpu_c4(aa.w, aa.w + regs.y.w);                       //3a
  op_write(OPMODE_DBR, aa.w + regs.y.w,     regs.a.l); //4
  op_write(OPMODE_DBR, aa.w + regs.y.w + 1, regs.a.h); //4a
}

/**********************
 *** 0x95: sta dp,x ***
 **********************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io
  [3 ] pbr,pc+1   ; io
  [4 ] 0,d+dp+x   ; data low
  [4a] 0,d+dp+x+1 ; data high
*/
void bCPU::op_sta_dpxb() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  op_write(OPMODE_DP, dp + regs.x.w, regs.a.l); //4
}

void bCPU::op_sta_dpxw() {
  dp = op_read();                                   //2
  cpu_c2();                                         //2a
  cpu_io();                                         //3
  op_write(OPMODE_DP, dp + regs.x.w,     regs.a.l); //4
  op_write(OPMODE_DP, dp + regs.x.w + 1, regs.a.h); //4a
}

/************************
 *** 0x81: sta (dp,x) ***
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
void bCPU::op_sta_idpxb() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  aa.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //5
  op_write(OPMODE_DBR, aa.w, regs.a.l);         //6
}

void bCPU::op_sta_idpxw() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  aa.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //5
  op_write(OPMODE_DBR, aa.w,     regs.a.l);     //6
  op_write(OPMODE_DBR, aa.w + 1, regs.a.h);     //6a
}

/************************
 *** 0x91: sta (dp),y ***
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
void bCPU::op_sta_idpyb() {
  dp = op_read();                                  //2
  cpu_c2();                                        //2a
  aa.l = op_read(OPMODE_DP, dp);                   //3
  aa.h = op_read(OPMODE_DP, dp + 1);               //4
  cpu_c4(aa.w, aa.w + regs.y.w);                   //4a
  op_write(OPMODE_DBR, aa.w + regs.y.w, regs.a.l); //5
}

void bCPU::op_sta_idpyw() {
  dp = op_read();                                      //2
  cpu_c2();                                            //2a
  aa.l = op_read(OPMODE_DP, dp);                       //3
  aa.h = op_read(OPMODE_DP, dp + 1);                   //4
  cpu_c4(aa.w, aa.w + regs.y.w);                       //4a
  op_write(OPMODE_DBR, aa.w + regs.y.w,     regs.a.l); //5
  op_write(OPMODE_DBR, aa.w + regs.y.w + 1, regs.a.h); //5a
}

/************************
 *** 0x97: sta [dp],y ***
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
void bCPU::op_sta_ildpyb() {
  dp = op_read();                                   //2
  cpu_c2();                                         //2a
  aa.l = op_read(OPMODE_DP, dp);                    //3
  aa.h = op_read(OPMODE_DP, dp + 1);                //4
  aa.b = op_read(OPMODE_DP, dp + 2);                //5
  op_write(OPMODE_LONG, aa.d + regs.y.w, regs.a.l); //6
}

void bCPU::op_sta_ildpyw() {
  dp = op_read();                                       //2
  cpu_c2();                                             //2a
  aa.l = op_read(OPMODE_DP, dp);                        //3
  aa.h = op_read(OPMODE_DP, dp + 1);                    //4
  aa.b = op_read(OPMODE_DP, dp + 2);                    //5
  op_write(OPMODE_LONG, aa.d + regs.y.w,     regs.a.l); //6
  op_write(OPMODE_LONG, aa.d + regs.y.w + 1, regs.a.h); //6a
}

/**********************
 *** 0x83: sta sr,s ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; sp
  [3 ] pbr,pc+1 ; io
  [4 ] 0,s+sp   ; data low
  [4a] 0,s+sp+1 ; data high [1]
*/
void bCPU::op_sta_srb() {
  sp = op_read();                    //2
  cpu_io();                          //3
  op_write(OPMODE_SP, sp, regs.a.l); //4
}

void bCPU::op_sta_srw() {
  sp = op_read();                        //2
  cpu_io();                              //3
  op_write(OPMODE_SP, sp,     regs.a.l); //4
  op_write(OPMODE_SP, sp + 1, regs.a.h); //4a
}

/**************************
 *** 0x93: sta (sr,s),y ***
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
void bCPU::op_sta_isryb() {
  sp = op_read();                                  //2
  cpu_io();                                        //3
  aa.l = op_read(OPMODE_SP, sp);                   //4
  aa.h = op_read(OPMODE_SP, sp + 1);               //5
  cpu_io();                                        //6
  op_write(OPMODE_DBR, aa.w + regs.y.w, regs.a.l); //7
}

void bCPU::op_sta_isryw() {
  sp = op_read();                                      //2
  cpu_io();                                            //3
  aa.l = op_read(OPMODE_SP, sp);                       //4
  aa.h = op_read(OPMODE_SP, sp + 1);                   //5
  cpu_io();                                            //6
  op_write(OPMODE_DBR, aa.w + regs.y.w,     regs.a.l); //7
  op_write(OPMODE_DBR, aa.w + regs.y.w + 1, regs.a.h); //7a
}
