// S-SMP CPU instruction emulation

// snes_spc 0.9.5. http://www.slack.net/~ant/

// Truncation
#undef BYTE
#define BYTE(  n ) ((BOOST::uint8_t ) (n)) /* (unsigned) n & 0xFF */

//// Memory access

#if SPC_MORE_ACCURACY
	#define SUSPICIOUS_OPCODE( name ) ((void) 0)
#else
	#define SUSPICIOUS_OPCODE( name ) dprintf( "SPC: suspicious opcode: %s\n", name )
#endif

#define TIME( delay )                   (rel_time + (delay))

#define DP_ADDR( addr )                 (dp + (addr))

#define READ_DP(  delay, addr )         READ ( delay, DP_ADDR( addr ) )
#define WRITE_DP( delay, addr, data )   WRITE( delay, DP_ADDR( addr ), data )

#define READ_PROG16( addr )             GET_LE16( ram + (addr) )

#define SET_PC( n )     (pc = ram + (n))
#define GET_PC()        (pc - ram)
#define READ_PC( pc )   (*(pc))
#define READ_PC16( pc ) GET_LE16( pc )

#define SET_SP( v )     (sp = ram + 0x101 + (v))
#define GET_SP()        (sp - 0x101 - ram)

#define PUSH16( data )\
{\
	int addr = (sp -= 2) - ram;\
	if ( addr > 0x100 )\
	{\
		SET_LE16( sp, data );\
	}\
	else\
	{\
		ram [BYTE( addr ) + 0x100] = BYTE( data );\
		sp [1] = BYTE( data >> 8 );\
		sp += 0x100;\
	}\
}

#define PUSH( data )\
{\
	*--sp = BYTE( data );\
	if ( sp - ram == 0x100 )\
		sp += 0x100;\
}

#define POP( out )\
{\
	out = *sp++;\
	if ( sp - ram == 0x201 )\
	{\
		out = sp [-0x101];\
		sp -= 0x100;\
	}\
}

#define MEM_BIT( rel ) cpu_mem_bit( rel_time + rel, pc )

unsigned Spc_Core::cpu_mem_bit( rel_time_t rel_time, byte const* pc )
{
	unsigned addr = READ_PC16( pc );
	unsigned t = READ( 0, addr & 0x1FFF ) >> (addr >> 13);
	return t << 8 & 0x100;
}

//// Status flag handling

// Flags with hex value for clarity when used as mask.
// Stored in indicated variable during emulation.
int const n80 = 0x80; // nz
int const v40 = 0x40; // psw
int const p20 = 0x20; // dp
int const b10 = 0x10; // psw
int const h08 = 0x08; // psw
int const i04 = 0x04; // psw
int const z02 = 0x02; // nz
int const c01 = 0x01; // c

int const nz_neg_mask = 0x880; // either bit set indicates N flag set

#define GET_PSW( out )\
{\
	out = psw & ~(n80 | p20 | z02 | c01);\
	out |= c  >> 8 & c01;\
	out |= dp >> 3 & p20;\
	out |= ((nz >> 4) | nz) & n80;\
	if ( !BYTE( nz ) ) out |= z02;\
}

#define SET_PSW( in )\
{\
	psw = in;\
	c   = in << 8;\
	dp  = in << 3 & 0x100;\
	nz  = (in << 4 & 0x800) | (~in & z02);\
}

