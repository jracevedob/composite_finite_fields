/******************************************************************************
*
* Copyright (C) 2021 TUD - Telekom Chair for Communication Networks
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited to applications:
* (a) running on any x86 architecture
* (b) GCC compiler v7.5.0
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the TUD shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from TUD.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file	main.c
*
* @note
* This example illustrates the arithmetic calculation of composite finite
* fields using GF(2) as basis field for the composition of fields of higher
* order.
*
* MODIFICATION HISTORY:
* <pre>
* Ver   Who    Date     Changes
* ----- ------ -------- -------------------------------------------------
* 1.0   acv    12/05/21 First Release
*
* </pre>
******************************************************************************/

/***************************** Include Files *********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/************************** Data Types Definitions ***************************/
typedef u_int8_t uint8_t;

static int prim_poly[33] = 
{ 0, 
/*  1 */     1, 
/*  2 */    07,
/*  3 */    013,
/*  4 */    023,
/*  5 */    045,
/*  6 */    0103,
/*  7 */    0211,
/*  8 */    0435,
/*  9 */    01021,
/* 10 */    02011,
/* 11 */    04005,
/* 12 */    010123,
/* 13 */    020033,
/* 14 */    042103,
/* 15 */    0100003,
/* 16 */    0210013,
/* 17 */    0400011,
/* 18 */    01000201,
/* 19 */    02000047,
/* 20 */    04000011,
/* 21 */    010000005,
/* 22 */    020000003,
/* 23 */    040000041,
/* 24 */    0100000207,
/* 25 */    0200000011,
/* 26 */    0400000107,
/* 27 */    01000000047,
/* 28 */    02000000011,
/* 29 */    04000000005,
/* 30 */    010040000007,
/* 31 */    020000000011, 
/* 32 */    00020000007 };  


/*Fill table with values in the range 2^n */
static int nw[33] = { 0, (1 << 1), (1 << 2), (1 << 3), (1 << 4), 
(1 << 5), (1 << 6), (1 << 7), (1 << 8), (1 << 9), (1 << 10),
(1 << 11), (1 << 12), (1 << 13), (1 << 14), (1 << 15), (1 << 16),
(1 << 17), (1 << 18), (1 << 19), (1 << 20), (1 << 21), (1 << 22),
(1 << 23), (1 << 24), (1 << 25), (1 << 26), (1 << 27), (1 << 28),
(1 << 29), (1 << 30), (1 << 31), -1 };

/*Fill table with values in the range (2^n)-1 */
static int nwm1[33] = { 0, (1 << 1)-1, (1 << 2)-1, (1 << 3)-1, (1 << 4)-1, 
(1 << 5)-1, (1 << 6)-1, (1 << 7)-1, (1 << 8)-1, (1 << 9)-1, (1 << 10)-1,
(1 << 11)-1, (1 << 12)-1, (1 << 13)-1, (1 << 14)-1, (1 << 15)-1, (1 << 16)-1,
(1 << 17)-1, (1 << 18)-1, (1 << 19)-1, (1 << 20)-1, (1 << 21)-1, (1 << 22)-1,
(1 << 23)-1, (1 << 24)-1, (1 << 25)-1, (1 << 26)-1, (1 << 27)-1, (1 << 28)-1,
(1 << 29)-1, (1 << 30)-1, 0x7fffffff, 0xffffffff };
   


/************************** Function Definitions ******************************/

void composite_addition_full_table()
{
	FILE *table_add;
	int i,j;

	if(table_add == NULL){
		fprintf(stderr, "Failed to create file to store computation \n");
	}else{
	table_add=fopen("table_addition.csv", "w");
	for(i=0 ; i<256 ; i++){
		for(j=0 ; j<256 ; j++){
			fprintf(table_add,"%d, ", i^j);				
		}
		fprintf(table_add,"\n");
	}
	fclose(table_add);
	}	
}

