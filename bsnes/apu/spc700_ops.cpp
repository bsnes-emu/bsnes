#define spc700_setn() spc700->regs.p |=  SPF_N
#define spc700_clrn() spc700->regs.p &= ~SPF_N
#define spc700_setv() spc700->regs.p |=  SPF_V
#define spc700_clrv() spc700->regs.p &= ~SPF_V
#define spc700_setp() spc700->regs.p |=  SPF_P;spc700->regs.dp = 0x0100
#define spc700_clrp() spc700->regs.p &= ~SPF_P;spc700->regs.dp = 0x0000
#define spc700_setb() spc700->regs.p |=  SPF_B
#define spc700_clrb() spc700->regs.p &= ~SPF_B
#define spc700_seth() spc700->regs.p |=  SPF_H
#define spc700_clrh() spc700->regs.p &= ~SPF_H
#define spc700_seti() spc700->regs.p |=  SPF_I
#define spc700_clri() spc700->regs.p &= ~SPF_I
#define spc700_setz() spc700->regs.p |=  SPF_Z
#define spc700_clrz() spc700->regs.p &= ~SPF_Z
#define spc700_setc() spc700->regs.p |=  SPF_C
#define spc700_clrc() spc700->regs.p &= ~SPF_C
#define spc700_testn(x) if(x)spc700_setn(); else spc700_clrn()
#define spc700_testv(x) if(x)spc700_setv(); else spc700_clrv()
#define spc700_testp(x) if(x)spc700_setp(); else spc700_clrp()
#define spc700_testb(x) if(x)spc700_setb(); else spc700_clrb()
#define spc700_testh(x) if(x)spc700_seth(); else spc700_clrh()
#define spc700_testi(x) if(x)spc700_seti(); else spc700_clri()
#define spc700_testz(x) if(x)spc700_setz(); else spc700_clrz()
#define spc700_testc(x) if(x)spc700_setc(); else spc700_clrc()

#define spc700_incpc(__n) spc700->regs.pc += __n

#define spc700_prefetchb() \
  byte arg = spc700->ram[(spc700->regs.pc + 1) & 0xffff]

#define spc700_prefetch2b() \
  byte arg0 = spc700->ram[(spc700->regs.pc + 1) & 0xffff]; \
  byte arg1 = spc700->ram[(spc700->regs.pc + 2) & 0xffff]

#define spc700_prefetchw() \
  word arg = spc700->ram[(spc700->regs.pc + 1) & 0xffff] | (spc700->ram[(spc700->regs.pc + 2) & 0xffff] << 8)

#define APUMODE_NONE    0
#define APUMODE_DP      1
#define APUMODE_DPX     2
#define APUMODE_DPY     3
#define APUMODE_IDP     4
#define APUMODE_ADDR    5
#define APUMODE_IX      6
#define APUMODE_ADDRX   7
#define APUMODE_ADDRY   8
#define APUMODE_IDPX    9
#define APUMODE_IDPY   10
#define APUMODE_IADDRX 11
#define APUMODE_IY     12

word sony_spc700::convert_address(byte mode, word addr) {
word r;
  switch(mode) {
  case APUMODE_NONE:
    r = addr;
  case APUMODE_DP:
    r = spc700->regs.dp | (byte)addr;
    break;
  case APUMODE_DPX:
    r = spc700->regs.dp | (byte)(addr + spc700->regs.x);
    break;
  case APUMODE_DPY:
    r = spc700->regs.dp | (byte)(addr + spc700->regs.y);
    break;
  case APUMODE_IDP:
    r = spc700->regs.dp | (byte)addr;
    r = spc700->ram[r] | (spc700->ram[r + 1] << 8);
    break;
  case APUMODE_ADDR:
    r = addr;
    break;
  case APUMODE_IX:
    r = spc700->regs.dp | spc700->regs.x;
    break;
  case APUMODE_ADDRX:
    r = addr + spc700->regs.x;
    break;
  case APUMODE_ADDRY:
    r = addr + spc700->regs.y;
    break;
  case APUMODE_IDPX:
    r = spc700->regs.dp | (byte)(addr + spc700->regs.x);
    r = spc700->ram[r] | (spc700->ram[r + 1] << 8);
    break;
  case APUMODE_IDPY:
    r = spc700->regs.dp | (byte)addr;
    r = spc700->ram[r] | (spc700->ram[r + 1] << 8);
    r += spc700->regs.y;
    break;
  case APUMODE_IADDRX:
    r = (spc700->ram[(addr + spc700->regs.x    )]) |
        (spc700->ram[(addr + spc700->regs.x + 1)] << 8);
    break;
  case APUMODE_IY:
    r = spc700->regs.dp | spc700->regs.y;
    break;
  }
  return r;
}

