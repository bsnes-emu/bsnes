void g65816_flags_bit_b(void) {
  g65816_testn(gx816->op.r.b & 0x80);
  g65816_testv(gx816->op.r.b & 0x40);
  g65816_testz((gx816->op.r.b & gx816->regs.a.b) == 0);
}

void g65816_flags_bit_w(void) {
  g65816_testn(gx816->op.r.w & 0x8000);
  g65816_testv(gx816->op.r.w & 0x4000);
  g65816_testz((gx816->op.r.w & gx816->regs.a.w) == 0);
}

/************************
 *** 0x89: bit #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+2 ; idh [1]
*/

void g65816_op_bit_constb(void) {
  gx816->op.r.b = gx816->read_operand(1); //1,2 [op fetch]
  g65816_flags_bit_b();
  g65816_incpc(2);
}

void g65816_op_bit_constw(void) {
  gx816->op.r.w = gx816->read_operand(2); //1,2,2a [op fetch]
  g65816_flags_bit_w();
  g65816_incpc(3);
}

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

void g65816_op_bit_addrb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                    //1-3 [op fetch]
  gx816->op.r.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w); //4 [read]
  g65816_flags_bit_b();
  g65816_incpc(3);
}

void g65816_op_bit_addrw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                          //1-3 [op fetch]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + 1); //4a [read high]
  g65816_flags_bit_w();
  g65816_incpc(3);
}

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

void g65816_op_bit_addrxb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                    //1-3 [op fetch]
  gx816->op_cond(4, gx816->op.aa.w, gx816->op.aa.w + gx816->regs.x);          //3a [pbc or p.x=0]
  gx816->op.r.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x); //4 [read]
  g65816_flags_bit_b();
  g65816_incpc(3);
}

void g65816_op_bit_addrxw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                          //1-3 [op fetch]
  gx816->op_cond(4, gx816->op.aa.w, gx816->op.aa.w + gx816->regs.x);                //3a [pbc or p.x=0]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x + 1); //4a [read high]
  g65816_flags_bit_w();
  g65816_incpc(3);
}

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

void g65816_op_bit_dpb(void) {
  gx816->op.dp = gx816->read_operand(1);                   //1,2 [op fetch]
  gx816->op_cond(2);                                       //2a [dl!=0]
  gx816->op.r.b = gx816->op_read(OPMODE_DP, gx816->op.dp); //3 [read]
  g65816_flags_bit_b();
  g65816_incpc(2);
}

void g65816_op_bit_dpw(void) {
  gx816->op.dp = gx816->read_operand(1);                         //1,2 [op fetch]
  gx816->op_cond(2);                                             //2a [dl!=0]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);     //3 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1); //3a [read high]
  g65816_flags_bit_w();
  g65816_incpc(2);
}

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

void g65816_op_bit_dpxb(void) {
  gx816->op.dp = gx816->read_operand(1);                                   //1,2 [op fetch]
  gx816->op_cond(2);                                                       //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                           //3 [i/o]
  gx816->op.r.b = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x); //4 [read]
  g65816_flags_bit_b();
  g65816_incpc(2);
}

void g65816_op_bit_dpxw(void) {
  gx816->op.dp = gx816->read_operand(1);                                         //1,2 [op fetch]
  gx816->op_cond(2);                                                             //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                                 //3 [i/o]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1); //4a [read high]
  g65816_flags_bit_w();
  g65816_incpc(2);
}

void g65816_flags_cpx_b(void) {
int r = gx816->regs.x - gx816->op.r.b;
  g65816_testn(r & 0x80);
  g65816_testz((byte)r == 0);
  g65816_testc(r >= 0);
}

void g65816_flags_cpx_w(void) {
int r = gx816->regs.x - gx816->op.r.w;
  g65816_testn(r & 0x8000);
  g65816_testz((word)r == 0);
  g65816_testc(r >= 0);
}

/************************
 *** 0xe0: cpx #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+2 ; idh [1]
*/

void g65816_op_cpx_constb(void) {
  gx816->op.r.b = gx816->read_operand(1); //1,2 [op fetch]
  g65816_flags_cpx_b();
  g65816_incpc(2);
}

void g65816_op_cpx_constw(void) {
  gx816->op.r.w = gx816->read_operand(2); //1,2,2a [op fetch]
  g65816_flags_cpx_w();
  g65816_incpc(3);
}

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

void g65816_op_cpx_addrb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                    //1-3 [op fetch]
  gx816->op.r.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w); //4 [read]
  g65816_flags_cpx_b();
  g65816_incpc(3);
}