uint8_t composite_addition_single_values(uint8_t  x, uint8_t y)
{
	/*XOR operation*/
	printf("Addition result: %d\n", x^y);
	//return x^y;
}




uint8_t composite_mult_gf2_2(uint8_t x, uint8_t y, uint8_t w){
	
	uint8_t prod;
	uint8_t i, j, ind, k;
	uint8_t aux_vec[33];

	prod = 0;

	for (i = 0; i < w; i++) {
	    	aux_vec[i] = y;
	    	if (y & (1 << (w-1))) {
	      		y = y << 1;
	      		y = (y ^ prim_poly[w]) & nwm1[w];
	    	} else { y = y << 1;}
	 }

	 for (i = 0; i < w; i++) {
	    	ind = (1 << i);
	    	if (ind & x) {
	      		j = 1;
	      		for (k = 0; k < w; k++) {
				prod = prod ^ (j & aux_vec[i]);
				j = (j << 1);
	      		}
	    	}
	  }

	return prod;
}

uint8_t composite_mult_gf2_4(uint8_t x, uint8_t y, uint8_t w){

	/*Start masking*/
	uint8_t irreducible_pol_2_4 = 0b00010110;
	uint8_t aux_irreducible_pol;
	uint8_t aux_irreducible_pol3;
	uint8_t aux_irreducible_pol2;
	uint8_t aux_irreducible_pol1;
	uint8_t prod3, prod2, prod1;

	uint8_t x_2lsb, x_2msb, y_2lsb, y_2msb;
	

	if((x*y) < 16){
		return x*y;
	} else {
		/*Bit selection*/
		x_2lsb = x & 0x03;
		x_2msb = x >> 2;
		y_2lsb = y & 0x03;
		y_2msb = y >> 2;

		//printf("%d %d %d %d \n", x_2msb, x_2lsb, y_2msb, y_2lsb);


		/*Polynomial multiplication*/
		aux_irreducible_pol3 = composite_mult_gf2_2(x_2msb, y_2msb,2);
		aux_irreducible_pol2 = composite_addition_single_values(composite_mult_gf2_2(x_2msb, y_2lsb,2),composite_mult_gf2_2(x_2lsb, y_2msb,2));
		aux_irreducible_pol1 = composite_mult_gf2_2(x_2lsb, y_2lsb,2);
		
		aux_irreducible_pol = (aux_irreducible_pol3<<4)+ (aux_irreducible_pol2<<2)+(aux_irreducible_pol1);
		
		//printf("%d %d %d %d\n",aux_irreducible_pol3, aux_irreducible_pol2, aux_irreducible_pol1, aux_irreducible_pol);
		if (aux_irreducible_pol3 != ((irreducible_pol_2_4>>4) & 0x3)){
			/*Transformation of irreducible polynomial*/
			aux_irreducible_pol3 = composite_mult_gf2_2((aux_irreducible_pol3), ((irreducible_pol_2_4 >> 4)& 0x03),2);
			aux_irreducible_pol2 = composite_mult_gf2_2((aux_irreducible_pol3), ((irreducible_pol_2_4 >> 2)& 0x03),2);
			aux_irreducible_pol1 = composite_mult_gf2_2((aux_irreducible_pol3), (irreducible_pol_2_4 & 0x03),2);	

			/*Comparison XOR between polynomials */
			prod3 = (((aux_irreducible_pol >> 4) & 0x03)^aux_irreducible_pol3);
			prod2 = (((aux_irreducible_pol >> 2) & 0x03)^aux_irreducible_pol2);
			prod1 = ((aux_irreducible_pol & 0x03)^aux_irreducible_pol1);

			
			//printf("The result is: %d\n", (prod3<<4)+(prod2<<2)+prod1);
			//printf("%d %d %d\n",prod3, prod2, prod1);
			//printf("%d %d %d\n",((irreducible_pol_2_4 >> 4)& 0x03), ((irreducible_pol_2_4 >> 2)& 0x03), (irreducible_pol_2_4 & 0x03));
			//printf("%d %d %d\n",aux_irreducible_pol3, aux_irreducible_pol2, aux_irreducible_pol1);
		} else {
			
			prod3 = (((irreducible_pol_2_4 >> 4) & 0x03)^aux_irreducible_pol3);
			prod2 = (((irreducible_pol_2_4 >> 2) & 0x03)^aux_irreducible_pol2);
			prod1 = ((irreducible_pol_2_4 & 0x03)^aux_irreducible_pol1);
			
			//printf("The result is: %d\n", 4*prod2+prod1);
			//printf("%d %d %d\n",prod3, prod2, prod1);
			//printf("%d %d %d\n",(irreducible_pol_2_4 >> 4), ((irreducible_pol_2_4>>2) & 0x03), (irreducible_pol_2_4 & 0x03));
			//printf("%d %d %d\n",aux_irreducible_pol3, aux_irreducible_pol2, aux_irreducible_pol1);
	
		}


		return (prod2<<2)+prod1;
		
	}
}

