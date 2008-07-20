const uint8_t SPC7110Codec::EvolutionTable[53][4] = {
	//prob, nextlps, nextmps, toggle invert
	{0x5a,  1,  1,1}, //0  l,m 
	{0x25,  6,  2,0}, //1  l,m 
	{0x11,  8,  3,0}, //2  l,m 
	{0x08, 10,  4,0}, //3   ,m 
	{0x03, 12,  5,0}, //4   ,m 
	{0x01, 15,  5,0}, //5   ,m 

	{0x5a,  7,  7,1}, //6  l, 
	{0x3f, 19,  8,0}, //7  l,m 
	{0x2c, 21,  9,0}, //8  l,m 
	{0x20, 22, 10,0}, //9   ,m 
	{0x17, 23, 11,0}, //10  ,m 
	{0x11, 25, 12,0}, //11  ,m 
	{0x0c, 26, 13,0}, //12  ,m 
	{0x09, 28, 14,0}, //13  ,m 
	{0x07, 29, 15,0}, //14  ,m 
	{0x05, 31, 16,0}, //15  ,m 
	{0x04, 32, 17,0}, //16  ,m 
	{0x03, 34, 18,0}, //17  ,m 
	{0x02, 35, 5,0},  //18  ,m 

	{0x5a, 20, 20,1}, //19 l,m 
	{0x48, 39, 21,0}, //20 l,m 
	{0x3a, 40, 22,0}, //21 l,m 
	{0x2e, 42, 23,0}, //22 l,m 
	{0x26, 44, 24,0}, //23 l,m 
	{0x1f, 45, 25,0}, //24 l,m 
	{0x19, 46, 26,0}, //25 l,m 
	{0x15, 25, 27,0}, //26 l,m 
	{0x11, 26, 28,0}, //27 l,m 
	{0x0e, 26, 29,0}, //28 l,m 
	{0x0b, 27, 30,0}, //29  ,m 
	{0x09, 28, 31,0}, //30  ,m 
	{0x08, 29, 32,0}, //31 l,m 
	{0x07, 30, 33,0}, //32 l,m 
	{0x05, 31, 34,0}, //33 l,m  <--- changed lps 
	{0x04, 33, 35,0}, //34  ,m ... this is NOT skipped 
	{0x04, 33, 36,0}, //35  ,m 
	{0x03, 34, 37,0}, //36  ,m 
	{0x02, 35, 38,0}, //37  ,m ... this is NOT skipped 
	{0x02, 36,  5,0}, //38  ,m 

	{0x58, 39, 40,1}, //39 l,m 
	{0x4d, 47, 41,0}, //40 l,m 
	{0x43, 48, 42,0}, //41  ,m 
	{0x3b, 49, 43,0}, //42  ,m 
	{0x34, 50, 44,0}, //43 l,m 
	{0x2e, 51, 45,0}, //44 l,m 
	{0x29, 44, 46,0}, //45 l,m 
	{0x25, 45, 24,0}, //46  ,m 

	{0x56, 47, 48,1}, //47 l,m 
	{0x4f, 47, 49,0}, //48 l,m 
	{0x47, 48, 50,0}, //49 l,m 
	{0x41, 49, 51,0}, //50 l,m 
	{0x3c, 50, 52,0}, //51 l,m 
	{0x37, 51, 43,0}  //52  ,m 
};

