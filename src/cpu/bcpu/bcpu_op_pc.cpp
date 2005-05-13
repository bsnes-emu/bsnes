/**********************
 *** 0x4c: jmp addr ***
 **********************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; new pcl
  [3] pbr,pc+2 ; new pch
*/
void bCPU::op_jmp_addr() {
  rd.l = op_read(); //2
  rd.h = op_read(); //3
  regs.pc.w = rd.w;
}

/**********************
 *** 0x5c: jmp long ***
 **********************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; new pcl
  [3] pbr,pc+2 ; new pch
  [4] pbr,pc+3 ; new pbr
*/
void bCPU::op_jmp_long() {
  rd.l = op_read(); //2
  rd.h = op_read(); //3
  rd.b = op_read(); //4
  regs.pc.d = rd.d;
}

/************************
 *** 0x6c: jmp (addr) ***
 ************************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; aal
  [3] pbr,pc+2 ; aah
  [4] 0,aa     ; new pcl
  [5] 0,aa+1   ; new pch
*/
void bCPU::op_jmp_iaddr() {
  aa.l = op_read();                      //2
  aa.h = op_read();                      //3
  rd.l = op_read(OPMODE_ADDR, aa.w);     //4
  rd.h = op_read(OPMODE_ADDR, aa.w + 1); //5
  regs.pc.w = rd.w;
}

/**************************
 *** 0x7c: jmp (addr,x) ***
 **************************
cycles:
  [1] pbr,pc     ; opcode
  [2] pbr,pc+1   ; aal
  [3] pbr,pc+2   ; aah
  [4] pbr,pc+2   ; io
  [5] pbr,aa+x   ; new pcl
  [6] pbr,aa+x+1 ; new pch
*/
void bCPU::op_jmp_iaddrx() {
  aa.l = op_read();                                //2
  aa.h = op_read();                                //3
  cpu_io();                                        //4
  rd.l = op_read(OPMODE_PBR, aa.w + regs.x.w);     //5
  rd.h = op_read(OPMODE_PBR, aa.w + regs.x.w + 1); //6
  regs.pc.w = rd.w;
}

/************************
 *** 0xdc: jmp [addr] ***
 ************************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; aal
  [3] pbr,pc+2 ; aah
  [4] 0,aa     ; new pcl
  [5] 0,aa+1   ; new pch
  [6] 0,aa+2   ; new pbr
*/
void bCPU::op_jmp_iladdr() {
  aa.l = op_read();                      //2
  aa.h = op_read();                      //3
  rd.l = op_read(OPMODE_ADDR, aa.w);     //4
  rd.h = op_read(OPMODE_ADDR, aa.w + 1); //5
  rd.b = op_read(OPMODE_ADDR, aa.w + 2); //6
  regs.pc.d = rd.d;
}

/**********************
 *** 0x20: jsr addr ***
 **********************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; new pcl
  [3] pbr,pc+2 ; new pch
  [4] pbr,pc+2 ; io
  [5] 0,s      ; pch
  [6] 0,s-1    ; pcl
*/
void bCPU::op_jsr_addr() {
  aa.l = op_read();       //2
  aa.h = op_read();       //3
  cpu_io();               //4
  regs.pc.w--;
  stack_write(regs.pc.h); //5
  stack_write(regs.pc.l); //6
  regs.pc.w = aa.w;
}

/**********************
 *** 0x22: jsr long ***
 **********************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; new pcl
  [3] pbr,pc+2 ; new pch
  [4] 0,s      ; pbr
  [5] 0,s      ; io
  [6] pbr,pc+3 ; new pbr
  [7] 0,s-1    ; pch
  [8] 0,s-2    ; pcl
*/
void bCPU::op_jsr_long() {
  aa.l    = op_read();    //2
  aa.h    = op_read();    //3
  stack_write(regs.pc.b); //4
  cpu_io();               //5
  aa.b = op_read();       //6
  regs.pc.w--;
  stack_write(regs.pc.h); //7
  stack_write(regs.pc.l); //8
  regs.pc.d = aa.d;
}

/**************************
 *** 0xfc: jsr (addr,x) ***
 **************************
cycles:
  [1] pbr,pc     ; opcode
  [2] pbr,pc+1   ; aal
  [3] 0,s        ; pch
  [4] 0,s-1      ; pcl
  [5] pbr,pc+2   ; aah
  [6] pbr,pc+2   ; io
  [7] pbr,aa+x   ; new pcl
  [8] pbr,aa+x+1 ; new pch
*/
void bCPU::op_jsr_iaddrx() {
  aa.l = op_read();                                //2
  stack_write(regs.pc.h);                          //3
  stack_write(regs.pc.l);                          //4
  aa.h = op_read();                                //5
  cpu_io();                                        //6
  rd.l = op_read(OPMODE_PBR, aa.w + regs.x.w);     //7
  rd.h = op_read(OPMODE_PBR, aa.w + regs.x.w + 1); //8
  regs.pc.w = rd.w;
}

/*****************
 *** 0x40: rti ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
  [3] pbr,pc+1 ; io
  [4] 0,s+1    ; p
  [5] 0,s+2    ; new pcl
  [6] 0,s+3    ; new pch
  [7] 0,s+4    ; pbr [7]
*/
void bCPU::op_rtie() {
  cpu_io();              //2
  cpu_io();              //3
  regs.p = stack_read(); //4
  rd.l   = stack_read(); //5
  rd.h   = stack_read(); //6
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
  regs.pc.w = rd.w;
}

void bCPU::op_rtin() {
  cpu_io();              //2
  cpu_io();              //3
  regs.p = stack_read(); //4
  rd.l   = stack_read(); //5
  rd.h   = stack_read(); //6
  rd.b   = stack_read(); //7
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
  regs.pc.d = rd.d;
}