void g65816_op_cpx_addrw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                          //1-3 [op fetch]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + 1); //4a [read high]
  g65816_flags_cpx_w();
  g65816_incpc(3);
}

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

void g65816_op_cpx_dpb(void) {
  gx816->op.dp = gx816->read_operand(1);                   //1,2 [op fetch]
  gx816->op_cond(2);                                       //2a [dl!=0]
  gx816->op.r.b = gx816->op_read(OPMODE_DP, gx816->op.dp); //3 [read]
  g65816_flags_cpx_b();
  g65816_incpc(2);
}

void g65816_op_cpx_dpw(void) {
  gx816->op.dp = gx816->read_operand(1);                         //1,2 [op fetch]
  gx816->op_cond(2);                                             //2a [dl!=0]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);     //3 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1); //3a [read high]
  g65816_flags_cpx_w();
  g65816_incpc(2);
}

void g65816_flags_cpy_b(void) {
int r = gx816->regs.y - gx816->op.r.b;
  g65816_testn(r & 0x80);
  g65816_testz((byte)r == 0);
  g65816_testc(r >= 0);
}

void g65816_flags_cpy_w(void) {
int r = gx816->regs.y - gx816->op.r.w;
  g65816_testn(r & 0x8000);
  g65816_testz((word)r == 0);
  g65816_testc(r >= 0);
}

/************************
 *** 0xc0: cpy #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+2 ; idh [1]
*/

void g65816_op_cpy_constb(void) {
  gx816->op.r.b = gx816->read_operand(1); //1,2 [op fetch]
  g65816_flags_cpy_b();
  g65816_incpc(2);
}

void g65816_op_cpy_constw(void) {
  gx816->op.r.w = gx816->read_operand(2); //1,2,2a [op fetch]
  g65816_flags_cpy_w();
  g65816_incpc(3);
}

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

void g65816_op_cpy_addrb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                    //1-3 [op fetch]
  gx816->op.r.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w); //4 [read]
  g65816_flags_cpy_b();
  g65816_incpc(3);
}

void g65816_op_cpy_addrw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                          //1-3 [op fetch]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + 1); //4a [read high]
  g65816_flags_cpy_w();
  g65816_incpc(3);
}

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

void g65816_op_cpy_dpb(void) {
  gx816->op.dp = gx816->read_operand(1);                   //1,2 [op fetch]
  gx816->op_cond(2);                                       //2a [dl!=0]
  gx816->op.r.b = gx816->op_read(OPMODE_DP, gx816->op.dp); //3 [read]
  g65816_flags_cpy_b();
  g65816_incpc(2);
}

void g65816_op_cpy_dpw(void) {
  gx816->op.dp = gx816->read_operand(1);                         //1,2 [op fetch]
  gx816->op_cond(2);                                             //2a [dl!=0]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);     //3 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1); //3a [read high]
  g65816_flags_cpy_w();
  g65816_incpc(2);
}

void g65816_flags_ldx_b(void) {
  g65816_testn(gx816->regs.x & 0x80);
  g65816_testz(gx816->regs.x == 0);
}

void g65816_flags_ldx_w(void) {
  g65816_testn(gx816->regs.x & 0x8000);
  g65816_testz(gx816->regs.x == 0);
}

/************************
 *** 0xa2: ldx #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+2 ; idh [1]
*/

void g65816_op_ldx_constb(void) {
  gx816->regs.x = gx816->read_operand(1); //1,2 [op fetch]
  g65816_flags_ldx_b();
  g65816_incpc(2);
}

void g65816_op_ldx_constw(void) {
  gx816->regs.x = gx816->read_operand(2); //1,2,2a [op fetch]
  g65816_flags_ldx_w();
  g65816_incpc(3);
}

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

void g65816_op_ldx_addrb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                    //1-3 [op fetch]
  gx816->regs.x = gx816->op_read(OPMODE_DBR, gx816->op.aa.w); //4 [read]
  g65816_flags_ldx_b();
  g65816_incpc(3);
}

void g65816_op_ldx_addrw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                              //1-3 [op fetch]
  gx816->regs.x  = gx816->op_read(OPMODE_DBR, gx816->op.aa.w);          //4 [read low]
  gx816->regs.x |= gx816->op_read(OPMODE_DBR, gx816->op.aa.w + 1) << 8; //4a [read high]
  g65816_flags_ldx_w();
  g65816_incpc(3);
}

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

