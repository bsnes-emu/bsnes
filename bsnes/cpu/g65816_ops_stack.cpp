/*****************
 *** 0x48: pha ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3a] 0,s      ; reg high [1]
  [3 ] 0,s-1    ; reg low
*/

void g65816_op_phab(void) {
  snes_time->add_cpu_pcycles(1);       //1 [op fetch]
  snes_time->add_cpu_icycles(1);       //2 [i/o]
  gx816->stack_write(gx816->regs.a.b); //3 [reg low]
  g65816_incpc(1);
}

void g65816_op_phaw(void) {
  snes_time->add_cpu_pcycles(1);         //1 [op fetch]
  snes_time->add_cpu_icycles(1);         //2 [i/o]
  gx816->stack_write(gx816->regs.a.p.h); //3a [reg high]
  gx816->stack_write(gx816->regs.a.p.l); //3 [reg low]
  g65816_incpc(1);
}

/*****************
 *** 0x8b: phb ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3a] 0,s      ; reg high [1]
  [3 ] 0,s-1    ; reg low
*/

void g65816_op_phb(void) {
  snes_time->add_cpu_pcycles(1);      //1 [op fetch]
  snes_time->add_cpu_icycles(1);      //2 [i/o]
  gx816->stack_write(gx816->regs.db); //3 [reg low]
  g65816_incpc(1);
}

/*****************
 *** 0x0b: phd ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3a] 0,s      ; reg high [1]
  [3 ] 0,s-1    ; reg low
*/

void g65816_op_phd(void) {
  snes_time->add_cpu_pcycles(1);          //1 [op fetch]
  snes_time->add_cpu_icycles(1);          //2 [i/o]
  gx816->stack_write(gx816->regs.d >> 8); //3a [reg high]
  gx816->stack_write(gx816->regs.d);      //3 [reg low]
  g65816_incpc(1);
}

/*****************
 *** 0x4b: phk ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3a] 0,s      ; reg high [1]
  [3 ] 0,s-1    ; reg low
*/

void g65816_op_phk(void) {
  snes_time->add_cpu_pcycles(1);            //1 [op fetch]
  snes_time->add_cpu_icycles(1);            //2 [i/o]
  gx816->stack_write(gx816->regs.pc >> 16); //3 [reg low]
  g65816_incpc(1);
}

/*****************
 *** 0x08: php ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3a] 0,s      ; reg high [1]
  [3 ] 0,s-1    ; reg low
*/

void g65816_op_php(void) {
  snes_time->add_cpu_pcycles(1);     //1 [op fetch]
  snes_time->add_cpu_icycles(1);     //2 [i/o]
  gx816->stack_write(gx816->regs.p); //3 [reg low]
  g65816_incpc(1);
}

/*****************
 *** 0xda: phx ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3a] 0,s      ; reg high [1]
  [3 ] 0,s-1    ; reg low
*/

void g65816_op_phxb(void) {
  snes_time->add_cpu_pcycles(1);     //1 [op fetch]
  snes_time->add_cpu_icycles(1);     //2 [i/o]
  gx816->stack_write(gx816->regs.x); //3 [reg low]
  g65816_incpc(1);
}

void g65816_op_phxw(void) {
  snes_time->add_cpu_pcycles(1);          //1 [op fetch]
  snes_time->add_cpu_icycles(1);          //2 [i/o]
  gx816->stack_write(gx816->regs.x >> 8); //3a [reg high]
  gx816->stack_write(gx816->regs.x);      //3 [reg low]
  g65816_incpc(1);
}

/*****************
 *** 0x5a: phy ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3a] 0,s      ; reg high [1]
  [3 ] 0,s-1    ; reg low
*/

void g65816_op_phyb(void) {
  snes_time->add_cpu_pcycles(1);     //1 [op fetch]
  snes_time->add_cpu_icycles(1);     //2 [i/o]
  gx816->stack_write(gx816->regs.y); //3 [reg low]
  g65816_incpc(1);
}

void g65816_op_phyw(void) {
  snes_time->add_cpu_pcycles(1);          //1 [op fetch]
  snes_time->add_cpu_icycles(1);          //2 [i/o]
  gx816->stack_write(gx816->regs.y >> 8); //3a [reg high]
  gx816->stack_write(gx816->regs.y);      //3 [reg low]
  g65816_incpc(1);
}

/*****************
 *** 0x68: pla ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3 ] pbr,pc+1 ; io
  [4 ] 0,s+1    ; reg low
  [4a] 0,s+2    ; reg high [1]
*/

void g65816_op_plab(void) {
  snes_time->add_cpu_pcycles(1);         //1 [op fetch]
  snes_time->add_cpu_icycles(2);         //2,3 [i/o]
  gx816->regs.a.b = gx816->stack_read(); //4 [reg low]
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
}

void g65816_op_plaw(void) {
  snes_time->add_cpu_pcycles(1);           //1 [op fetch]
  snes_time->add_cpu_icycles(2);           //2,3 [i/o]
  gx816->regs.a.p.l = gx816->stack_read(); //4 [reg low]
  gx816->regs.a.p.h = gx816->stack_read(); //4a [reg high]
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
}

/*****************
 *** 0xab: plb ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3 ] pbr,pc+1 ; io
  [4 ] 0,s+1    ; reg low
  [4a] 0,s+2    ; reg high [1]
*/