const uint8_t SPC7110Codec::Mode2ContextTable[32][4] = {
	// "bit" = (lps^invert)
	//next_0, use ref pixel, next_1, use ref pixel
	// if use ref pixel, then add on the 0-4 bell number grouping
	{1, 0, 2, 0}, //0

	{3, 1, 8, 1}, //1  prev bit 0
	{13,0, 14,0}, //2  prev bit 1

	{15,0, 16,0}, //3  prev bit 00
	{17,0, 18,0}, //4
	{19,0, 20,0}, //5
	{21,0, 22,0}, //6
	{23,0, 24,0}, //7
	{25,0, 26,0}, //8  prev bit 01
	{25,0, 26,0}, //9
	{25,0, 26,0}, //10
	{25,0, 26,0}, //11
	{25,0, 26,0}, //12
	{27,0, 28,0}, //13 prev bit 10
	{29,0, 30,0}, //14 prev bit 11

	{31,0, 31,0}, //15  000 ref group 0
	{31,0, 31,0}, //16  001 ref group 0
	{31,0, 31,0}, //17  000 ref group 1
	{31,0, 31,0}, //18  001 ref group 1
	{31,0, 31,0}, //19  000 ref group 2
	{31,0, 31,0}, //20  001 ref group 2
	{31,0, 31,0}, //21  000 ref group 3
	{31,0, 31,0}, //22  001 ref group 3
	{31,0, 31,0}, //23  000 ref group 4
	{31,0, 31,0}, //24  001 ref group 4
	{31,0, 31,0}, //25  010
	{31,0, 31,0}, //26  011
	{31,0, 31,0}, //27  100
	{31,0, 31,0}, //28  101
	{31,0, 31,0}, //29  110
	{31,0, 31,0}, //30  111

	{31,0, 31,0} //31 -- used as a trap for testing purposes --
};

#define PROB(x) EvolutionTable[Contexts[x].index][0]
#define NEXT_LPS(x) EvolutionTable[Contexts[x].index][1]
#define NEXT_MPS(x) EvolutionTable[Contexts[x].index][2]
#define TOGGLE_INVERT(x) EvolutionTable[Contexts[x].index][3]
#define BIT(x,y) ((x>>y)&1)

void SPC7110Codec::decomp_mode0(int len) {
    uint8_t *datain  = buffer;
    uint8_t *dataout = output;
    static const unsigned NUM_CONTEXTS = 30;

	uint8 top,val;
	uint8 con,mps,prob;
	uint8 flag_lps,shift,mask;

	int out=0;
	int inverts=0;
	int lps=0;

	unsigned char in;
	int in_count;

	int i,bit;

	//setup 
	top=0xFF;

	val=*datain;
	datain++;

	in=*datain;
	datain++;
	in_count=8;

	//reset context states
	for(i=0;i<NUM_CONTEXTS;i++)
	{
		Contexts[i].index=0;
		Contexts[i].invert=0;
	}

	for(i=0;i<len;i++)
	{
		if(i==-1800)
		{
			int k;
			printf("\nEvolution table:\n");
			//for(k=0;k<53;k++)
				//printf("  %d,%d //%d\n",SeenEvolution[k][0],SeenEvolution[k][1],k);
		}

		
		for(bit=0;bit<8;bit++)
		{
			//get context
			mask = (1<<(bit&3)) - 1;
			con = mask + ((inverts&mask)^(lps&mask));
			if(bit>3)
				con+=15;
			
			//get PROB and MPS
			prob = PROB(con);
			mps  = (BIT(out,15) ^ Contexts[con].invert);

			if(i>=15 && i<=18 && 0)
				printf("byte %d bit %d: val=%.2X top=%.2X prob=%.2X mps=%d   con=%d state=%d\n",
					i,bit,val,top,prob,mps,con,Contexts[con].index); 

			//get bit
			if (val <= top-prob)
			{
				//mps
				top = top - prob;
				out = (out << 1) + mps;

				flag_lps=0;
			}
			else
			{
				//lps
				val = val - (top - (prob - 1));
				top = prob - 1; 
				out = (out << 1) + 1-mps;
			
				flag_lps=1;
			}

			// renormalize
			shift=0;
			while(top<0x7F) // NOTE: not 0x80, it's a strange border case 
			{
				shift++;

				top = (top<<1)+1;
				val = (val<<1)+(in>>7);

				in = (in<<1);
				if(--in_count==0)
				{
					in=*datain;
					datain++;
					in_count=8;
				}
			}

			//update processing info
			lps = (lps<<1) + flag_lps;
			inverts  = (inverts<<1) + Contexts[con].invert;

			//update context state
			if(flag_lps & TOGGLE_INVERT(con))
				Contexts[con].invert ^= 1;

			if(flag_lps)
			{
				//SeenEvolution[Contexts[con].index][0]=1;
				Contexts[con].index = NEXT_LPS(con);
			}
			else if(shift)
			{
				//SeenEvolution[Contexts[con].index][1]=1;
				Contexts[con].index = NEXT_MPS(con);
			}
		}

		//save byte
		*dataout = (out & 0xFF);
		dataout++;
	}
}

