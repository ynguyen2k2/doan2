
#define LAT       GPIO_PIN_12  //E
#define OE        GPIO_PIN_15  //E pwm PB11
#define CLK        GPIO_PIN_9 //E

#define A    7       //E
#define B    10      //E
#define C    8      //E
#define D    11     //E

#define B2        4 //A   0x10
#define R2        2 //A		0x04
#define G2        7 //A		0x80

#define B1        3 //A   0x08
#define R1        1 //A   0x01
#define G1        5 //A   0x20

#define Control_P 	GPIOE
#define A_SSET 0x00000001<<A
#define B_SSET 0x00000001<<B
#define C_SSET 0x00000001<<C
#define D_SSET 0x00000001<<D
#define A_RSET 0x00000001<<(A+16)
#define B_RSET 0x00000001<<(B+16)
#define C_RSET 0x00000001<<(C+16)
#define D_RSET 0x00000001<<(D+16)
unsigned char bufferGPIO[8][16][128]; // 5 bit pwwm  16 hang quet 128 pixel
unsigned char BufferRGB[3][64][64]; // 3 mau 64 y 64 x
unsigned char vitri_bit=0,hang_led=0;
const unsigned char scan_data[16]=
{
	 0,8,4,12, 2,10,6,14, 1,9,5,13, 3,11,7,15
//	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
};
void scan16S(unsigned char so)
{
	switch(so)
	{
		case 0: Control_P->BSRR= A_RSET|B_RSET|C_RSET|D_RSET;  return;
		case 1: Control_P->BSRR= A_RSET|B_RSET|C_RSET|D_SSET;  return;
		case 2: Control_P->BSRR= A_RSET|B_RSET|C_SSET|D_RSET;  return;
		case 3: Control_P->BSRR= A_RSET|B_RSET|C_SSET|D_SSET;  return;
		case 4: Control_P->BSRR= A_RSET|B_SSET|C_RSET|D_RSET;  return;
		case 5: Control_P->BSRR= A_RSET|B_SSET|C_RSET|D_SSET;  return;
		case 6: Control_P->BSRR= A_RSET|B_SSET|C_SSET|D_RSET;  return;
		case 7: Control_P->BSRR= A_RSET|B_SSET|C_SSET|D_SSET;  return;
		case 8: Control_P->BSRR= A_SSET|B_RSET|C_RSET|D_RSET;  return;
		case 9: Control_P->BSRR= A_SSET|B_RSET|C_RSET|D_SSET;  return;
		case 10:Control_P->BSRR= A_SSET|B_RSET|C_SSET|D_RSET;  return;
		case 11:Control_P->BSRR= A_SSET|B_RSET|C_SSET|D_SSET;  return;
		case 12:Control_P->BSRR= A_SSET|B_SSET|C_RSET|D_RSET;  return;
		case 13:Control_P->BSRR= A_SSET|B_SSET|C_RSET|D_SSET;  return;
		case 14:Control_P->BSRR= A_SSET|B_SSET|C_SSET|D_RSET;  return;
		case 15:Control_P->BSRR= A_SSET|B_SSET|C_SSET|D_SSET;  return;
	}
}
void chotxuatmaquet(unsigned char h) {	
		
		//chon hang
		scan16S(h);
				
}
void update()
{  
 for(int hang=0;hang<16;hang++)
 {
	for(int bit_pos=0;bit_pos<5;bit_pos++)
	{
	 for(int i=0;i<64;i++)
	 {
		 bufferGPIO[bit_pos][hang][64+i]=0;
		 if((BufferRGB[1][16+hang] [i] & (1<<bit_pos)) != 0 ){bufferGPIO[bit_pos][hang][64+i]|=(0x0001<<G2);} //ghi bit G2
		 if((BufferRGB[1][0+hang]  [i] & (1<<bit_pos)) != 0 ){bufferGPIO[bit_pos][hang][64+i]|=(0x0001<<G1);} //ghi bit G1
		 if((BufferRGB[0][0+hang] [i] & (1<<bit_pos)) != 0 ){bufferGPIO[bit_pos][hang][64+i]|=(0x0001<<R1);} //ghi bit R1
		 if((BufferRGB[2][0+hang]  [i] & (1<<bit_pos)) != 0 ){bufferGPIO[bit_pos][hang][64+i]|=(0x0001<<B1);} //ghi bit B1
		 if((BufferRGB[0][16+hang] [i] & (1<<bit_pos)) != 0 ){bufferGPIO[bit_pos][hang][64+i]|=(0x0001<<R2);} //ghi bit R2
		 if((BufferRGB[2][16+hang] [i] & (1<<bit_pos)) != 0 ){bufferGPIO[bit_pos][hang][64+i]|=(0x0001<<B2);} //ghi bit B2	 
		 
		 bufferGPIO[bit_pos][hang][63-i]=0;
		 if((BufferRGB[1][47-hang] [i] & (1<<bit_pos)) != 0 ){bufferGPIO[bit_pos][hang][63-i]|=(0x0001<<G2);} //ghi bit G2
		 if((BufferRGB[1][63-hang]  [i] & (1<<bit_pos)) != 0 ){bufferGPIO[bit_pos][hang][63-i]|=(0x0001<<G1);} //ghi bit G1
		 if((BufferRGB[0][63-hang]  [i] & (1<<bit_pos)) != 0 ){bufferGPIO[bit_pos][hang][63-i]|=(0x0001<<R1);} //ghi bit R1
		 if((BufferRGB[2][63-hang]  [i] & (1<<bit_pos)) != 0 ){bufferGPIO[bit_pos][hang][63-i]|=(0x0001<<B1);} //ghi bit B1
		 if((BufferRGB[0][47-hang] [i] & (1<<bit_pos)) != 0 ){bufferGPIO[bit_pos][hang][63-i]|=(0x0001<<R2);} //ghi bit R2
		 if((BufferRGB[2][47-hang] [i] & (1<<bit_pos)) != 0 ){bufferGPIO[bit_pos][hang][63-i]|=(0x0001<<B2);} //ghi bit B2	 
	 }
	} 
 }
}