void g65816_op_ldx_addryb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                    //1-3 [op fetch]
  gx816->op_cond(4, gx816->op.aa.w, gx816->op.aa.w + gx816->regs.y);          //3a [pbc or p.x=0]
  gx816->regs.x = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y); //4 [read]
  g65816_flags_ldx_b();
  g65816_incpc(3);
}

void g65816_op_ldx_addryw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                              //1-3 [op fetch]
  gx816->op_cond(4, gx816->op.aa.w, gx816->op.aa.w + gx816->regs.y);                    //3a [pbc or p.x=0]
  gx816->regs.x  = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y);          //4 [read low]
  gx816->regs.x |= gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y + 1) << 8; //4a [read high]
  g65816_flags_ldx_w();
  g65816_incpc(3);
}

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

void g65816_op_ldx_dpb(void) {
  gx816->op.dp = gx816->read_operand(1);                   //1,2 [op fetch]
  gx816->op_cond(2);                                       //2a [dl!=0]
  gx816->regs.x = gx816->op_read(OPMODE_DP, gx816->op.dp); //3 [read]
  g65816_flags_ldx_b();
  g65816_incpc(2);
}

void g65816_op_ldx_dpw(void) {
  gx816->op.dp = gx816->read_operand(1);                             //1,2 [op fetch]
  gx816->op_cond(2);                                                 //2a [dl!=0]
  gx816->regs.x  = gx816->op_read(OPMODE_DP, gx816->op.dp);          //3 [read low]
  gx816->regs.x |= gx816->op_read(OPMODE_DP, gx816->op.dp + 1) << 8; //3a [read high]
  g65816_flags_ldx_w();
  g65816_incpc(2);
}

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

void g65816_op_ldx_dpyb(void) {
  gx816->op.dp = gx816->read_operand(1);                                     //1,2 [op fetch]
  gx816->op_cond(2);                                                         //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                             //3 [i/o]
  gx816->regs.x = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.y); //4 [read]
  g65816_flags_ldx_b();
  g65816_incpc(2);
}

void g65816_op_ldx_dpyw(void) {
  gx816->op.dp = gx816->read_operand(1);                                             //1,2 [op fetch]
  gx816->op_cond(2);                                                                 //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                                     //3 [i/o]
  gx816->regs.x  = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.y);          //4 [read low]
  gx816->regs.x |= gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.y + 1) << 8; //4a [read high]
  g65816_flags_ldx_w();
  g65816_incpc(2);
}

void g65816_flags_ldy_b(void) {
  g65816_testn(gx816->regs.y & 0x80);
  g65816_testz(gx816->regs.y == 0);
}

void g65816_flags_ldy_w(void) {
  g65816_testn(gx816->regs.y & 0x8000);
  g65816_testz(gx816->regs.y == 0);
}

/************************
 *** 0xa0: ldy #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+2 ; idh [1]
*/

void g65816_op_ldy_constb(void) {
  gx816->regs.y = gx816->read_operand(1); //1,2 [op fetch]
  g65816_flags_ldy_b();
  g65816_incpc(2);
}

void g65816_op_ldy_constw(void) {
  gx816->regs.y = gx816->read_operand(2); //1,2,2a [op fetch]
  g65816_flags_ldy_w();
  g65816_incpc(3);
}

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

void g65816_op_ldy_addrb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                    //1-3 [op fetch]
  gx816->regs.y = gx816->op_read(OPMODE_DBR, gx816->op.aa.w); //4 [read]
  g65816_flags_ldy_b();
  g65816_incpc(3);
}

void g65816_op_ldy_addrw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                              //1-3 [op fetch]
  gx816->regs.y  = gx816->op_read(OPMODE_DBR, gx816->op.aa.w);          //4 [read low]
  gx816->regs.y |= gx816->op_read(OPMODE_DBR, gx816->op.aa.w + 1) << 8; //4a [read high]
  g65816_flags_ldy_w();
  g65816_incpc(3);
}

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

void g65816_op_ldy_addrxb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                    //1-3 [op fetch]
  gx816->op_cond(4, gx816->op.aa.w, gx816->op.aa.w + gx816->regs.x);          //3a [pbc or p.x=0]
  gx816->regs.y = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x); //4 [read]
  g65816_flags_ldy_b();
  g65816_incpc(3);
}

void g65816_op_ldy_addrxw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                              //1-3 [op fetch]
  gx816->op_cond(4, gx816->op.aa.w, gx816->op.aa.w + gx816->regs.x);                    //3a [pbc or p.x=0]
  gx816->regs.y  = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x);          //4 [read low]
  gx816->regs.y |= gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x + 1) << 8; //4a [read high]
  g65816_flags_ldy_w();
  g65816_incpc(3);
}

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

