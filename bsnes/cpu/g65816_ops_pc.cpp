/**********************
 *** 0x4c: jmp addr ***
 **********************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; new pcl
  [3] pbr,pc+2 ; new pch
*/

void g65816_op_jmp_addr(void) {
  gx816->op.r.w = gx816->read_operand(2); //1-3 [op fetch]

  gx816->regs.pc = (gx816->regs.pc & 0xff0000) | gx816->op.r.w;
}

/**********************
 *** 0x5c: jml long ***
 **********************
cycles:
  [1] pbr,pc   ; opcode
  [2] pbr,pc+1 ; new pcl
  [3] pbr,pc+2 ; new pch
  [4] pbr,pc+3 ; new pbr
*/

void g65816_op_jmp_long(void) {
  gx816->regs.pc = gx816->read_operand(3); //1-4 [op fetch]
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

void g65816_op_jmp_iaddr(void) {
  gx816->op.aa.w = gx816->read_operand(2);                           //1-3 [op fetch]
  gx816->op.r.p.l = gx816->op_read(OPMODE_ADDR, gx816->op.aa.w);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_ADDR, gx816->op.aa.w + 1); //5 [read high]

  gx816->regs.pc = (gx816->regs.pc & 0xff0000) | gx816->op.r.w;
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

void g65816_op_jmp_iaddrx(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                          //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);                                                    //4 [i/o]
  gx816->op.r.p.l = gx816->op_read(OPMODE_PBR, gx816->op.aa.w + gx816->regs.x);     //5 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_PBR, gx816->op.aa.w + gx816->regs.x + 1); //6 [read high]

  gx816->regs.pc = (gx816->regs.pc & 0xff0000) | gx816->op.r.w;
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

void g65816_op_jmp_iladdr(void) {
  gx816->op.aa.w = gx816->read_operand(2);                           //1-3 [op fetch]
  gx816->op.r.p.l = gx816->op_read(OPMODE_ADDR, gx816->op.aa.w);     //4 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_ADDR, gx816->op.aa.w + 1); //5 [read high]
  gx816->op.r.p.b = gx816->op_read(OPMODE_ADDR, gx816->op.aa.w + 2); //6 [read bank]

  gx816->regs.pc = gx816->op.r.l;
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

void g65816_op_jsr_addr(void) {
  gx816->op.r.w = gx816->read_operand(2);   //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);            //4 [i/o]
  gx816->regs.pc += 2;
  gx816->stack_write(gx816->regs.pc >> 8);  //5 [write high]
  gx816->stack_write(gx816->regs.pc);       //6 [write low]

  gx816->regs.pc = (gx816->regs.pc & 0xff0000) | gx816->op.r.w;
}

/**********************
 *** 0x22: jsl long ***
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

void g65816_op_jsr_long(void) {
  gx816->op.r.l = gx816->read_operand(3);                //1-3,6 [op fetch]
  gx816->regs.pc += 3;
  gx816->stack_write(gx816->regs.pc >> 16);              //4 [write bank]
  snes_time->add_cpu_icycles(1);                         //5 [i/o]
  gx816->stack_write(gx816->regs.pc >> 8);               //7 [write high]
  gx816->stack_write(gx816->regs.pc);                    //8 [write low]

  gx816->regs.pc = gx816->op.r.l;
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

void g65816_op_jsr_iaddrx(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                          //1,2,5 [op fetch]
  gx816->regs.pc += 2;
  gx816->stack_write(gx816->regs.pc >> 8);                                          //3 [write high]
  gx816->stack_write(gx816->regs.pc);                                               //4 [write low]
  snes_time->add_cpu_icycles(1);                                                    //6 [i/o]
  gx816->op.r.p.l = gx816->op_read(OPMODE_PBR, gx816->op.aa.w + gx816->regs.x);     //7 [read low]
  gx816->op.r.p.h = gx816->op_read(OPMODE_PBR, gx816->op.aa.w + gx816->regs.x + 1); //8 [read high]

  gx816->regs.pc = (gx816->regs.pc & 0xff0000) | gx816->op.r.w;
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

void g65816_op_rtie(void) {
  snes_time->add_cpu_pcycles(1);         //1 [op fetch]
  snes_time->add_cpu_icycles(2);         //2,3 [i/o]
  gx816->regs.p   = gx816->stack_read(); //4 [read p]
  gx816->op.r.p.l = gx816->stack_read(); //5 [read pcl]
  gx816->op.r.p.h = gx816->stack_read(); //6 [read pch]

  gx816->regs.pc = (gx816->regs.pc & 0xff0000) | gx816->op.r.w;
  if(gx816->regs.p & PF_X) { gx816->regs.x &= 0xff; gx816->regs.y &= 0xff; }
  gx816->regs.p &= ~ PF_I;
}

void g65816_op_rtin(void) {
  snes_time->add_cpu_pcycles(1);         //1 [op fetch]
  snes_time->add_cpu_icycles(2);         //2,3 [i/o]
  gx816->regs.p   = gx816->stack_read(); //4 [read p]
  gx816->op.r.p.l = gx816->stack_read(); //5 [read pcl]
  gx816->op.r.p.h = gx816->stack_read(); //6 [read pch]
  gx816->op.r.p.b = gx816->stack_read(); //7 [read pbr]

  gx816->regs.pc = gx816->op.r.l;
  if(gx816->regs.p & PF_X) { gx816->regs.x &= 0xff; gx816->regs.y &= 0xff; }
  gx816->regs.p &= ~ PF_I;
}

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

void g65816_op_rts(void) {
  snes_time->add_cpu_pcycles(1);         //1 [op fetch]
  snes_time->add_cpu_icycles(2);         //2,3 [i/o]
  gx816->op.r.p.l = gx816->stack_read(); //4 [read low]
  gx816->op.r.p.h = gx816->stack_read(); //5 [read high]
  snes_time->add_cpu_icycles(1);         //6 [i/o]

  gx816->regs.pc = (gx816->regs.pc & 0xff0000) | gx816->op.r.w;
  g65816_incpc(1);
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

void g65816_op_rtl(void) {
  snes_time->add_cpu_pcycles(1);         //1 [op fetch]
  snes_time->add_cpu_icycles(2);         //2,3 [i/o]
  gx816->op.r.p.l = gx816->stack_read(); //4 [read low]
  gx816->op.r.p.h = gx816->stack_read(); //5 [read high]
  gx816->op.r.p.b = gx816->stack_read(); //6 [read bank]

  gx816->regs.pc = gx816->op.r.l;
  g65816_incpc(1);
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

void g65816_op_bra(void) {
word r;
  gx816->op.r.b = gx816->read_operand(1);           //1,2 [op fetch]
  snes_time->add_cpu_icycles(1);                    //2a [i/o]
  r = gx816->regs.pc + (signed char)gx816->op.r.b;
  if(gx816->regs.e == true) {
    if((gx816->regs.pc & 0xff00) != ((r + 2) & 0xff00)) {
      snes_time->add_cpu_icycles(1);                //2b [i/o]
    }
  }
  gx816->regs.pc = (gx816->regs.pc & 0xff0000) | r;
  g65816_incpc(2);
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

void g65816_op_brl(void) {
word r;
  gx816->op.r.w = gx816->read_operand(2); //1-3 [op fetch]
  snes_time->add_cpu_icycles(1);          //4 [i/o]
  r = gx816->regs.pc + (signed short)gx816->op.r.w;
  gx816->regs.pc = (gx816->regs.pc & 0xff0000) | r;
  g65816_incpc(3);
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

void g65816_op_bcc(void) {
word r;
  gx816->op.r.b = gx816->read_operand(1);             //1,2 [op fetch]
  if(!(gx816->regs.p & PF_C)) {
    snes_time->add_cpu_icycles(1);                    //2a [i/o]
    r = gx816->regs.pc + (signed char)gx816->op.r.b;
    if(gx816->regs.e == true) {
      if((gx816->regs.pc & 0xff00) != ((r + 2) & 0xff00)) {
        snes_time->add_cpu_icycles(1);                //2b [i/o]
      }
    }
    gx816->regs.pc = (gx816->regs.pc & 0xff0000) | r;
  }
  g65816_incpc(2);
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

void g65816_op_bcs(void) {
word r;
  gx816->op.r.b = gx816->read_operand(1);             //1,2 [op fetch]
  if(gx816->regs.p & PF_C) {
    snes_time->add_cpu_icycles(1);                    //2a [i/o]
    r = gx816->regs.pc + (signed char)gx816->op.r.b;
    if(gx816->regs.e == true) {
      if((gx816->regs.pc & 0xff00) != ((r + 2) & 0xff00)) {
        snes_time->add_cpu_icycles(1);                //2b [i/o]
      }
    }
    gx816->regs.pc = (gx816->regs.pc & 0xff0000) | r;
  }
  g65816_incpc(2);
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

void g65816_op_bne(void) {
word r;
  gx816->op.r.b = gx816->read_operand(1);             //1,2 [op fetch]
  if(!(gx816->regs.p & PF_Z)) {
    snes_time->add_cpu_icycles(1);                    //2a [i/o]
    r = gx816->regs.pc + (signed char)gx816->op.r.b;
    if(gx816->regs.e == true) {
      if((gx816->regs.pc & 0xff00) != ((r + 2) & 0xff00)) {
        snes_time->add_cpu_icycles(1);                //2b [i/o]
      }
    }
    gx816->regs.pc = (gx816->regs.pc & 0xff0000) | r;
  }
  g65816_incpc(2);
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

void g65816_op_beq(void) {
word r;
  gx816->op.r.b = gx816->read_operand(1);             //1,2 [op fetch]
  if(gx816->regs.p & PF_Z) {
    snes_time->add_cpu_icycles(1);                    //2a [i/o]
    r = gx816->regs.pc + (signed char)gx816->op.r.b;
    if(gx816->regs.e == true) {
      if((gx816->regs.pc & 0xff00) != ((r + 2) & 0xff00)) {
        snes_time->add_cpu_icycles(1);                //2b [i/o]
      }
    }
    gx816->regs.pc = (gx816->regs.pc & 0xff0000) | r;
  }
  g65816_incpc(2);
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

void g65816_op_bpl(void) {
word r;
  gx816->op.r.b = gx816->read_operand(1);             //1,2 [op fetch]
  if(!(gx816->regs.p & PF_N)) {
    snes_time->add_cpu_icycles(1);                    //2a [i/o]
    r = gx816->regs.pc + (signed char)gx816->op.r.b;
    if(gx816->regs.e == true) {
      if((gx816->regs.pc & 0xff00) != ((r + 2) & 0xff00)) {
        snes_time->add_cpu_icycles(1);                //2b [i/o]
      }
    }
    gx816->regs.pc = (gx816->regs.pc & 0xff0000) | r;
  }
  g65816_incpc(2);
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

void g65816_op_bmi(void) {
word r;
  gx816->op.r.b = gx816->read_operand(1);             //1,2 [op fetch]
  if(gx816->regs.p & PF_N) {
    snes_time->add_cpu_icycles(1);                    //2a [i/o]
    r = gx816->regs.pc + (signed char)gx816->op.r.b;
    if(gx816->regs.e == true) {
      if((gx816->regs.pc & 0xff00) != ((r + 2) & 0xff00)) {
        snes_time->add_cpu_icycles(1);                //2b [i/o]
      }
    }
    gx816->regs.pc = (gx816->regs.pc & 0xff0000) | r;
  }
  g65816_incpc(2);
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

void g65816_op_bvc(void) {
word r;
  gx816->op.r.b = gx816->read_operand(1);             //1,2 [op fetch]
  if(!(gx816->regs.p & PF_V)) {
    snes_time->add_cpu_icycles(1);                    //2a [i/o]
    r = gx816->regs.pc + (signed char)gx816->op.r.b;
    if(gx816->regs.e == true) {
      if((gx816->regs.pc & 0xff00) != ((r + 2) & 0xff00)) {
        snes_time->add_cpu_icycles(1);                //2b [i/o]
      }
    }
    gx816->regs.pc = (gx816->regs.pc & 0xff0000) | r;
  }
  g65816_incpc(2);
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

void g65816_op_bvs(void) {
word r;
  gx816->op.r.b = gx816->read_operand(1);             //1,2 [op fetch]
  if(gx816->regs.p & PF_V) {
    snes_time->add_cpu_icycles(1);                    //2a [i/o]
    r = gx816->regs.pc + (signed char)gx816->op.r.b;
    if(gx816->regs.e == true) {
      if((gx816->regs.pc & 0xff00) != ((r + 2) & 0xff00)) {
        snes_time->add_cpu_icycles(1);                //2b [i/o]
      }
    }
    gx816->regs.pc = (gx816->regs.pc & 0xff0000) | r;
  }
  g65816_incpc(2);
}
