#ifndef __LCDFONT_H__
#define __LCDFONT_H__ 	  

typedef struct 
{
	unsigned char Index[2];	
	unsigned char Msk[32];
}typFNT_GB16;

typedef struct 
{
	unsigned char Index[2];	
	unsigned char Msk[72];
}typFNT_GB24;

typedef struct 
{
	unsigned char Index[2];	
	unsigned char Msk[128];
}typFNT_GB32; 

extern const unsigned char ascii_1608[][16];
extern const unsigned char ascii_3216[][64];
extern const typFNT_GB16 tfont16[18];
extern const typFNT_GB24 tfont24[5];
extern const typFNT_GB32 tfont32[5];

extern const unsigned char gImage_pic[32768];
extern const unsigned char gImage_TTT[2048]; 
extern const unsigned char gImage_power[68];
extern const unsigned char wifi[24];
extern const unsigned char gImage_yy[1808];
extern const unsigned char rainy[128];
extern const unsigned char Centigrade[32];

#endif /* __LCDFONT_H__ */