void g65816_op_ldy_dpb(void) {
  gx816->op.dp = gx816->read_operand(1);                   //1,2 [op fetch]
  gx816->op_cond(2);                                       //2a [dl!=0]
  gx816->regs.y = gx816->op_read(OPMODE_DP, gx816->op.dp); //3 [read]
  g65816_flags_ldy_b();
  g65816_incpc(2);
}

void g65816_op_ldy_dpw(void) {
  gx816->op.dp = gx816->read_operand(1);                             //1,2 [op fetch]
  gx816->op_cond(2);                                                 //2a [dl!=0]
  gx816->regs.y  = gx816->op_read(OPMODE_DP, gx816->op.dp);          //3 [read low]
  gx816->regs.y |= gx816->op_read(OPMODE_DP, gx816->op.dp + 1) << 8; //3a [read high]
  g65816_flags_ldy_w();
  g65816_incpc(2);
}

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

void g65816_op_ldy_dpxb(void) {
  gx816->op.dp = gx816->read_operand(1);                                     //1,2 [op fetch]
  gx816->op_cond(2);                                                         //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                             //3 [i/o]
  gx816->regs.y = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x); //4 [read]
  g65816_flags_ldy_b();
  g65816_incpc(2);
}

void g65816_op_ldy_dpxw(void) {
  gx816->op.dp = gx816->read_operand(1);                                             //1,2 [op fetch]
  gx816->op_cond(2);                                                                 //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                                     //3 [i/o]
  gx816->regs.y  = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x);          //4 [read low]
  gx816->regs.y |= gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1) << 8; //4a [read high]
  g65816_flags_ldy_w();
  g65816_incpc(2);
}

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

void g65816_op_stx_addrb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                    //1-3 [op fetch]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w, gx816->regs.x); //4 [write]
  g65816_incpc(3);
}

void g65816_op_stx_addrw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                             //1-3 [op fetch]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w,     gx816->regs.x);      //4 [write low]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + 1, gx816->regs.x >> 8); //4a [write high]
  g65816_incpc(3);
}

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

void g65816_op_stx_dpb(void) {
  gx816->op.dp = gx816->read_operand(1);                   //1,2 [op fetch]
  gx816->op_cond(2);                                       //2a [dl!=0]
  gx816->op_write(OPMODE_DP, gx816->op.dp, gx816->regs.x); //3 [write]
  g65816_incpc(2);
}

void g65816_op_stx_dpw(void) {
  gx816->op.dp = gx816->read_operand(1);                            //1,2 [op fetch]
  gx816->op_cond(2);                                                //2a [dl!=0]
  gx816->op_write(OPMODE_DP, gx816->op.dp,     gx816->regs.x);      //3 [write low]
  gx816->op_write(OPMODE_DP, gx816->op.dp + 1, gx816->regs.x >> 8); //3a [write high]
  g65816_incpc(2);
}

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

void g65816_op_stx_dpyb(void) {
  gx816->op.dp = gx816->read_operand(1);                                   //1,2 [op fetch]
  gx816->op_cond(2);                                                       //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                           //3 [i/o]
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.y, gx816->regs.x); //4 [write]
  g65816_incpc(2);
}

void g65816_op_stx_dpyw(void) {
  gx816->op.dp = gx816->read_operand(1);                                            //1,2 [op fetch]
  gx816->op_cond(2);                                                                //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                                    //3 [i/o]
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.y,     gx816->regs.x);      //4 [write low]
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.y + 1, gx816->regs.x >> 8); //4a [write high]
  g65816_incpc(2);
}

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

void g65816_op_sty_addrb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                    //1-3 [op fetch]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w, gx816->regs.y); //4 [write]
  g65816_incpc(3);
}

void g65816_op_sty_addrw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                             //1-3 [op fetch]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w,     gx816->regs.y);      //4 [write low]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + 1, gx816->regs.y >> 8); //4a [write high]
  g65816_incpc(3);
}

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

void g65816_op_sty_dpb(void) {
  gx816->op.dp = gx816->read_operand(1);                   //1,2 [op fetch]
  gx816->op_cond(2);                                       //2a [dl!=0]
  gx816->op_write(OPMODE_DP, gx816->op.dp, gx816->regs.y); //3 [write]
  g65816_incpc(2);
}

