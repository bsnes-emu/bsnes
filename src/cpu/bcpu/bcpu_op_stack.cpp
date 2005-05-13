/*****************
 *** 0x48: pha ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3a] 0,s      ; reg high [1]
  [3 ] 0,s-1    ; reg low
*/
void bCPU::op_phab() {
  cpu_io();              //2
  stack_write(regs.a.l); //3
}

void bCPU::op_phaw() {
  cpu_io();              //2
  stack_write(regs.a.h); //3a
  stack_write(regs.a.l); //3
}

void bCPU::op_pha() { (regs.p.m)?op_phab():op_phaw(); }

/*****************
 *** 0x8b: phb ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3 ] 0,s-1    ; dbr
*/
void bCPU::op_phb() {
  cpu_io();             //2
  stack_write(regs.db); //3
}

/*****************
 *** 0x0b: phd ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3a] 0,s      ; reg high
  [3 ] 0,s-1    ; reg low
*/
void bCPU::op_phd() {
  cpu_io();              //2
  stack_write(regs.d.h); //3a
  stack_write(regs.d.l); //3
}

/*****************
 *** 0x4b: phk ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3 ] 0,s-1    ; pcb
*/
void bCPU::op_phk() {
  cpu_io();               //2
  stack_write(regs.pc.b); //3
}

/*****************
 *** 0x08: php ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3 ] 0,s-1    ; reg low
*/
void bCPU::op_php() {
  cpu_io();            //2
  stack_write(regs.p); //3
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
void bCPU::op_phxb() {
  cpu_io();              //2
  stack_write(regs.x.l); //3
}

void bCPU::op_phxw() {
  cpu_io();              //2
  stack_write(regs.x.h); //3a
  stack_write(regs.x.l); //3
}

void bCPU::op_phx() { (regs.p.x)?op_phxb():op_phxw(); }

/*****************
 *** 0x5a: phy ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3a] 0,s      ; reg high [1]
  [3 ] 0,s-1    ; reg low
*/
void bCPU::op_phyb() {
  cpu_io();              //2
  stack_write(regs.y.l); //3
}

void bCPU::op_phyw() {
  cpu_io();              //2
  stack_write(regs.y.h); //3a
  stack_write(regs.y.l); //3
}

void bCPU::op_phy() { (regs.p.x)?op_phyb():op_phyw(); }

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
void bCPU::op_plab() {
  cpu_io();                //2
  cpu_io();                //3
  regs.a.l = stack_read(); //4
  regs.p.n = !!(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

void bCPU::op_plaw() {
  cpu_io();                //2
  cpu_io();                //3
  regs.a.l = stack_read(); //4
  regs.a.h = stack_read(); //4a
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

void bCPU::op_pla() { (regs.p.m)?op_plab():op_plaw(); }

/*****************
 *** 0xab: plb ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3 ] pbr,pc+1 ; io
  [4 ] 0,s+1    ; reg low
*/
void bCPU::op_plb() {
  cpu_io();               //2
  cpu_io();               //3
  regs.db = stack_read(); //4
  regs.p.n = !!(regs.db & 0x80);
  regs.p.z = (regs.db == 0);
}

/*****************
 *** 0x2b: pld ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3 ] pbr,pc+1 ; io
  [4 ] 0,s+1    ; reg low
  [4a] 0,s+2    ; reg high
*/
void bCPU::op_pld() {
  cpu_io();                //2
  cpu_io();                //3
  regs.d.l = stack_read(); //4
  regs.d.h = stack_read(); //4a
  regs.p.n = !!(regs.d.w & 0x8000);
  regs.p.z = (regs.d.w == 0);
}

/*****************
 *** 0x28: plp ***
 *****************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; io
  [3 ] pbr,pc+1 ; io
  [4 ] 0,s+1    ; reg low
*/
void bCPU::op_plp() {
  cpu_io();              //2
  cpu_io();              //3
  regs.p = stack_read(); //4
  if(regs.e)regs.p |= 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
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
void bCPU::op_plxb() {
  cpu_io();                //2
  cpu_io();                //3
  regs.x.l = stack_read(); //4
  regs.p.n = !!(regs.x.l & 0x80);
  regs.p.z = (regs.x.l == 0);
}

void bCPU::op_plxw() {
  cpu_io();                //2
  cpu_io();                //3
  regs.x.l = stack_read(); //4
  regs.x.h = stack_read(); //4a
  regs.p.n = !!(regs.x.w & 0x8000);
  regs.p.z = (regs.x.w == 0);
}

void bCPU::op_plx() { (regs.p.x)?op_plxb():op_plxw(); }

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
void bCPU::op_plyb() {
  cpu_io();                //2
  cpu_io();                //3
  regs.y.l = stack_read(); //4
  regs.p.n = !!(regs.y.l & 0x80);
  regs.p.z = (regs.y.l == 0);
}

void bCPU::op_plyw() {
  cpu_io();                //2
  cpu_io();                //3
  regs.y.l = stack_read(); //4
  regs.y.h = stack_read(); //4a
  regs.p.n = !!(regs.y.w & 0x8000);
  regs.p.z = (regs.y.w == 0);
}

void bCPU::op_ply() { (regs.p.x)?op_plyb():op_plyw(); }

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
void bCPU::op_pea() {
  aa.l = op_read();  //2
  aa.h = op_read();  //3
  stack_write(aa.h); //4
  stack_write(aa.l); //5
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
void bCPU::op_pei() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  aa.l = op_read(OPMODE_DP, dp);     //3
  aa.h = op_read(OPMODE_DP, dp + 1); //4
  stack_write(aa.h);                 //5
  stack_write(aa.l);                 //6
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
void bCPU::op_per() {
  aa.l = op_read();               //2
  aa.h = op_read();               //3
  cpu_io();                       //4
  rd.w = regs.pc.d + (int16)aa.w;
  stack_write(rd.h);              //5
  stack_write(rd.l);              //6
}