SPC_CPU_RUN_FUNC
{
	byte* const ram = this->ram();
	int a = BYTE( cpu_regs.a );
	int x = BYTE( cpu_regs.x );
	int y = BYTE( cpu_regs.y );
	byte const* pc;
	byte* sp;
	int psw;
	int c;
	int nz;
	int dp;
	
	SET_PC( cpu_regs.pc & 0xFFFF );
	SET_SP( BYTE( cpu_regs.sp ) );
	SET_PSW( cpu_regs.psw );
	
	goto next_instr;
	
	
	// Main loop
	
cbranch_taken_loop:
	pc += *(BOOST::int8_t const*) pc;
inc_pc_loop:
	pc++;
next_instr:
{
	check( (unsigned) a < 0x100 );
	check( (unsigned) x < 0x100 );
	check( (unsigned) y < 0x100 );
	
	static byte const cycle_table [256] = {
	//  0 1 2 3 4 5 6 7 8 9 A B C D E F
		2,8,4,7,3,4,3,6,2,6,5,4,5,4,6,8,// 0
		4,8,4,7,4,5,5,6,5,5,6,5,2,2,4,6,// 1
		2,8,4,7,3,4,3,6,2,6,5,4,5,4,7,4,// 2
		4,8,4,7,4,5,5,6,5,5,6,5,2,2,3,8,// 3
		2,8,4,7,3,4,3,6,2,6,4,4,5,4,6,6,// 4
		4,8,4,7,4,5,5,6,5,5,4,5,2,2,4,3,// 5
		2,8,4,7,3,4,3,6,2,6,4,4,5,4,7,5,// 6
		4,8,4,7,4,5,5,6,5,5,5,5,2,2,3,6,// 7
		2,8,4,7,3,4,3,6,2,6,5,4,5,2,4,5,// 8
		4,8,4,7,4,5,5,6,5,5,5,5,2,2,12,5,//9
		3,8,4,7,3,4,3,6,2,6,4,4,5,2,4,4,// A
		4,8,4,7,4,5,5,6,5,5,5,5,2,2,3,4,// B
		3,8,4,7,4,5,4,7,2,5,6,4,5,2,4,9,// C
		4,8,4,7,5,6,6,7,4,5,5,5,2,2,8,3,// D
		2,8,4,7,3,4,3,6,2,4,5,3,4,3,4,0,// E
		4,8,4,7,4,5,5,6,3,4,5,4,2,2,6,0// F
	};
	
	unsigned opcode = *pc;
	if ( rel_time >= 0 )
		goto stop;
	
	#ifdef CPU_INSTR_HOOK
		CPU_INSTR_HOOK( GET_PC(), pc, a, x, y, GET_SP(), rel_time );
	#endif
	
	rel_time += cycle_table [opcode];
	
	// TODO: if PC is at end of memory, this will get wrong operand (very obscure)
	unsigned data = *++pc;
	switch ( opcode )
	{
	
// Common instructions

#define BRANCH( cond )\
{\
	pc++;\
	pc += (BOOST::int8_t) data;\
	if ( cond )\
		goto next_instr;\
	pc -= (BOOST::int8_t) data;\
	rel_time -= 2;\
	goto next_instr;\
}

	case 0xF0: // BEQ
		BRANCH( !BYTE( nz ) ) // 89% taken
	
	case 0xD0: // BNE
		BRANCH( BYTE( nz ) )
	
	case 0x3F:{// CALL
		int old_addr = GET_PC() + 2;
		SET_PC( READ_PC16( pc ) );
		PUSH16( old_addr );
		goto next_instr;
	}
	
	case 0x6F:{// RET
		int addr = sp - ram;
		SET_PC( GET_LE16( sp ) );
		sp += 2;
		if ( addr < 0x1FF )
			goto next_instr;
		
		SET_PC( sp [-0x101] * 0x100 + ram [BYTE( addr ) + 0x100] );
		sp -= 0x100;
		goto next_instr;
	}
	
	case 0xE4: // MOV a,dp
		++pc;
		READ_TIMER( 0, DP_ADDR( data ), a = nz ); // 80% from timer
		goto next_instr;
	
	case 0xFA:{// MOV dp,dp
		int temp;
		READ_TIMER( -2, DP_ADDR( data ), temp );
		data = temp + no_read_before_write ;
	}
	// fall through
	case 0x8F:{// MOV dp,#imm
		int temp = READ_PC( pc + 1 );
		pc += 2;
		WRITE_DP_FAST( temp, data ); // 76% $F0-$FF
		goto next_instr;
	}
	
	case 0xC4: // MOV dp,a
		++pc;
		WRITE_DP_FAST( data, a ); // 39% $F0-$FF
		goto next_instr;
	
#define CASE( n )   case n:

// Define common address modes based on opcode for immediate mode. Execution
// ends with data set to the address of the operand.
#define ADDR_MODES_( op )\
	CASE( op - 0x02 ) /* (X) */\
		data = x + dp;\
		pc--;\
		goto end_##op;\
	CASE( op + 0x0F ) /* (dp)+Y */\
		data = READ_PROG16( data + dp ) + y;\
		goto end_##op;\
	CASE( op - 0x01 ) /* (dp+X) */\
		data = READ_PROG16( (BYTE( data + x ) ) + dp );\
		goto end_##op;\
	CASE( op + 0x0E ) /* abs+Y */\
		data += y;\
		goto abs_##op;\
	CASE( op + 0x0D ) /* abs+X */\
		data += x;\
	CASE( op - 0x03 ) /* abs */\
	abs_##op:\
		data += 0x100 * READ_PC( ++pc );\
		goto end_##op;\
	CASE( op + 0x0C ) /* dp+X */\
		data = BYTE( data + x );

#define ADDR_MODES_NO_DP( op )\
	ADDR_MODES_( op )\
		data += dp;\
	end_##op:

#define ADDR_MODES( op )\
	ADDR_MODES_( op )\
	CASE( op - 0x04 ) /* dp */\
		data += dp;\
	end_##op:

// 1. 8-bit Data Transmission Commands. Group I

	ADDR_MODES_NO_DP( 0xE8 ) // MOV A,addr
		a = nz = READ( 0, data );
		goto inc_pc_loop;
	
	case 0xBF:{// MOV A,(X)+
		int temp = x + dp;
		x = BYTE( x + 1 );
		a = nz = READ( -1, temp );
		goto next_instr;
	}
	
	case 0xE8: // MOV A,imm
		a  = data;
		nz = data;
		goto inc_pc_loop;
	
	case 0xF9: // MOV X,dp+Y
		data = BYTE( data + y );
	case 0xF8: // MOV X,dp
		READ_TIMER( 0, DP_ADDR( data ), x = nz );
		goto inc_pc_loop;
	
	case 0xE9: // MOV X,abs
		data = READ_PC16( pc );
		++pc;
		data = READ( 0, data );
	case 0xCD: // MOV X,imm
		x  = data;
		nz = data;
		goto inc_pc_loop;
	
	case 0xFB: // MOV Y,dp+X
		data = BYTE( data + x );
	case 0xEB: // MOV Y,dp
		pc++;
		READ_TIMER( 0, DP_ADDR( data ), y = nz ); // 70% from timer
		goto next_instr;
	
	case 0xEC:{// MOV Y,abs
		int temp = READ_PC16( pc );
		pc += 2;
		READ_TIMER( 0, temp, y = nz );
		goto next_instr;
	}
	
	case 0x8D: // MOV Y,imm
		y  = data;
		nz = data;
		goto inc_pc_loop;
	
// 2. 8-BIT DATA TRANSMISSION COMMANDS, GROUP 2

	ADDR_MODES_NO_DP( 0xC8 ) // MOV addr,A
		WRITE( 0, data, a );
		goto inc_pc_loop;
	
	{
		int temp;
	case 0xCC: // MOV abs,Y
		temp = y;
		goto mov_abs_temp;
	case 0xC9: // MOV abs,X
		temp = x;
	mov_abs_temp:
		WRITE( 0, READ_PC16( pc ), temp );
		pc += 2;
		goto next_instr;
	}
	
	case 0xD9: // MOV dp+Y,X
		data = BYTE( data + y );
	case 0xD8: // MOV dp,X
		WRITE( 0, data + dp, x );
		goto inc_pc_loop;
	
	case 0xDB: // MOV dp+X,Y
		data = BYTE( data + x );
	case 0xCB: // MOV dp,Y
		WRITE( 0, data + dp, y );
		goto inc_pc_loop;

// 3. 8-BIT DATA TRANSMISSIN COMMANDS, GROUP 3.
	
	case 0x7D: // MOV A,X
		a  = x;
		nz = x;
		goto next_instr;
	
	case 0xDD: // MOV A,Y
		a  = y;
		nz = y;
		goto next_instr;
	
	case 0x5D: // MOV X,A
		x  = a;
		nz = a;
		goto next_instr;
	
	case 0xFD: // MOV Y,A
		y  = a;
		nz = a;
		goto next_instr;
	
	case 0x9D: // MOV X,SP
		x = nz = GET_SP();
		goto next_instr;
	
	case 0xBD: // MOV SP,X
		SET_SP( x );
		goto next_instr;
	
	//case 0xC6: // MOV (X),A (handled by MOV addr,A in group 2)
	
	case 0xAF: // MOV (X)+,A
		WRITE_DP( 0, x, a + no_read_before_write  );
		x++;
		goto next_instr;
	
// 5. 8-BIT LOGIC OPERATION COMMANDS
	
#define LOGICAL_OP( op, func )\
	ADDR_MODES( op ) /* addr */\
		data = READ( 0, data );\
	case op: /* imm */\
		nz = a func##= data;\
		goto inc_pc_loop;\
	{   unsigned addr;\
	case op + 0x11: /* X,Y */\
		data = READ_DP( -2, y );\
		addr = x + dp;\
		goto addr_##op;\
	case op + 0x01: /* dp,dp */\
		data = READ_DP( -3, data );\
	case op + 0x10:{/*dp,imm*/\
		byte const* addr2 = pc + 1;\
		pc += 2;\
		addr = READ_PC( addr2 ) + dp;\
	}\
	addr_##op:\
		nz = data func READ( -1, addr );\
		WRITE( 0, addr, nz );\
		goto next_instr;\
	}
	
	LOGICAL_OP( 0x28, & ); // AND
	
	LOGICAL_OP( 0x08, | ); // OR
	
	LOGICAL_OP( 0x48, ^ ); // EOR
	
// 4. 8-BIT ARITHMETIC OPERATION COMMANDS

	ADDR_MODES( 0x68 ) // CMP addr
		data = READ( 0, data );
	case 0x68: // CMP imm
		nz = a - data;
		c = ~nz;
		nz &= 0xFF;
		goto inc_pc_loop;
	
	case 0x79: // CMP (X),(Y)
		data = READ_DP( -2, y );
		nz = READ_DP( -1, x ) - data;
		c = ~nz;
		nz &= 0xFF;
		goto next_instr;
	
	case 0x69: // CMP dp,dp
		data = READ_DP( -3, data );
	case 0x78: // CMP dp,imm
		nz = READ_DP( -1, READ_PC( ++pc ) ) - data;
		c = ~nz;
		nz &= 0xFF;
		goto inc_pc_loop;
	
	case 0x3E: // CMP X,dp
		data += dp;
		goto cmp_x_addr;
	case 0x1E: // CMP X,abs
		data = READ_PC16( pc );
		pc++;
	cmp_x_addr:
		data = READ( 0, data );
	case 0xC8: // CMP X,imm
		nz = x - data;
		c = ~nz;
		nz &= 0xFF;
		goto inc_pc_loop;
	
	case 0x7E: // CMP Y,dp
		data += dp;
		goto cmp_y_addr;
	case 0x5E: // CMP Y,abs
		data = READ_PC16( pc );
		pc++;
	cmp_y_addr:
		data = READ( 0, data );
	case 0xAD: // CMP Y,imm
		nz = y - data;
		c = ~nz;
		nz &= 0xFF;
		goto inc_pc_loop;
	
	{
		int addr;
	case 0xB9: // SBC (x),(y)
	case 0x99: // ADC (x),(y)
		pc--; // compensate for inc later
		data = READ_DP( -2, y );
		addr = x + dp;
		goto adc_addr;
	case 0xA9: // SBC dp,dp
	case 0x89: // ADC dp,dp
		data = READ_DP( -3, data );
	case 0xB8: // SBC dp,imm
	case 0x98: // ADC dp,imm
		addr = READ_PC( ++pc ) + dp;
	adc_addr:
		nz = READ( -1, addr );
		goto adc_data;
		
// catch ADC and SBC together, then decode later based on operand
#undef CASE
#define CASE( n ) case n: case (n) + 0x20:
	ADDR_MODES( 0x88 ) // ADC/SBC addr
		data = READ( 0, data );
	case 0xA8: // SBC imm
	case 0x88: // ADC imm
		addr = -1; // A
		nz = a;
	adc_data: {
		int flags;
		if ( opcode >= 0xA0 ) // SBC
			data ^= 0xFF;
		
		flags = data ^ nz;
		nz += data + (c >> 8 & 1);
		flags ^= nz;
		
		psw = (psw & ~(v40 | h08)) |
				(flags >> 1 & h08) |
				((flags + 0x80) >> 2 & v40);
		c = nz;
		if ( addr < 0 )
		{
			a = BYTE( nz );
			goto inc_pc_loop;
		}
		WRITE( 0, addr, /*(byte)*/ nz );
		goto inc_pc_loop;
	}
	
	}
	
// 6. ADDITION & SUBTRACTION COMMANDS

#define INC_DEC_REG( reg, op )\
		nz  = reg op;\
		reg = BYTE( nz );\
		goto next_instr;

	case 0xBC: INC_DEC_REG( a, + 1 ) // INC A
	case 0x3D: INC_DEC_REG( x, + 1 ) // INC X
	case 0xFC: INC_DEC_REG( y, + 1 ) // INC Y
	
	case 0x9C: INC_DEC_REG( a, - 1 ) // DEC A
	case 0x1D: INC_DEC_REG( x, - 1 ) // DEC X
	case 0xDC: INC_DEC_REG( y, - 1 ) // DEC Y

	case 0x9B: // DEC dp+X
	case 0xBB: // INC dp+X
		data = BYTE( data + x );
	case 0x8B: // DEC dp
	case 0xAB: // INC dp
		data += dp;
		goto inc_abs;
	case 0x8C: // DEC abs
	case 0xAC: // INC abs
		data = READ_PC16( pc );
		pc++;
	inc_abs:
		nz = (opcode >> 4 & 2) - 1;
		nz += READ( -1, data );
		WRITE( 0, data, /*(byte)*/ nz );
		goto inc_pc_loop;
	
// 7. SHIFT, ROTATION COMMANDS

	case 0x5C: // LSR A
		c = 0;
	case 0x7C:{// ROR A
		nz = (c >> 1 & 0x80) | (a >> 1);
		c = a << 8;
		a = nz;
		goto next_instr;
	}
	
	case 0x1C: // ASL A
		c = 0;
	case 0x3C:{// ROL A
		int temp = c >> 8 & 1;
		c = a << 1;
		nz = c | temp;
		a = BYTE( nz );
		goto next_instr;
	}
	
	case 0x0B: // ASL dp
		c = 0;
		data += dp;
		goto rol_mem;
	case 0x1B: // ASL dp+X
		c = 0;
	case 0x3B: // ROL dp+X
		data = BYTE( data + x );
	case 0x2B: // ROL dp
		data += dp;
		goto rol_mem;
	case 0x0C: // ASL abs
		c = 0;
	case 0x2C: // ROL abs
		data = READ_PC16( pc );
		pc++;
	rol_mem:
		nz = c >> 8 & 1;
		nz |= (c = READ( -1, data ) << 1);
		WRITE( 0, data, /*(byte)*/ nz );
		goto inc_pc_loop;
	
	case 0x4B: // LSR dp
		c = 0;
		data += dp;
		goto ror_mem;
	case 0x5B: // LSR dp+X
		c = 0;
	case 0x7B: // ROR dp+X
		data = BYTE( data + x );
	case 0x6B: // ROR dp
		data += dp;
		goto ror_mem;
	case 0x4C: // LSR abs
		c = 0;
	case 0x6C: // ROR abs
		data = READ_PC16( pc );
		pc++;
	ror_mem: {
		int temp = READ( -1, data );
		nz = (c >> 1 & 0x80) | (temp >> 1);
		c = temp << 8;
		WRITE( 0, data, nz );
		goto inc_pc_loop;
	}

	case 0x9F: // XCN
		nz = a = (a >> 4) | BYTE( a << 4 );
		goto next_instr;

// 8. 16-BIT TRANSMISION COMMANDS

	case 0xBA: // MOVW YA,dp
		a = READ_DP( -2, data );
		nz = (a & 0x7F) | (a >> 1);
		y = READ_DP( 0, BYTE( data + 1 ) );
		nz |= y;
		goto inc_pc_loop;
	
	case 0xDA: // MOVW dp,YA
		WRITE_DP( -1, data, a );
		WRITE_DP( 0, BYTE( data + 1 ), y + no_read_before_write  );
		goto inc_pc_loop;
	
// 9. 16-BIT OPERATION COMMANDS

	case 0x3A: // INCW dp
	case 0x1A:{// DECW dp
		int temp;
		// low byte
		data += dp;
		temp = READ( -3, data );
		temp += (opcode >> 4 & 2) - 1; // +1 for INCW, -1 for DECW
		nz = ((temp >> 1) | temp) & 0x7F;
		WRITE( -2, data, /*(byte)*/ temp );
		
		// high byte
		data = BYTE( data + 1 ) + dp;
		temp = BYTE( (temp >> 8) + READ( -1, data ) );
		nz |= temp;
		WRITE( 0, data, temp );
		
		goto inc_pc_loop;
	}
		
	case 0x7A: // ADDW YA,dp
	case 0x9A:{// SUBW YA,dp
		int lo = READ_DP( -2, data );
		int hi = READ_DP( 0, BYTE( data + 1 ) );
		int result;
		int flags;
		
		if ( opcode == 0x9A ) // SUBW
		{
			lo = (lo ^ 0xFF) + 1;
			hi ^= 0xFF;
		}
		
		lo += a;
		result = y + hi + (lo >> 8);
		flags = hi ^ y ^ result;
		
		psw = (psw & ~(v40 | h08)) |
				(flags >> 1 & h08) |
				((flags + 0x80) >> 2 & v40);
		c = result;
		a = BYTE( lo );
		result = BYTE( result );
		y = result;
		nz = (((lo >> 1) | lo) & 0x7F) | result;
		
		goto inc_pc_loop;
	}
	
	case 0x5A: { // CMPW YA,dp
		int temp = a - READ_DP( -1, data );
		nz = ((temp >> 1) | temp) & 0x7F;
		temp = y + (temp >> 8);
		temp -= READ_DP( 0, BYTE( data + 1 ) );
		nz |= temp;
		c  = ~temp;
		nz &= 0xFF;
		goto inc_pc_loop;
	}
	
// 10. MULTIPLICATION & DIVISON COMMANDS

	case 0xCF: { // MUL YA
		unsigned temp = y * a;
		a = BYTE( temp );
		nz = ((temp >> 1) | temp) & 0x7F;
		y = temp >> 8;
		nz |= y;
		goto next_instr;
	}
	
	case 0x9E: // DIV YA,X
	{
		unsigned ya = y * 0x100 + a;
		
		psw &= ~(h08 | v40);
		
		if ( y >= x )
			psw |= v40;
		
		if ( (y & 15) >= (x & 15) )
			psw |= h08;
		
		if ( y < x * 2 )
		{
			a = ya / x;
			y = ya - a * x;
		}
		else
		{
			a = 255 - (ya - x * 0x200) / (256 - x);
			y = x   + (ya - x * 0x200) % (256 - x);
		}
		
		nz = BYTE( a );
		a = BYTE( a );
		
		goto next_instr;
	}
	
// 11. DECIMAL COMPENSATION COMMANDS
	
	case 0xDF: // DAA
		SUSPICIOUS_OPCODE( "DAA" );
		if ( a > 0x99 || c & 0x100 )
		{
			a += 0x60;
			c = 0x100;
		}
		
		if ( (a & 0x0F) > 9 || psw & h08 )
			a += 0x06;
		
		nz = a;
		a = BYTE( a );
		goto next_instr;
	
	case 0xBE: // DAS
		SUSPICIOUS_OPCODE( "DAS" );
		if ( a > 0x99 || !(c & 0x100) )
		{
			a -= 0x60;
			c = 0;
		}
		
		if ( (a & 0x0F) > 9 || !(psw & h08) )
			a -= 0x06;
		
		nz = a;
		a = BYTE( a );
		goto next_instr;
	
// 12. BRANCHING COMMANDS

	case 0x2F: // BRA rel
		pc += (BOOST::int8_t) data;
		goto inc_pc_loop;
	
	case 0x30: // BMI
		BRANCH( (nz & nz_neg_mask) )
	
	case 0x10: // BPL
		BRANCH( !(nz & nz_neg_mask) )
	
	case 0xB0: // BCS
		BRANCH( c & 0x100 )
	
	case 0x90: // BCC
		BRANCH( !(c & 0x100) )
	
	case 0x70: // BVS
		BRANCH( psw & v40 )
	
	case 0x50: // BVC
		BRANCH( !(psw & v40) )
	
	#define CBRANCH( cond )\
	{\
		pc++;\
		if ( cond )\
			goto cbranch_taken_loop;\
		rel_time -= 2;\
		goto inc_pc_loop;\
	}
	
	case 0x03: // BBS dp.bit,rel
	case 0x23:
	case 0x43:
	case 0x63:
	case 0x83:
	case 0xA3:
	case 0xC3:
	case 0xE3:
		CBRANCH( READ_DP( -4, data ) >> (opcode >> 5) & 1 )
	
	case 0x13: // BBC dp.bit,rel
	case 0x33:
	case 0x53:
	case 0x73:
	case 0x93:
	case 0xB3:
	case 0xD3:
	case 0xF3:
		CBRANCH( !(READ_DP( -4, data ) >> (opcode >> 5) & 1) )
	
	case 0xDE: // CBNE dp+X,rel
		data = BYTE( data + x );
		// fall through
	case 0x2E:{// CBNE dp,rel
		int temp;
		READ_TIMER( -4, DP_ADDR( data ), temp ); // 61% from timer
		CBRANCH( temp != a )
	}
	
	case 0x6E: { // DBNZ dp,rel
		unsigned temp = READ_DP( -4, data ) - 1;
		WRITE_DP( -3, BYTE( data ), /*(byte)*/ temp + no_read_before_write  );
		CBRANCH( temp )
	}
	
	case 0xFE: // DBNZ Y,rel
		y = BYTE( y - 1 );
		BRANCH( y )
	
	case 0x1F: // JMP [abs+X]
		SET_PC( READ_PC16( pc ) + x );
		// fall through
	case 0x5F: // JMP abs
		SET_PC( READ_PC16( pc ) );
		goto next_instr;
	
// 13. SUB-ROUTINE CALL RETURN COMMANDS
	
	case 0x0F:{// BRK
		int temp;
		int ret_addr = GET_PC();
		SUSPICIOUS_OPCODE( "BRK" );
		SET_PC( READ_PROG16( 0xFFDE ) ); // vector address verified
		PUSH16( ret_addr );
		GET_PSW( temp );
		psw = (psw | b10) & ~i04;
		PUSH( temp );
		goto next_instr;
	}
	
	case 0x4F:{// PCALL offset
		int ret_addr = GET_PC() + 1;
		SET_PC( 0xFF00 | data );
		PUSH16( ret_addr );
		goto next_instr;
	}
	
	case 0x01: // TCALL n
	case 0x11:
	case 0x21:
	case 0x31:
	case 0x41:
	case 0x51:
	case 0x61:
	case 0x71:
	case 0x81:
	case 0x91:
	case 0xA1:
	case 0xB1:
	case 0xC1:
	case 0xD1:
	case 0xE1:
	case 0xF1: {
		int ret_addr = GET_PC();
		SET_PC( READ_PROG16( 0xFFDE - (opcode >> 3) ) );
		PUSH16( ret_addr );
		goto next_instr;
	}
	
// 14. STACK OPERATION COMMANDS

	{
		int temp;
	case 0x7F: // RET1
		temp = *sp;
		SET_PC( GET_LE16( sp + 1 ) );
		sp += 3;
		goto set_psw;
	case 0x8E: // POP PSW
		POP( temp );
	set_psw:
		SET_PSW( temp );
		goto next_instr;
	}
	
	case 0x0D: { // PUSH PSW
		int temp;
		GET_PSW( temp );
		PUSH( temp );
		goto next_instr;
	}

	case 0x2D: // PUSH A
		PUSH( a );
		goto next_instr;
	
	case 0x4D: // PUSH X
		PUSH( x );
		goto next_instr;
	
	case 0x6D: // PUSH Y
		PUSH( y );
		goto next_instr;
	
	case 0xAE: // POP A
		POP( a );
		goto next_instr;
	
	case 0xCE: // POP X
		POP( x );
		goto next_instr;
	
	case 0xEE: // POP Y
		POP( y );
		goto next_instr;
	
// 15. BIT OPERATION COMMANDS

	case 0x02: // SET1
	case 0x22:
	case 0x42:
	case 0x62:
	case 0x82:
	case 0xA2:
	case 0xC2:
	case 0xE2:
	case 0x12: // CLR1
	case 0x32:
	case 0x52:
	case 0x72:
	case 0x92:
	case 0xB2:
	case 0xD2:
	case 0xF2: {
		int bit = 1 << (opcode >> 5);
		int mask = ~bit;
		if ( opcode & 0x10 )
			bit = 0;
		data += dp;
		WRITE( 0, data, (READ( -1, data ) & mask) | bit );
		goto inc_pc_loop;
	}
		
	case 0x0E: // TSET1 abs
	case 0x4E: // TCLR1 abs
		data = READ_PC16( pc );
		pc += 2;
		{
			unsigned temp = READ( -2, data );
			nz = BYTE( a - temp );
			temp &= ~a;
			if ( opcode == 0x0E )
				temp |= a;
			WRITE( 0, data, temp );
		}
		goto next_instr;
	
	case 0x4A: // AND1 C,mem.bit
		c &= MEM_BIT( 0 );
		pc += 2;
		goto next_instr;
	
	case 0x6A: // AND1 C,/mem.bit
		c &= ~MEM_BIT( 0 );
		pc += 2;
		goto next_instr;
	
	case 0x0A: // OR1 C,mem.bit
		c |= MEM_BIT( -1 );
		pc += 2;
		goto next_instr;
	
	case 0x2A: // OR1 C,/mem.bit
		c |= ~MEM_BIT( -1 );
		pc += 2;
		goto next_instr;
	
	case 0x8A: // EOR1 C,mem.bit
		c ^= MEM_BIT( -1 );
		pc += 2;
		goto next_instr;
	
	case 0xEA: // NOT1 mem.bit
		data = READ_PC16( pc );
		pc += 2;
		{
			unsigned temp = READ( -1, data & 0x1FFF );
			temp ^= 1 << (data >> 13);
			WRITE( 0, data & 0x1FFF, temp );
		}
		goto next_instr;
	
	case 0xCA: // MOV1 mem.bit,C
		data = READ_PC16( pc );
		pc += 2;
		{
			unsigned temp = READ( -2, data & 0x1FFF );
			unsigned bit = data >> 13;
			temp = (temp & ~(1 << bit)) | ((c >> 8 & 1) << bit);
			WRITE( 0, data & 0x1FFF, temp + no_read_before_write  );
		}
		goto next_instr;
	
	case 0xAA: // MOV1 C,mem.bit
		c = MEM_BIT( 0 );
		pc += 2;
		goto next_instr;
	
// 16. PROGRAM PSW FLAG OPERATION COMMANDS

	case 0x60: // CLRC
		c = 0;
		goto next_instr;
		
	case 0x80: // SETC
		c = ~0;
		goto next_instr;
	
	case 0xED: // NOTC
		c ^= 0x100;
		goto next_instr;
		
	case 0xE0: // CLRV
		psw &= ~(v40 | h08);
		goto next_instr;
	
	case 0x20: // CLRP
		dp = 0;
		goto next_instr;
	
	case 0x40: // SETP
		dp = 0x100;
		goto next_instr;
	
	case 0xA0: // EI
		SUSPICIOUS_OPCODE( "EI" );
		psw |= i04;
		goto next_instr;
	
	case 0xC0: // DI
		SUSPICIOUS_OPCODE( "DI" );
		psw &= ~i04;
		goto next_instr;
	
// 17. OTHER COMMANDS

	case 0x00: // NOP
		goto next_instr;
	
	case 0xFF:{// STOP
		// handle PC wrap-around
		unsigned addr = GET_PC() - 1;
		if ( addr >= 0x10000 )
		{
			addr &= 0xFFFF;
			SET_PC( addr );
			if ( !SPC_MORE_ACCURACY )
				dprintf( "SPC: PC wrapped around\n" );
			goto next_instr;
		}
	}
	// fall through
	case 0xEF: // SLEEP
		SUSPICIOUS_OPCODE( "STOP/SLEEP" );
		--pc;
		
		// consume rest of available time
		rel_time = 0;
		
		cpu_error = "SPC emulation error";
		goto stop;
	} // switch
	
	assert( 0 ); // catch any unhandled instructions
}   
stop:
	
	// Uncache registers
	if ( GET_PC() >= 0x10000 && !SPC_MORE_ACCURACY )
		dprintf( "SPC: PC wrapped around\n" );
	
	cpu_regs.pc = ((unsigned) GET_PC()) & 0xFFFF;
	cpu_regs.sp = BYTE( GET_SP() );
	cpu_regs.a  = BYTE( a );
	cpu_regs.x  = BYTE( x );
	cpu_regs.y  = BYTE( y );
	GET_PSW( cpu_regs.psw );
}
