#include <stdio.h>

short byteToShort(unsigned char MSB, unsigned char LSB);

int main(int argc, char const *argv[])
{
	printf("%d", (int)byteToShort(1,0));
	return 0;
}

short byteToShort(unsigned char MSB, unsigned char LSB){
	short s = 0;
	s = (MSB<<8) + LSB;
}