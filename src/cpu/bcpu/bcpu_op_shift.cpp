/*****************
 *** 0x0a: asl ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_aslb() {
  cpu_io(); //2
  regs.p.c = !!(regs.a.l & 0x80);
  regs.a.l <<= 1;
  regs.p.n = !!(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

void bCPU::op_aslw() {
  cpu_io(); //2
  regs.p.c = !!(regs.a.w & 0x8000);
  regs.a.w <<= 1;
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

void bCPU::op_asl() { (regs.p.m)?op_aslb():op_aslw(); }

/**********************
 *** 0x0e: asl addr ***
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
void bCPU::op_asl_addrb() {
  aa.l = op_read();                 //2
  aa.h = op_read();                 //3
  rd.l = op_read(OPMODE_DBR, aa.w); //4
  regs.p.c = !!(rd.l & 0x80);
  rd.l <<= 1;
  cpu_io();                         //5
  op_write(OPMODE_DBR, aa.w, rd.l); //6
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_asl_addrw() {
  aa.l = op_read();                     //2
  aa.h = op_read();                     //3
  rd.l = op_read(OPMODE_DBR, aa.w);     //4
  rd.h = op_read(OPMODE_DBR, aa.w + 1); //4a
  regs.p.c = !!(rd.w & 0x8000);
  rd.w <<= 1;
  cpu_io();                             //5
  op_write(OPMODE_DBR, aa.w + 1, rd.h); //6a
  op_write(OPMODE_DBR, aa.w,     rd.l); //6
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_asl_addr() { (regs.p.m)?op_asl_addrb():op_asl_addrw(); }

/************************
 *** 0x1e: asl addr,x ***
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
void bCPU::op_asl_addrxb() {
  aa.l = op_read();                            //2
  aa.h = op_read();                            //3
  cpu_io();                                    //4
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w); //5
  regs.p.c = !!(rd.l & 0x80);
  rd.l <<= 1;
  cpu_io();                                    //6
  op_write(OPMODE_DBR, aa.w + regs.x.w, rd.l); //7
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_asl_addrxw() {
  aa.l = op_read();                                //2
  aa.h = op_read();                                //3
  cpu_io();                                        //4
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);     //5
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1); //5a
  regs.p.c = !!(rd.w & 0x8000);
  rd.w <<= 1;
  cpu_io();                                        //6
  op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h); //7a
  op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l); //7
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_asl_addrx() { (regs.p.m)?op_asl_addrxb():op_asl_addrxw(); }

/********************
 *** 0x06: asl dp ***
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
void bCPU::op_asl_dpb() {
  dp = op_read();                //2
  cpu_c2();                      //2a
  rd.l = op_read(OPMODE_DP, dp); //3
  regs.p.c = !!(rd.l & 0x80);
  rd.l <<= 1;
  cpu_io();                      //4
  op_write(OPMODE_DP, dp, rd.l); //5
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_asl_dpw() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  rd.l = op_read(OPMODE_DP, dp);     //3
  rd.h = op_read(OPMODE_DP, dp + 1); //3a
  regs.p.c = !!(rd.w & 0x8000);
  rd.w <<= 1;
  cpu_io();                          //4
  op_write(OPMODE_DP, dp + 1, rd.h); //5a
  op_write(OPMODE_DP, dp,     rd.l); //5
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_asl_dp() { (regs.p.m)?op_asl_dpb():op_asl_dpw(); }

/**********************
 *** 0x16: asl dp,x ***
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
void bCPU::op_asl_dpxb() {
  dp = op_read();                           //2
  cpu_c2();                                 //2a
  cpu_io();                                 //3
  rd.l = op_read(OPMODE_DP, dp + regs.x.w); //4
  regs.p.c = !!(rd.l & 0x80);
  rd.l <<= 1;
  cpu_io();                                 //5
  op_write(OPMODE_DP, dp + regs.x.w, rd.l); //6
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_asl_dpxw() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //4a
  regs.p.c = !!(rd.w & 0x8000);
  rd.w <<= 1;
  cpu_io();                                     //5
  op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h); //6a
  op_write(OPMODE_DP, dp + regs.x.w,     rd.l); //6
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_asl_dpx() { (regs.p.m)?op_asl_dpxb():op_asl_dpxw(); }

/*****************
 *** 0x4a: lsr ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_lsrb() {
  cpu_io(); //2
  regs.p.c = regs.a.l & 1;
  regs.a.l >>= 1;
  regs.p.n = !!(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

void bCPU::op_lsrw() {
  cpu_io(); //2
  regs.p.c = regs.a.w & 1;
  regs.a.w >>= 1;
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

void bCPU::op_lsr() { (regs.p.m)?op_lsrb():op_lsrw(); }

/**********************
 *** 0x4e: lsr addr ***
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
void bCPU::op_lsr_addrb() {
  aa.l = op_read();                 //2
  aa.h = op_read();                 //3
  rd.l = op_read(OPMODE_DBR, aa.w); //4
  regs.p.c = rd.l & 1;
  rd.l >>= 1;
  cpu_io();                         //5
  op_write(OPMODE_DBR, aa.w, rd.l); //6
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_lsr_addrw() {
  aa.l = op_read();                     //2
  aa.h = op_read();                     //3
  rd.l = op_read(OPMODE_DBR, aa.w);     //4
  rd.h = op_read(OPMODE_DBR, aa.w + 1); //4a
  regs.p.c = rd.w & 1;
  rd.w >>= 1;
  cpu_io();                             //5
  op_write(OPMODE_DBR, aa.w + 1, rd.h); //6a
  op_write(OPMODE_DBR, aa.w,     rd.l); //6
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_lsr_addr() { (regs.p.m)?op_lsr_addrb():op_lsr_addrw(); }

/************************
 *** 0x5e: lsr addr,x ***
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
void bCPU::op_lsr_addrxb() {
  aa.l = op_read();                            //2
  aa.h = op_read();                            //3
  cpu_io();                                    //4
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w); //5
  regs.p.c = rd.l & 1;
  rd.l >>= 1;
  cpu_io();                                    //6
  op_write(OPMODE_DBR, aa.w + regs.x.w, rd.l); //7
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_lsr_addrxw() {
  aa.l = op_read();                                //2
  aa.h = op_read();                                //3
  cpu_io();                                        //4
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);     //5
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1); //5a
  regs.p.c = rd.w & 1;
  rd.w >>= 1;
  cpu_io();                                        //6
  op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h); //7a
  op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l); //7
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_lsr_addrx() { (regs.p.m)?op_lsr_addrxb():op_lsr_addrxw(); }

/********************
 *** 0x46: lsr dp ***
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
void bCPU::op_lsr_dpb() {
  dp = op_read();                //2
  cpu_c2();                      //2a
  rd.l = op_read(OPMODE_DP, dp); //3
  regs.p.c = rd.l & 1;
  rd.l >>= 1;
  cpu_io();                      //4
  op_write(OPMODE_DP, dp, rd.l); //5
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_lsr_dpw() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  rd.l = op_read(OPMODE_DP, dp);     //3
  rd.h = op_read(OPMODE_DP, dp + 1); //3a
  regs.p.c = rd.w & 1;
  rd.w >>= 1;
  cpu_io();                          //4
  op_write(OPMODE_DP, dp + 1, rd.h); //5a
  op_write(OPMODE_DP, dp,     rd.l); //5
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_lsr_dp() { (regs.p.m)?op_lsr_dpb():op_lsr_dpw(); }

/**********************
 *** 0x56: lsr dp,x ***
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
void bCPU::op_lsr_dpxb() {
  dp = op_read();                           //2
  cpu_c2();                                 //2a
  cpu_io();                                 //3
  rd.l = op_read(OPMODE_DP, dp + regs.x.w); //4
  regs.p.c = rd.l & 1;
  rd.l >>= 1;
  cpu_io();                                 //5
  op_write(OPMODE_DP, dp + regs.x.w, rd.l); //6
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_lsr_dpxw() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //4a
  regs.p.c = rd.w & 1;
  rd.w >>= 1;
  cpu_io();                                     //5
  op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h); //6a
  op_write(OPMODE_DP, dp + regs.x.w,     rd.l); //6
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_lsr_dpx() { (regs.p.m)?op_lsr_dpxb():op_lsr_dpxw(); }

/*****************
 *** 0x2a: rol ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_rolb() {
uint8 c = regs.p.c;
  cpu_io(); //2
  regs.p.c = !!(regs.a.l & 0x80);
  regs.a.l <<= 1;
  regs.a.l |= c;
  regs.p.n = !!(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

void bCPU::op_rolw() {
uint16 c = regs.p.c;
  cpu_io(); //2
  regs.p.c = !!(regs.a.w & 0x8000);
  regs.a.w <<= 1;
  regs.a.w |= c;
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

void bCPU::op_rol() { (regs.p.m)?op_rolb():op_rolw(); }

/**********************
 *** 0x2e: rol addr ***
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
void bCPU::op_rol_addrb() {
uint8 c = regs.p.c;
  aa.l = op_read();                 //2
  aa.h = op_read();                 //3
  rd.l = op_read(OPMODE_DBR, aa.w); //4
  regs.p.c = !!(rd.l & 0x80);
  rd.l <<= 1;
  rd.l |= c;
  cpu_io();                         //5
  op_write(OPMODE_DBR, aa.w, rd.l); //6
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_rol_addrw() {
uint16 c = regs.p.c;
  aa.l = op_read();                     //2
  aa.h = op_read();                     //3
  rd.l = op_read(OPMODE_DBR, aa.w);     //4
  rd.h = op_read(OPMODE_DBR, aa.w + 1); //4a
  regs.p.c = !!(rd.w & 0x8000);
  rd.w <<= 1;
  rd.w |= c;
  cpu_io();                             //5
  op_write(OPMODE_DBR, aa.w + 1, rd.h); //6a
  op_write(OPMODE_DBR, aa.w,     rd.l); //6
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_rol_addr() { (regs.p.m)?op_rol_addrb():op_rol_addrw(); }

/************************
 *** 0x3e: rol addr,x ***
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
void bCPU::op_rol_addrxb() {
uint8 c = regs.p.c;
  aa.l = op_read();                            //2
  aa.h = op_read();                            //3
  cpu_io();                                    //4
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w); //5
  regs.p.c = !!(rd.l & 0x80);
  rd.l <<= 1;
  rd.l |= c;
  cpu_io();                                    //6
  op_write(OPMODE_DBR, aa.w + regs.x.w, rd.l); //7
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_rol_addrxw() {
uint16 c = regs.p.c;
  aa.l = op_read();                                //2
  aa.h = op_read();                                //3
  cpu_io();                                        //4
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);     //5
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1); //5a
  regs.p.c = !!(rd.w & 0x8000);
  rd.w <<= 1;
  rd.w |= c;
  cpu_io();                                        //6
  op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h); //7a
  op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l); //7
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_rol_addrx() { (regs.p.m)?op_rol_addrxb():op_rol_addrxw(); }

/********************
 *** 0x26: rol dp ***
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
void bCPU::op_rol_dpb() {
uint8 c = regs.p.c;
  dp = op_read();                //2
  cpu_c2();                      //2a
  rd.l = op_read(OPMODE_DP, dp); //3
  regs.p.c = !!(rd.l & 0x80);
  rd.l <<= 1;
  rd.l |= c;
  cpu_io();                      //4
  op_write(OPMODE_DP, dp, rd.l); //5
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_rol_dpw() {
uint16 c = regs.p.c;
  dp = op_read();                    //2
  cpu_c2();                          //2a
  rd.l = op_read(OPMODE_DP, dp);     //3
  rd.h = op_read(OPMODE_DP, dp + 1); //3a
  regs.p.c = !!(rd.w & 0x8000);
  rd.w <<= 1;
  rd.w |= c;
  cpu_io();                          //4
  op_write(OPMODE_DP, dp + 1, rd.h); //5a
  op_write(OPMODE_DP, dp,     rd.l); //5
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_rol_dp() { (regs.p.m)?op_rol_dpb():op_rol_dpw(); }

/**********************
 *** 0x36: rol dp,x ***
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
void bCPU::op_rol_dpxb() {
uint8 c = regs.p.c;
  dp = op_read();                           //2
  cpu_c2();                                 //2a
  cpu_io();                                 //3
  rd.l = op_read(OPMODE_DP, dp + regs.x.w); //4
  regs.p.c = !!(rd.l & 0x80);
  rd.l <<= 1;
  rd.l |= c;
  cpu_io();                                 //5
  op_write(OPMODE_DP, dp + regs.x.w, rd.l); //6
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_rol_dpxw() {
uint16 c = regs.p.c;
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //4a
  regs.p.c = !!(rd.w & 0x8000);
  rd.w <<= 1;
  rd.w |= c;
  cpu_io();                                     //5
  op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h); //6a
  op_write(OPMODE_DP, dp + regs.x.w,     rd.l); //6
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_rol_dpx() { (regs.p.m)?op_rol_dpxb():op_rol_dpxw(); }

/*****************
 *** 0x6a: ror ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/
void bCPU::op_rorb() {
uint8 c = (regs.p.c)?0x80:0;
  cpu_io(); //2
  regs.p.c = regs.a.l & 1;
  regs.a.l >>= 1;
  regs.a.l |= c;
  regs.p.n = !!(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

void bCPU::op_rorw() {
uint16 c = (regs.p.c)?0x8000:0;
  cpu_io(); //2
  regs.p.c = regs.a.w & 1;
  regs.a.w >>= 1;
  regs.a.w |= c;
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

void bCPU::op_ror() { (regs.p.m)?op_rorb():op_rorw(); }

/**********************
 *** 0x6e: ror addr ***
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
void bCPU::op_ror_addrb() {
uint8 c = (regs.p.c)?0x80:0;
  aa.l = op_read();                 //2
  aa.h = op_read();                 //3
  rd.l = op_read(OPMODE_DBR, aa.w); //4
  regs.p.c = rd.l & 1;
  rd.l >>= 1;
  rd.l |= c;
  cpu_io();                         //5
  op_write(OPMODE_DBR, aa.w, rd.l); //6
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_ror_addrw() {
uint16 c = (regs.p.c)?0x8000:0;
  aa.l = op_read();                     //2
  aa.h = op_read();                     //3
  rd.l = op_read(OPMODE_DBR, aa.w);     //4
  rd.h = op_read(OPMODE_DBR, aa.w + 1); //4a
  regs.p.c = rd.w & 1;
  rd.w >>= 1;
  rd.w |= c;
  cpu_io();                             //5
  op_write(OPMODE_DBR, aa.w + 1, rd.h); //6a
  op_write(OPMODE_DBR, aa.w,     rd.l); //6
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_ror_addr() { (regs.p.m)?op_ror_addrb():op_ror_addrw(); }

/************************
 *** 0x7e: ror addr,x ***
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
void bCPU::op_ror_addrxb() {
uint8 c = (regs.p.c)?0x80:0;
  aa.l = op_read();                            //2
  aa.h = op_read();                            //3
  cpu_io();                                    //4
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w); //5
  regs.p.c = rd.l & 1;
  rd.l >>= 1;
  rd.l |= c;
  cpu_io();                                    //6
  op_write(OPMODE_DBR, aa.w + regs.x.w, rd.l); //7
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_ror_addrxw() {
uint16 c = (regs.p.c)?0x8000:0;
  aa.l = op_read();                                //2
  aa.h = op_read();                                //3
  cpu_io();                                        //4
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);     //5
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1); //5a
  regs.p.c = rd.w & 1;
  rd.w >>= 1;
  rd.w |= c;
  cpu_io();                                        //6
  op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h); //7a
  op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l); //7
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_ror_addrx() { (regs.p.m)?op_ror_addrxb():op_ror_addrxw(); }

/********************
 *** 0x66: ror dp ***
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
void bCPU::op_ror_dpb() {
uint8 c = (regs.p.c)?0x80:0;
  dp = op_read();                //2
  cpu_c2();                      //2a
  rd.l = op_read(OPMODE_DP, dp); //3
  regs.p.c = rd.l & 1;
  rd.l >>= 1;
  rd.l |= c;
  cpu_io();                      //4
  op_write(OPMODE_DP, dp, rd.l); //5
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_ror_dpw() {
uint16 c = (regs.p.c)?0x8000:0;
  dp = op_read();                    //2
  cpu_c2();                          //2a
  rd.l = op_read(OPMODE_DP, dp);     //3
  rd.h = op_read(OPMODE_DP, dp + 1); //3a
  regs.p.c = rd.w & 1;
  rd.w >>= 1;
  rd.w |= c;
  cpu_io();                          //4
  op_write(OPMODE_DP, dp + 1, rd.h); //5a
  op_write(OPMODE_DP, dp,     rd.l); //5
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_ror_dp() { (regs.p.m)?op_ror_dpb():op_ror_dpw(); }

/**********************
 *** 0x76: ror dp,x ***
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
void bCPU::op_ror_dpxb() {
uint8 c = (regs.p.c)?0x80:0;
  dp = op_read();                           //2
  cpu_c2();                                 //2a
  cpu_io();                                 //3
  rd.l = op_read(OPMODE_DP, dp + regs.x.w); //4
  regs.p.c = rd.l & 1;
  rd.l >>= 1;
  rd.l |= c;
  cpu_io();                                 //5
  op_write(OPMODE_DP, dp + regs.x.w, rd.l); //6
  regs.p.n = !!(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

void bCPU::op_ror_dpxw() {
uint16 c = (regs.p.c)?0x8000:0;
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //4a
  regs.p.c = rd.w & 1;
  rd.w >>= 1;
  rd.w |= c;
  cpu_io();                                     //5
  op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h); //6a
  op_write(OPMODE_DP, dp + regs.x.w,     rd.l); //6
  regs.p.n = !!(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

void bCPU::op_ror_dpx() { (regs.p.m)?op_ror_dpxb():op_ror_dpxw(); }