byte sony_spc700::mem_getbyte(word addr) {
  if(addr >= 0x00f4 && addr <= 0x00f7) {
    return cpu_apu_bridge->apu_read(addr - 0x00f4);
  } else {
    return spc700->ram[addr];
  }
}

void sony_spc700::mem_putbyte(word addr, byte value) {
  if(addr >= 0x00f4 && addr <= 0x00f7) {
    cpu_apu_bridge->apu_write(addr - 0x00f4, value);
  } else if(addr < 0xffc0) {
    spc700->ram[addr] = value;
  }
}

word sony_spc700::mem_read(byte mode, byte size, word addr) {
word r;
  addr = convert_address(mode, addr);
  switch(size) {
  case MEMSIZE_BYTE:
    r = mem_getbyte(addr);
    break;
  case MEMSIZE_WORD:
    r = mem_getbyte(addr) | (mem_getbyte(addr + 1) << 8);
    break;
  }
  return r;
}

void sony_spc700::mem_write(byte mode, byte size, word addr, word value) {
  addr = convert_address(mode, addr);
  switch(size) {
  case MEMSIZE_BYTE:
    mem_putbyte(addr, value);
    break;
  case MEMSIZE_WORD:
    mem_putbyte(addr    , value);
    mem_putbyte(addr + 1, value >> 8);
    break;
  }
}

byte sony_spc700::stack_read(void) {
word addr;
  spc700->regs.sp++;
  addr = 0x0100 | spc700->regs.sp;
  return mem_getbyte(addr);
}

void sony_spc700::stack_write(byte value) {
word addr;
  addr = 0x0100 | spc700->regs.sp;
  spc700->regs.sp--;
  mem_putbyte(addr, value);
}

#include "spc700_ops_adc.cpp"
#include "spc700_ops_and.cpp"
#include "spc700_ops_cmp.cpp"
#include "spc700_ops_eor.cpp"
#include "spc700_ops_flags.cpp"
#include "spc700_ops_incdec.cpp"
#include "spc700_ops_misc.cpp"
#include "spc700_ops_mov.cpp"
#include "spc700_ops_or.cpp"
#include "spc700_ops_pc.cpp"
#include "spc700_ops_sbc.cpp"
#include "spc700_ops_shift.cpp"
#include "spc700_ops_stack.cpp"