uint8_t composite_mult_gf2_8(uint8_t x, uint8_t y, uint8_t w){


	unsigned short irreducible_pol_2_8 = 0x0119;
	/*Start masking*/
	
	unsigned short aux_irreducible_pol;
	unsigned short aux_irreducible_pol3;
	unsigned short aux_irreducible_pol2;
	unsigned short aux_irreducible_pol1;
	unsigned short prod3, prod2, prod1;

	uint8_t x_4lsb, x_4msb, y_4lsb, y_4msb;
	

	if((x*y) < 256){
		return x*y;
	} else {
		/*Bit selection*/
		x_4lsb = x & 0x0f;
		x_4msb = x >> 4;
		y_4lsb = y & 0x0f;
		y_4msb = y >> 4;

		//printf("%d %d %d %d \n", x_4msb, x_4lsb, y_4msb, y_4lsb);
		//printf("%d %d %d %d\n",((irreducible_pol_2_8 >> 8)&0x0f), ((irreducible_pol_2_8 >> 4)&0x0f), (irreducible_pol_2_8 & 0x0f), irreducible_pol_2_8);

		/*Polynomial multiplication*/
		aux_irreducible_pol3 = composite_mult_gf2_4(x_4msb, y_4msb,4);
		aux_irreducible_pol2 = composite_addition_single_values(composite_mult_gf2_4(x_4msb, y_4lsb,4),composite_mult_gf2_4(x_4lsb, y_4msb,4));
		aux_irreducible_pol1 = composite_mult_gf2_4(x_4lsb, y_4lsb,4);
		
		aux_irreducible_pol = (aux_irreducible_pol3<<8)+ (aux_irreducible_pol2<<4)+(aux_irreducible_pol1);

		//printf("%d %d %d\n",aux_irreducible_pol3, aux_irreducible_pol2, aux_irreducible_pol1);
		if (aux_irreducible_pol3 != ((irreducible_pol_2_8>>8) & 0x0f)){
			/*Transformation of irreducible polynomial*/
			aux_irreducible_pol3 = composite_mult_gf2_4((aux_irreducible_pol3), ((irreducible_pol_2_8 >> 8)& 0x0f),4);
			aux_irreducible_pol2 = composite_mult_gf2_4((aux_irreducible_pol3), ((irreducible_pol_2_8 >> 4)& 0x0f),4);
			aux_irreducible_pol1 = composite_mult_gf2_4((aux_irreducible_pol3), (irreducible_pol_2_8 & 0x0f),4);	

			/*Comparison XOR between polynomials */
			prod3 = (((aux_irreducible_pol >> 8) & 0x0f)^aux_irreducible_pol3);
			prod2 = (((aux_irreducible_pol >> 4) & 0x0f)^aux_irreducible_pol2);
			prod1 = ((aux_irreducible_pol & 0x0f)^aux_irreducible_pol1);

			
			//printf("The result is: %d\n", ((aux_irreducible_pol >> 8)));
			//printf("%d %d %d\n",prod3, prod2, prod1);
			//printf("%d %d %d\n",((irreducible_pol_2_8 >> 8)&0x0f), ((irreducible_pol_2_8 >> 4)&0x0f), (irreducible_pol_2_8 & 0x0f));
			//printf("%d %d %d\n",aux_irreducible_pol3, aux_irreducible_pol2, aux_irreducible_pol1);
		} else {
			
			prod3 = (((irreducible_pol_2_8 >> 8) & 0x0f)^aux_irreducible_pol3);
			prod2 = (((irreducible_pol_2_8 >> 4) & 0x0f)^aux_irreducible_pol2);
			prod1 = ((irreducible_pol_2_8 & 0x0f)^aux_irreducible_pol1);
			
			//printf("The result is: %d\n", 4*prod2+prod1);
			//printf("%d %d %d\n",prod3, prod2, prod1);
			//printf("%d %d %d\n",(irreducible_pol_2_4 >> 4), ((irreducible_pol_2_4>>2) & 0x03), (irreducible_pol_2_4 & 0x03));
			//printf("%d %d %d\n",aux_irreducible_pol3, aux_irreducible_pol2, aux_irreducible_pol1);
	
		}


		return (prod2<<4)+prod1;

	}

}

