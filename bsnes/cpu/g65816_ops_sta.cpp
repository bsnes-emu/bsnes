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

void g65816_op_sta_addrb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                      //1-3 [op fetch]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w, gx816->regs.a.b); //4 [write]

  g65816_incpc(3);
}

void g65816_op_sta_addrw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                            //1-3 [op fetch]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w,     gx816->regs.a.p.l); //4 [write low]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + 1, gx816->regs.a.p.h); //4a [write high]

  g65816_incpc(3);
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

void g65816_op_sta_addrxb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                      //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                                //3a [write i/o]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x, gx816->regs.a.b); //4 [write]

  g65816_incpc(3);
}

void g65816_op_sta_addrxw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                            //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                                      //3a [write i/o]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x,     gx816->regs.a.p.l); //4 [write low]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x + 1, gx816->regs.a.p.h); //4a [write high]

  g65816_incpc(3);
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

void g65816_op_sta_dpb(void) {
  gx816->op.dp = gx816->read_operand(1);                     //1,2 [op fetch]
  gx816->op_cond(2);                                         //2a [dl!=0]
  gx816->op_write(OPMODE_DP, gx816->op.dp, gx816->regs.a.b); //3 [write]

  g65816_incpc(2);
}

void g65816_op_sta_dpw(void) {
  gx816->op.dp = gx816->read_operand(1);                           //1,2 [op fetch]
  gx816->op_cond(2);                                               //2a [dl!=0]
  gx816->op_write(OPMODE_DP, gx816->op.dp,     gx816->regs.a.p.l); //3 [write low]
  gx816->op_write(OPMODE_DP, gx816->op.dp + 1, gx816->regs.a.p.h); //3a [write high]

  g65816_incpc(2);
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

void g65816_op_sta_idpb(void) {
  gx816->op.dp = gx816->read_operand(1);                          //1,2 [op fetch]
  gx816->op_cond(2);                                              //2a [dl!=0]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);     //3 [aal]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1); //4 [aah]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w, gx816->regs.a.b);   //5 [write]

  g65816_incpc(2);
}

void g65816_op_sta_idpw(void) {
  gx816->op.dp = gx816->read_operand(1);                              //1,2 [op fetch]
  gx816->op_cond(2);                                                  //2a [dl!=0]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);         //3 [aal]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1);     //4 [aah]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w,     gx816->regs.a.p.l); //5 [write low]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + 1, gx816->regs.a.p.h); //5a [write high]

  g65816_incpc(2);
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

void g65816_op_sta_ildpb(void) {
  gx816->op.dp = gx816->read_operand(1);                          //1,2 [op fetch]
  gx816->op_cond(2);                                              //2a [dl!=0]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);     //3 [aal]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1); //4 [aah]
  gx816->op.aa.p.b = gx816->op_read(OPMODE_DP, gx816->op.dp + 2); //5 [aab]
  gx816->op_write(OPMODE_LONG, gx816->op.aa.l, gx816->regs.a.b);  //6 [write]

  g65816_incpc(2);
}

void g65816_op_sta_ildpw(void) {
  gx816->op.dp = gx816->read_operand(1);                               //1,2 [op fetch]
  gx816->op_cond(2);                                                   //2a [dl!=0]
  gx816->op.aa.p.l  = gx816->op_read(OPMODE_DP, gx816->op.dp);         //3 [aal]
  gx816->op.aa.p.h  = gx816->op_read(OPMODE_DP, gx816->op.dp + 1);     //4 [aah]
  gx816->op.aa.p.b  = gx816->op_read(OPMODE_DP, gx816->op.dp + 2);     //5 [aab]
  gx816->op_write(OPMODE_LONG, gx816->op.aa.l,     gx816->regs.a.p.l); //6 [write low]
  gx816->op_write(OPMODE_LONG, gx816->op.aa.l + 1, gx816->regs.a.p.h); //6a [write high]

  g65816_incpc(2);
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

void g65816_op_sta_longb(void) {
  gx816->op.aa.l = gx816->read_operand(3);                       //1-4 [op fetch]
  gx816->op_write(OPMODE_LONG, gx816->op.aa.l, gx816->regs.a.b); //5 [write]

  g65816_incpc(4);
}

void g65816_op_sta_longw(void) {
  gx816->op.aa.l = gx816->read_operand(3);                             //1-4 [op fetch]
  gx816->op_write(OPMODE_LONG, gx816->op.aa.l,     gx816->regs.a.p.l); //5 [write]
  gx816->op_write(OPMODE_LONG, gx816->op.aa.l + 1, gx816->regs.a.p.h); //5a [write]

  g65816_incpc(4);
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

void g65816_op_sta_longxb(void) {
  gx816->op.aa.l = gx816->read_operand(3);                                       //1-4 [op fetch]
  gx816->op_write(OPMODE_LONG, gx816->op.aa.l + gx816->regs.x, gx816->regs.a.b); //5 [write]

  g65816_incpc(4);
}

void g65816_op_sta_longxw(void) {
  gx816->op.aa.l = gx816->read_operand(3);                                             //1-4 [op fetch]
  gx816->op_write(OPMODE_LONG, gx816->op.aa.l + gx816->regs.x,     gx816->regs.a.p.l); //5 [write]
  gx816->op_write(OPMODE_LONG, gx816->op.aa.l + gx816->regs.x + 1, gx816->regs.a.p.h); //5a [write]

  g65816_incpc(4);
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

void g65816_op_sta_addryb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                      //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                                //3a [write i/o]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y, gx816->regs.a.b); //4 [write]

  g65816_incpc(3);
}

void g65816_op_sta_addryw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                            //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                                      //3a [write i/o]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y,     gx816->regs.a.p.l); //4 [write low]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y + 1, gx816->regs.a.p.h); //4a [write high]

  g65816_incpc(3);
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

void g65816_op_sta_dpxb(void) {
  gx816->op.dp = gx816->read_operand(1);                                     //1,2 [op fetch]
  gx816->op_cond(2);                                                         //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                             //3 [i/o]
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x, gx816->regs.a.b); //4 [write]

  g65816_incpc(2);
}

void g65816_op_sta_dpxw(void) {
  gx816->op.dp = gx816->read_operand(1);                                           //1,2 [op fetch]
  gx816->op_cond(2);                                                               //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                                   //3 [i/o]
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x,     gx816->regs.a.p.l); //4 [write low]
  gx816->op_write(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1, gx816->regs.a.p.h); //4a [write high]

  g65816_incpc(2);
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

void g65816_op_sta_idpxb(void) {
  gx816->op.dp = gx816->read_operand(1);                                          //1,2 [op fetch]
  gx816->op_cond(2);                                                              //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                                  //3 [i/o]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x);     //4 [aal]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1); //5 [aah]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w, gx816->regs.a.b);                   //6 [write]

  g65816_incpc(2);
}

void g65816_op_sta_idpxw(void) {
  gx816->op.dp = gx816->read_operand(1);                                          //1,2 [op fetch]
  gx816->op_cond(2);                                                              //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                                  //3 [i/o]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x);     //4 [aal]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1); //5 [aah]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w,     gx816->regs.a.p.l);             //6 [write low]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + 1, gx816->regs.a.p.h);             //6 [write high]

  g65816_incpc(2);
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

void g65816_op_sta_idpyb(void) {
  gx816->op.dp = gx816->read_operand(1);                                        //1,2 [op fetch]
  gx816->op_cond(2);                                                            //2a [dl!=0]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);                   //3 [aal]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1);               //4 [aah]
  snes_time->add_cpu_icycles(1);                                                //4a [write i/o]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y, gx816->regs.a.b); //5 [write]

  g65816_incpc(2);
}

void g65816_op_sta_idpyw(void) {
  gx816->op.dp = gx816->read_operand(1);                                              //1,2 [op fetch]
  gx816->op_cond(2);                                                                  //2a [dl!=0]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);                         //3 [aal]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1);                     //4 [aah]
  snes_time->add_cpu_icycles(1);                                                      //4a [write i/o]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y,     gx816->regs.a.p.l); //5 [write low]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y + 1, gx816->regs.a.p.h); //5a [write high]

  g65816_incpc(2);
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

void g65816_op_sta_ildpyb(void) {
  gx816->op.dp = gx816->read_operand(1);                                         //1,2 [op fetch]
  gx816->op_cond(2);                                                             //2a [dl!=0]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);                    //3 [aal]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1);                //4 [aah]
  gx816->op.aa.p.b = gx816->op_read(OPMODE_DP, gx816->op.dp + 2);                //5 [aab]
  gx816->op_write(OPMODE_LONG, gx816->op.aa.l + gx816->regs.y, gx816->regs.a.b); //6 [write]

  g65816_incpc(2);
}

void g65816_op_sta_ildpyw(void) {
  gx816->op.dp = gx816->read_operand(1);                                               //1,2 [op fetch]
  gx816->op_cond(2);                                                                   //2a [dl!=0]
  gx816->op.aa.p.l  = gx816->op_read(OPMODE_DP, gx816->op.dp);                         //3 [aal]
  gx816->op.aa.p.h  = gx816->op_read(OPMODE_DP, gx816->op.dp + 1);                     //4 [aah]
  gx816->op.aa.p.b  = gx816->op_read(OPMODE_DP, gx816->op.dp + 2);                     //5 [aab]
  gx816->op_write(OPMODE_LONG, gx816->op.aa.l + gx816->regs.y,     gx816->regs.a.p.l); //6 [write low]
  gx816->op_write(OPMODE_LONG, gx816->op.aa.l + gx816->regs.y + 1, gx816->regs.a.p.h); //6a [write high]

  g65816_incpc(2);
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

void g65816_op_sta_srb(void) {
  gx816->op.sp = gx816->read_operand(1);                     //1,2 [op fetch]
  snes_time->add_cpu_icycles(1);                             //3 [i/o]
  gx816->op_write(OPMODE_SP, gx816->op.sp, gx816->regs.a.b); //4 [write]

  g65816_incpc(2);
}

void g65816_op_sta_srw(void) {
  gx816->op.sp = gx816->read_operand(1);                           //1,2 [op fetch]
  snes_time->add_cpu_icycles(1);                                   //3 [i/o]
  gx816->op_write(OPMODE_SP, gx816->op.sp,     gx816->regs.a.p.l); //4 [write low]
  gx816->op_write(OPMODE_SP, gx816->op.sp + 1, gx816->regs.a.p.h); //4a [write high]

  g65816_incpc(2);
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

void g65816_op_sta_isryb(void) {
  gx816->op.sp = gx816->read_operand(1);                                        //1,2 [op fetch]
  snes_time->add_cpu_icycles(1);                                                //3 [i/o]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_SP, gx816->op.sp);                   //4 [aal]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_SP, gx816->op.sp + 1);               //5 [aah]
  snes_time->add_cpu_icycles(1);                                                //6 [i/o]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y, gx816->regs.a.b); //7 [write]

  g65816_incpc(2);
}

void g65816_op_sta_isryw(void) {
  gx816->op.sp = gx816->read_operand(1);                                              //1,2 [op fetch]
  snes_time->add_cpu_icycles(1);                                                      //3 [i/o]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_SP, gx816->op.sp);                         //4 [aal]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_SP, gx816->op.sp + 1);                     //5 [aah]
  snes_time->add_cpu_icycles(1);                                                      //6 [i/o]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y,     gx816->regs.a.p.l); //7 [write low]
  gx816->op_write(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y + 1, gx816->regs.a.p.h); //7a [write high]

  g65816_incpc(2);
}