void bCPU::op_rti() { (regs.e)?op_rtie():op_rtin(); }

/*****************
 *** 0x60: rts ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
  [3] pbr,pc+1 ; io
  [4] 0,s+1    ; pcl
  [5] 0,s+2    ; pch
  [6] 0,s+2    ; io
*/
void bCPU::op_rts() {
  cpu_io();            //2
  cpu_io();            //3
  rd.l = stack_read(); //4
  rd.h = stack_read(); //5
  cpu_io();            //6
  regs.pc.w = rd.w;
  regs.pc.w++;
}

/*****************
 *** 0x6b: rtl ***
 *****************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; io
  [3] pbr,pc+1 ; io
  [4] 0,s+1    ; pcl
  [5] 0,s+2    ; pch
  [6] 0,s+3    ; pbr
*/
void bCPU::op_rtl() {
  cpu_io();            //2
  cpu_io();            //3
  rd.l = stack_read(); //4
  rd.h = stack_read(); //5
  rd.b = stack_read(); //6
  regs.pc.d = rd.d;
  regs.pc.w++;
}

/**********************
 *** 0x80: bra near ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; offset
  [2a] pbr,pc+1 ; io [5]
  [2b] pbr,pc+1 ; io [6]
*/
void bCPU::op_bra() {
uint16 r;
  rd.l = op_read();           //2
  cpu_io();                   //2a
  r = regs.pc + (int8)rd.l;
  cpu_c6(r);                  //2b
  regs.pc.w = r;
}

/*********************
 *** 0x82: brl far ***
 *********************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; offset low
  [3] pbr,pc+2 ; offset high
  [4] pbr,pc+2 ; io
*/
void bCPU::op_brl() {
uint16 r;
  rd.l = op_read();            //2
  rd.h = op_read();            //3
  cpu_io();                    //4
  r = regs.pc + (int16)rd.w;
  regs.pc.w = r;
}

/**********************
 *** 0x90: bcc near ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; offset
  [2a] pbr,pc+1 ; io [5]
  [2b] pbr,pc+1 ; io [6]
*/
void bCPU::op_bcc() {
uint16 r;
  rd.l = op_read();             //2
  if(!regs.p.c) {
    cpu_io();                   //2a
    r = regs.pc + (int8)rd.l;
    cpu_c6(r);                  //2b
    regs.pc.w = r;
  }
}

/**********************
 *** 0xb0: bcs near ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; offset
  [2a] pbr,pc+1 ; io [5]
  [2b] pbr,pc+1 ; io [6]
*/
void bCPU::op_bcs() {
uint16 r;
  rd.l = op_read();             //2
  if(regs.p.c) {
    cpu_io();                   //2a
    r = regs.pc + (int8)rd.l;
    cpu_c6(r);                  //2b
    regs.pc.w = r;
  }
}

/**********************
 *** 0xd0: bne near ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; offset
  [2a] pbr,pc+1 ; io [5]
  [2b] pbr,pc+1 ; io [6]
*/
void bCPU::op_bne() {
uint16 r;
  rd.l = op_read();             //2
  if(!regs.p.z) {
    cpu_io();                   //2a
    r = regs.pc + (int8)rd.l;
    cpu_c6(r);                  //2b
    regs.pc.w = r;
  }
}

/**********************
 *** 0xf0: beq near ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; offset
  [2a] pbr,pc+1 ; io [5]
  [2b] pbr,pc+1 ; io [6]
*/
void bCPU::op_beq() {
uint16 r;
  rd.l = op_read();             //2
  if(regs.p.z) {
    cpu_io();                   //2a
    r = regs.pc + (int8)rd.l;
    cpu_c6(r);                  //2b
    regs.pc.w = r;
  }
}

/**********************
 *** 0x10: bpl near ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; offset
  [2a] pbr,pc+1 ; io [5]
  [2b] pbr,pc+1 ; io [6]
*/
void bCPU::op_bpl() {
uint16 r;
  rd.l = op_read();             //2
  if(!regs.p.n) {
    cpu_io();                   //2a
    r = regs.pc + (int8)rd.l;
    cpu_c6(r);                  //2b
    regs.pc.w = r;
  }
}

/**********************
 *** 0x30: bmi near ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; offset
  [2a] pbr,pc+1 ; io [5]
  [2b] pbr,pc+1 ; io [6]
*/
void bCPU::op_bmi() {
uint16 r;
  rd.l = op_read();             //2
  if(regs.p.n) {
    cpu_io();                   //2a
    r = regs.pc + (int8)rd.l;
    cpu_c6(r);                  //2b
    regs.pc.w = r;
  }
}

/**********************
 *** 0x50: bvc near ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; offset
  [2a] pbr,pc+1 ; io [5]
  [2b] pbr,pc+1 ; io [6]
*/
void bCPU::op_bvc() {
uint16 r;
  rd.l = op_read();             //2
  if(!regs.p.v) {
    cpu_io();                   //2a
    r = regs.pc + (int8)rd.l;
    cpu_c6(r);                  //2b
    regs.pc.w = r;
  }
}

/**********************
 *** 0x70: bvs near ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; offset
  [2a] pbr,pc+1 ; io [5]
  [2b] pbr,pc+1 ; io [6]
*/
void bCPU::op_bvs() {
uint16 r;
  rd.l = op_read();             //2
  if(regs.p.v) {
    cpu_io();                   //2a
    r = regs.pc + (int8)rd.l;
    cpu_c6(r);                  //2b
    regs.pc.w = r;
  }
}