void g65816_op_sty_dpw(void) {
  gx816->op.dp = gx816->read_operand(1);                            //1,2 [op fetch]
  gx816->op_cond(2);                                                //2a [dl!=0]
  gx816->op_write(OPMODE_DP, gx816->op.dp,     gx816->regs.y);      //3 [write low]
  gx816->op_write(OPMODE_DP, gx816->op.dp + 1, gx816->regs.y >> 8); //3a [write high]
  g65816_incpc(2);
}

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

void g65816_op_sty_dpxb(void) {
  gx816->op.dp = gx816->read_operand(1);                                   //1,2 [op fetch]
  gx816->op_cond(2);                                                       //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                           //3 [i/o]
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x, gx816->regs.y); //4 [write]
  g65816_incpc(2);
}

void g65816_op_sty_dpxw(void) {
  gx816->op.dp = gx816->read_operand(1);                                            //1,2 [op fetch]
  gx816->op_cond(2);                                                                //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                                    //3 [i/o]
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x,     gx816->regs.y);      //4 [write low]
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1, gx816->regs.y >> 8); //4a [write high]
  g65816_incpc(2);
}

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

void g65816_op_stz_addrb(void) {
  gx816->op.aa.w = gx816->read_operand(2);        //1-3 [op fetch]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w, 0); //4 [write]
  g65816_incpc(3);
}

void g65816_op_stz_addrw(void) {
  gx816->op.aa.w = gx816->read_operand(2);            //1-3 [op fetch]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w,     0); //4 [write low]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + 1, 0); //4a [write high]
  g65816_incpc(3);
}

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

void g65816_op_stz_addrxb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                        //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                  //3a [write i/o]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x, 0); //4 [write]
  g65816_incpc(3);
}

void g65816_op_stz_addrxw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                            //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                      //3a [write i/o]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x,     0); //4 [write low]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x + 1, 0); //4a [write high]
  g65816_incpc(3);
}

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

void g65816_op_stz_dpb(void) {
  gx816->op.dp = gx816->read_operand(1);       //1,2 [op fetch]
  gx816->op_cond(2);                           //2a [dl!=0]
  gx816->op_write(OPMODE_DP, gx816->op.dp, 0); //3 [write]
  g65816_incpc(2);
}

void g65816_op_stz_dpw(void) {
  gx816->op.dp = gx816->read_operand(1);           //1,2 [op fetch]
  gx816->op_cond(2);                               //2a [dl!=0]
  gx816->op_write(OPMODE_DP, gx816->op.dp,     0); //3 [write low]
  gx816->op_write(OPMODE_DP, gx816->op.dp + 1, 0); //3a [write high]
  g65816_incpc(2);
}

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

void g65816_op_stz_dpxb(void) {
  gx816->op.dp = gx816->read_operand(1);                       //1,2 [op fetch]
  gx816->op_cond(2);                                           //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                               //3 [i/o]
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x, 0); //4 [write]
  g65816_incpc(2);
}

void g65816_op_stz_dpxw(void) {
  gx816->op.dp = gx816->read_operand(1);                           //1,2 [op fetch]
  gx816->op_cond(2);                                               //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                   //3 [i/o]
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x,     0); //4 [write low]
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1, 0); //4a [write high]
  g65816_incpc(2);
}

/*****************
 *** 0xeb: xba ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
  [3] pbr,pc+1 ; io
*/

void g65816_op_xba(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(2); //2,3 [i/o]
  gx816->regs.a.p.l ^= gx816->regs.a.p.h;
  gx816->regs.a.p.h ^= gx816->regs.a.p.l;
  gx816->regs.a.p.l ^= gx816->regs.a.p.h;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
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

void g65816_op_trb_addrb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                    //1-3 [op fetch]
  gx816->op.r.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w); //4 [read]
  snes_time->add_cpu_icycles(1);                              //5 [i/o]
  g65816_testz((gx816->op.r.b & gx816->regs.a.b) == 0);
  gx816->op.r.b &= ~gx816->regs.a.b;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w, gx816->op.r.b); //6 [write]
  g65816_incpc(3);
}

void g65816_op_trb_addrw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                          //1-3 [op fetch]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + 1); //4a [read high]
  snes_time->add_cpu_icycles(1);                                    //5 [i/o]
  g65816_testz((gx816->op.r.w & gx816->regs.a.w) == 0);
  gx816->op.r.w &= ~gx816->regs.a.w;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + 1, gx816->op.r.p.h); //6a [write high]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w,     gx816->op.r.p.l); //6 [write low]
  g65816_incpc(3);
}

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

