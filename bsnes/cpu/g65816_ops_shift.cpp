/*****************
 *** 0x0a: asl ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_aslb(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  g65816_testc(gx816->regs.a.b & 0x80);
  gx816->regs.a.b <<= 1;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
}

void g65816_op_aslw(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  g65816_testc(gx816->regs.a.w & 0x8000);
  gx816->regs.a.w <<= 1;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
}

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

void g65816_op_asl_addrb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                    //1-3 [op fetch]
  gx816->op.r.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w); //4 [read]
  snes_time->add_cpu_icycles(1);                              //5 [i/o]
  g65816_testc(gx816->op.r.b & 0x80);
  gx816->op.r.b <<= 1;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w, gx816->op.r.b); //6 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(3);
}

void g65816_op_asl_addrw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                          //1-3 [op fetch]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + 1); //4a [read high]
  snes_time->add_cpu_icycles(1);                                    //5 [i/o]
  g65816_testc(gx816->op.r.w & 0x8000);
  gx816->op.r.w <<= 1;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + 1, gx816->op.r.p.h); //6a [write high]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w,     gx816->op.r.p.l); //6 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(3);
}

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

void g65816_op_asl_addrxb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                    //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                              //4 [i/o]
  gx816->op.r.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x); //5 [read]
  snes_time->add_cpu_icycles(1);                                              //6 [i/o]
  g65816_testc(gx816->op.r.b & 0x80);
  gx816->op.r.b <<= 1;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x, gx816->op.r.b); //7 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(3);
}

void g65816_op_asl_addrxw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                          //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                                    //4 [i/o]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x);     //5 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x + 1); //5a [read high]
  snes_time->add_cpu_icycles(1);                                                    //6 [i/o]
  g65816_testc(gx816->op.r.w & 0x8000);
  gx816->op.r.w <<= 1;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x + 1, gx816->op.r.p.h); //7a [write high]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x,     gx816->op.r.p.l); //7 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(3);
}

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

void g65816_op_asl_dpb(void) {
  gx816->op.dp = gx816->read_operand(1);                   //1-2 [op fetch]
  gx816->op_cond(2);                                       //2a [dl!=0]
  gx816->op.r.b = gx816->op_read(OPMODE_DP, gx816->op.dp); //3 [read]
  snes_time->add_cpu_icycles(1);                           //4 [i/o]
  g65816_testc(gx816->op.r.b & 0x80);
  gx816->op.r.b <<= 1;
  gx816->op_write(OPMODE_DP, gx816->op.dp, gx816->op.r.b); //5 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(2);
}

void g65816_op_asl_dpw(void) {
  gx816->op.dp = gx816->read_operand(1);                         //1-2 [op fetch]
  gx816->op_cond(2);                                             //2a [dl!=0]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);     //3 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1); //3a [read high]
  snes_time->add_cpu_icycles(1);                                 //4 [i/o]
  g65816_testc(gx816->op.r.w & 0x8000);
  gx816->op.r.w <<= 1;
  gx816->op_write(OPMODE_DP, gx816->op.dp + 1, gx816->op.r.p.h); //5a [write high]
  gx816->op_write(OPMODE_DP, gx816->op.dp,     gx816->op.r.p.l); //5 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(2);
}

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

void g65816_op_asl_dpxb(void) {
  gx816->op.dp = gx816->read_operand(1);                                   //1-2 [op fetch]
  gx816->op_cond(2);                                                       //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                           //3 [i/o]
  gx816->op.r.b = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x); //4 [read]
  snes_time->add_cpu_icycles(1);                                           //5 [i/o]
  g65816_testc(gx816->op.r.b & 0x80);
  gx816->op.r.b <<= 1;
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x, gx816->op.r.b); //6 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(2);
}

void g65816_op_asl_dpxw(void) {
  gx816->op.dp = gx816->read_operand(1);                                         //1-2 [op fetch]
  gx816->op_cond(2);                                                             //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                                 //3 [i/o]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1); //4a [read high]
  snes_time->add_cpu_icycles(1);                                                 //5 [i/o]
  g65816_testc(gx816->op.r.w & 0x8000);
  gx816->op.r.w <<= 1;
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1, gx816->op.r.p.h); //6a [write high]
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x,     gx816->op.r.p.l); //6 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(2);
}

/*****************
 *** 0x4a: lsr ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_lsrb(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  g65816_testc(gx816->regs.a.b & 0x01);
  gx816->regs.a.b >>= 1;
  g65816_clrn();
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
}

void g65816_op_lsrw(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  g65816_testc(gx816->regs.a.w & 0x0001);
  gx816->regs.a.w >>= 1;
  g65816_clrn();
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
}


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

void g65816_op_lsr_addrb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                    //1-3 [op fetch]
  gx816->op.r.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w); //4 [read]
  snes_time->add_cpu_icycles(1);                              //5 [i/o]
  g65816_testc(gx816->op.r.b & 0x01);
  gx816->op.r.b >>= 1;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w, gx816->op.r.b); //6 [write]

  g65816_clrn();
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(3);
}

void g65816_op_lsr_addrw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                          //1-3 [op fetch]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + 1); //4a [read high]
  snes_time->add_cpu_icycles(1);                                    //5 [i/o]
  g65816_testc(gx816->op.r.w & 0x0001);
  gx816->op.r.w >>= 1;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + 1, gx816->op.r.p.h); //6a [write high]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w,     gx816->op.r.p.l); //6 [write low]

  g65816_clrn();
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(3);
}

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

void g65816_op_lsr_addrxb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                    //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                              //4 [i/o]
  gx816->op.r.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x); //5 [read]
  snes_time->add_cpu_icycles(1);                                              //6 [i/o]
  g65816_testc(gx816->op.r.b & 0x01);
  gx816->op.r.b >>= 1;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x, gx816->op.r.b); //7 [write]

  g65816_clrn();
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(3);
}

void g65816_op_lsr_addrxw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                          //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                                    //4 [i/o]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x);     //5 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x + 1); //5a [read high]
  snes_time->add_cpu_icycles(1);                                                    //6 [i/o]
  g65816_testc(gx816->op.r.w & 0x0001);
  gx816->op.r.w >>= 1;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x + 1, gx816->op.r.p.h); //7a [write high]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x,     gx816->op.r.p.l); //7 [write low]

  g65816_clrn();
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(3);
}

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

void g65816_op_lsr_dpb(void) {
  gx816->op.dp = gx816->read_operand(1);                   //1-2 [op fetch]
  gx816->op_cond(2);                                       //2a [dl!=0]
  gx816->op.r.b = gx816->op_read(OPMODE_DP, gx816->op.dp); //3 [read]
  snes_time->add_cpu_icycles(1);                           //4 [i/o]
  g65816_testc(gx816->op.r.b & 0x01);
  gx816->op.r.b >>= 1;
  gx816->op_write(OPMODE_DP, gx816->op.dp, gx816->op.r.b); //5 [write]

  g65816_clrn();
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(2);
}

void g65816_op_lsr_dpw(void) {
  gx816->op.dp = gx816->read_operand(1);                         //1-2 [op fetch]
  gx816->op_cond(2);                                             //2a [dl!=0]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);     //3 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1); //3a [read high]
  snes_time->add_cpu_icycles(1);                                 //4 [i/o]
  g65816_testc(gx816->op.r.w & 0x0001);
  gx816->op.r.w >>= 1;
  gx816->op_write(OPMODE_DP, gx816->op.dp + 1, gx816->op.r.p.h); //5a [write high]
  gx816->op_write(OPMODE_DP, gx816->op.dp,     gx816->op.r.p.l); //5 [write low]

  g65816_clrn();
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(2);
}

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

void g65816_op_lsr_dpxb(void) {
  gx816->op.dp = gx816->read_operand(1);                                   //1-2 [op fetch]
  gx816->op_cond(2);                                                       //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                           //3 [i/o]
  gx816->op.r.b = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x); //4 [read]
  snes_time->add_cpu_icycles(1);                                           //5 [i/o]
  g65816_testc(gx816->op.r.b & 0x80);
  gx816->op.r.b >>= 1;
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x, gx816->op.r.b); //6 [write]

  g65816_clrn();
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(2);
}

void g65816_op_lsr_dpxw(void) {
  gx816->op.dp = gx816->read_operand(1);                                         //1-2 [op fetch]
  gx816->op_cond(2);                                                             //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                                 //3 [i/o]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1); //4a [read high]
  snes_time->add_cpu_icycles(1);                                                 //5 [i/o]
  g65816_testc(gx816->op.r.w & 0x001);
  gx816->op.r.w >>= 1;
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1, gx816->op.r.p.h); //6a [write high]
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x,     gx816->op.r.p.l); //6 [write low]

  g65816_clrn();
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(2);
}

/*****************
 *** 0x2a: rol ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_rolb(void) {
byte c = (gx816->regs.p & PF_C)?0x01:0x00;
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  g65816_testc(gx816->regs.a.b & 0x80);
  gx816->regs.a.b <<= 1;
  gx816->regs.a.b  |= c;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
}

void g65816_op_rolw(void) {
word c = (gx816->regs.p & PF_C)?0x0001:0x0000;
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  g65816_testc(gx816->regs.a.w & 0x8000);
  gx816->regs.a.w <<= 1;
  gx816->regs.a.w  |= c;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
}

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

void g65816_op_rol_addrb(void) {
byte c = (gx816->regs.p & PF_C)?0x01:0x00;
  gx816->op.aa.w = gx816->read_operand(2);                    //1-3 [op fetch]
  gx816->op.r.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w); //4 [read]
  snes_time->add_cpu_icycles(1);                              //5 [i/o]
  g65816_testc(gx816->op.r.b & 0x80);
  gx816->op.r.b <<= 1;
  gx816->op.r.b  |= c;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w, gx816->op.r.b); //6 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(3);
}

void g65816_op_rol_addrw(void) {
word c = (gx816->regs.p & PF_C)?0x0001:0x0000;
  gx816->op.aa.w = gx816->read_operand(2);                          //1-3 [op fetch]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + 1); //4a [read high]
  snes_time->add_cpu_icycles(1);                                    //5 [i/o]
  g65816_testc(gx816->op.r.w & 0x8000);
  gx816->op.r.w <<= 1;
  gx816->op.r.w  |= c;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + 1, gx816->op.r.p.h); //6a [write high]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w,     gx816->op.r.p.l); //6 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(3);
}

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

void g65816_op_rol_addrxb(void) {
byte c = (gx816->regs.p & PF_C)?0x01:0x00;
  gx816->op.aa.w = gx816->read_operand(2);                                    //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                              //4 [i/o]
  gx816->op.r.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x); //5 [read]
  snes_time->add_cpu_icycles(1);                                              //6 [i/o]
  g65816_testc(gx816->op.r.b & 0x80);
  gx816->op.r.b <<= 1;
  gx816->op.r.b  |= c;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x, gx816->op.r.b); //7 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(3);
}

void g65816_op_rol_addrxw(void) {
word c = (gx816->regs.p & PF_C)?0x0001:0x0000;
  gx816->op.aa.w = gx816->read_operand(2);                                          //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                                    //4 [i/o]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x);     //5 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x + 1); //5a [read high]
  snes_time->add_cpu_icycles(1);                                                    //6 [i/o]
  g65816_testc(gx816->op.r.w & 0x8000);
  gx816->op.r.w <<= 1;
  gx816->op.r.w  |= c;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x + 1, gx816->op.r.p.h); //7a [write high]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x,     gx816->op.r.p.l); //7 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(3);
}

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

void g65816_op_rol_dpb(void) {
byte c = (gx816->regs.p & PF_C)?0x01:0x00;
  gx816->op.dp = gx816->read_operand(1);                   //1-2 [op fetch]
  gx816->op_cond(2);                                       //2a [dl!=0]
  gx816->op.r.b = gx816->op_read(OPMODE_DP, gx816->op.dp); //3 [read]
  snes_time->add_cpu_icycles(1);                           //4 [i/o]
  g65816_testc(gx816->op.r.b & 0x80);
  gx816->op.r.b <<= 1;
  gx816->op.r.b  |= c;
  gx816->op_write(OPMODE_DP, gx816->op.dp, gx816->op.r.b); //5 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(2);
}

void g65816_op_rol_dpw(void) {
word c = (gx816->regs.p & PF_C)?0x0001:0x0000;
  gx816->op.dp = gx816->read_operand(1);                         //1-2 [op fetch]
  gx816->op_cond(2);                                             //2a [dl!=0]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);     //3 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1); //3a [read high]
  snes_time->add_cpu_icycles(1);                                 //4 [i/o]
  g65816_testc(gx816->op.r.w & 0x8000);
  gx816->op.r.w <<= 1;
  gx816->op.r.w  |= c;
  gx816->op_write(OPMODE_DP, gx816->op.dp + 1, gx816->op.r.p.h); //5a [write high]
  gx816->op_write(OPMODE_DP, gx816->op.dp,     gx816->op.r.p.l); //5 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(2);
}

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

void g65816_op_rol_dpxb(void) {
byte c = (gx816->regs.p & PF_C)?0x01:0x00;
  gx816->op.dp = gx816->read_operand(1);                                   //1-2 [op fetch]
  gx816->op_cond(2);                                                       //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                           //3 [i/o]
  gx816->op.r.b = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x); //4 [read]
  snes_time->add_cpu_icycles(1);                                           //5 [i/o]
  g65816_testc(gx816->op.r.b & 0x80);
  gx816->op.r.b <<= 1;
  gx816->op.r.b  |= c;
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x, gx816->op.r.b); //6 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(2);
}

void g65816_op_rol_dpxw(void) {
word c = (gx816->regs.p & PF_C)?0x0001:0x0000;
  gx816->op.dp = gx816->read_operand(1);                                         //1-2 [op fetch]
  gx816->op_cond(2);                                                             //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                                 //3 [i/o]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1); //4a [read high]
  snes_time->add_cpu_icycles(1);                                                 //5 [i/o]
  g65816_testc(gx816->op.r.w & 0x8000);
  gx816->op.r.w <<= 1;
  gx816->op.r.w  |= c;
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1, gx816->op.r.p.h); //6a [write high]
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x,     gx816->op.r.p.l); //6 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(2);
}

/*****************
 *** 0x6a: ror ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_rorb(void) {
byte c = (gx816->regs.p & PF_C)?0x80:0x00;
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  g65816_testc(gx816->regs.a.b & 0x01);
  gx816->regs.a.b >>= 1;
  gx816->regs.a.b  |= c;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
}

void g65816_op_rorw(void) {
word c = (gx816->regs.p & PF_C)?0x8000:0x0000;
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  g65816_testc(gx816->regs.a.w & 0x0001);
  gx816->regs.a.w >>= 1;
  gx816->regs.a.w  |= c;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
}


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

void g65816_op_ror_addrb(void) {
byte c = (gx816->regs.p & PF_C)?0x80:0x00;
  gx816->op.aa.w = gx816->read_operand(2);                    //1-3 [op fetch]
  gx816->op.r.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w); //4 [read]
  snes_time->add_cpu_icycles(1);                              //5 [i/o]
  g65816_testc(gx816->op.r.b & 0x01);
  gx816->op.r.b >>= 1;
  gx816->op.r.b  |= c;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w, gx816->op.r.b); //6 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(3);
}

void g65816_op_ror_addrw(void) {
word c = (gx816->regs.p & PF_C)?0x8000:0x0000;
  gx816->op.aa.w = gx816->read_operand(2);                          //1-3 [op fetch]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + 1); //4a [read high]
  snes_time->add_cpu_icycles(1);                                    //5 [i/o]
  g65816_testc(gx816->op.r.w & 0x0001);
  gx816->op.r.w >>= 1;
  gx816->op.r.w  |= c;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + 1, gx816->op.r.p.h); //6a [write high]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w,     gx816->op.r.p.l); //6 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(3);
}

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

void g65816_op_ror_addrxb(void) {
byte c = (gx816->regs.p & PF_C)?0x80:0x00;
  gx816->op.aa.w = gx816->read_operand(2);                                    //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                              //4 [i/o]
  gx816->op.r.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x); //5 [read]
  snes_time->add_cpu_icycles(1);                                              //6 [i/o]
  g65816_testc(gx816->op.r.b & 0x01);
  gx816->op.r.b >>= 1;
  gx816->op.r.b  |= c;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x, gx816->op.r.b); //7 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(3);
}

void g65816_op_ror_addrxw(void) {
word c = (gx816->regs.p & PF_C)?0x8000:0x0000;
  gx816->op.aa.w = gx816->read_operand(2);                                          //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                                    //4 [i/o]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x);     //5 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x + 1); //5a [read high]
  snes_time->add_cpu_icycles(1);                                                    //6 [i/o]
  g65816_testc(gx816->op.r.w & 0x0001);
  gx816->op.r.w >>= 1;
  gx816->op.r.w  |= c;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x + 1, gx816->op.r.p.h); //7a [write high]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x,     gx816->op.r.p.l); //7 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(3);
}

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

void g65816_op_ror_dpb(void) {
byte c = (gx816->regs.p & PF_C)?0x80:0x00;
  gx816->op.dp = gx816->read_operand(1);                   //1-2 [op fetch]
  gx816->op_cond(2);                                       //2a [dl!=0]
  gx816->op.r.b = gx816->op_read(OPMODE_DP, gx816->op.dp); //3 [read]
  snes_time->add_cpu_icycles(1);                           //4 [i/o]
  g65816_testc(gx816->op.r.b & 0x01);
  gx816->op.r.b >>= 1;
  gx816->op.r.b  |= c;
  gx816->op_write(OPMODE_DP, gx816->op.dp, gx816->op.r.b); //5 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(2);
}

void g65816_op_ror_dpw(void) {
word c = (gx816->regs.p & PF_C)?0x8000:0x0000;
  gx816->op.dp = gx816->read_operand(1);                         //1-2 [op fetch]
  gx816->op_cond(2);                                             //2a [dl!=0]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);     //3 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1); //3a [read high]
  snes_time->add_cpu_icycles(1);                                 //4 [i/o]
  g65816_testc(gx816->op.r.w & 0x0001);
  gx816->op.r.w >>= 1;
  gx816->op.r.w  |= c;
  gx816->op_write(OPMODE_DP, gx816->op.dp + 1, gx816->op.r.p.h); //5a [write high]
  gx816->op_write(OPMODE_DP, gx816->op.dp,     gx816->op.r.p.l); //5 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(2);
}

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

void g65816_op_ror_dpxb(void) {
byte c = (gx816->regs.p & PF_C)?0x80:0x00;
  gx816->op.dp = gx816->read_operand(1);                                   //1-2 [op fetch]
  gx816->op_cond(2);                                                       //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                           //3 [i/o]
  gx816->op.r.b = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x); //4 [read]
  snes_time->add_cpu_icycles(1);                                           //5 [i/o]
  g65816_testc(gx816->op.r.b & 0x80);
  gx816->op.r.b >>= 1;
  gx816->op.r.b  |= c;
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x, gx816->op.r.b); //6 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(2);
}

void g65816_op_ror_dpxw(void) {
word c = (gx816->regs.p & PF_C)?0x8000:0x0000;
  gx816->op.dp = gx816->read_operand(1);                                         //1-2 [op fetch]
  gx816->op_cond(2);                                                             //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                                 //3 [i/o]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1); //4a [read high]
  snes_time->add_cpu_icycles(1);                                                 //5 [i/o]
  g65816_testc(gx816->op.r.w & 0x001);
  gx816->op.r.w >>= 1;
  gx816->op.r.w  |= c;
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1, gx816->op.r.p.h); //6a [write high]
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x,     gx816->op.r.p.l); //6 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(2);
}
