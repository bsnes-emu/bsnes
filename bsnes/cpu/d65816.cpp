#include "../base.h"
#include "g65816.h"
extern g65816     *gx816;
extern debugstate debugger;

ulong _disas_relb(byte addr) {
  return gx816->regs.pc + (signed char)(addr + 2);
}

ulong _disas_relw(word addr) {
  return gx816->regs.pc + (signed short)(addr + 3);
}

char __disas_op_str[256];
void __disas_op(byte op, byte op0, byte op1, byte op2) {
char *s = (char*)__disas_op_str;
  switch(op) {
  case 0x00:sprintf(s, "brk #$%0.2x               ", op0);break;
  case 0x01:sprintf(s, "ora ($%0.2x,x)   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDPX, op0));break;
  case 0x02:sprintf(s, "cop #$%0.2x               ", op0);break;
  case 0x03:sprintf(s, "ora $%0.2x,s     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_SR, op0));break;
  case 0x04:sprintf(s, "tsb $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0x05:sprintf(s, "ora $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0x06:sprintf(s, "asl $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0x07:sprintf(s, "ora [$%0.2x]     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ILDP, op0));break;
  case 0x08:sprintf(s, "php                    ");break;
  case 0x09:
    if(gx816->regs.e == true || (gx816->regs.e == false && (gx816->regs.p & 0x20)))sprintf(s, "ora #$%0.2x               ", op0);
    else sprintf(s, "ora #$%0.4x             ", op0|op1<<8);break;
  case 0x0a:sprintf(s, "asl a                  ");break;
  case 0x0b:sprintf(s, "phd                    ");break;
  case 0x0c:sprintf(s, "tsb $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0x0d:sprintf(s, "ora $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0x0e:sprintf(s, "asl $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0x0f:sprintf(s, "ora $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, gx816->convert_offset(MEMMODE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0x10:sprintf(s, "bpl $%0.4x     [$%0.6x]", _disas_relb(op0)&0xffff, _disas_relb(op0)&0xffffff);break;
  case 0x11:sprintf(s, "ora ($%0.2x),y   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDPY, op0));break;
  case 0x12:sprintf(s, "ora ($%0.2x)     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDP, op0));break;
  case 0x13:sprintf(s, "ora ($%0.2x,s),y [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ISRY, op0));break;
  case 0x14:sprintf(s, "trb $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0x15:sprintf(s, "ora $%0.2x,x     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPX, op0));break;
  case 0x16:sprintf(s, "asl $%0.2x,x     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPX, op0));break;
  case 0x17:sprintf(s, "ora [$%0.2x],y   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ILDPY, op0));break;
  case 0x18:sprintf(s, "clc                    ");break;
  case 0x19:sprintf(s, "ora $%0.4x,y   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRY, (op0|op1<<8)));break;
  case 0x1a:sprintf(s, "inc                    ");break;
  case 0x1b:sprintf(s, "tcs                    ");break;
  case 0x1c:sprintf(s, "trb $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0x1d:sprintf(s, "ora $%0.4x,x   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRX, (op0|op1<<8)));break;
  case 0x1e:sprintf(s, "asl $%0.4x,x   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRX, (op0|op1<<8)));break;
  case 0x1f:sprintf(s, "ora $%0.6x,x [$%0.6x]", op0|op1<<8|op2<<16, gx816->convert_offset(MEMMODE_LONGX, (op0|op1<<8|op2<<16)));break;
  case 0x20:sprintf(s, "jsr $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR_PC, (op0|op1<<8)));break;
  case 0x21:sprintf(s, "and ($%0.2x,x)   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDPX, op0));break;
  case 0x22:sprintf(s, "jsl $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, gx816->convert_offset(MEMMODE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0x23:sprintf(s, "and $%0.2x,s     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_SR, op0));break;
  case 0x24:sprintf(s, "bit $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0x25:sprintf(s, "and $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0x26:sprintf(s, "rol $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0x27:sprintf(s, "and [$%0.2x]     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ILDP, op0));break;
  case 0x28:sprintf(s, "plp                    ");break;
  case 0x29:
    if(gx816->regs.e == true || (gx816->regs.e == false && (gx816->regs.p & 0x20)))sprintf(s, "and #$%0.2x               ", op0);
    else sprintf(s, "and #$%0.4x             ", op0|op1<<8);break;
  case 0x2a:sprintf(s, "rol a                  ");break;
  case 0x2b:sprintf(s, "pld                    ");break;
  case 0x2c:sprintf(s, "bit $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0x2d:sprintf(s, "and $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0x2e:sprintf(s, "rol $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0x2f:sprintf(s, "and $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, gx816->convert_offset(MEMMODE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0x30:sprintf(s, "bmi $%0.4x     [$%0.6x]", _disas_relb(op0)&0xffff, _disas_relb(op0)&0xffffff);break;
  case 0x31:sprintf(s, "and ($%0.2x),y   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDPY, op0));break;
  case 0x32:sprintf(s, "and ($%0.2x)     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDP, op0));break;
  case 0x33:sprintf(s, "and ($%0.2x,s),y [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ISRY, op0));break;
  case 0x34:sprintf(s, "bit $%0.2x,x     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPX, op0));break;
  case 0x35:sprintf(s, "and $%0.2x,x     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPX, op0));break;
  case 0x36:sprintf(s, "rol $%0.2x,x     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPX, op0));break;
  case 0x37:sprintf(s, "and [$%0.2x],y   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ILDPY, op0));break;
  case 0x38:sprintf(s, "sec                    ");break;
  case 0x39:sprintf(s, "and $%0.4x,y   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRY, (op0|op1<<8)));break;
  case 0x3a:sprintf(s, "dec                    ");break;
  case 0x3b:sprintf(s, "tsc                    ");break;
  case 0x3c:sprintf(s, "bit $%0.4x,x   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRX, (op0|op1<<8)));break;
  case 0x3d:sprintf(s, "and $%0.4x,x   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRX, (op0|op1<<8)));break;
  case 0x3e:sprintf(s, "rol $%0.4x,x   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRX, (op0|op1<<8)));break;
  case 0x3f:sprintf(s, "and $%0.6x,x [$%0.6x]", op0|op1<<8|op2<<16, gx816->convert_offset(MEMMODE_LONGX, (op0|op1<<8|op2<<16)));break;
  case 0x40:sprintf(s, "rti                    ");break;
  case 0x41:sprintf(s, "eor ($%0.2x,x)   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDPX, op0));break;
  case 0x42:sprintf(s, "wdm                    ");break;
  case 0x43:sprintf(s, "eor $%0.2x,s     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_SR, op0));break;
  case 0x44:sprintf(s, "mvp $%0.2x,$%0.2x            ", op1, op0);break;
  case 0x45:sprintf(s, "eor $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0x46:sprintf(s, "lsr $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0x47:sprintf(s, "eor [$%0.2x]     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ILDP, op0));break;
  case 0x48:sprintf(s, "pha                    ");break;
  case 0x49:
    if(gx816->regs.e == true || (gx816->regs.e == false && (gx816->regs.p & 0x20)))sprintf(s, "eor #$%0.2x               ", op0);
    else sprintf(s, "eor #$%0.4x             ", op0|op1<<8);break;
  case 0x4a:sprintf(s, "lsr a                  ");break;
  case 0x4b:sprintf(s, "phk                    ");break;
  case 0x4c:sprintf(s, "jmp $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR_PC, (op0|op1<<8)));break;
  case 0x4d:sprintf(s, "eor $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0x4e:sprintf(s, "lsr $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0x4f:sprintf(s, "eor $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, gx816->convert_offset(MEMMODE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0x50:sprintf(s, "bvc $%0.4x     [$%0.6x]", _disas_relb(op0)&0xffff, _disas_relb(op0)&0xffffff);break;
  case 0x51:sprintf(s, "eor ($%0.2x),y   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDPY, op0));break;
  case 0x52:sprintf(s, "eor ($%0.2x)     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDP, op0));break;
  case 0x53:sprintf(s, "eor ($%0.2x,s),y [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ISRY, op0));break;
  case 0x54:sprintf(s, "mvn $%0.2x,$%0.2x            ", op1, op0);break;
  case 0x55:sprintf(s, "eor $%0.2x,x     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPX, op0));break;
  case 0x56:sprintf(s, "lsr $%0.2x,x     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPX, op0));break;
  case 0x57:sprintf(s, "eor [$%0.2x],y   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ILDPY, op0));break;
  case 0x58:sprintf(s, "cli                    ");break;
  case 0x59:sprintf(s, "eor $%0.4x,y   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRY, (op0|op1<<8)));break;
  case 0x5a:sprintf(s, "phy                    ");break;
  case 0x5b:sprintf(s, "tcd                    ");break;
  case 0x5c:sprintf(s, "jml $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, gx816->convert_offset(MEMMODE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0x5d:sprintf(s, "eor $%0.4x,x   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRX, (op0|op1<<8)));break;
  case 0x5e:sprintf(s, "lsr $%0.4x,x   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRX, (op0|op1<<8)));break;
  case 0x5f:sprintf(s, "eor $%0.6x,x [$%0.6x]", op0|op1<<8|op2<<16, gx816->convert_offset(MEMMODE_LONGX, (op0|op1<<8|op2<<16)));break;
  case 0x60:sprintf(s, "rts                    ");break;
  case 0x61:sprintf(s, "adc ($%0.2x,x)   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDPX, op0));break;
  case 0x62:sprintf(s, "per $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0x63:sprintf(s, "adc $%0.2x,s     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_SR, op0));break;
  case 0x64:sprintf(s, "stz $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0x65:sprintf(s, "adc $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0x66:sprintf(s, "ror $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0x67:sprintf(s, "adc [$%0.2x]     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ILDP, op0));break;
  case 0x68:sprintf(s, "pla                    ");break;
  case 0x69:
    if(gx816->regs.e == true || (gx816->regs.e == false && (gx816->regs.p & 0x20)))sprintf(s, "adc #$%0.2x               ", op0);
    else sprintf(s, "adc #$%0.4x             ", op0|op1<<8);break;
  case 0x6a:sprintf(s, "ror a                  ");break;
  case 0x6b:sprintf(s, "rtl                    ");break;
  case 0x6c:sprintf(s, "jmp ($%0.4x)   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_IADDR_PC, (op0|op1<<8)));break;
  case 0x6d:sprintf(s, "adc $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0x6e:sprintf(s, "ror $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0x6f:sprintf(s, "adc $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, gx816->convert_offset(MEMMODE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0x70:sprintf(s, "bvs $%0.4x     [$%0.6x]", _disas_relb(op0)&0xffff, _disas_relb(op0)&0xffffff);break;
  case 0x71:sprintf(s, "adc ($%0.2x),y   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDPY, op0));break;
  case 0x72:sprintf(s, "adc ($%0.2x)     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDP, op0));break;
  case 0x73:sprintf(s, "adc ($%0.2x,s),y [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ISRY, op0));break;
  case 0x74:sprintf(s, "stz $%0.2x,x     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPX, op0));break;
  case 0x75:sprintf(s, "adc $%0.2x,x     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPX, op0));break;
  case 0x76:sprintf(s, "ror $%0.2x,x     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPX, op0));break;
  case 0x77:sprintf(s, "adc [$%0.2x],y   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ILDPY, op0));break;
  case 0x78:sprintf(s, "sei                    ");break;
  case 0x79:sprintf(s, "adc $%0.4x,y   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRY, (op0|op1<<8)));break;
  case 0x7a:sprintf(s, "ply                    ");break;
  case 0x7b:sprintf(s, "tdc                    ");break;
  case 0x7c:sprintf(s, "jmp ($%0.4x,x) [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_IADDRX_PC, (op0|op1<<8)));break;
  case 0x7d:sprintf(s, "adc $%0.4x,x   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRX, (op0|op1<<8)));break;
  case 0x7e:sprintf(s, "ror $%0.4x,x   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRX, (op0|op1<<8)));break;
  case 0x7f:sprintf(s, "adc $%0.6x,x [$%0.6x]", op0|op1<<8|op2<<16, gx816->convert_offset(MEMMODE_LONGX, (op0|op1<<8|op2<<16)));break;
  case 0x80:sprintf(s, "bra $%0.4x     [$%0.6x]", _disas_relb(op0)&0xffff, _disas_relb(op0)&0xffffff);break;
  case 0x81:sprintf(s, "sta ($%0.2x,x)   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDPX, op0));break;
  case 0x82:sprintf(s, "brl $%0.4x     [$%0.6x]", _disas_relw(op0|op1<<8)&0xffff, _disas_relw(op0|op1<<8)&0xffffff);break;
  case 0x83:sprintf(s, "sta $%0.2x,s     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_SR, op0));break;
  case 0x84:sprintf(s, "sty $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0x85:sprintf(s, "sta $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0x86:sprintf(s, "stx $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0x87:sprintf(s, "sta [$%0.2x]     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ILDP, op0));break;
  case 0x88:sprintf(s, "dey                    ");break;
  case 0x89:
    if(gx816->regs.e == true || (gx816->regs.e == false && (gx816->regs.p & 0x20)))sprintf(s, "bit #$%0.2x               ", op0);
    else sprintf(s, "bit #$%0.4x             ", op0|op1<<8);break;
  case 0x8a:sprintf(s, "txa                    ");break;
  case 0x8b:sprintf(s, "phb                    ");break;
  case 0x8c:sprintf(s, "sty $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0x8d:sprintf(s, "sta $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0x8e:sprintf(s, "stx $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0x8f:sprintf(s, "sta $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, gx816->convert_offset(MEMMODE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0x90:sprintf(s, "bcc $%0.4x     [$%0.6x]", _disas_relb(op0)&0xffff, _disas_relb(op0)&0xffffff);break;
  case 0x91:sprintf(s, "sta ($%0.2x),y   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDPY, op0));break;
  case 0x92:sprintf(s, "sta ($%0.2x)     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDP, op0));break;
  case 0x93:sprintf(s, "sta ($%0.2x,s),y [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ISRY, op0));break;
  case 0x94:sprintf(s, "sty $%0.2x,x     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPX, op0));break;
  case 0x95:sprintf(s, "sta $%0.2x,x     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPX, op0));break;
  case 0x96:sprintf(s, "stx $%0.2x,y     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPY, op0));break;
  case 0x97:sprintf(s, "sta [$%0.2x],y   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ILDPY, op0));break;
  case 0x98:sprintf(s, "tya                    ");break;
  case 0x99:sprintf(s, "sta $%0.4x,y   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRY, (op0|op1<<8)));break;
  case 0x9a:sprintf(s, "txs                    ");break;
  case 0x9b:sprintf(s, "txy                    ");break;
  case 0x9c:sprintf(s, "stz $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0x9d:sprintf(s, "sta $%0.4x,x   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRX, (op0|op1<<8)));break;
  case 0x9e:sprintf(s, "stz $%0.4x,x   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRX, (op0|op1<<8)));break;
  case 0x9f:sprintf(s, "sta $%0.6x,x [$%0.6x]", op0|op1<<8|op2<<16, gx816->convert_offset(MEMMODE_LONGX, (op0|op1<<8|op2<<16)));break;
  case 0xa0:
    if(gx816->regs.e == true || (gx816->regs.e == false && (gx816->regs.p & PF_X)))sprintf(s, "ldy #$%0.2x               ", op0);
    else sprintf(s, "ldy #$%0.4x             ", op0|op1<<8);break;
  case 0xa1:sprintf(s, "lda ($%0.2x,x)   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDPX, op0));break;
  case 0xa2:
    if(gx816->regs.e == true || (gx816->regs.e == false && (gx816->regs.p & PF_X)))sprintf(s, "ldx #$%0.2x               ", op0);
    else sprintf(s, "ldx #$%0.4x             ", op0|op1<<8);break;
  case 0xa3:sprintf(s, "lda $%0.2x,s     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_SR, op0));break;
  case 0xa4:sprintf(s, "ldy $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0xa5:sprintf(s, "lda $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0xa6:sprintf(s, "ldx $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0xa7:sprintf(s, "lda [$%0.2x]     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ILDP, op0));break;
  case 0xa8:sprintf(s, "tay                    ");break;
  case 0xa9:
    if(gx816->regs.e == true || (gx816->regs.e == false && (gx816->regs.p & 0x20)))sprintf(s, "lda #$%0.2x               ", op0);
    else sprintf(s, "lda #$%0.4x             ", op0|op1<<8);break;
  case 0xaa:sprintf(s, "tax                    ");break;
  case 0xab:sprintf(s, "plb                    ");break;
  case 0xac:sprintf(s, "ldy $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0xad:sprintf(s, "lda $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0xae:sprintf(s, "ldx $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0xaf:sprintf(s, "lda $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, gx816->convert_offset(MEMMODE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0xb0:sprintf(s, "bcs $%0.4x     [$%0.6x]", _disas_relb(op0)&0xffff, _disas_relb(op0)&0xffffff);break;
  case 0xb1:sprintf(s, "lda ($%0.2x),y   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDPY, op0));break;
  case 0xb2:sprintf(s, "lda ($%0.2x)     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDP, op0));break;
  case 0xb3:sprintf(s, "lda ($%0.2x,s),y [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ISRY, op0));break;
  case 0xb4:sprintf(s, "ldy $%0.2x,x     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPX, op0));break;
  case 0xb5:sprintf(s, "lda $%0.2x,x     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPX, op0));break;
  case 0xb6:sprintf(s, "ldx $%0.2x,y     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPY, op0));break;
  case 0xb7:sprintf(s, "lda [$%0.2x],y   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ILDPY, op0));break;
  case 0xb8:sprintf(s, "clv                    ");break;
  case 0xb9:sprintf(s, "lda $%0.4x,y   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRY, (op0|op1<<8)));break;
  case 0xba:sprintf(s, "tsx                    ");break;
  case 0xbb:sprintf(s, "tyx                    ");break;
  case 0xbc:sprintf(s, "ldy $%0.4x,x   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRX, (op0|op1<<8)));break;
  case 0xbd:sprintf(s, "lda $%0.4x,x   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRX, (op0|op1<<8)));break;
  case 0xbe:sprintf(s, "ldx $%0.4x,y   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRY, (op0|op1<<8)));break;
  case 0xbf:sprintf(s, "lda $%0.6x,x [$%0.6x]", op0|op1<<8|op2<<16, gx816->convert_offset(MEMMODE_LONGX, (op0|op1<<8|op2<<16)));break;
  case 0xc0:
    if(gx816->regs.e == true || (gx816->regs.e == false && (gx816->regs.p & PF_X)))sprintf(s, "cpy #$%0.2x               ", op0);
    else sprintf(s, "cpy #$%0.4x             ", op0|op1<<8);break;
  case 0xc1:sprintf(s, "cmp ($%0.2x,x)   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDPX, op0));break;
  case 0xc2:sprintf(s, "rep #$%0.2x               ", op0);break;
  case 0xc3:sprintf(s, "cmp $%0.2x,s     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_SR, op0));break;
  case 0xc4:sprintf(s, "cpy $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0xc5:sprintf(s, "cmp $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0xc6:sprintf(s, "dec $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0xc7:sprintf(s, "cmp [$%0.2x]     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ILDP, op0));break;
  case 0xc8:sprintf(s, "iny                    ");break;
  case 0xc9:
    if(gx816->regs.e == true || (gx816->regs.e == false && (gx816->regs.p & 0x20)))sprintf(s, "cmp #$%0.2x               ", op0);
    else sprintf(s, "cmp #$%0.4x             ", op0|op1<<8);break;
  case 0xca:sprintf(s, "dex                    ");break;
  case 0xcb:sprintf(s, "wai                    ");break;
  case 0xcc:sprintf(s, "cpy $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0xcd:sprintf(s, "cmp $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0xce:sprintf(s, "dec $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0xcf:sprintf(s, "cmp $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, gx816->convert_offset(MEMMODE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0xd0:sprintf(s, "bne $%0.4x     [$%0.6x]", _disas_relb(op0)&0xffff, _disas_relb(op0)&0xffffff);break;
  case 0xd1:sprintf(s, "cmp ($%0.2x),y   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDPY, op0));break;
  case 0xd2:sprintf(s, "cmp ($%0.2x)     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDP, op0));break;
  case 0xd3:sprintf(s, "cmp ($%0.2x,s),y [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ISRY, op0));break;
  case 0xd4:sprintf(s, "pei ($%0.2x)     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDP, op0));break;
  case 0xd5:sprintf(s, "cmp $%0.2x,x     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPX, op0));break;
  case 0xd6:sprintf(s, "dec $%0.2x,x     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPX, op0));break;
  case 0xd7:sprintf(s, "cmp [$%0.2x],y   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ILDPY, op0));break;
  case 0xd8:sprintf(s, "cld                    ");break;
  case 0xd9:sprintf(s, "cmp $%0.4x,y   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRY, (op0|op1<<8)));break;
  case 0xda:sprintf(s, "phx                    ");break;
  case 0xdb:sprintf(s, "stp                    ");break;
  case 0xdc:sprintf(s, "jmp [$%0.4x]   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ILADDR_PC, (op0|op1<<8)));break;
  case 0xdd:sprintf(s, "cmp $%0.4x,x   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRX, (op0|op1<<8)));break;
  case 0xde:sprintf(s, "dec $%0.4x,x   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRX, (op0|op1<<8)));break;
  case 0xdf:sprintf(s, "cmp $%0.6x,x [$%0.6x]", op0|op1<<8|op2<<16, gx816->convert_offset(MEMMODE_LONGX, (op0|op1<<8|op2<<16)));break;
  case 0xe0:
    if(gx816->regs.e == true || (gx816->regs.e == false && (gx816->regs.p & PF_X)))sprintf(s, "cpx #$%0.2x               ", op0);
    else sprintf(s, "cpx #$%0.4x             ", op0|op1<<8);break;
  case 0xe1:sprintf(s, "sbc ($%0.2x,x)   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDPX, op0));break;
  case 0xe2:sprintf(s, "sep #$%0.2x               ", op0);break;
  case 0xe3:sprintf(s, "sbc $%0.2x,s     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_SR, op0));break;
  case 0xe4:sprintf(s, "cpx $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0xe5:sprintf(s, "sbc $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0xe6:sprintf(s, "inc $%0.2x       [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DP, op0));break;
  case 0xe7:sprintf(s, "sbc [$%0.2x]     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ILDP, op0));break;
  case 0xe8:sprintf(s, "inx                    ");break;
  case 0xe9:
    if(gx816->regs.e == true || (gx816->regs.e == false && (gx816->regs.p & 0x20)))sprintf(s, "sbc #$%0.2x               ", op0);
    else sprintf(s, "sbc #$%0.4x             ", op0|op1<<8);break;
  case 0xea:sprintf(s, "nop                    ");break;
  case 0xeb:sprintf(s, "xba                    ");break;
  case 0xec:sprintf(s, "cpx $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0xed:sprintf(s, "sbc $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0xee:sprintf(s, "inc $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0xef:sprintf(s, "sbc $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, gx816->convert_offset(MEMMODE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0xf0:sprintf(s, "beq $%0.4x     [$%0.6x]", _disas_relb(op0)&0xffff, _disas_relb(op0)&0xffffff);break;
  case 0xf1:sprintf(s, "sbc ($%0.2x),y   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDPY, op0));break;
  case 0xf2:sprintf(s, "sbc ($%0.2x)     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_IDP, op0));break;
  case 0xf3:sprintf(s, "sbc ($%0.2x,s),y [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ISRY, op0));break;
  case 0xf4:sprintf(s, "pea $%0.4x     [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDR, (op0|op1<<8)));break;
  case 0xf5:sprintf(s, "sbc $%0.2x,x     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPX, op0));break;
  case 0xf6:sprintf(s, "inc $%0.2x,x     [$%0.6x]", op0, gx816->convert_offset(MEMMODE_DPX, op0));break;
  case 0xf7:sprintf(s, "sbc [$%0.2x],y   [$%0.6x]", op0, gx816->convert_offset(MEMMODE_ILDPY, op0));break;
  case 0xf8:sprintf(s, "sed                    ");break;
  case 0xf9:sprintf(s, "sbc $%0.4x,y   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRY, (op0|op1<<8)));break;
  case 0xfa:sprintf(s, "plx                    ");break;
  case 0xfb:sprintf(s, "xce                    ");break;
  case 0xfc:sprintf(s, "jsr ($%0.4x,x) [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_IADDRX_PC, (op0|op1<<8)));break;
  case 0xfd:sprintf(s, "sbc $%0.4x,x   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRX, (op0|op1<<8)));break;
  case 0xfe:sprintf(s, "inc $%0.4x,x   [$%0.6x]", op0|op1<<8, gx816->convert_offset(MEMMODE_ADDRX, (op0|op1<<8)));break;
  case 0xff:sprintf(s, "sbc $%0.6x,x [$%0.6x]", op0|op1<<8|op2<<16, gx816->convert_offset(MEMMODE_LONGX, (op0|op1<<8|op2<<16)));break;
  }
}

void disas_g65816_op(void) {
byte op, op0, op1, op2;
char str0[256], str1[256], str2[256];
  if(debug_write_status() == DEBUGWRITE_NONE)return;

  op  = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, gx816->regs.pc,     MEMACCESS_DEBUGGER);
  op0 = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, gx816->regs.pc + 1, MEMACCESS_DEBUGGER);
  op1 = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, gx816->regs.pc + 2, MEMACCESS_DEBUGGER);
  op2 = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, gx816->regs.pc + 3, MEMACCESS_DEBUGGER);

  strcpy(__disas_op_str, "???                    ");
  __disas_op(op, op0, op1, op2);

  if(gx816->regs.e == true) {
    sprintf(str1, "%c%c%c%c%c%c%c%c E",
      (gx816->regs.p & 0x80)?'N':'n',
      (gx816->regs.p & 0x40)?'V':'v',
      (gx816->regs.p & 0x20)?'1':'0',
      (gx816->regs.p & 0x10)?'B':'b',
      (gx816->regs.p & 0x08)?'D':'d',
      (gx816->regs.p & 0x04)?'I':'i',
      (gx816->regs.p & 0x02)?'Z':'z',
      (gx816->regs.p & 0x01)?'C':'c');
  } else {
    sprintf(str1, "%c%c%c%c%c%c%c%c N",
      (gx816->regs.p & 0x80)?'N':'n',
      (gx816->regs.p & 0x40)?'V':'v',
      (gx816->regs.p & 0x20)?'M':'m',
      (gx816->regs.p & 0x10)?'X':'x',
      (gx816->regs.p & 0x08)?'D':'d',
      (gx816->regs.p & 0x04)?'I':'i',
      (gx816->regs.p & 0x02)?'Z':'z',
      (gx816->regs.p & 0x01)?'C':'c');
  }

  dprintf(DEBUGMSG_CPU, "%0.6x %s A:%0.4x X:%0.4x Y:%0.4x S:%0.4x D:%0.4x DB:%0.2x %s", gx816->regs.pc, __disas_op_str,
    gx816->regs.a.w, gx816->regs.x, gx816->regs.y, gx816->regs.s,
    gx816->regs.d, gx816->regs.db, str1);
}
