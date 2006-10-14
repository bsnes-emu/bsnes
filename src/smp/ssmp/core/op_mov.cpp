//mov_a_x
case 0x7d: {
  op_io();
  regs.a = regs.x;
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
} break;

//mov_a_y
case 0xdd: {
  op_io();
  regs.a = regs.y;
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
} break;

//mov_x_a
case 0x5d: {
  op_io();
  regs.x = regs.a;
  regs.p.n = !!(regs.x & 0x80);
  regs.p.z = (regs.x == 0);
} break;

//mov_y_a
case 0xfd: {
  op_io();
  regs.y = regs.a;
  regs.p.n = !!(regs.y & 0x80);
  regs.p.z = (regs.y == 0);
} break;

//mov_x_sp
case 0x9d: {
  op_io();
  regs.x = regs.sp;
  regs.p.n = !!(regs.x & 0x80);
  regs.p.z = (regs.x == 0);
} break;

//mov_sp_x
case 0xbd: {
  op_io();
  regs.sp = regs.x;
} break;

//mov_a_const
case 0xe8: {
  regs.a = op_readpc();
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
} break;

//mov_x_const
case 0xcd: {
  regs.x = op_readpc();
  regs.p.n = !!(regs.x & 0x80);
  regs.p.z = (regs.x == 0);
} break;

//mov_y_const
case 0x8d: {
  regs.y = op_readpc();
  regs.p.n = !!(regs.y & 0x80);
  regs.p.z = (regs.y == 0);
} break;

//mov_a_ix
case 0xe6: {
  op_io();
  regs.a = op_readdp(regs.x);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
} break;

//mov_a_ixinc
case 0xbf: {
  op_io();
  op_io();
  regs.a = op_readdp(regs.x++);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
} break;

//mov_a_dp
case 0xe4: {
  sp = op_readpc();
  regs.a = op_readdp(sp);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
} break;

//mov_x_dp
case 0xf8: {
  sp = op_readpc();
  regs.x = op_readdp(sp);
  regs.p.n = !!(regs.x & 0x80);
  regs.p.z = (regs.x == 0);
} break;

//mov_y_dp
case 0xeb: {
  sp = op_readpc();
  regs.y = op_readdp(sp);
  regs.p.n = !!(regs.y & 0x80);
  regs.p.z = (regs.y == 0);
} break;

//mov_a_dpx
case 0xf4: {
  sp = op_readpc();
  op_io();
  regs.a = op_readdp(sp + regs.x);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
} break;

//mov_x_dpy
case 0xf9: {
  sp = op_readpc();
  op_io();
  regs.x = op_readdp(sp + regs.y);
  regs.p.n = !!(regs.x & 0x80);
  regs.p.z = (regs.x == 0);
} break;

//mov_y_dpx
case 0xfb: {
  sp = op_readpc();
  op_io();
  regs.y = op_readdp(sp + regs.x);
  regs.p.n = !!(regs.y & 0x80);
  regs.p.z = (regs.y == 0);
} break;

//mov_a_addr
case 0xe5: {
  sp  = op_readpc();
  sp |= op_readpc() << 8;
  regs.a = op_readaddr(sp);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
} break;

//mov_x_addr
case 0xe9: {
  sp  = op_readpc();
  sp |= op_readpc() << 8;
  regs.x = op_readaddr(sp);
  regs.p.n = !!(regs.x & 0x80);
  regs.p.z = (regs.x == 0);
} break;

//mov_y_addr
case 0xec: {
  sp  = op_readpc();
  sp |= op_readpc() << 8;
  regs.y = op_readaddr(sp);
  regs.p.n = !!(regs.y & 0x80);
  regs.p.z = (regs.y == 0);
} break;

//mov_a_addrx
case 0xf5: {
  sp  = op_readpc();
  sp |= op_readpc() << 8;
  op_io();
  regs.a = op_readaddr(sp + regs.x);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
} break;

//mov_a_addry
case 0xf6: {
  sp  = op_readpc();
  sp |= op_readpc() << 8;
  op_io();
  regs.a = op_readaddr(sp + regs.y);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
} break;

