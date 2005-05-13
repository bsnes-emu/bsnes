/*****************
 *** 0x1a: inc ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_incb() {
  cpu_io(); //2
  regs.a.l++;
  regs.p.n = !!(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

void bCPU::op_incw() {
  cpu_io(); //2
  regs.a.w++;
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

void bCPU::op_inc() { (regs.p.m)?op_incb():op_incw(); }

/**********************
 *** 0xee: inc addr ***
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
void bCPU::op_inc_addrb() {
  aa.l = op_read();                 //2
  aa.h = op_read();                 //3
  rd.l = op_read(OPMODE_DBR, aa.w); //4
  rd.l++;
  cpu_io();                         //5
  op_write(OPMODE_DBR, aa.w, rd.l); //6
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_inc_addrw() {
  aa.l = op_read();                     //2
  aa.h = op_read();                     //3
  rd.l = op_read(OPMODE_DBR, aa.w);     //4
  rd.h = op_read(OPMODE_DBR, aa.w + 1); //4a
  rd.w++;
  cpu_io();                             //5
  op_write(OPMODE_DBR, aa.w + 1, rd.h); //6a
  op_write(OPMODE_DBR, aa.w,     rd.l); //6
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_inc_addr() { (regs.p.m)?op_inc_addrb():op_inc_addrw(); }

/************************
 *** 0xfe: inc addr,x ***
 ************************
cycles:
  [1 ] pbr,pc         ; opcode
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [4 ] dbr,aah,aal+xl ; io
  [5 ] dbr,aa+x       ; data low
  [5a] dbr,aa+x+1     ; data high [1]
  [6 ] dbr,aa+x+1     ; io
  [7a] dbr,aa+x+1     ; data high [1]
  [7 ] dbr,aa+x       ; data low
*/
void bCPU::op_inc_addrxb() {
  aa.l = op_read();                            //2
  aa.h = op_read();                            //3
  cpu_io();                                    //4
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w); //5
  rd.l++;
  cpu_io();                                    //6
  op_write(OPMODE_DBR, aa.w + regs.x.w, rd.l); //7
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_inc_addrxw() {
  aa.l = op_read();                                //2
  aa.h = op_read();                                //3
  cpu_io();                                        //4
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);     //5
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1); //5a
  rd.w++;
  cpu_io();                                        //6
  op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h); //7a
  op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l); //7
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_inc_addrx() { (regs.p.m)?op_inc_addrxb():op_inc_addrxw(); }

/********************
 *** 0xe6: inc dp ***
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
void bCPU::op_inc_dpb() {
  dp = op_read();                //2
  cpu_c2();                      //2a
  rd.l = op_read(OPMODE_DP, dp); //3
  rd.l++;
  cpu_io();                      //4
  op_write(OPMODE_DP, dp, rd.l); //5
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_inc_dpw() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  rd.l = op_read(OPMODE_DP, dp);     //3
  rd.h = op_read(OPMODE_DP, dp + 1); //3a
  rd.w++;
  cpu_io();                          //4
  op_write(OPMODE_DP, dp + 1, rd.h); //5a
  op_write(OPMODE_DP, dp,     rd.l); //5
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_inc_dp() { (regs.p.m)?op_inc_dpb():op_inc_dpw(); }

/**********************
 *** 0xf6: inc dp,x ***
 **********************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io [2]
  [3 ] pbr,pc+1   ; io
  [4 ] 0,d+dp+x   ; data low
  [4a] 0,d+dp+x+1 ; data high [1]
  [5 ] 0,d+dp+x+1 ; io
  [6a] 0,d+dp+x+1 ; data high [1]
  [6 ] 0,d+dp+x   ; data low
*/
void bCPU::op_inc_dpxb() {
  dp = op_read();                           //2
  cpu_c2();                                 //2a
  cpu_io();                                 //3
  rd.l = op_read(OPMODE_DP, dp + regs.x.w); //4
  rd.l++;
  cpu_io();                                 //5
  op_write(OPMODE_DP, dp + regs.x.w, rd.l); //6
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_inc_dpxw() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //4a
  rd.w++;
  cpu_io();                                     //5
  op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h); //6a
  op_write(OPMODE_DP, dp + regs.x.w,     rd.l); //6
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_inc_dpx() { (regs.p.m)?op_inc_dpxb():op_inc_dpxw(); }