void SPC7110Codec::decomp_mode1(int len) {
    uint8_t *datain  = buffer;
    uint8_t *dataout = output;
    static const unsigned NUM_CONTEXTS = 15;

	int pixelorder[4]={0,1,2,3};
	int realorder[4];
	int a,b,c;
	int m,n;

	uint8 top,val;
	uint8 con,prob;
	uint8 flag_lps,shift;

	int out=0;
	int inverts=0;
	int lps=0;

	unsigned char in;
	int in_count;
	int in_len=0;

	int i,j,pixel;

	//setup 
	top=0xFF;

	val=datain[in_len++];

	in=datain[in_len++];
	in_count=8;

	//reset context states
	for(i=0;i<NUM_CONTEXTS;i++)
	{
		Contexts[i].index=0;
		Contexts[i].invert=0;
	}

	for(i=0;i<len;i+=2)
	{
		if(i!=0)
		{
			//turn pixel data into bitplanes
			//and save as output
			*dataout = (BIT(out,15)<<7) + (BIT(out,13)<<6) + (BIT(out,11)<<5) + (BIT(out,9)<<4)
					+ (BIT(out,7)<<3) + (BIT(out,5)<<2) + (BIT(out,3)<<1) + BIT(out,1);
			dataout++;
			*dataout = (BIT(out,14)<<7) + (BIT(out,12)<<6) + (BIT(out,10)<<5) + (BIT(out,8)<<4)
					+ (BIT(out,6)<<3) + (BIT(out,4)<<2) + (BIT(out,2)<<1) + BIT(out,0);
			dataout++;
		}
		
		for(pixel=0;pixel<8;pixel++)
		{
			//get first symbol context
			a = ((out >> (1*2)) & 0x3);
			b = ((out >> (7*2)) & 0x3);
			c = ((out >> (8*2)) & 0x3);
			if(a==b && b==c)
				con=0;
			else if (a==b && b!=c)
				con=1;
			else if (a!=b && b==c)
				con=2;
			else if (a==c && b!=c)
				con=3;
			else
				con=4;

			//update pixel order
			for(m=0;m<4;m++)
				if(pixelorder[m]==a)
					break;
			for(n=m;n>0;n--)
			{
				j=pixelorder[n-1];
				pixelorder[n-1]=pixelorder[n];
				pixelorder[n]=j;
			}


			//get PROB
			prob = PROB(con);

			//get symbol
			if (val <= top-prob)
			{
				//mps
				top = top - prob;
				flag_lps=0;
			}
			else
			{
				//lps
				val = val - (top - (prob - 1));
				top = prob - 1; 
				flag_lps=1;
			}

			// renormalize
			shift=0;
			while(top<0x7F)
			{
				shift++;

				top = (top<<1)+1;
				val = (val<<1)+(in>>7);

				in = (in<<1);
				if(--in_count==0)
				{
					in=datain[in_len++];
					in_count=8;
				}
			}

			//update processing info
			lps = (lps<<1) + flag_lps;
			inverts  = (inverts<<1) + Contexts[con].invert;

			//update context state
			if(flag_lps & TOGGLE_INVERT(con))
				Contexts[con].invert ^= 1;
			if(flag_lps)
				Contexts[con].index = NEXT_LPS(con);
			else if(shift)
				Contexts[con].index = NEXT_MPS(con);

			//get context of second symbol
			con = 5 + con*2 + ((lps^inverts)&1);

			//get PROB
			prob = PROB(con);

			//get symbol
			if (val <= top-prob)
			{
				//mps
				top = top - prob;
				flag_lps=0;
			}
			else
			{
				//lps
				val = val - (top - (prob - 1));
				top = prob - 1; 
				flag_lps=1;
			}

			// renormalize
			shift=0;
			while(top<0x7F)
			{
				shift++;

				top = (top<<1)+1;
				val = (val<<1)+(in>>7);

				in = (in<<1);
				if(--in_count==0)
				{
					in=datain[in_len++];
					in_count=8;
				}
			}

			//calculate the real pixel order
			for(m=0;m<4;m++)
				realorder[m]=pixelorder[m];
			//shift refence pixel c value to top
			for(m=0;m<4;m++)
				if(realorder[m]==c)
					break;
			for(n=m;n>0;n--)
			{
				j=realorder[n-1];
				realorder[n-1]=realorder[n];
				realorder[n]=j;
			}
			//shift refence pixel b value to top
			for(m=0;m<4;m++)
				if(realorder[m]==b)
					break;
			for(n=m;n>0;n--)
			{
				j=realorder[n-1];
				realorder[n-1]=realorder[n];
				realorder[n]=j;
			}
			//shift refence pixel a value to top
			for(m=0;m<4;m++)
				if(realorder[m]==a)
					break;
			for(n=m;n>0;n--)
			{
				j=realorder[n-1];
				realorder[n-1]=realorder[n];
				realorder[n]=j;
			}

			
			//update processing info
			lps = (lps<<1) + flag_lps;
			inverts  = (inverts<<1) + Contexts[con].invert;

			//update context state
			if(flag_lps & TOGGLE_INVERT(con))
				Contexts[con].invert ^= 1;
			if(flag_lps)
				Contexts[con].index = NEXT_LPS(con);
			else if(shift)
				Contexts[con].index = NEXT_MPS(con);

			//get pixel
			b=realorder[(lps^inverts)&3];
			out = (out<<2) + b;
		}
	}

	//turn pixel data into bitplanes
	//and save as output.. BUT don't save second byte unless asked to
	*dataout = (BIT(out,15)<<7) + (BIT(out,13)<<6) + (BIT(out,11)<<5) + (BIT(out,9)<<4)
			+ (BIT(out,7)<<3) + (BIT(out,5)<<2) + (BIT(out,3)<<1) + BIT(out,1);
	dataout++;
	if((len&1)==0)
	{
		*dataout = (BIT(out,14)<<7) + (BIT(out,12)<<6) + (BIT(out,10)<<5) + (BIT(out,8)<<4)
				+ (BIT(out,6)<<3) + (BIT(out,4)<<2) + (BIT(out,2)<<1) + BIT(out,0);
		dataout++;
	}

	if(in_count==8)
		in_len--;
	//printf("Used %d bytes of input.\n",in_len);
	//return in_len;
}