void g65816_op_plb(void) {
  snes_time->add_cpu_pcycles(1);        //1 [op fetch]
  snes_time->add_cpu_icycles(2);        //2,3 [i/o]
  gx816->regs.db = gx816->stack_read(); //4 [reg low]
  g65816_testn(gx816->regs.db & 0x80);
  g65816_testz(gx816->regs.db == 0);
  g65816_incpc(1);
}

/*****************
 *** 0x2b: pld ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3 ] pbr,pc+1 ; io
  [4 ] 0,s+1    ; reg low
  [4a] 0,s+2    ; reg high [1]
*/

void g65816_op_pld(void) {
  snes_time->add_cpu_pcycles(1);             //1 [op fetch]
  snes_time->add_cpu_icycles(2);             //2,3 [i/o]
  gx816->regs.d  = gx816->stack_read();      //4 [reg low]
  gx816->regs.d |= gx816->stack_read() << 8; //4a [reg high]
  g65816_testn(gx816->regs.d & 0x8000);
  g65816_testz(gx816->regs.d == 0);
  g65816_incpc(1);
}

/*****************
 *** 0x28: plp ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3 ] pbr,pc+1 ; io
  [4 ] 0,s+1    ; reg low
  [4a] 0,s+2    ; reg high [1]
*/

void g65816_op_plp(void) {
  snes_time->add_cpu_pcycles(1);       //1 [op fetch]
  snes_time->add_cpu_icycles(2);       //2,3 [i/o]
  gx816->regs.p = gx816->stack_read(); //4 [reg low]
  g65816_incpc(1);
  if(gx816->regs.e == true)gx816->regs.p |= 0x30;
  if(gx816->regs.p & PF_X) { gx816->regs.x &= 0xff; gx816->regs.y &= 0xff; }
}

/*****************
 *** 0xfa: plx ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3 ] pbr,pc+1 ; io
  [4 ] 0,s+1    ; reg low
  [4a] 0,s+2    ; reg high [1]
*/

void g65816_op_plxb(void) {
  snes_time->add_cpu_pcycles(1);       //1 [op fetch]
  snes_time->add_cpu_icycles(2);       //2,3 [i/o]
  gx816->regs.x = gx816->stack_read(); //4 [reg low]
  g65816_testn(gx816->regs.x & 0x80);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(1);
}

void g65816_op_plxw(void) {
  snes_time->add_cpu_pcycles(1);             //1 [op fetch]
  snes_time->add_cpu_icycles(2);             //2,3 [i/o]
  gx816->regs.x  = gx816->stack_read();      //4 [reg low]
  gx816->regs.x |= gx816->stack_read() << 8; //4a [reg high]
  g65816_testn(gx816->regs.x & 0x8000);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(1);
}

/*****************
 *** 0x7a: ply ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3 ] pbr,pc+1 ; io
  [4 ] 0,s+1    ; reg low
  [4a] 0,s+2    ; reg high [1]
*/

void g65816_op_plyb(void) {
  snes_time->add_cpu_pcycles(1);       //1 [op fetch]
  snes_time->add_cpu_icycles(2);       //2,3 [i/o]
  gx816->regs.y = gx816->stack_read(); //4 [reg low]
  g65816_testn(gx816->regs.y & 0x80);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(1);
}

void g65816_op_plyw(void) {
  snes_time->add_cpu_pcycles(1);             //1 [op fetch]
  snes_time->add_cpu_icycles(2);             //2,3 [i/o]
  gx816->regs.y  = gx816->stack_read();      //4 [reg low]
  gx816->regs.y |= gx816->stack_read() << 8; //4a [reg high]
  g65816_testn(gx816->regs.y & 0x8000);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(1);
}

/**********************
 *** 0xf4: pea addr ***
 **********************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; aal
  [3] pbr,pc+2 ; aah
  [4] 0,s      ; aah
  [5] 0,s-1    ; aal
*/

void g65816_op_pea(void) {
  gx816->op.aa.w = gx816->read_operand(2); //1-3 [op fetch]
  gx816->stack_write(gx816->op.aa.p.h);    //4 [write high]
  gx816->stack_write(gx816->op.aa.p.l);    //5 [write low]
  g65816_incpc(3);
}

/**********************
 *** 0xd4: pei (dp) ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io
  [3 ] 0,d+dp   ; aal
  [4 ] 0,d+dp+1 ; aah
  [5 ] 0,s      ; aah
  [6 ] 0,s-1    ; aal
*/

void g65816_op_pei(void) {
  gx816->op.dp = gx816->read_operand(1);                          //1,2 [op fetch]
  gx816->op_cond(2);                                              //2a [i/o]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);     //3 [read aal]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1); //4 [read aah]
  gx816->stack_write(gx816->op.aa.p.h);                           //5 [write aah]
  gx816->stack_write(gx816->op.aa.p.l);                           //6 [write aal]
  g65816_incpc(2);
}

/**********************
 *** 0x62: per addr ***
 **********************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; offset low
  [3] pbr,pc+2 ; offset high
  [4] pbr,pc+2 ; io
  [5] 0,s      ; pch+offset
  [6] 0,s-1    ; pcl+offset
*/

void g65816_op_per(void) {
word r;
  gx816->op.aa.w = gx816->read_operand(2); //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);           //4 [i/o]
  r = (gx816->regs.pc + (signed short)gx816->op.aa.w + 3);
  gx816->stack_write(r >> 8);              //5 [write pch]
  gx816->stack_write(r);                   //6 [write pcl]
  g65816_incpc(3);
}
