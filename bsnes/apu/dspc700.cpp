#include "../base.h"
#include "spc700.h"
extern sony_spc700 *spc700;
extern debugstate  debugger;

char __disas_spc700_str[256];

word __disas_spc700_relb(byte arg, byte offset) {
word r = spc700->regs.pc + (signed char)arg;
  r += offset;
  return r;
}

void __disas_spc700_op(byte op, byte op0, byte op1) {
char *s = (char*)__disas_spc700_str;
word opw = (op1 << 8) | (op0);
  switch(op) {
  case 0x00:sprintf(s, "nop");break;
  case 0x01:sprintf(s, "tcall 0");break;
  case 0x02:sprintf(s, "set0  $%0.2x", op0);break;
  case 0x03:sprintf(s, "bbs0  $%0.2x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0x04:sprintf(s, "or    a,$%0.2x", op0);break;
  case 0x05:sprintf(s, "or    a,$%0.4x", opw);break;
  case 0x06:sprintf(s, "or    a,(x)");break;
  case 0x07:sprintf(s, "or    a,($%0.2x+x)", op0);break;
  case 0x08:sprintf(s, "or    a,#$%0.2x", op0);break;
  case 0x09:sprintf(s, "or    ($%0.2x),($%0.2x)", op1, op0);break;
  case 0x0a:sprintf(s, "or1   c,$%0.4x,%d", opw & 0x1fff, opw >> 13);break;
  case 0x0b:sprintf(s, "asl   $%0.2x", op0);break;
  case 0x0c:sprintf(s, "mov   $%0.4x,y", opw);break;
  case 0x0d:sprintf(s, "push  psw");break;
  case 0x0e:sprintf(s, "tset1 $%0.4x", opw);break;
  case 0x0f:sprintf(s, "brk");break;
  case 0x10:sprintf(s, "bpl   $%0.4x", __disas_spc700_relb(op0, 2));break;
  case 0x11:sprintf(s, "tcall 1");break;
  case 0x12:sprintf(s, "clr0  $%0.2x", op0);break;
  case 0x13:sprintf(s, "bbc0  $%0.2x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0x14:sprintf(s, "or    a,$%0.2x+x", op0);break;
  case 0x15:sprintf(s, "or    a,$%0.4x+x", opw);break;
  case 0x16:sprintf(s, "or    a,$%0.4x+y", opw);break;
  case 0x17:sprintf(s, "or    a,($%0.2x)+y", op0);break;
  case 0x18:sprintf(s, "or    $%0.2x,#$%0.2x", op1, op0);break;
  case 0x19:sprintf(s, "or    (x),(y)");break;
  case 0x1a:sprintf(s, "decw  $%0.2x", op0);break;
  case 0x1b:sprintf(s, "asl   $%0.2x+x", op0);break;
  case 0x1c:sprintf(s, "asl   a");break;
  case 0x1d:sprintf(s, "dec   x");break;
  case 0x1e:sprintf(s, "cmp   x,$%0.4x", opw);break;
  case 0x1f:sprintf(s, "jmp   ($%0.4x+x)", opw);break;
  case 0x20:sprintf(s, "clrp");break;
  case 0x21:sprintf(s, "tcall 2");break;
  case 0x22:sprintf(s, "set1  $%0.2x", op0);break;
  case 0x23:sprintf(s, "bbs1  $%0.2x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0x24:sprintf(s, "and   a,$%0.2x", op0);break;
  case 0x25:sprintf(s, "and   a,$%0.4x", opw);break;
  case 0x26:sprintf(s, "and   a,(x)", op0);break;
  case 0x27:sprintf(s, "and   a,($%0.2x+x)", op0);break;
  case 0x28:sprintf(s, "and   a,#$%0.2x", op0);break;
  case 0x29:sprintf(s, "and   ($%0.2x),($%0.2x)", op1, op0);break;
  case 0x2a:sprintf(s, "or1   c,!$%0.4x,%d", opw & 0x1fff, opw >> 13);break;
  case 0x2b:sprintf(s, "rol   $%0.2x", op0);break;
  case 0x2c:sprintf(s, "rol   $%0.4x", opw);break;
  case 0x2d:sprintf(s, "push  a");break;
  case 0x2e:sprintf(s, "cbne  $%0.2x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0x2f:sprintf(s, "bra   $%0.4x", __disas_spc700_relb(op0, 2));break;
  case 0x30:sprintf(s, "bmi   $%0.4x", __disas_spc700_relb(op0, 2));break;
  case 0x31:sprintf(s, "tcall 3");break;
  case 0x32:sprintf(s, "clr1  $%0.2x", op0);break;
  case 0x33:sprintf(s, "bbc1  $%0.2x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0x34:sprintf(s, "and   a,$%0.2x+x", op0);break;
  case 0x35:sprintf(s, "and   a,$%0.4x+x", opw);break;
  case 0x36:sprintf(s, "and   a,$%0.4x+y", opw);break;
  case 0x37:sprintf(s, "and   a,($%0.2x)+y", op0);break;
  case 0x38:sprintf(s, "and   $%0.2x,#$%0.2x", op1, op0);break;
  case 0x39:sprintf(s, "and   (x),(y)");break;
  case 0x3a:sprintf(s, "incw  $%0.2x", op0);break;
  case 0x3b:sprintf(s, "rol   $%0.2x+x", op0);break;
  case 0x3c:sprintf(s, "rol   a");break;
  case 0x3d:sprintf(s, "inc   x");break;
  case 0x3e:sprintf(s, "cmp   x,$%0.2x", op0);break;
  case 0x3f:sprintf(s, "call  $%0.4x", opw);break;
  case 0x40:sprintf(s, "setp");break;
  case 0x41:sprintf(s, "tcall 4");break;
  case 0x42:sprintf(s, "set2  $%0.2x", op0);break;
  case 0x43:sprintf(s, "bbs2  $%0.2x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0x44:sprintf(s, "eor   a,$%0.2x", op0);break;
  case 0x45:sprintf(s, "eor   a,$%0.4x", opw);break;
  case 0x46:sprintf(s, "eor   a,(x)");break;
  case 0x47:sprintf(s, "eor   a,($%0.2x+x)", op0);break;
  case 0x48:sprintf(s, "eor   a,#$%0.2x", op0);break;
  case 0x49:sprintf(s, "eor   ($%0.2x),($%0.2x)", op1, op0);break;
  case 0x4a:sprintf(s, "and1  c,$%0.4x,%d", opw & 0x1fff, opw >> 13);break;
  case 0x4b:sprintf(s, "lsr   $%0.2x", op0);break;
  case 0x4c:sprintf(s, "lsr   $%0.4x", opw);break;
  case 0x4d:sprintf(s, "push  x");break;
  case 0x4e:sprintf(s, "tclr1 $%0.4x", opw);break;
  case 0x4f:sprintf(s, "pcall $%0.2x", op0);break;
  case 0x50:sprintf(s, "bvc   $%0.4x", __disas_spc700_relb(op0, 2));break;
  case 0x51:sprintf(s, "tcall 5");break;
  case 0x52:sprintf(s, "clr2  $%0.2x", op0);break;
  case 0x53:sprintf(s, "bbc2  $%0.2x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0x54:sprintf(s, "eor   a,$%0.2x+x", op0);break;
  case 0x55:sprintf(s, "eor   a,$%0.4x+x", opw);break;
  case 0x56:sprintf(s, "eor   a,$%0.4x+y", opw);break;
  case 0x57:sprintf(s, "eor   a,($%0.2x)+y", op0);break;
  case 0x58:sprintf(s, "eor   $%0.2x,#$%0.2x", op1, op0);break;
  case 0x59:sprintf(s, "eor   (x),(y)", op0);break;
  case 0x5a:sprintf(s, "cmpw  ya,$%0.2x", op0);break;
  case 0x5b:sprintf(s, "lsr   $%0.2x+x", op0);break;
  case 0x5c:sprintf(s, "lsr   a");break;
  case 0x5d:sprintf(s, "mov   x,a");break;
  case 0x5e:sprintf(s, "cmp   y,$%0.4x", opw);break;
  case 0x5f:sprintf(s, "jmp   $%0.4x", opw);break;
  case 0x60:sprintf(s, "clrc");break;
  case 0x61:sprintf(s, "tcall 6");break;
  case 0x62:sprintf(s, "set3  $%0.2x", op0);break;
  case 0x63:sprintf(s, "bbs3  $%0.2x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0x64:sprintf(s, "cmp   a,$%0.2x", op0);break;
  case 0x65:sprintf(s, "cmp   a,$%0.4x", opw);break;
  case 0x66:sprintf(s, "cmp   a,(x)", op0);break;
  case 0x67:sprintf(s, "cmp   a,($%0.2x+x)", op0);break;
  case 0x68:sprintf(s, "cmp   a,#$%0.2x", op0);break;
  case 0x69:sprintf(s, "cmp   ($%0.2x),($%0.2x)", op1, op0);break;
  case 0x6a:sprintf(s, "and1  c,!$%0.4x,%d", opw & 0x1fff, opw >> 13);break;
  case 0x6b:sprintf(s, "ror   $%0.2x", op0);break;
  case 0x6c:sprintf(s, "ror   $%0.4x", opw);break;
  case 0x6d:sprintf(s, "push  y");break;
  case 0x6e:sprintf(s, "dbnz  $%0.2x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0x6f:sprintf(s, "ret");break;
  case 0x70:sprintf(s, "bvs   $%0.4x", __disas_spc700_relb(op0, 2));break;
  case 0x71:sprintf(s, "tcall 7");break;
  case 0x72:sprintf(s, "clr3  $%0.2x", op0);break;
  case 0x73:sprintf(s, "bbc3  $%0.2x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0x74:sprintf(s, "cmp   a,$%0.2x+x", op0);break;
  case 0x75:sprintf(s, "cmp   a,$%0.4x+x", opw);break;
  case 0x76:sprintf(s, "cmp   a,$%0.4x+y", opw);break;
  case 0x77:sprintf(s, "cmp   a,($%0.2x)+y", op0);break;
  case 0x78:sprintf(s, "cmp   $%0.2x,#$%0.2x", op1, op0);break;
  case 0x79:sprintf(s, "cmp   (x),(y)");break;
  case 0x7a:sprintf(s, "addw  ya,$%0.2x", op0);break;
  case 0x7b:sprintf(s, "ror   $%0.2x+x", op0);break;
  case 0x7c:sprintf(s, "ror   a");break;
  case 0x7d:sprintf(s, "mov   a,x");break;
  case 0x7e:sprintf(s, "cmp   y,$%0.2x", op0);break;
  case 0x7f:sprintf(s, "reti");break;
  case 0x80:sprintf(s, "setc");break;
  case 0x81:sprintf(s, "tcall 8");break;
  case 0x82:sprintf(s, "set4  $%0.2x", op0);break;
  case 0x83:sprintf(s, "bbs4  $%0.2x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0x84:sprintf(s, "adc   a,$%0.2x", op0);break;
  case 0x85:sprintf(s, "adc   a,$%0.4x", opw);break;
  case 0x86:sprintf(s, "adc   a,(x)", op0);break;
  case 0x87:sprintf(s, "adc   a,($%0.2x+x)", op0);break;
  case 0x88:sprintf(s, "adc   a,#$%0.2x", op0);break;
  case 0x89:sprintf(s, "adc   ($%0.2x),($%0.2x)", op1, op0);break;
  case 0x8a:sprintf(s, "eor1  c,$%0.4x,%d", opw & 0x1fff, opw >> 13);break;
  case 0x8b:sprintf(s, "dec   $%0.2x", op0);break;
  case 0x8c:sprintf(s, "dec   $%0.4x", opw);break;
  case 0x8d:sprintf(s, "mov   y,#$%0.2x", op0);break;
  case 0x8e:sprintf(s, "pop   psw");break;
  case 0x8f:sprintf(s, "mov   $%0.2x,#$%0.2x", op1, op0);break;
  case 0x90:sprintf(s, "bcc   $%0.4x", __disas_spc700_relb(op0, 2));break;
  case 0x91:sprintf(s, "tcall 9");break;
  case 0x92:sprintf(s, "clr4  $%0.2x", op0);break;
  case 0x93:sprintf(s, "bbc4  $%0.2x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0x94:sprintf(s, "adc   a,$%0.2x+x", op0);break;
  case 0x95:sprintf(s, "adc   a,$%0.4x+x", opw);break;
  case 0x96:sprintf(s, "adc   a,$%0.4x+y", opw);break;
  case 0x97:sprintf(s, "adc   a,($%0.2x)+y", op0);break;
  case 0x98:sprintf(s, "adc   $%0.2x,#$%0.2x", op1, op0);break;
  case 0x99:sprintf(s, "adc   (x),(y)", op0);break;
  case 0x9a:sprintf(s, "subw  ya,$%0.2x", op0);break;
  case 0x9b:sprintf(s, "dec   $%0.2x+x", op0);break;
  case 0x9c:sprintf(s, "dec   a");break;
  case 0x9d:sprintf(s, "mov   x,sp");break;
  case 0x9e:sprintf(s, "div   ya,x");break;
  case 0x9f:sprintf(s, "xcn   a");break;
  case 0xa0:sprintf(s, "ei");break;
  case 0xa1:sprintf(s, "tcall 10");break;
  case 0xa2:sprintf(s, "set5  $%0.2x", op0);break;
  case 0xa3:sprintf(s, "bbs5  $%0.2x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0xa4:sprintf(s, "sbc   a,$%0.2x", op0);break;
  case 0xa5:sprintf(s, "sbc   a,$%0.4x", opw);break;
  case 0xa6:sprintf(s, "sbc   a,(x)", op0);break;
  case 0xa7:sprintf(s, "sbc   a,($%0.2x+x)", op0);break;
  case 0xa8:sprintf(s, "sbc   a,#$%0.2x", op0);break;
  case 0xa9:sprintf(s, "sbc   ($%0.2x),($%0.2x)", op1, op0);break;
  case 0xaa:sprintf(s, "mov1  c,$%0.4x,%d", opw & 0x1fff, opw >> 13);break;
  case 0xab:sprintf(s, "inc   $%0.2x", op0);break;
  case 0xac:sprintf(s, "inc   $%0.4x", opw);break;
  case 0xad:sprintf(s, "cmp   y,#$%0.2x", op0);break;
  case 0xae:sprintf(s, "pop   a");break;
  case 0xaf:sprintf(s, "mov   (x)+,a");break;
  case 0xb0:sprintf(s, "bcs   $%0.4x", __disas_spc700_relb(op0, 2));break;
  case 0xb1:sprintf(s, "tcall 11");break;
  case 0xb2:sprintf(s, "clr5  $%0.2x", op0);break;
  case 0xb3:sprintf(s, "bbc5  $%0.2x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0xb4:sprintf(s, "sbc   a,$%0.2x+x", op0);break;
  case 0xb5:sprintf(s, "sbc   a,$%0.4x+x", opw);break;
  case 0xb6:sprintf(s, "sbc   a,$%0.4x+y", opw);break;
  case 0xb7:sprintf(s, "sbc   a,($%0.2x)+y", op0);break;
  case 0xb8:sprintf(s, "sbc   $%0.2x,#$%0.2x", op1, op0);break;
  case 0xb9:sprintf(s, "sbc   (x),(y)", op0);break;
  case 0xba:sprintf(s, "movw  ya,$%0.2x", op0);break;
  case 0xbb:sprintf(s, "inc   $%0.2x+x", op0);break;
  case 0xbc:sprintf(s, "inc   a");break;
  case 0xbd:sprintf(s, "mov   sp,x");break;
  case 0xbe:sprintf(s, "das   a");break;
  case 0xbf:sprintf(s, "mov   a,(x)+");break;
  case 0xc0:sprintf(s, "di");break;
  case 0xc1:sprintf(s, "tcall 12");break;
  case 0xc2:sprintf(s, "set6  $%0.2x", op0);break;
  case 0xc3:sprintf(s, "bbs6  $%0.2x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0xc4:sprintf(s, "mov   $%0.2x,a", op0);break;
  case 0xc5:sprintf(s, "mov   $%0.4x,a", opw);break;
  case 0xc6:sprintf(s, "mov   (x),a");break;
  case 0xc7:sprintf(s, "mov   ($%0.2x+x),a", op0);break;
  case 0xc8:sprintf(s, "cmp   x,#$%0.2x", op0);break;
  case 0xc9:sprintf(s, "mov   $%0.4x,x", opw);break;
  case 0xca:sprintf(s, "mov1  $%0.4x,%d,c", opw & 0x1fff, opw >> 13);break;
  case 0xcb:sprintf(s, "mov   $%0.2x,y", op0);break;
  case 0xcc:sprintf(s, "asl   $%0.4x", opw);break;
  case 0xcd:sprintf(s, "mov   x,#$%0.2x", op0);break;
  case 0xce:sprintf(s, "pop   x");break;
  case 0xcf:sprintf(s, "mul   ya");break;
  case 0xd0:sprintf(s, "bne   $%0.4x", __disas_spc700_relb(op0, 2));break;
  case 0xd1:sprintf(s, "tcall 13");break;
  case 0xd2:sprintf(s, "clr6  $%0.2x", op0);break;
  case 0xd3:sprintf(s, "bbc6  $%0.2x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0xd4:sprintf(s, "mov   $%0.2x+x,a", op0);break;
  case 0xd5:sprintf(s, "mov   $%0.4x+x,a", opw);break;
  case 0xd6:sprintf(s, "mov   $%0.4x+y,a", opw);break;
  case 0xd7:sprintf(s, "mov   ($%0.2x)+y,a", op0);break;
  case 0xd8:sprintf(s, "mov   $%0.2x,x", op0);break;
  case 0xd9:sprintf(s, "mov   $%0.2x+y,x", op0);break;
  case 0xda:sprintf(s, "movw  $%0.2x,ya", op0);break;
  case 0xdb:sprintf(s, "mov   $%0.2x+x,y", op0);break;
  case 0xdc:sprintf(s, "dec   y");break;
  case 0xdd:sprintf(s, "mov   a,y");break;
  case 0xde:sprintf(s, "cbne  $%0.2x+x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0xdf:sprintf(s, "daa   a");break;
  case 0xe0:sprintf(s, "clrv");break;
  case 0xe1:sprintf(s, "tcall 14");break;
  case 0xe2:sprintf(s, "set7  $%0.2x", op0);break;
  case 0xe3:sprintf(s, "bbs7  $%0.2x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0xe4:sprintf(s, "mov   a,$%0.2x", op0);break;
  case 0xe5:sprintf(s, "mov   a,$%0.4x", opw);break;
  case 0xe6:sprintf(s, "mov   a,(x)");break;
  case 0xe7:sprintf(s, "mov   a,($%0.2x+x)", op0);break;
  case 0xe8:sprintf(s, "mov   a,#$%0.2x", op0);break;
  case 0xe9:sprintf(s, "mov   x,$%0.4x", opw);break;
  case 0xea:sprintf(s, "not1  $%0.4x,%d", opw & 0x1fff, opw >> 13);break;
  case 0xeb:sprintf(s, "mov   y,$%0.2x", op0);break;
  case 0xec:sprintf(s, "mov   y,$%0.4x", opw);break;
  case 0xed:sprintf(s, "notc");break;
  case 0xee:sprintf(s, "pop   y");break;
  case 0xef:sprintf(s, "sleep");break;
  case 0xf0:sprintf(s, "beq   $%0.4x", __disas_spc700_relb(op0, 2));break;
  case 0xf1:sprintf(s, "tcall 15");break;
  case 0xf2:sprintf(s, "clr7  $%0.2x", op0);break;
  case 0xf3:sprintf(s, "bbc7  $%0.2x,$%0.4x", op0, __disas_spc700_relb(op1, 3));break;
  case 0xf4:sprintf(s, "mov   a,$%0.2x+x", op0);break;
  case 0xf5:sprintf(s, "mov   a,$%0.4x+x", opw);break;
  case 0xf6:sprintf(s, "mov   a,$%0.4x+y", opw);break;
  case 0xf7:sprintf(s, "mov   a,($%0.2x)+y", op0);break;
  case 0xf8:sprintf(s, "mov   x,$%0.2x", op0);break;
  case 0xf9:sprintf(s, "mov   x,$%0.2x+y", op0);break;
  case 0xfa:sprintf(s, "mov   ($%0.2x),($%0.2x)", op1, op0);break;
  case 0xfb:sprintf(s, "mov   y,$%0.2x+x", op0);break;
  case 0xfc:sprintf(s, "inc   y");break;
  case 0xfd:sprintf(s, "mov   y,a");break;
  case 0xfe:sprintf(s, "dbnz  y,$%0.4x", __disas_spc700_relb(op0, 2));break;
  case 0xff:sprintf(s, "stop");break;
  }
}

void disas_spc700_op(void) {
byte op;
byte op0, op1;
word offset;
char flags[256];
char *s = (char*)__disas_spc700_str;
int  i;
  if(debug_write_status() == DEBUGWRITE_NONE)return;

//use offset instead of spc700->regs.pc + n to avoid going above 65535
  offset = spc700->regs.pc;
  op  = spc700->ram[offset++];
  op0 = spc700->ram[offset++];
  op1 = spc700->ram[offset  ];

  strcpy(__disas_spc700_str, "???");
  __disas_spc700_op(op, op0, op1);
  i = strlen(s);
  while(i < 23) {
    s[i++] = ' ';
    s[i]   = 0;
  }

  sprintf(flags, "%c%c%c%c%c%c%c%c",
    (spc700->regs.p & 0x80)?'N':'n',
    (spc700->regs.p & 0x40)?'V':'v',
    (spc700->regs.p & 0x20)?'P':'p',
    (spc700->regs.p & 0x10)?'B':'b',
    (spc700->regs.p & 0x08)?'H':'h',
    (spc700->regs.p & 0x04)?'I':'i',
    (spc700->regs.p & 0x02)?'Z':'z',
    (spc700->regs.p & 0x01)?'C':'c');

  dprintf(DEBUGMSG_APU, "--%0.4x %s A:%0.2x X:%0.2x Y:%0.2x SP:%0.2x YA:%0.4x %s",
    spc700->regs.pc, __disas_spc700_str,
    spc700->regs.a, spc700->regs.x, spc700->regs.y, spc700->regs.sp,
    (spc700->regs.y << 8) | (spc700->regs.a), flags);
}
