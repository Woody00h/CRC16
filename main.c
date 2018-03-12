/*
 * main.c
 *
 *  Created on: 2018Äê3ÔÂ2ÈÕ
 *      Author: woody
 */


#include <stdio.h>

typedef unsigned short UINT16;
typedef unsigned char UINT8;
typedef unsigned char BYTE;

#define CRC16_POLY	0x8005
#define CRC_INIT	0xFFFF
#define PRINT_BIN_FILE	0
UINT16 culCalcCRC(BYTE crcData, UINT16 crcReg)
{
	UINT8 i;
	for (i = 0; i < 8; i++)
	{
		if (((crcReg & 0x8000) >> 8) ^ (crcData & 0x80))
			crcReg = (crcReg << 1) ^ CRC16_POLY;
		else
			crcReg = (crcReg << 1);

		crcData <<= 1;
	}
	return crcReg;
}// culCalcCRC

int main(int argc,char *argv[])
{
	FILE *bin;
	int out;
	UINT16 checksum = CRC_INIT;

	if(argc != 2)
	{
		printf("useage: CRC16 file.bin\n");
		return -1;
	}

	bin = fopen(argv[1],"rb");
	if(!bin)
	{
		printf("could not open the binary file: %s\n",argv[1]);
		return -1;
	}

	printf("CRC-16/IBM\n");
	printf("Width : 16\n");
	printf("Poly : 0x%X\n", CRC16_POLY);
	printf("Init : 0x%X\n", CRC_INIT);

#if PRINT_BIN_FILE
	unsigned int i = 0;
#endif
	while(1)
	{
		out = fgetc(bin);

		if(out != EOF)
		{
#if PRINT_BIN_FILE
			printf("%.2x ",out);
#endif
			checksum = culCalcCRC(out, checksum);
		}
		else
		{
			printf("\nThe CRC16 value of the file %s is 0x%X\n",argv[1], checksum);
			break;
		}
#if PRINT_BIN_FILE
		i++;
		if(i%16==0)
		{
			printf("\n");
		}
#endif
	}

	fclose(bin);
	
	return 0;
}