void g65816_op_trb_dpb(void) {
  gx816->op.dp = gx816->read_operand(1);                   //1-2 [op fetch]
  gx816->op_cond(2);                                       //2a [dl!=0]
  gx816->op.r.b = gx816->op_read(OPMODE_DP, gx816->op.dp); //3 [read]
  snes_time->add_cpu_icycles(1);                           //4 [i/o]
  g65816_testz((gx816->op.r.b & gx816->regs.a.b) == 0);
  gx816->op.r.b &= ~gx816->regs.a.b;
  gx816->op_write(OPMODE_DP, gx816->op.dp, gx816->op.r.b); //5 [write]
  g65816_incpc(2);
}

void g65816_op_trb_dpw(void) {
  gx816->op.dp = gx816->read_operand(1);                         //1-2 [op fetch]
  gx816->op_cond(2);                                             //2a [dl!=0]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);     //3 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1); //3a [read high]
  snes_time->add_cpu_icycles(1);                                 //4 [i/o]
  g65816_testz((gx816->op.r.w & gx816->regs.a.w) == 0);
  gx816->op.r.w &= ~gx816->regs.a.w;
  gx816->op_write(OPMODE_DP, gx816->op.dp + 1, gx816->op.r.p.h); //5a [write high]
  gx816->op_write(OPMODE_DP, gx816->op.dp,     gx816->op.r.p.l); //5 [write low]
  g65816_incpc(2);
}

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

void g65816_op_tsb_addrb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                    //1-3 [op fetch]
  gx816->op.r.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w); //4 [read]
  snes_time->add_cpu_icycles(1);                              //5 [i/o]
  g65816_testz((gx816->op.r.b & gx816->regs.a.b) == 0);
  gx816->op.r.b |= gx816->regs.a.b;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w, gx816->op.r.b); //6 [write]
  g65816_incpc(3);
}

void g65816_op_tsb_addrw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                          //1-3 [op fetch]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + 1); //4a [read high]
  snes_time->add_cpu_icycles(1);                                    //5 [i/o]
  g65816_testz((gx816->op.r.w & gx816->regs.a.w) == 0);
  gx816->op.r.w |= gx816->regs.a.w;
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + 1, gx816->op.r.p.h); //6a [write high]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w,     gx816->op.r.p.l); //6 [write low]
  g65816_incpc(3);
}

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

void g65816_op_tsb_dpb(void) {
  gx816->op.dp = gx816->read_operand(1);                   //1-2 [op fetch]
  gx816->op_cond(2);                                       //2a [dl!=0]
  gx816->op.r.b = gx816->op_read(OPMODE_DP, gx816->op.dp); //3 [read]
  snes_time->add_cpu_icycles(1);                           //4 [i/o]
  g65816_testz((gx816->op.r.b & gx816->regs.a.b) == 0);
  gx816->op.r.b |= gx816->regs.a.b;
  gx816->op_write(OPMODE_DP, gx816->op.dp, gx816->op.r.b); //5 [write]
  g65816_incpc(2);
}

void g65816_op_tsb_dpw(void) {
  gx816->op.dp = gx816->read_operand(1);                         //1-2 [op fetch]
  gx816->op_cond(2);                                             //2a [dl!=0]
  gx816->op.r.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);     //3 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1); //3a [read high]
  snes_time->add_cpu_icycles(1);                                 //4 [i/o]
  g65816_testz((gx816->op.r.w & gx816->regs.a.w) == 0);
  gx816->op.r.w |= gx816->regs.a.w;
  gx816->op_write(OPMODE_DP, gx816->op.dp + 1, gx816->op.r.p.h); //5a [write high]
  gx816->op_write(OPMODE_DP, gx816->op.dp,     gx816->op.r.p.l); //5 [write low]
  g65816_incpc(2);
}

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