/*****************
 *** 0xe8: inx ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_inxb() {
  cpu_io(); //2
  regs.x.l++;
  regs.p.n = !!(regs.x.l & 0x80);
  regs.p.z = (regs.x.l == 0);
}

void bCPU::op_inxw() {
  cpu_io(); //2
  regs.x.w++;
  regs.p.n = !!(regs.x.w & 0x8000);
  regs.p.z = (regs.x.w == 0);
}

void bCPU::op_inx() { (regs.p.x)?op_inxb():op_inxw(); }

/*****************
 *** 0xc8: iny ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_inyb() {
  cpu_io(); //2
  regs.y.l++;
  regs.p.n = !!(regs.y.l & 0x80);
  regs.p.z = (regs.y.l == 0);
}

void bCPU::op_inyw() {
  cpu_io(); //2
  regs.y.w++;
  regs.p.n = !!(regs.y.w & 0x8000);
  regs.p.z = (regs.y.w == 0);
}

void bCPU::op_iny() { (regs.p.x)?op_inyb():op_inyw(); }

/*****************
 *** 0x3a: dec ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_decb() {
  cpu_io(); //2
  regs.a.l--;
  regs.p.n = !!(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

void bCPU::op_decw() {
  cpu_io(); //2
  regs.a.w--;
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

void bCPU::op_dec() { (regs.p.m)?op_decb():op_decw(); }

/**********************
 *** 0xce: dec addr ***
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
void bCPU::op_dec_addrb() {
  aa.l = op_read();                 //2
  aa.h = op_read();                 //3
  rd.l = op_read(OPMODE_DBR, aa.w); //4
  rd.l--;
  cpu_io();                         //5
  op_write(OPMODE_DBR, aa.w, rd.l); //6
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_dec_addrw() {
  aa.l = op_read();                     //2
  aa.h = op_read();                     //3
  rd.l = op_read(OPMODE_DBR, aa.w);     //4
  rd.h = op_read(OPMODE_DBR, aa.w + 1); //4a
  rd.w--;
  cpu_io();                             //5
  op_write(OPMODE_DBR, aa.w + 1, rd.h); //6a
  op_write(OPMODE_DBR, aa.w,     rd.l); //6
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_dec_addr() { (regs.p.m)?op_dec_addrb():op_dec_addrw(); }

/************************
 *** 0xde: dec addr,x ***
 ************************
cycles:
  [1 ] pbr,pc         ; opcode
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [4 ] dbr,aah,aal+xl ; io
  [5 ] dbr,aa+x       ; data low
  [5a] dbr,aa+x+1     ; data high [1]
  [6 ] dbr,aa+x+1     ; io
  [7a] dbr,aa+x+1     ; data high [1]
  [7 ] dbr,aa+x       ; data low
*/
void bCPU::op_dec_addrxb() {
  aa.l = op_read();                            //2
  aa.h = op_read();                            //3
  cpu_io();                                    //4
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w); //5
  rd.l--;
  cpu_io();                                    //6
  op_write(OPMODE_DBR, aa.w + regs.x.w, rd.l); //7
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_dec_addrxw() {
  aa.l = op_read();                                //2
  aa.h = op_read();                                //3
  cpu_io();                                        //4
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);     //5
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1); //5a
  rd.w--;
  cpu_io();                                        //6
  op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h); //7a
  op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l); //7
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_dec_addrx() { (regs.p.m)?op_dec_addrxb():op_dec_addrxw(); }

/********************
 *** 0xc6: dec dp ***
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
void bCPU::op_dec_dpb() {
  dp = op_read();                //2
  cpu_c2();                      //2a
  rd.l = op_read(OPMODE_DP, dp); //3
  rd.l--;
  cpu_io();                      //4
  op_write(OPMODE_DP, dp, rd.l); //5
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_dec_dpw() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  rd.l = op_read(OPMODE_DP, dp);     //3
  rd.h = op_read(OPMODE_DP, dp + 1); //3a
  rd.w--;
  cpu_io();                          //4
  op_write(OPMODE_DP, dp + 1, rd.h); //5a
  op_write(OPMODE_DP, dp,     rd.l); //5
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_dec_dp() { (regs.p.m)?op_dec_dpb():op_dec_dpw(); }

/**********************
 *** 0xd6: dec dp,x ***
 **********************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io [2]
  [3 ] pbr,pc+1   ; io
  [4 ] 0,d+dp+x   ; data low
  [4a] 0,d+dp+x+1 ; data high [1]
  [5 ] 0,d+dp+x+1 ; io
  [6a] 0,d+dp+x+1 ; data high [1]
  [6 ] 0,d+dp+x   ; data low
*/
void bCPU::op_dec_dpxb() {
  dp = op_read();                           //2
  cpu_c2();                                 //2a
  cpu_io();                                 //3
  rd.l = op_read(OPMODE_DP, dp + regs.x.w); //4
  rd.l--;
  cpu_io();                                 //5
  op_write(OPMODE_DP, dp + regs.x.w, rd.l); //6
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_dec_dpxw() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //4a
  rd.w--;
  cpu_io();                                     //5
  op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h); //6a
  op_write(OPMODE_DP, dp + regs.x.w,     rd.l); //6
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_dec_dpx() { (regs.p.m)?op_dec_dpxb():op_dec_dpxw(); }

/*****************
 *** 0xca: dex ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_dexb() {
  cpu_io(); //2
  regs.x.l--;
  regs.p.n = !!(regs.x.l & 0x80);
  regs.p.z = (regs.x.l == 0);
}

void bCPU::op_dexw() {
  cpu_io(); //2
  regs.x.w--;
  regs.p.n = !!(regs.x.w & 0x8000);
  regs.p.z = (regs.x.w == 0);
}

void bCPU::op_dex() { (regs.p.x)?op_dexb():op_dexw(); }

/*****************
 *** 0x88: dey ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_deyb() {
  cpu_io(); //2
  regs.y.l--;
  regs.p.n = !!(regs.y.l & 0x80);
  regs.p.z = (regs.y.l == 0);
}

void bCPU::op_deyw() {
  cpu_io(); //2
  regs.y.w--;
  regs.p.n = !!(regs.y.w & 0x8000);
  regs.p.z = (regs.y.w == 0);
}

void bCPU::op_dey() { (regs.p.x)?op_deyb():op_deyw(); }
