#include "stdio.h"
#include "x86intrin.h"

typedef union {
	__m128i 			int128;
	
	unsigned char		m128_u8[16];
	signed char			m128_i8[16];

	unsigned short		m128_u16[8];
	signed short		m128_i16[8];
} intVec;

void print_u8 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=15; i>0; i--) {
		printf ("%X, ", tmp.m128_u8[i]);
	}
	printf ("%X]\n\n", tmp.m128_u8[0]);
}

void print_u16 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=7; i>0; i--) {
		printf ("%X, ", tmp.m128_u16[i]);
	}
	printf ("%X]\n\n", tmp.m128_u16[0]);
}


unsigned char intArray [16] = {	0X00, 0X11, 0X22, 0X33, 0X44, 0X55, 0X66, 0X77,
								0X88, 0X99, 0XAA, 0XBB, 0XCC, 0XDD, 0XEE, 0XFF};

int main(void)
{
	__m128i a;
	a = _mm_load_si128((const __m128i*)intArray);

	printf ("Unsigned byte: ");
	print_u8 (a);

	printf ("Unsigned word: ");
	print_u16 (a);
	

	return 0;
}