void g65816_op_mvn(void) {
ulong sp, dp;
  gx816->op.aa.w = gx816->read_operand(2); //1-3 [op fetch]
  gx816->regs.a.w--;
  dp = (gx816->op.aa.p.l << 16) | gx816->regs.y;
  sp = (gx816->op.aa.p.h << 16) | gx816->regs.x;
  gx816->op.r.b = gx816->op_read(OPMODE_LONG, sp); //4 [src data]
  gx816->op_write(OPMODE_LONG, dp, gx816->op.r.b); //5 [dest data]
  gx816->regs.x++;
  gx816->regs.y++;
  if(gx816->regs.e == true || (gx816->regs.p & PF_X)) {
    gx816->regs.x &= 0xff;
    gx816->regs.y &= 0xff;
  }
  if(gx816->regs.a.w == 0xffff) {
    g65816_incpc(3);
  }
  snes_time->add_cpu_icycles(2); //6,7 [i/o]
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

void g65816_op_mvp(void) {
ulong sp, dp;
  gx816->op.aa.w = gx816->read_operand(2); //1-3 [op fetch]
  gx816->regs.a.w--;
  dp = (gx816->op.aa.p.l << 16) | gx816->regs.y;
  sp = (gx816->op.aa.p.h << 16) | gx816->regs.x;
  gx816->op.r.b = gx816->op_read(OPMODE_LONG, sp); //4 [src data]
  gx816->op_write(OPMODE_LONG, dp, gx816->op.r.b); //5 [dest data]
  gx816->regs.x--;
  gx816->regs.y--;
  if(gx816->regs.e == true || (gx816->regs.p & PF_X)) {
    gx816->regs.x &= 0xff;
    gx816->regs.y &= 0xff;
  }
  if(gx816->regs.a.w == 0xffff) {
    g65816_incpc(3);
  }
  snes_time->add_cpu_icycles(2); //6,7 [i/o]
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

void g65816_op_brk(void) {
  snes_time->add_cpu_pcycles(2);                          //1,2 [op fetch]
  gx816->stack_write(gx816->regs.pc >> 16);               //3 [pbr]
  gx816->stack_write(gx816->regs.pc >> 8);                //4 [pch]
  gx816->stack_write(gx816->regs.pc);                     //5 [pcl]
  gx816->stack_write(gx816->regs.p);                      //6 [p]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_LONG, 0xffe6); //7 [aavl]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_LONG, 0xffe7); //8 [aavh]
  gx816->regs.pc = gx816->op.aa.w;
  g65816_seti();
  g65816_clrd();
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

void g65816_op_cop(void) {
  snes_time->add_cpu_pcycles(2);                          //1,2 [op fetch]
  gx816->stack_write(gx816->regs.pc >> 16);               //3 [pbr]
  gx816->stack_write(gx816->regs.pc >> 8);                //4 [pch]
  gx816->stack_write(gx816->regs.pc);                     //5 [pcl]
  gx816->stack_write(gx816->regs.p);                      //6 [p]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_LONG, 0xfff4); //7 [aavl]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_LONG, 0xfff5); //8 [aavh]
  gx816->regs.pc = gx816->op.aa.w;
  g65816_seti();
  g65816_clrd();
}

/*****************
 *** 0xdb: stp ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
  [3] pbr,pc+1 ; io
*/

void g65816_op_stp(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(2); //2,3 [i/o]
  gx816->cpu_state = CPUSTATE_STP;
}

/*****************
 *** 0xcb: wai ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
  [3] pbr,pc+1 ; io
*/

void g65816_op_wai(void) {
byte i = 0;
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(2); //2,3 [i/o]
  if(gx816->nmi_enabled == true)i = 1;
  if((ppu.vcounter_enabled == true || ppu.hcounter_enabled == true) && !(gx816->regs.p & PF_I))i = 1;
  if(gx816->wai_interrupt_occurred == true || i == 0) {
    gx816->wai_interrupt_occurred = false;
    g65816_incpc(1);
  }
}

/*****************
 *** 0xfb: xce ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_xce(void) {
bool t = gx816->regs.e;
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  if(gx816->regs.p & PF_C) {
    gx816->regs.e = true;
  } else {
    gx816->regs.e = false;
  }
  g65816_setm();
  g65816_setx();
  g65816_testc(t == true);
  if(gx816->regs.e == true) {
    gx816->regs.s = 0x0100 | (gx816->regs.s & 0xff);
    gx816->regs.x &= 0xff;
    gx816->regs.y &= 0xff;
  }
  g65816_incpc(1);
}

/*****************
 *** 0xea: nop ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_nop(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  g65816_incpc(1);
}

/*****************
 *** 0x42: wdm ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_wdm(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  g65816_incpc(2);
}

/*****************
 *** 0x18: clc ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_clc(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  g65816_clrc();
  g65816_incpc(1);
}

/*****************
 *** 0xd8: cld ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_cld(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  g65816_clrd();
  g65816_incpc(1);
}

/*****************
 *** 0x58: cli ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_cli(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  g65816_clri();
  g65816_incpc(1);
}

/*****************
 *** 0xb8: clv ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_clv(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  g65816_clrv();
  g65816_incpc(1);
}

/*****************
 *** 0x38: sec ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_sec(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  g65816_setc();
  g65816_incpc(1);
}

/*****************
 *** 0xf8: sed ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_sed(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  g65816_setd();
  g65816_incpc(1);
}

/*****************
 *** 0x78: sei ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_sei(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  g65816_seti();
  g65816_incpc(1);
}

/************************
 *** 0xc2: rep #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+1 ; idl
*/

void g65816_op_rep(void) {
  gx816->op.r.b = gx816->read_operand(1); //1,2 [op fetch]
  snes_time->add_cpu_icycles(1);          //2a [i/o]
  gx816->regs.p &= ~gx816->op.r.b;
  g65816_incpc(2);
  if(gx816->regs.e == true) {
    gx816->regs.p |= 0x30;
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

void g65816_op_sep(void) {
  gx816->op.r.b = gx816->read_operand(1); //1,2 [op fetch]
  snes_time->add_cpu_icycles(1);          //2a [i/o]
  gx816->regs.p |= gx816->op.r.b;
  if(gx816->regs.p & 0x10) {
    gx816->regs.x &= 0xff;
    gx816->regs.y &= 0xff;
  }
  g65816_incpc(2);
}

/*****************
 *** 0xaa: tax ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_taxb(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.x = gx816->regs.a.b;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
}

void g65816_op_taxw(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.x = gx816->regs.a.w;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
}

/*****************
 *** 0xa8: tay ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_tayb(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.y = gx816->regs.a.b;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
}

void g65816_op_tayw(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.y = gx816->regs.a.w;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
}

/*****************
 *** 0x5b: tcd ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_tcd(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.d = gx816->regs.a.w;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
}

/*****************
 *** 0x1b: tcs ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_tcse(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.s = 0x0100 | gx816->regs.a.b;
  g65816_incpc(1);
}

void g65816_op_tcsn(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.s = gx816->regs.a.w;
  g65816_incpc(1);
}

/*****************
 *** 0x7b: tdc ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_tdc(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.a.w = gx816->regs.d;
  g65816_testn(gx816->regs.d & 0x8000);
  g65816_testz(gx816->regs.d == 0);
  g65816_incpc(1);
}

/*****************
 *** 0x3b: tsc ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_tsce(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.a.w = gx816->regs.s;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
}

void g65816_op_tscn(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.a.w = gx816->regs.s;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
}

/*****************
 *** 0xba: tsx ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_tsxb(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.x = gx816->regs.s & 0xff;
  g65816_testn(gx816->regs.s & 0x80);
  g65816_testz((gx816->regs.s & 0xff) == 0);
  g65816_incpc(1);
}

void g65816_op_tsxw(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.x = gx816->regs.s;
  g65816_testn(gx816->regs.s & 0x8000);
  g65816_testz(gx816->regs.s == 0);
  g65816_incpc(1);
}

/*****************
 *** 0x8a: txa ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_txab(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.a.b = gx816->regs.x;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
}

void g65816_op_txaw(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.a.w = gx816->regs.x;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
}

/*****************
 *** 0x9a: txs ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_txsb(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.s = gx816->regs.x;
  g65816_testn(gx816->regs.x & 0x80);
  g65816_testz((gx816->regs.x & 0xff) == 0);
  g65816_incpc(1);
}

void g65816_op_txsw(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.s = gx816->regs.x;
  g65816_testn(gx816->regs.x & 0x8000);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(1);
}

/*****************
 *** 0x9b: txy ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_txyb(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.y = gx816->regs.x;
  g65816_testn(gx816->regs.x & 0x80);
  g65816_testz((gx816->regs.x & 0xff) == 0);
  g65816_incpc(1);
}

void g65816_op_txyw(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.y = gx816->regs.x;
  g65816_testn(gx816->regs.x & 0x8000);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(1);
}

/*****************
 *** 0x98: tya ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_tyab(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.a.b = gx816->regs.y;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
}

void g65816_op_tyaw(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.a.w = gx816->regs.y;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
}

/*****************
 *** 0xbb: tyx ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
*/

void g65816_op_tyxb(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.x = gx816->regs.y;
  g65816_testn(gx816->regs.y & 0x80);
  g65816_testz((gx816->regs.y & 0xff) == 0);
  g65816_incpc(1);
}

void g65816_op_tyxw(void) {
  snes_time->add_cpu_pcycles(1); //1 [op fetch]
  snes_time->add_cpu_icycles(1); //2 [i/o]
  gx816->regs.x = gx816->regs.y;
  g65816_testn(gx816->regs.y & 0x8000);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(1);
}