//mov_a_idpx
case 0xe7: {
  dp = op_readpc() + regs.x;
  op_io();
  sp  = op_readdp(dp);
  sp |= op_readdp(dp + 1) << 8;
  regs.a = op_readaddr(sp);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
} break;

//mov_a_idpy
case 0xf7: {
  dp = op_readpc();
  op_io();
  sp  = op_readdp(dp);
  sp |= op_readdp(dp + 1) << 8;
  regs.a = op_readaddr(sp + regs.y);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
} break;

//mov_dp_dp
case 0xfa: {
  sp = op_readpc();
  dp = op_readpc();
  rd = op_readdp(sp);
  op_writedp(dp, rd);
} break;

//mov_dp_const
case 0x8f: {
  rd = op_readpc();
  dp = op_readpc();
  op_io();
  op_writedp(dp, rd);
} break;

//mov_ix_a
case 0xc6: {
  op_io();
  op_io();
  op_writedp(regs.x, regs.a);
} break;

//mov_ixinc_a
case 0xaf: {
  op_io();
  op_io();
  op_writedp(regs.x++, regs.a);
} break;

//mov_dp_a
case 0xc4: {
  dp = op_readpc();
  op_io();
  op_writedp(dp, regs.a);
} break;

//mov_dp_x
case 0xd8: {
  dp = op_readpc();
  op_io();
  op_writedp(dp, regs.x);
} break;

//mov_dp_y
case 0xcb: {
  dp = op_readpc();
  op_io();
  op_writedp(dp, regs.y);
} break;

//mov_dpx_a
case 0xd4: {
  dp = op_readpc();
  op_io();
  op_io();
  op_writedp(dp + regs.x, regs.a);
} break;

//mov_dpy_x
case 0xd9: {
  dp = op_readpc();
  op_io();
  op_io();
  op_writedp(dp + regs.y, regs.x);
} break;

//mov_dpx_y
case 0xdb: {
  dp = op_readpc();
  op_io();
  op_io();
  op_writedp(dp + regs.x, regs.y);
} break;

//mov_addr_a
case 0xc5: {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  op_writeaddr(dp, regs.a);
} break;

//mov_addr_x
case 0xc9: {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  op_writeaddr(dp, regs.x);
} break;

//mov_addr_y
case 0xcc: {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  op_writeaddr(dp, regs.y);
} break;

//mov_addrx_a
case 0xd5: {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  op_io();
  op_writeaddr(dp + regs.x, regs.a);
} break;

//mov_addry_a
case 0xd6: {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  op_io();
  op_writeaddr(dp + regs.y, regs.a);
} break;

//mov_idpx_a
case 0xc7: {
  sp = op_readpc() + regs.x;
  op_io();
  dp  = op_readdp(sp);
  dp |= op_readdp(sp + 1) << 8;
  op_io();
  op_writeaddr(dp, regs.a);
} break;

//mov_idpy_a
case 0xd7: {
  sp = op_readpc();
  op_io();
  dp  = op_readdp(sp);
  dp |= op_readdp(sp + 1) << 8;
  op_io();
  op_writeaddr(dp + regs.y, regs.a);
} break;

//movw_ya_dp
case 0xba: {
  sp = op_readpc();
  op_io();
  regs.a = op_readdp(sp);
  regs.y = op_readdp(sp + 1);
  regs.p.n = !!(regs.ya & 0x8000);
  regs.p.z = (regs.ya == 0);
} break;

//movw_dp_ya
case 0xda: {
  dp = op_readpc();
  op_io();
  op_writedp(dp,     regs.a);
  op_writedp(dp + 1, regs.y);
} break;

//mov1_c_bit
case 0xaa: {
  sp  = op_readpc();
  sp |= op_readpc() << 8;
  bit = sp >> 13;
  sp &= 0x1fff;
  rd = op_readaddr(sp);
  regs.p.c = !!(rd & (1 << bit));
} break;

//mov1_bit_c
case 0xca: {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  bit = dp >> 13;
  dp &= 0x1fff;
  rd = op_readaddr(dp);
  if(regs.p.c)rd |=  (1 << bit);
  else        rd &= ~(1 << bit);
  op_writeaddr(dp, rd);
} break;