void composite_mult_single_values(uint8_t x, uint8_t y, uint8_t w){

	/*Staged multiplication*/
	uint8_t prod;
	/*For GF(2^2)*/
	if(x == 0 || y == 0)
		printf("Result %d\n", 0);

	if((x<4) && (y<4)){ prod = composite_mult_gf2_2(x, y, w);	
	} else if ((x<16) && (y<16)) {	prod = composite_mult_gf2_4(x, y, w);
	} else { prod = composite_mult_gf2_8(x, y, w);
	}

	//printf("Multiplciation result ready: %d\n", prod);
}

void composite_mult_full_table()
{
	FILE *table_mult;
	/*Internal operators*/
	int i,j;

	if(table_mult == NULL){
		fprintf(stderr, "Failed to create file to store computation \n");
	}else{
	table_mult=fopen("table_mult.csv", "w");
	for(i=0 ; i<256 ; i++){
		for(j=0 ; j<256 ; j++){
			if((i<4) & (j<4)){
			fprintf(table_mult,"%d, ", composite_mult_gf2_2(i,j,2));
			} else if ((i<16) & (j<16)){
			fprintf(table_mult,"%d, ", composite_mult_gf2_4(i,j,4));
			} else { fprintf(table_mult,"%d, ", composite_mult_gf2_8(i,j,8)); }
			
		}
		fprintf(table_mult,"\n");
	}
	fclose(table_mult);
	}	
}

int main(int argc, char *argv[])
{
	if(argc == 1){
		/* Print out full tables and export to file*/
		composite_addition_full_table();
		composite_mult_full_table();
		exit(1);
	} else if (argc == 3) {
	/*Compute addition based on input */
	composite_addition_single_values(atoi(argv[1]), atoi(argv[2]));
	} else if (argc == 4) {
	/*Check finite field order */
	if(atoi(argv[3]) > 8){
		fprintf(stderr, "Maximum finite field order is 8\n");
		exit(1);
	}
	/* Check for operants correctness */
	if ( ((atoi(argv[1]) < 0) || (atoi(argv[2]) < 0)) || ((atoi(argv[1]) > 255) || (atoi(argv[2]) > 255)) )
	{
		fprintf(stderr, "Operative values only in the range of 0 - 255 \n");
		exit(1);
	}
	
	/* Multiplcation of two values in GF(2^w)*/
	composite_mult_single_values(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
	} else { exit(1); }

	return 0;
}