vfunc spc700_optbl[256] = {
      // -----------------------------x0, -----------------------------x1, -----------------------------x2, -----------------------------x3, -------- -----------------------------x4, -----------------------------x5, -----------------------------x6, -----------------------------x7, -------- -----------------------------x8, -----------------------------x9, -----------------------------xa, -----------------------------xb, -------- -----------------------------xc, -----------------------------xd, -----------------------------xe, -----------------------------xf,
/* 0x */                   spc700_op_nop,               spc700_op_tcall_0,               spc700_op_set0_dp,                  spc700_op_bbc0, /* 0x */               spc700_op_or_a_dp,             spc700_op_or_a_addr,               spc700_op_or_a_ix,             spc700_op_or_a_idpx, /* 0x */            spc700_op_or_a_const,              spc700_op_or_dp_dp,               spc700_op_or1_bit,                spc700_op_asl_dp, /* 0x */            spc700_op_mov_addr_y,                spc700_op_push_p,            spc700_op_tset1_addr,                   spc700_op_brk,
/* 1x */                   spc700_op_bpl,               spc700_op_tcall_1,               spc700_op_clr0_dp,                  spc700_op_bbs0, /* 1x */              spc700_op_or_a_dpx,            spc700_op_or_a_addrx,            spc700_op_or_a_addry,             spc700_op_or_a_idpy, /* 1x */             spc700_op_or_dp_imm,              spc700_op_or_ix_iy,               spc700_op_decw_dp,               spc700_op_asl_dpx, /* 1x */                 spc700_op_asl_a,                 spc700_op_dec_x,            spc700_op_cmp_x_addr,            spc700_op_jmp_iaddrx,
/* 2x */                  spc700_op_clrp,               spc700_op_tcall_2,               spc700_op_set1_dp,                  spc700_op_bbc1, /* 2x */              spc700_op_and_a_dp,            spc700_op_and_a_addr,              spc700_op_and_a_ix,            spc700_op_and_a_idpx, /* 2x */           spc700_op_and_a_const,             spc700_op_and_dp_dp,            spc700_op_or1_notbit,                spc700_op_rol_dp, /* 2x */              spc700_op_rol_addr,                spc700_op_push_a,               spc700_op_cbne_dp,                   spc700_op_bra,
/* 3x */                   spc700_op_bmi,               spc700_op_tcall_3,               spc700_op_clr1_dp,                  spc700_op_bbs1, /* 3x */             spc700_op_and_a_dpx,           spc700_op_and_a_addrx,           spc700_op_and_a_addry,            spc700_op_and_a_idpy, /* 3x */            spc700_op_and_dp_imm,             spc700_op_and_ix_iy,               spc700_op_incw_dp,               spc700_op_rol_dpx, /* 3x */                 spc700_op_rol_a,                 spc700_op_inc_x,              spc700_op_cmp_x_dp,                  spc700_op_call,

/* 4x */                  spc700_op_setp,               spc700_op_tcall_4,               spc700_op_set2_dp,                  spc700_op_bbc2, /* 4x */              spc700_op_eor_a_dp,            spc700_op_eor_a_addr,              spc700_op_eor_a_ix,            spc700_op_eor_a_idpx, /* 4x */           spc700_op_eor_a_const,             spc700_op_eor_dp_dp,              spc700_op_and1_bit,                spc700_op_lsr_dp, /* 4x */              spc700_op_lsr_addr,                spc700_op_push_x,            spc700_op_tclr1_addr,                 spc700_op_pcall,
/* 5x */                   spc700_op_bvc,               spc700_op_tcall_5,               spc700_op_clr2_dp,                  spc700_op_bbs2, /* 5x */             spc700_op_eor_a_dpx,           spc700_op_eor_a_addrx,           spc700_op_eor_a_addry,            spc700_op_eor_a_idpy, /* 5x */            spc700_op_eor_dp_imm,             spc700_op_eor_ix_iy,            spc700_op_cmpw_ya_dp,               spc700_op_lsr_dpx, /* 5x */                 spc700_op_lsr_a,               spc700_op_mov_x_a,            spc700_op_cmp_y_addr,              spc700_op_jmp_addr,
/* 6x */                  spc700_op_clrc,               spc700_op_tcall_6,               spc700_op_set3_dp,                  spc700_op_bbc3, /* 6x */              spc700_op_cmp_a_dp,            spc700_op_cmp_a_addr,              spc700_op_cmp_a_ix,            spc700_op_cmp_a_idpx, /* 6x */           spc700_op_cmp_a_const,             spc700_op_cmp_dp_dp,           spc700_op_and1_notbit,                spc700_op_ror_dp, /* 6x */              spc700_op_ror_addr,                spc700_op_push_y,               spc700_op_dbnz_dp,                   spc700_op_ret,
/* 7x */                   spc700_op_bvs,               spc700_op_tcall_7,               spc700_op_clr3_dp,                  spc700_op_bbs3, /* 7x */             spc700_op_cmp_a_dpx,           spc700_op_cmp_a_addrx,           spc700_op_cmp_a_addry,            spc700_op_cmp_a_idpy, /* 7x */            spc700_op_cmp_dp_imm,             spc700_op_cmp_ix_iy,            spc700_op_addw_ya_dp,               spc700_op_ror_dpx, /* 7x */                 spc700_op_ror_a,               spc700_op_mov_a_x,              spc700_op_cmp_y_dp,                  spc700_op_reti,

/* 8x */                  spc700_op_setc,               spc700_op_tcall_8,               spc700_op_set4_dp,                  spc700_op_bbc4, /* 8x */              spc700_op_adc_a_dp,            spc700_op_adc_a_addr,              spc700_op_adc_a_ix,            spc700_op_adc_a_idpx, /* 8x */           spc700_op_adc_a_const,             spc700_op_adc_dp_dp,              spc700_op_eor1_bit,                spc700_op_dec_dp, /* 8x */              spc700_op_dec_addr,           spc700_op_mov_y_const,                 spc700_op_pop_p,            spc700_op_mov_dp_imm,
/* 9x */                   spc700_op_bcc,               spc700_op_tcall_9,               spc700_op_clr4_dp,                  spc700_op_bbs4, /* 9x */             spc700_op_adc_a_dpx,           spc700_op_adc_a_addrx,           spc700_op_adc_a_addry,            spc700_op_adc_a_idpy, /* 9x */            spc700_op_adc_dp_imm,             spc700_op_adc_ix_iy,            spc700_op_subw_ya_dp,               spc700_op_dec_dpx, /* 9x */                 spc700_op_dec_a,              spc700_op_mov_x_sp,              spc700_op_div_ya_x,                 spc700_op_xcn_a,
/* ax */                    spc700_op_ei,              spc700_op_tcall_10,               spc700_op_set5_dp,                  spc700_op_bbc5, /* ax */              spc700_op_sbc_a_dp,            spc700_op_sbc_a_addr,              spc700_op_sbc_a_ix,            spc700_op_sbc_a_idpx, /* ax */           spc700_op_sbc_a_const,             spc700_op_sbc_dp_dp,            spc700_op_mov1_c_bit,                spc700_op_inc_dp, /* ax */              spc700_op_inc_addr,           spc700_op_cmp_y_const,                 spc700_op_pop_a,           spc700_op_mov_ixinc_a,
/* bx */                   spc700_op_bcs,              spc700_op_tcall_11,               spc700_op_clr5_dp,                  spc700_op_bbs5, /* bx */             spc700_op_sbc_a_dpx,           spc700_op_sbc_a_addrx,           spc700_op_sbc_a_addry,            spc700_op_sbc_a_idpy, /* bx */            spc700_op_sbc_dp_imm,             spc700_op_sbc_ix_iy,             spc700_op_mov_ya_dp,               spc700_op_inc_dpx, /* bx */                 spc700_op_inc_a,              spc700_op_mov_sp_x,                 spc700_op_das_a,           spc700_op_mov_a_ixinc,

/* cx */                    spc700_op_di,              spc700_op_tcall_12,               spc700_op_set6_dp,                  spc700_op_bbc6, /* cx */              spc700_op_mov_dp_a,            spc700_op_mov_addr_a,              spc700_op_mov_ix_a,            spc700_op_mov_idpx_a, /* cx */           spc700_op_cmp_x_const,            spc700_op_mov_addr_x,            spc700_op_mov1_bit_c,              spc700_op_mov_dp_y, /* cx */              spc700_op_asl_addr,           spc700_op_mov_x_const,                 spc700_op_pop_x,                spc700_op_mul_ya,
/* dx */                   spc700_op_bne,              spc700_op_tcall_13,               spc700_op_clr6_dp,                  spc700_op_bbs6, /* dx */             spc700_op_mov_dpx_a,           spc700_op_mov_addrx_a,           spc700_op_mov_addry_a,            spc700_op_mov_idpy_a, /* dx */              spc700_op_mov_dp_x,             spc700_op_mov_dpy_x,             spc700_op_mov_dp_ya,             spc700_op_mov_dpx_y, /* dx */                 spc700_op_dec_y,               spc700_op_mov_a_y,              spc700_op_cbne_dpx,                 spc700_op_daa_a,
/* ex */                  spc700_op_clrv,              spc700_op_tcall_14,               spc700_op_set7_dp,                  spc700_op_bbc7, /* ex */              spc700_op_mov_a_dp,            spc700_op_mov_a_addr,              spc700_op_mov_a_ix,            spc700_op_mov_a_idpx, /* ex */           spc700_op_mov_a_const,            spc700_op_mov_x_addr,              spc700_op_not1_bit,              spc700_op_mov_y_dp, /* ex */            spc700_op_mov_y_addr,                  spc700_op_notc,                 spc700_op_pop_y,                 spc700_op_sleep,
/* fx */                   spc700_op_beq,              spc700_op_tcall_15,               spc700_op_clr7_dp,                  spc700_op_bbs7, /* fx */             spc700_op_mov_a_dpx,           spc700_op_mov_a_addrx,           spc700_op_mov_a_addry,            spc700_op_mov_a_idpy, /* fx */              spc700_op_mov_x_dp,             spc700_op_mov_x_dpy,             spc700_op_mov_dp_dp,             spc700_op_mov_y_dpx, /* fx */                 spc700_op_inc_y,               spc700_op_mov_y_a,                spc700_op_dbnz_y,                  spc700_op_stop
};