void SPC7110Codec::decomp_mode2(int len) {
    uint8_t *datain  = buffer;
    uint8_t *dataout = output;
    static const unsigned NUM_CONTEXTS = 32;

	int pixelorder[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	int realorder[16];
	int a,b,c;
	int m,n;

	uint8 bitplanebuffer[16];
	uint8 buf_idx=0;

	uint8 top,val,prob;
	uint8 con,refcon;
	uint8 flag_lps,invertbit,shift;

	int out=0;
	int out2=0;
	int inverts=0;
	int lps=0;

	unsigned char in;
	int in_count;
	int in_len=0;

	int i,j,pixel,bit;

	//setup 
	top=0xFF;

	val=datain[in_len++];

	in=datain[in_len++];
	in_count=8;

	//reset context states
	for(i=0;i<NUM_CONTEXTS;i++)
	{
		Contexts[i].index=0;
		Contexts[i].invert=0;
	}

	for(i=0;i<len;i+=2)
	{
		for(pixel=0;pixel<8;pixel++)
		{
			//get first symbol context
			a = ((out >> (0*4)) & 0x0F);
			b = ((out >> (7*4)) & 0x0F);
			c = ((out2>> (0*4)) & 0x0F);
			if(a==b && b==c)
				refcon=0;
			else if (a==b && b!=c)
				refcon=1;
			else if (a!=b && b==c)
				refcon=2;
			else if (a==c && b!=c)
				refcon=3;
			else
				refcon=4;

			con=0;

			//update pixel order
			for(m=0;m<16;m++)
				if(pixelorder[m]==a)
					break;
			for(n=m;n>0;n--)
			{
				j=pixelorder[n-1];
				pixelorder[n-1]=pixelorder[n];
				pixelorder[n]=j;
			}

			//calculate the real pixel order
			for(m=0;m<16;m++)
				realorder[m]=pixelorder[m];
			//shift refence pixel c value to top
			for(m=0;m<16;m++)
				if(realorder[m]==c)
					break;
			for(n=m;n>0;n--)
			{
				j=realorder[n-1];
				realorder[n-1]=realorder[n];
				realorder[n]=j;
			}
			//shift refence pixel b value to top
			for(m=0;m<16;m++)
				if(realorder[m]==b)
					break;
			for(n=m;n>0;n--)
			{
				j=realorder[n-1];
				realorder[n-1]=realorder[n];
				realorder[n]=j;
			}
			//shift refence pixel a value to top
			for(m=0;m<16;m++)
				if(realorder[m]==a)
					break;
			for(n=m;n>0;n--)
			{
				j=realorder[n-1];
				realorder[n-1]=realorder[n];
				realorder[n]=j;
			}

			//get 4 symbols
			for(bit=0;bit<4;bit++)
			{
				//get PROB
				prob = PROB(con);

				//get symbol
				if (val <= top-prob)
				{
					//mps
					top = top - prob;
					flag_lps=0;
				}
				else
				{
					//lps
					val = val - (top - (prob - 1));
					top = prob - 1; 
					flag_lps=1;
				}

				// renormalize
				shift=0;
				while(top<0x7F)
				{
					shift++;
	
					top = (top<<1)+1;
					val = (val<<1)+(in>>7);

					in = (in<<1);
					if(--in_count==0)
					{
						in=datain[in_len++];
						in_count=8;
					}
				}

				//update processing info
				lps = (lps<<1) + flag_lps;
				invertbit = Contexts[con].invert;
				inverts  = (inverts<<1) + Contexts[con].invert;

				//update context state
				if(flag_lps & TOGGLE_INVERT(con))
					Contexts[con].invert ^= 1;
				if(flag_lps)
					Contexts[con].index = NEXT_LPS(con);
				else if(shift)
					Contexts[con].index = NEXT_MPS(con);

				//get next context
				if(Mode2ContextTable[con][2*(flag_lps^invertbit)+1])
					con=Mode2ContextTable[con][2*(flag_lps^invertbit)]+refcon;		
				else
					con=Mode2ContextTable[con][2*(flag_lps^invertbit)];		
			}

			//get pixel
			b=realorder[(lps^inverts)&0x0F];
			out2 = (out2<<4) + ((out>>28)&0x0F);
			out = (out<<4) + b;
		}

		//cludge to convert pixel data into bitplanes and respect len parameter for output buf
		*dataout = (BIT(out,31)<<7) + (BIT(out,27)<<6) + (BIT(out,23)<<5) + (BIT(out,19)<<4)
				+ (BIT(out,15)<<3) + (BIT(out,11)<<2) + (BIT(out,7)<<1) + BIT(out,3);
		dataout++;

		if((i+1)<len)
		{
			*dataout = (BIT(out,30)<<7) + (BIT(out,26)<<6) + (BIT(out,22)<<5) + (BIT(out,18)<<4)
				+ (BIT(out,14)<<3) + (BIT(out,10)<<2) + (BIT(out,6)<<1) + BIT(out,2);
			dataout++;
		}

		bitplanebuffer[buf_idx++] =
				(BIT(out,29)<<7) + (BIT(out,25)<<6) + (BIT(out,21)<<5) + (BIT(out,17)<<4)
				+ (BIT(out,13)<<3) + (BIT(out,9)<<2) + (BIT(out,5)<<1) + BIT(out,1);
		bitplanebuffer[buf_idx++] =
				(BIT(out,28)<<7) + (BIT(out,24)<<6) + (BIT(out,20)<<5) + (BIT(out,16)<<4)
				+ (BIT(out,12)<<3) + (BIT(out,8)<<2) + (BIT(out,4)<<1) + BIT(out,0);

		if(buf_idx==16)
		{
			for(m=0;m<16 && i+2<len;m++,i++)
			{
				*dataout = bitplanebuffer[m];
				dataout++;
			}

			buf_idx=0;
		}
	}

	if(in_count==8)
		in_len--;
	//printf("Used %d bytes of input.\n",in_len);
	//return in_len;
}

#undef PROB
#undef NEXT_LPS
#undef NEXT_MPS
#undef TOGGLE_INVERT
#undef BIT

SPC7110Codec::SPC7110Codec() {
  buffer = new(zeromemory) uint8_t[65536];
  output = new(zeromemory) uint8_t[65536];
}

SPC7110Codec::~SPC7110Codec() {
  delete[] buffer;
  delete[] output;
}
