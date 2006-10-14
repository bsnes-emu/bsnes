//inc_a
case 0xbc: {
  op_io();
  regs.a = op_inc(regs.a);
} break;

//inc_x
case 0x3d: {
  op_io();
  regs.x = op_inc(regs.x);
} break;

//inc_y
case 0xfc: {
  op_io();
  regs.y = op_inc(regs.y);
} break;

//dec_a
case 0x9c: {
  op_io();
  regs.a = op_dec(regs.a);
} break;

//dec_x
case 0x1d: {
  op_io();
  regs.x = op_dec(regs.x);
} break;

//dec_y
case 0xdc: {
  op_io();
  regs.y = op_dec(regs.y);
} break;

//asl_a
case 0x1c: {
  op_io();
  regs.a = op_asl(regs.a);
} break;

//lsr_a
case 0x5c: {
  op_io();
  regs.a = op_lsr(regs.a);
} break;

//rol_a
case 0x3c: {
  op_io();
  regs.a = op_rol(regs.a);
} break;

//ror_a
case 0x7c: {
  op_io();
  regs.a = op_ror(regs.a);
} break;

//inc_dp
case 0xab: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd = op_inc(rd);
  op_writedp(dp, rd);
} break;

//dec_dp
case 0x8b: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd = op_dec(rd);
  op_writedp(dp, rd);
} break;

//asl_dp
case 0x0b: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd = op_asl(rd);
  op_writedp(dp, rd);
} break;

//lsr_dp
case 0x4b: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd = op_lsr(rd);
  op_writedp(dp, rd);
} break;

//rol_dp
case 0x2b: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd = op_rol(rd);
  op_writedp(dp, rd);
} break;

//ror_dp
case 0x6b: {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd = op_ror(rd);
  op_writedp(dp, rd);
} break;

//inc_dpx
case 0xbb: {
  dp = op_readpc();
  op_io();
  rd = op_readdp(dp + regs.x);
  rd = op_inc(rd);
  op_writedp(dp + regs.x, rd);
} break;

//dec_dpx
case 0x9b: {
  dp = op_readpc();
  op_io();
  rd = op_readdp(dp + regs.x);
  rd = op_dec(rd);
  op_writedp(dp + regs.x, rd);
} break;

//asl_dpx
case 0x1b: {
  dp = op_readpc();
  op_io();
  rd = op_readdp(dp + regs.x);
  rd = op_asl(rd);
  op_writedp(dp + regs.x, rd);
} break;

//lsr_dpx
case 0x5b: {
  dp = op_readpc();
  op_io();
  rd = op_readdp(dp + regs.x);
  rd = op_lsr(rd);
  op_writedp(dp + regs.x, rd);
} break;

//rol_dpx
case 0x3b: {
  dp = op_readpc();
  op_io();
  rd = op_readdp(dp + regs.x);
  rd = op_rol(rd);
  op_writedp(dp + regs.x, rd);
} break;

//ror_dpx
case 0x7b: {
  dp = op_readpc();
  op_io();
  rd = op_readdp(dp + regs.x);
  rd = op_ror(rd);
  op_writedp(dp + regs.x, rd);
} break;

//inc_addr
case 0xac: {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  rd = op_inc(rd);
  op_writeaddr(dp, rd);
} break;

//dec_addr
case 0x8c: {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  rd = op_dec(rd);
  op_writeaddr(dp, rd);
} break;

//asl_addr
case 0x0c: {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  rd = op_asl(rd);
  op_writeaddr(dp, rd);
} break;

//lsr_addr
case 0x4c: {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  rd = op_lsr(rd);
  op_writeaddr(dp, rd);
} break;

//rol_addr
case 0x2c: {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  rd = op_rol(rd);
  op_writeaddr(dp, rd);
} break;

//ror_addr
case 0x6c: {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  rd = op_ror(rd);
  op_writeaddr(dp, rd);
} break;

//incw_dp
case 0x3a: {
  dp = op_readpc();
  rd  = op_readdp(dp);
  rd |= op_readdp(dp + 1) << 8;
  rd = op_incw(rd);
  op_writedp(dp + 1, rd >> 8);
  op_writedp(dp,     rd);
} break;

//decw_dp
case 0x1a: {
  dp = op_readpc();
  rd  = op_readdp(dp);
  rd |= op_readdp(dp + 1) << 8;
  rd = op_decw(rd);
  op_writedp(dp + 1, rd >> 8);
  op_writedp(dp,     rd);
} break;

