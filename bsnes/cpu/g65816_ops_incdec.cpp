/*****************
 *** 0x1a: inc ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_incb(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  gx816->regs.a.b++;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
}

void g65816_op_incw(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  gx816->regs.a.w++;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
}

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

void g65816_op_inc_addrb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                    //1-3 [op fetch]
  gx816->op.r.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w); //4 [read]
  snes_time->add_cpu_icycles(1);                              //5 [i/o]
  gx816->op.r.b++;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w, gx816->op.r.b); //6 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(3);
}

void g65816_op_inc_addrw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                          //1-3 [op fetch]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + 1); //4a [read high]
  snes_time->add_cpu_icycles(1);                                    //5 [i/o]
  gx816->op.r.w++;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + 1, gx816->op.r.p.h); //6a [write high]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w,     gx816->op.r.p.l); //6 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(3);
}

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

void g65816_op_inc_addrxb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                    //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                              //4 [i/o]
  gx816->op.r.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x); //5 [read]
  snes_time->add_cpu_icycles(1);                                              //6 [i/o]
  gx816->op.r.b++;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x, gx816->op.r.b); //7 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(3);
}

void g65816_op_inc_addrxw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                          //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                                    //4 [i/o]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x);     //5 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x + 1); //5a [read high]
  snes_time->add_cpu_icycles(1);                                                    //6 [i/o]
  gx816->op.r.w++;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x + 1, gx816->op.r.p.h); //7a [write high]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x,     gx816->op.r.p.l); //7 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(3);
}

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

void g65816_op_inc_dpb(void) {
  gx816->op.dp = gx816->read_operand(1);                   //1-2 [op fetch]
  gx816->op_cond(2);                                       //2a [dl!=0]
  gx816->op.r.b = gx816->op_read(OPMODE_DP, gx816->op.dp); //3 [read]
  snes_time->add_cpu_icycles(1);                           //4 [i/o]
  gx816->op.r.b++;
  gx816->op_write(OPMODE_DP, gx816->op.dp, gx816->op.r.b); //5 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(2);
}

void g65816_op_inc_dpw(void) {
  gx816->op.dp = gx816->read_operand(1);                         //1-2 [op fetch]
  gx816->op_cond(2);                                             //2a [dl!=0]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);     //3 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1); //3a [read high]
  snes_time->add_cpu_icycles(1);                                 //4 [i/o]
  gx816->op.r.w++;
  gx816->op_write(OPMODE_DP, gx816->op.dp + 1, gx816->op.r.p.h); //5a [write high]
  gx816->op_write(OPMODE_DP, gx816->op.dp,     gx816->op.r.p.l); //5 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(2);
}

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

void g65816_op_inc_dpxb(void) {
  gx816->op.dp = gx816->read_operand(1);                                   //1-2 [op fetch]
  gx816->op_cond(2);                                                       //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                           //3 [i/o]
  gx816->op.r.b = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x); //4 [read]
  snes_time->add_cpu_icycles(1);                                           //5 [i/o]
  gx816->op.r.b++;
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x, gx816->op.r.b); //6 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(2);
}

void g65816_op_inc_dpxw(void) {
  gx816->op.dp = gx816->read_operand(1);                                         //1-2 [op fetch]
  gx816->op_cond(2);                                                             //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                                 //3 [i/o]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1); //4a [read high]
  snes_time->add_cpu_icycles(1);                                                 //5 [i/o]
  gx816->op.r.w++;
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1, gx816->op.r.p.h); //6a [write high]
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x,     gx816->op.r.p.l); //6 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(2);
}

/*****************
 *** 0xe8: inx ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_inxb(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  gx816->regs.x++;
  gx816->regs.x &= 0xff;
  g65816_testn(gx816->regs.x & 0x80);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(1);
}

void g65816_op_inxw(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  gx816->regs.x++;
  g65816_testn(gx816->regs.x & 0x8000);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(1);
}

/*****************
 *** 0xc8: iny ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_inyb(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  gx816->regs.y++;
  gx816->regs.y &= 0xff;
  g65816_testn(gx816->regs.y & 0x80);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(1);
}

void g65816_op_inyw(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  gx816->regs.y++;
  g65816_testn(gx816->regs.y & 0x8000);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(1);
}

/*****************
 *** 0x3a: dec ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_decb(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  gx816->regs.a.b--;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
}

void g65816_op_decw(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  gx816->regs.a.w--;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
}

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

void g65816_op_dec_addrb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                    //1-3 [op fetch]
  gx816->op.r.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w); //4 [read]
  snes_time->add_cpu_icycles(1);                              //5 [i/o]
  gx816->op.r.b--;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w, gx816->op.r.b); //6 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(3);
}

void g65816_op_dec_addrw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                          //1-3 [op fetch]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + 1); //4a [read high]
  snes_time->add_cpu_icycles(1);                                    //5 [i/o]
  gx816->op.r.w--;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + 1, gx816->op.r.p.h); //6a [write high]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w,     gx816->op.r.p.l); //6 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(3);
}

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

void g65816_op_dec_addrxb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                    //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                              //4 [i/o]
  gx816->op.r.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x); //5 [read]
  snes_time->add_cpu_icycles(1);                                              //6 [i/o]
  gx816->op.r.b--;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x, gx816->op.r.b); //7 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(3);
}

void g65816_op_dec_addrxw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                          //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                                    //4 [i/o]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x);     //5 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x + 1); //5a [read high]
  snes_time->add_cpu_icycles(1);                                                    //6 [i/o]
  gx816->op.r.w--;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x + 1, gx816->op.r.p.h); //7a [write high]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x,     gx816->op.r.p.l); //7 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(3);
}

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

void g65816_op_dec_dpb(void) {
  gx816->op.dp = gx816->read_operand(1);                   //1-2 [op fetch]
  gx816->op_cond(2);                                       //2a [dl!=0]
  gx816->op.r.b = gx816->op_read(OPMODE_DP, gx816->op.dp); //3 [read]
  snes_time->add_cpu_icycles(1);                           //4 [i/o]
  gx816->op.r.b--;
  gx816->op_write(OPMODE_DP, gx816->op.dp, gx816->op.r.b); //5 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(2);
}

void g65816_op_dec_dpw(void) {
  gx816->op.dp = gx816->read_operand(1);                         //1-2 [op fetch]
  gx816->op_cond(2);                                             //2a [dl!=0]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);     //3 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1); //3a [read high]
  snes_time->add_cpu_icycles(1);                                 //4 [i/o]
  gx816->op.r.w--;
  gx816->op_write(OPMODE_DP, gx816->op.dp + 1, gx816->op.r.p.h); //5a [write high]
  gx816->op_write(OPMODE_DP, gx816->op.dp,     gx816->op.r.p.l); //5 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(2);
}

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

void g65816_op_dec_dpxb(void) {
  gx816->op.dp = gx816->read_operand(1);                                   //1-2 [op fetch]
  gx816->op_cond(2);                                                       //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                           //3 [i/o]
  gx816->op.r.b = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x); //4 [read]
  snes_time->add_cpu_icycles(1);                                           //5 [i/o]
  gx816->op.r.b--;
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x, gx816->op.r.b); //6 [write]

  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testz(gx816->op.r.b == 0);
  g65816_incpc(2);
}

void g65816_op_dec_dpxw(void) {
  gx816->op.dp = gx816->read_operand(1);                                         //1-2 [op fetch]
  gx816->op_cond(2);                                                             //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                                 //3 [i/o]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1); //4a [read high]
  snes_time->add_cpu_icycles(1);                                                 //5 [i/o]
  gx816->op.r.w--;
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1, gx816->op.r.p.h); //6a [write high]
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x,     gx816->op.r.p.l); //6 [write low]

  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testz(gx816->op.r.w == 0);
  g65816_incpc(2);
}

/*****************
 *** 0xca: dex ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_dexb(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  gx816->regs.x--;
  gx816->regs.x &= 0xff;
  g65816_testn(gx816->regs.x & 0x80);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(1);
}

void g65816_op_dexw(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  gx816->regs.x--;
  g65816_testn(gx816->regs.x & 0x8000);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(1);
}

/*****************
 *** 0x88: dey ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_deyb(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  gx816->regs.y--;
  gx816->regs.y &= 0xff;
  g65816_testn(gx816->regs.y & 0x80);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(1);
}

void g65816_op_deyw(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]

  gx816->regs.y--;
  g65816_testn(gx816->regs.y & 0x8000);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(1);
}