void xoabodem(void) {
	for(int hang=0;hang<16;hang++)
		for(int bit_pos=0;bit_pos<5;bit_pos++)
			for(int i=0;i<128;i++)
				bufferGPIO[bit_pos][hang][i]=0x00;
}

void xoabodem2(int hang_bat_dau, int hang_ket_thuc, int cot_bat_dau, int cot_ket_thuc) {
    for (int hang = hang_bat_dau; hang <= hang_ket_thuc; hang++) {
        for (int bit_pos = cot_bat_dau; bit_pos <= cot_ket_thuc; bit_pos++) {
            for (int i = 0; i < 127; i++) {
                bufferGPIO[bit_pos][hang][i] = 0x00;
            }
        }
    }
}

void khoitaoRGB(void) {
	for(int hang=0;hang<64;hang++)
		for(int i=0;i<64;i++)
		for(int bit_pos=0;bit_pos<3;bit_pos++)
				BufferRGB[bit_pos][hang][i]=0x00;
}


void xoa_1so(int x, int y, int size) {
    int hang, cot;
    int hang_max, cot_max;

    switch (size) {
        case 1:
            hang_max = 7;
            cot_max = 5;
            break;
        case 2:
            hang_max = 14;
            cot_max = 10;
            break;
        case 3:
            hang_max = 21;
            cot_max = 15;
            break;
        return;
    }

    for (hang = x; hang < x + hang_max; hang++) {
        for (cot = y; cot < y + cot_max; cot++) {
            for (int bit_pos = 0; bit_pos < 3; bit_pos++) {
                BufferRGB[bit_pos][hang][cot] = 0x00;
            }
        }
    }
}

void xoa_2so(int x, int y, int size) {
    int hang, cot;
    int hang_max, cot_max;

    switch (size) {
        case 1:
            hang_max = 15;
            cot_max = 11;
            break;
        case 2:
            hang_max = 29;
            cot_max = 21;
            break;
        case 3:
            hang_max = 43;
            cot_max = 31;
            break;
        return;
    }

    for (hang = x; hang < x + hang_max; hang++) {
        for (cot = y; cot < y + cot_max; cot++) {
            for (int bit_pos = 0; bit_pos < 3; bit_pos++) {
                BufferRGB[bit_pos][hang][cot] = 0x00;
            }
        }
    }
}



void matrix_vitri(unsigned char x , unsigned char y ){
}
void matrix_pixel_xy(unsigned char vitri_x, unsigned char vitri_y, unsigned char r, unsigned char g, unsigned char b ) {
	unsigned char _x = vitri_x,_y = vitri_y;
	if(_x > 63 || _y > 63) return ;
		BufferRGB[0][_y][_x] = r;
		BufferRGB[1][_y][_x] = g;
		BufferRGB[2][_y][_x] = b;
}

void setBrightness(int brightness) {
    for (int hang = 0; hang < 64; hang++) {
        for (int cot = 0; cot < 64; cot++) {          
            BufferRGB[0][hang][cot] = (BufferRGB[0][hang][cot] * brightness) / 255;  
            BufferRGB[1][hang][cot] = (BufferRGB[1][hang][cot] * brightness) / 255;         
            BufferRGB[2][hang][cot] = (BufferRGB[2][hang][cot] * brightness) / 255;
        }
    }
   // update();
}