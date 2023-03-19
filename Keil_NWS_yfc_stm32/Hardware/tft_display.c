#include "tft_display.h"

/************************************************************************
 @brief  LCD_GPIO配置
 @param  无
 @retval 无
************************************************************************/
void LCD_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//所有端口均为GPIOB时钟
	RCC_APB2PeriphClockCmd(LCD_GPIO_SCL_CLK,ENABLE);
	//NJTRST重映射到PB4功能（JTAG-DP Disabled and SW-DP Enabled）
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	
	
	//所有GPIO初始化
	GPIO_InitStructure.GPIO_Pin = (LCD_GPIO_SCL_PIN|LCD_GPIO_SDA_PIN| \
	LCD_GPIO_RES_PIN|LCD_GPIO_DC_PIN|LCD_GPIO_BLK_PIN); 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_GPIO_SCL_PORT,&GPIO_InitStructure);
	
	//所有端口默认为高电平
	GPIO_SetBits(LCD_GPIO_SCL_PORT,(LCD_GPIO_SCL_PIN|LCD_GPIO_SDA_PIN| \
	LCD_GPIO_RES_PIN|LCD_GPIO_DC_PIN|LCD_GPIO_BLK_PIN));
}

/************************************************************************
 @brief  LCD串行数据写入函数
 @param  dat  要写入的串行数据
 @retval 无
************************************************************************/
void LCD_Write_Bus(u8 dat) 
{	
	u8 i;
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr();
		if(dat&0x80)
		{
		   LCD_MOSI_Set();
		}
		else
		{
		   LCD_MOSI_Clr();
		}
		LCD_SCLK_Set();
		dat<<=1;
	}	
}

/************************************************************************
 @brief  LCD写入8位数据
 @param  dat 写入的数据
 @retval 无
************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	LCD_Write_Bus(dat);
}

/************************************************************************
 @brief  LCD写入16位数据
 @param  dat 写入的数据
 @retval 无
************************************************************************/
void LCD_WR_DATA16(u16 dat)
{
	LCD_Write_Bus(dat>>8);
	LCD_Write_Bus(dat);
}

/************************************************************************
 @brief  LCD写入命令
 @param  cmd 写入的命令
 @retval 无
************************************************************************/
void LCD_WR_REG(u8 cmd)
{
	LCD_DC_Clr();//写命令
	LCD_Write_Bus(cmd);
	LCD_DC_Set();//写数据
}

/************************************************************************
 @brief  设置起始和结束地址
 @param  x1,x2 设置列的起始和结束地址
		 y1,y2 设置行的起始和结束地址
 @retval 无
************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA16(x1);
		LCD_WR_DATA16(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA16(y1);
		LCD_WR_DATA16(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA16(x1);
		LCD_WR_DATA16(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA16(y1);
		LCD_WR_DATA16(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA16(x1);
		LCD_WR_DATA16(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA16(y1);
		LCD_WR_DATA16(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA16(x1);
		LCD_WR_DATA16(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA16(y1);
		LCD_WR_DATA16(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
}

/************************************************************************
 @brief  TFT screen RES
 @param  None
 @retval None
************************************************************************/
void Reset()
{
	LCD_RES_Clr();
    Delay_ms(100);
    LCD_RES_Set();
    Delay_ms(100);
}

/************************************************************************
 @brief  LCD初始化(注意:SPI传输所有颜色数据高位在前)
 @param  无
 @retval 无
************************************************************************/
void LCD_Init(void)
{
	LCD_GPIO_Config();	//初始化GPIO
	
	Reset();			//复位
	
	LCD_BLK_Set();		//打开背光
	//LCD_BLK_Clr();
	Delay_ms(100);
	
	LCD_WR_REG(0x11); 	//Sleep out
	Delay_ms(120); 		//Delay 120ms

	LCD_WR_REG(0X36); 	//Memory Data Access Control		(*)
	LCD_WR_DATA8(0x00);	//Normal Mode

	LCD_WR_REG(0X3A); 	//Interface Pixel Format    		(*)
	LCD_WR_DATA8(0x05);	//16bit/pixel 
	
	LCD_WR_REG(0xB2);	//Porch Setting(与0xC6命令有关)		(*)
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x33);
	
	LCD_WR_REG(0xB7); 	//Gate Control 
	LCD_WR_DATA8(0x35); //VGH:13.26V  VGL:-10.43V
	
	LCD_WR_REG(0xBB);	//VCOM Setting
	LCD_WR_DATA8(0x19);	//0.725V

	LCD_WR_REG(0xC0);	//LCM Control
	LCD_WR_DATA8(0x2C);

	LCD_WR_REG(0xC2);	//VDV and VRH Command Enable 
	LCD_WR_DATA8(0x01);	//VDV and VRH register value comes from command write.
	
	LCD_WR_REG(0xC3);	//VRH Set
	LCD_WR_DATA8(0x12);   

	LCD_WR_REG(0xC4);	//VDV Set
	LCD_WR_DATA8(0x20);  

	LCD_WR_REG(0xC6); 	//Frame Rate Control in Normal Mode (*)
	LCD_WR_DATA8(0x0F); //dot inversion，60Hz  

	LCD_WR_REG(0xD0); 	//Power Control 1 
	LCD_WR_DATA8(0xA4);
	LCD_WR_DATA8(0xA1);

	LCD_WR_REG(0xE0);	//Positive Voltage Gamma Control(与颜色调节有关)
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2B);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x54);
	LCD_WR_DATA8(0x4C);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x0B);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x23);

	LCD_WR_REG(0xE1);	//Negative Voltage Gamma Control(与颜色调节有关)
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2C);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x51);
	LCD_WR_DATA8(0x2F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x20);
	LCD_WR_DATA8(0x23);

	LCD_WR_REG(0x21); 	//Display Inversion On 				(*)

	LCD_WR_REG(0x11); 	//Sleep Out 						(*)

	LCD_WR_REG(0x29); 	//Display On						(*)
}

/************************************************************************
 @brief  在指定区域填充颜色
 @param  xsta,ysta   起始坐标
		 xend,yend   终止坐标
		 color       要填充的颜色
 @retval 无
************************************************************************/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//设置显示范围
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			LCD_WR_DATA16(color);
		}
	} 					  	    
}

/************************************************************************
 @brief  在指定位置画点
 @param  x,y 画点坐标
		 color 点的颜色
 @retval 无
************************************************************************/
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_Address_Set(x,y,x,y);//设置光标位置 
	LCD_WR_DATA16(color);
} 

/************************************************************************
 @brief  画线
 @param  x1,y1   起始坐标
		 x2,y2   终止坐标
		 color   线的颜色
 @retval 无
************************************************************************/
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	//计算坐标增量 
	delta_x=x2-x1;
	delta_y=y2-y1;
	//画线起点坐标
	uRow=x1;
	uCol=y1;
	//设置单步方向x 
	if(delta_x>0)incx=1; 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	//设置单步方向y
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_x;}
	//选取基本增量坐标轴 
	if(delta_x>delta_y)distance=delta_x; 
	else distance=delta_y;
	//比例画线算法
	for(t=0;t<distance+2;t++)//distance+1+1
	{
		LCD_DrawPoint(uRow,uCol,color);//画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

/************************************************************************
 @brief  画矩形
 @param  x1,y1   起始坐标
		 x2,y2   终止坐标
		 color   矩形的颜色
 @retval 无
************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}

/************************************************************************
 @brief  画圆
 @param  x0,y0   圆心坐标
		 r       半径
		 color   圆的颜色
 @retval 无
************************************************************************/
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
	a=0;b=r;	
	//8点画圆算法
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//判断要画的点是否过远
		{
			b--;
		}
	}
}


/************************************************************************
 @brief  显示单个16x16汉字
 @param  x,y显示坐标
		 *s 要显示的汉字
         fc 字的颜色
         bc 字的背景色
         sizey 字号
         mode:  0非叠加模式  1叠加模式
 @retval 无
************************************************************************/
void LCD_ShowChinese16x16(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j;
	u16 x0=x;							//始点横坐标
	u16 TypefaceNum=0;					//字符数据大小
	u8 Type_H=0;						//GB2312字体高八位
	u8 Type_L=0;						//GB2312字体低八位
	u8 Typeface_Dat=0;					//字形数据
	u32 Type_ADDR=GB2312_H16_BASE_ADDR;	//字体对应外部flash地址

	TypefaceNum=sizey/8*sizey;			//一个字符所占字节大小

	Type_H=*s;
	Type_L=*(s+1);
		
	Type_ADDR += (((Type_H-0xA0-1)*94) +(Type_L-0xA0-1))*TypefaceNum;
	LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
		
	for(i=0;i<TypefaceNum;i++)
	{
		W25qx_SPI_BufferRead(&Typeface_Dat,Type_ADDR,1);//获取字体的一个字节数据
		for(j=0;j<8;j++)
		{	
			if(!mode)//非叠加方式
			{
				if(Typeface_Dat&(0x01<<j))
					LCD_WR_DATA16(fc);
				else 
					LCD_WR_DATA16(bc);
			}
			else//叠加方式
			{
				if(Typeface_Dat&(0x01<<j))
					LCD_DrawPoint(x,y,fc);//画一个点
				x++;
				if((x-x0)==sizey)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
		Type_ADDR++;
	}	
} 

/************************************************************************
 @brief  显示单个24x24汉字
 @param  x,y显示坐标
		 *s 要显示的汉字
         fc 字的颜色
         bc 字的背景色
         sizey 字号
         mode:  0非叠加模式  1叠加模式
 @retval 无
************************************************************************/
void LCD_ShowChinese24x24(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j;
	u16 x0=x;							//始点横坐标
	u16 TypefaceNum=0;					//字符数据大小
	u8 Type_H=0;						//GB2312字体高八位
	u8 Type_L=0;						//GB2312字体低八位
	u8 Typeface_Dat=0;					//字形数据
	u32 Type_ADDR=GB2312_H24_BASE_ADDR;	//字体对应外部flash地址

	TypefaceNum=sizey/8*sizey;			//一个字符所占字节大小

	Type_H=*s;
	Type_L=*(s+1);
		
	Type_ADDR += (((Type_H-0xA0-1)*94) +(Type_L-0xA0-1))*TypefaceNum;
	LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
		
	for(i=0;i<TypefaceNum;i++)
	{
		W25qx_SPI_BufferRead(&Typeface_Dat,Type_ADDR,1);//获取字体的一个字节数据
		for(j=0;j<8;j++)
		{	
			if(!mode)//非叠加方式
			{
				if(Typeface_Dat&(0x01<<j))
					LCD_WR_DATA16(fc);
				else 
					LCD_WR_DATA16(bc);
			}
			else//叠加方式
			{
				if(Typeface_Dat&(0x01<<j))
					LCD_DrawPoint(x,y,fc);//画一个点
				x++;
				if((x-x0)==sizey)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
		Type_ADDR++;
	}	
} 

/************************************************************************
 @brief  显示单个32x32汉字
 @param  x,y显示坐标
		 *s 要显示的汉字
         fc 字的颜色
         bc 字的背景色
         sizey 字号
         mode:  0非叠加模式  1叠加模式
 @retval 无
************************************************************************/
void LCD_ShowChinese32x32(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j;
	u16 x0=x;							//始点横坐标
	u16 TypefaceNum=0;					//字符数据大小
	u8 Type_H=0;						//GB2312字体高八位
	u8 Type_L=0;						//GB2312字体低八位
	u8 Typeface_Dat=0;					//字形数据
	u32 Type_ADDR=GB2312_H32_BASE_ADDR;	//字体对应外部flash地址

	TypefaceNum=sizey/8*sizey;			//一个字符所占字节大小

	Type_H=*s;
	Type_L=*(s+1);
		
	Type_ADDR += (((Type_H-0xA0-1)*94) +(Type_L-0xA0-1))*TypefaceNum;
	LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
		
	for(i=0;i<TypefaceNum;i++)
	{
		W25qx_SPI_BufferRead(&Typeface_Dat,Type_ADDR,1);//获取字体的一个字节数据
		for(j=0;j<8;j++)
		{	
			if(!mode)//非叠加方式
			{
				if(Typeface_Dat&(0x01<<j))
					LCD_WR_DATA16(fc);
				else 
					LCD_WR_DATA16(bc);
			}
			else//叠加方式
			{
				if(Typeface_Dat&(0x01<<j))
					LCD_DrawPoint(x,y,fc);//画一个点
				x++;
				if((x-x0)==sizey)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
		Type_ADDR++;
	}	
}

/************************************************************************
 @brief  显示汉字串
 @param  x,y显示坐标
         *s 要显示的汉字串
         fc 字的颜色
         bc 字的背景色
         sizey 字号 可选 16 24 32
         mode:  0非叠加模式  1叠加模式
 @retval 无
************************************************************************/
void LCD_ShowChinese(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	while(*s!=0)
	{
		if(sizey==16) LCD_ShowChinese16x16(x,y,s,fc,bc,sizey,mode);
		else if(sizey==24) LCD_ShowChinese24x24(x,y,s,fc,bc,sizey,mode);
		else if(sizey==32) LCD_ShowChinese32x32(x,y,s,fc,bc,sizey,mode);
		else return;
		s+=2;
		x+=sizey;
	}
}

/************************************************************************
 @brief  显示单个字符
 @param  x,y显示坐标
         num 要显示的字符
         fc 字的颜色
         bc 字的背景色
         sizey 字号 16 32
         mode:  0非叠加模式  1叠加模式
 @retval 无
************************************************************************/
void LCD_ShowChar(u16 x,u16 y,char num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j,sizex=sizey/2;				//sizex:字宽为高的一半
	u16 x0=x;							//始点横坐标
	u16 TypefaceNum=0;					//字符数据大小
	u8 Typeface_Dat=0;					//字形数据
	u32 Type_ADDR=0;				
	
	if(sizey==16)
		Type_ADDR=ASCLL_H16_BASE_ADDR;	//字体对应外部flash地址
	else if(sizey==32)
		Type_ADDR=ASCLL_H32_BASE_ADDR;	//字体对应外部flash地址
	else if(sizey==48)
		Type_ADDR=ASCLL_H48_BASE_ADDR;	//字体对应外部flash地址
	
	TypefaceNum=sizex*sizey/8;			//一个字符所占字节大小

	Type_ADDR += (num*TypefaceNum);
	
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);
		
	for(i=0;i<TypefaceNum;i++)
	{
		W25qx_SPI_BufferRead(&Typeface_Dat,Type_ADDR,1);//获取字体的一个字节数据
		for(j=0;j<8;j++)
		{	
			if(!mode)//非叠加方式
			{
				if(Typeface_Dat&(0x01<<j))
					LCD_WR_DATA16(fc);
				else 
					LCD_WR_DATA16(bc);
			}
			else//叠加方式
			{
				if(Typeface_Dat&(0x01<<j))
					LCD_DrawPoint(x,y,fc);//画一个点
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
		Type_ADDR++;
	}
}

/************************************************************************
 @brief  显示字符串
 @param  x,y显示坐标
         *p 要显示的字符串
         fc 字的颜色
         bc 字的背景色
		 sizey 字号 16 32
         mode:  0非叠加模式  1叠加模式
 @retval 无
************************************************************************/
void LCD_ShowString(u16 x,u16 y,char *p,u16 fc,u16 bc,u8 sizey,u8 mode)
{         
	u8 x0=x;
	while(*p!='\0')
	{      
		if(*p=='\r' && *(p+1)=='\n')
		{
			x=x0;
			y+=sizey;
			p+=2;
			continue;
		}
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}

/************************************************************************
 @brief  显示任意文本
		 注意：仅当中文字体与英文字体高度（16 32）相同时能使用此函数
 @param  x,y显示坐标
         *p 要显示的字符串
         fc 字的颜色
         bc 字的背景色
		 sizey 字号 16 32
         mode:  0非叠加模式  1叠加模式
 @retval 无
************************************************************************/
void LCD_ShowText(u16 x,u16 y,char *p,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 x0=x;
	while(*p!='\0')
	{      
		if(*p=='\r' && *(p+1)=='\n')
		{
			x=x0;
			y+=sizey;
			p+=2;
			continue;
		}
		else if( (u8)*p < 0xA1)
		{
			LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
			x+=sizey/2;
			p++;
		}
		else
		{
			if(sizey==16) LCD_ShowChinese16x16(x,y,p,fc,bc,sizey,mode);
			else if(sizey==32) LCD_ShowChinese32x32(x,y,p,fc,bc,sizey,mode);
			else return;
			p+=2;
			x+=sizey;			
		}
		
	}  
}

/************************************************************************
 @brief  求m^n函数
 @param  m底数，n指数
 @retval 无
************************************************************************/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;
	return result;
}

/************************************************************************
 @brief  显示整数变量
 @param  x,y显示坐标
         num 要显示整数变量
         len 要显示的位数
         fc 字的颜色
         bc 字的背景色
         sizey 字号 16 32
 @retval 无
************************************************************************/
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp;
	u8 enshow=0;
	u8 sizex=sizey/2;//sizex:字宽为高的一半
	for(t=0;t<len;t++)
	{
		temp=( num/mypow(10,len-t-1) )%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				//LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);//没有部分以空显示，非叠加模式
				LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);//没有部分以零显示，非叠加模式
				continue;
			}
			else 
				enshow=1;//开始显示整数变量 
		 	 
		}
		LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);//非叠加模式,"temp+48"=="temp+'0'" 	
	}
} 

/************************************************************************
 @brief  显示两位小数变量
 @param  x,y显示坐标
         num 要显示小数变量
         len 要显示的位数
         fc 字的颜色
         bc 字的背景色
         sizey 字号 16 32
 @retval 无
************************************************************************/
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp,sizex;
	u16 num1;
	sizex=sizey/2;//sizex:字宽为高的一半
	num1=num*100;//浮点数扩大100倍
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);//非叠加模式
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);//非叠加模式,"temp+48"=="temp+'0'"
	}
}

/************************************************************************
 @brief  显示图片
 @param  x,y起点坐标
         length 图片长度
         width  图片宽度
         pic[]  图片数组   
 @retval 无
************************************************************************/
void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[])
{
	u16 i,j,k=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);//根据实际,长宽都要减1
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			if(pic[k*2]!=0 && pic[k*2+1!=0])
			{
				LCD_WR_DATA8(pic[k*2]);
				LCD_WR_DATA8(pic[k*2+1]);				
			}
			else
			{
				LCD_WR_DATA8(0xff);
				LCD_WR_DATA8(0xff);								
			}
			k++;
		}
	}			
}

/************************************************************************
 @brief  显示天气图片
 @param  x,y起点坐标
         length 	图片长度
         width  	图片宽度
         pic_code 	图片代号   
 @retval 无
************************************************************************/
void LCD_Show_Wea_Picture(u16 x,u16 y,u16 length,u16 width,u8 pic_code)
{
	u16 i,j;
	u16 len_byte=length*2;							//一行所写数据（按字节）
	u32 f_size_byte;								//文件大小（按字节）
	u16 row_num;									//有多少整行
	u16 lea_num;									//整行余数
	u16 rem_num;									//剩余显示数
	char pic_path[]="1:/wea_pic_code/yfcxx.bin";	//文件路径
	
	//图片文件路径转化
	pic_path[19]=(pic_code/10)+'0';
	pic_path[20]=(pic_code%10)+'0';

	//1、挂载文件系统	
	res_FR=f_mount(&fs,"1:",1);	
	if(res_FR != FR_OK)
	{
		printf("文件挂载失败，错误代码为：%d",res_FR);
		while(1)Indicator_LED(ON);//进入死循环	
	}

	//2、打开/创建文件
	res_FR=f_open(&fnew,pic_path,FA_OPEN_EXISTING|FA_READ);
	if(res_FR != FR_OK)
	{
		printf("图片数据文件打开失败，错误代码为：%d\r\n",res_FR);
		while(1)Indicator_LED(ON);//进入死循环
	}
	
	//3、计算文件大小
	f_size_byte=f_size(&fnew);
	row_num=f_size_byte/(length*2);
	lea_num=f_size_byte%(length*2);
	rem_num=length*width*2-f_size_byte;
	
	//4、读取文件数据并显示
	f_rewind(&fnew);//将文件指针偏移
	LCD_Address_Set(x,y,x+length-1,y+width-1);//根据实际,长宽都要减1
	for(i=0;i<row_num;i++)
	{
		res_FR=f_read(&fnew,fReadbuffer,length*2,&nbr);
		if(res_FR!=FR_OK)
		{
			printf("图片数据读取失败，错误代码为：%d\r\n",res_FR);
			while(1)Indicator_LED(ON);//进入死循环
		}
		for(j=0;j<len_byte;j+=2)
		{
			LCD_WR_DATA8(fReadbuffer[j]);
			LCD_WR_DATA8(fReadbuffer[j+1]);				
		}
	}

	res_FR=f_read(&fnew,fReadbuffer,lea_num,&nbr);
	if(res_FR!=FR_OK)
	{
		printf("图片数据读取失败，错误代码为：%d\r\n",res_FR);
		while(1)Indicator_LED(ON);//进入死循环
	}
	for(j=0;j<lea_num;j++)
	{
		LCD_WR_DATA8(fReadbuffer[j]);		
	}

	for(j=0;j<rem_num;j++)
	{
		LCD_WR_DATA8(0XFF);		
	}

	//5、关闭打开的文件
	res_FR=f_close(&fnew);//关闭文件系统
	if(res_FR != FR_OK)
	{
		printf("图片数据文件关闭异常，错误代码为%d",res_FR);
		while(1)Indicator_LED(ON);//进入死循环
	}

	//6、取消挂载文件系统
	res_FR=f_unmount("1:");//取消挂载文件系统
	if(res_FR != FR_OK)
	{
		printf("文件系统取消挂载异常，错误代码为%d",res_FR);
		while(1)Indicator_LED(ON);//进入死循环
	}	
}

/************************************************************************
 @brief  显示图标
 @param  x,y起点坐标
         length 图标长度
         width  图标宽度
         icon[]  图标数组   
         fc 图标的颜色
         bc 图标的背景色
 @retval 无
************************************************************************/
void LCD_ShowIcon(u16 x,u16 y,u16 length,u16 width,const u8 icon[],u16 fc,u16 bc)
{
	u16 i,j,k,temp=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);//根据实际,长宽都要减1
	k=length*width/8;
	for(i=0;i<k;i++)
	{
		temp=icon[i];
		for(j=0;j<8;j++)
		{
			if(temp&(0x01<<j))
				LCD_WR_DATA16(fc);
			else
				LCD_WR_DATA16(bc);
		}
	}			
}

/************************************************************************
 @brief  显示wifi图标
 @param  x,y起点坐标
         length 图标长度
         width  图标宽度
         fc 图标的颜色
         bc 图标的背景色
 @retval 无
************************************************************************/
void LCD_Show_Wifi_Icon(u16 x,u16 y,u16 length,u16 width,u16 fc,u16 bc)
{
	u16 i,j,k;
	char pic_path[]="1:/Icon_pic_code/yfcwifi.bin";	//文件路径

	//1、挂载文件系统	
	res_FR=f_mount(&fs,"1:",1);	
	if(res_FR != FR_OK)
	{
		printf("文件挂载失败，错误代码为：%d",res_FR);
		while(1)Indicator_LED(ON);//进入死循环	
	}

	//2、打开/创建文件
	res_FR=f_open(&fnew,pic_path,FA_OPEN_EXISTING|FA_READ);
	if(res_FR != FR_OK)
	{
		printf("图片数据文件打开失败，错误代码为：%d\r\n",res_FR);
		while(1)Indicator_LED(ON);//进入死循环
	}

	//4、读取文件数据并显示
	f_rewind(&fnew);//将文件指针偏移
	k=length*width/8;
	res_FR=f_read(&fnew,fReadbuffer,k,&nbr);
	if(res_FR!=FR_OK)
	{
		printf("图片数据读取失败，错误代码为：%d\r\n",res_FR);
		while(1)Indicator_LED(ON);//进入死循环
	}
	LCD_Address_Set(x,y,x+length-1,y+width-1);//根据实际,长宽都要减1
	for(i=0;i<k;i++)
	{
		for(j=0;j<8;j++)
		{
			if(fReadbuffer[i]&(0x80>>j))
				LCD_WR_DATA16(fc);
			else
				LCD_WR_DATA16(bc);
		}
	}			

	//5、关闭打开的文件
	res_FR=f_close(&fnew);//关闭文件系统
	if(res_FR != FR_OK)
	{
		printf("图片数据文件关闭异常，错误代码为%d",res_FR);
		while(1)Indicator_LED(ON);//进入死循环
	}

	//6、取消挂载文件系统
	res_FR=f_unmount("1:");//取消挂载文件系统
	if(res_FR != FR_OK)
	{
		printf("文件系统取消挂载异常，错误代码为%d",res_FR);
		while(1)Indicator_LED(ON);//进入死循环
	}	
}


//电量显示
void LCD_Display_power(uint8_t wifi_link)
{
	u8 pos_x=196,pos_y=3;
	//电池边框
	LCD_DrawRectangle(pos_x,pos_y,pos_x+37,pos_y+12,GRAY);
	LCD_Fill(pos_x+38,pos_y+3,pos_x+40,pos_y+9,GRAY);
	//电池填充
	LCD_Fill(pos_x+2,pos_y+2,pos_x+36,pos_y+11,GREEN);
	
	//wifi图标
	if(wifi_link==0)
	{
		//LCD_ShowIcon(pos_x-26,pos_y-2,24,19,wifi,RED,WHITE);
		LCD_Show_Wifi_Icon(pos_x-26,pos_y-2,24,19,RED,WHITE);
	}
	else if(wifi_link==1)
	{
		//LCD_ShowIcon(pos_x-26,pos_y-2,24,19,wifi,BLACK,WHITE);
		LCD_Show_Wifi_Icon(pos_x-26,pos_y-2,24,19,BLACK,WHITE);
	}
	
}

//城市及天气
void LCD_Display_City(struct cur_weather *currweather)
{
	u8 pos_x=5,pos_y=12;
	char wea_temp[20]={0};

	if(strcmp(currweather->cur_city,"Guangyuan")==0)
	{
		LCD_ShowChinese(pos_x + 80, pos_y, "广元市", BLUE, WHITE, 24, 1);
	}
	else if(strcmp(currweather->cur_city,"Jiaxing")==0)
	{
		LCD_ShowChinese(pos_x + 80, pos_y, "嘉兴市", BLUE, WHITE, 24, 1);
	}
	
	switch (currweather->cur_wea_code)
	{
		case 0:
		case 1:
		case 2:
		case 3:
			strcpy(wea_temp,"晴");
				break;
		case 4:
			strcpy(wea_temp,"多云");
				break;
		case 5:
		case 6:
			strcpy(wea_temp,"晴间多云");
				break;
		case 7:
		case 8:
			strcpy(wea_temp,"大部多云");
				break;
		case 9:
			strcpy(wea_temp,"阴");
				break;
		case 10:
			strcpy(wea_temp,"阵雨");
				break;
		case 11:
			strcpy(wea_temp,"雷阵雨");
				break;
		case 12:
			strcpy(wea_temp,"雷阵雨伴有冰雹");
				break;
		case 13:
			strcpy(wea_temp,"小雨");
				break;
		case 14:
			strcpy(wea_temp,"中雨");
				break;
		case 15:
			strcpy(wea_temp,"大雨");
				break;
		case 16:
			strcpy(wea_temp,"暴雨");
				break;
		case 17:
			strcpy(wea_temp,"大暴雨");
				break;
		case 18:
			strcpy(wea_temp,"特大暴雨");
				break;
		case 19:
			strcpy(wea_temp,"冻雨");
				break;
		case 20:
			strcpy(wea_temp,"雨夹雪");
				break;
		case 21:
			strcpy(wea_temp,"阵雪");
				break;
		case 22:
			strcpy(wea_temp,"小雪");
				break;
		case 23:
			strcpy(wea_temp,"中雪");
				break;
		case 24:
			strcpy(wea_temp,"大雪");
				break;
		case 25:
			strcpy(wea_temp,"暴雪");
				break;
		case 26:
			strcpy(wea_temp,"浮尘");
				break;
		case 27:
			strcpy(wea_temp,"扬沙");
				break;
		case 28:
			strcpy(wea_temp,"沙尘暴");
				break;
		case 29:
			strcpy(wea_temp,"强沙尘暴");
				break;
		case 30:
			strcpy(wea_temp,"雾");
				break;
		case 31:
			strcpy(wea_temp,"霾");
				break;
		case 32:
			strcpy(wea_temp,"风");
				break;
		case 33:
			strcpy(wea_temp,"大风");
				break;
		case 34:
			strcpy(wea_temp,"飓风");
				break;
		case 35:
			strcpy(wea_temp,"热带风暴");
				break;
		case 36:
			strcpy(wea_temp,"龙卷风");
				break;
		case 37:
			strcpy(wea_temp,"冷");
				break;
		case 38:
			strcpy(wea_temp,"热");
				break;
		case 99:
			strcpy(wea_temp,"未知");
				break;
	}
	LCD_ShowChinese(pos_x+80,pos_y+30,wea_temp,DSBLUE,WHITE,24,1);
}

//天气图标显示
void LCD_Display_weather(struct cur_weather *currweather)
{
	u8 pos_x=10,pos_y=6;

	if(currweather->cur_wea_code <= 99)
	{
		LCD_Show_Wea_Picture(pos_x,pos_y,60,70,currweather->cur_wea_code);
	}
	else
	{
		printf("天气图片获取异常\r\n");
		while(1);
	}
}

//时间
void LCD_Display_Time(struct rtc_time *time)
{	
	//时钟显示定位基准
	u8 pos_x=120,pos_y=100;
	
	//时钟
	LCD_ShowIntNum(pos_x+35,pos_y-24-32-5,time->tm_year,4,LIGHTGREEN,WHITE,32);//年 
	
	LCD_ShowIntNum(pos_x-24-24-48,pos_y-24,time->tm_hour,2,BLACK,WHITE,48);//时
	
	LCD_ShowChar(pos_x-24-24,pos_y-24,':',BLACK,WHITE,48,0);

	LCD_ShowIntNum(pos_x-24,pos_y-24,time->tm_min,2,BLACK,WHITE,48);//分
	
	LCD_ShowChar(pos_x+24,pos_y-24,':',BLACK,WHITE,48,0);
	
	LCD_ShowIntNum(pos_x+24+24,pos_y-24,time->tm_sec,2,ORANGE,WHITE,48);//秒
	
	//日期
	LCD_ShowIntNum(pos_x-96,pos_y+24,time->tm_mon,2,BLACK,WHITE,32);
	LCD_ShowChinese(pos_x-96+32,pos_y+24,"月",BRED,WHITE,32,1);
	LCD_ShowIntNum(pos_x-96+32+32,pos_y+24,time->tm_mday,2,BLACK,WHITE,32);
	LCD_ShowChinese(pos_x-96+32+32+32,pos_y+24,"日",BRED,WHITE,32,1);
	
	//星期
	LCD_ShowChinese(pos_x-24-24-48+32+32+32+32,pos_y+24,"周",BRED,WHITE,32,0);
	switch(time->tm_wday)
	{
		case 0:
			LCD_ShowChinese(pos_x-24-24-48+32+32+32+32+32,pos_y+24,"日",BLACK,WHITE,32,0);
			break;
		case 1:
			LCD_ShowChinese(pos_x-24-24-48+32+32+32+32+32,pos_y+24,"一",BLACK,WHITE,32,0);
			break;
		case 2:
			LCD_ShowChinese(pos_x-24-24-48+32+32+32+32+32,pos_y+24,"二",BLACK,WHITE,32,0);
			break;
		case 3:
			LCD_ShowChinese(pos_x-24-24-48+32+32+32+32+32,pos_y+24,"三",BLACK,WHITE,32,0);
			break;
		case 4:
			LCD_ShowChinese(pos_x-24-24-48+32+32+32+32+32,pos_y+24,"四",BLACK,WHITE,32,0);
			break;
		case 5:
			LCD_ShowChinese(pos_x-24-24-48+32+32+32+32+32,pos_y+24,"五",BLACK,WHITE,32,0);
			break;
		case 6:
			LCD_ShowChinese(pos_x-24-24-48+32+32+32+32+32,pos_y+24,"六",BLACK,WHITE,32,0);
			break;
	}
}

//温度及湿度
void LCD_Display_Tem_Hum(struct cur_weather *currweather)
{
	u8 pos_x=25,pos_y=175;
	char degc[2]={0XA1,0XE6};//摄氏度对应GB2312编码
	char centi[2]={0XA3,0XA5};//百分号对应GB2312编码
	float i=(currweather->cur_tem-(-20))/70.0*95.0;
	float j=(currweather->cur_humid)/100.0*95.0;
	
	/* 温度显示 */
	LCD_ShowChinese(pos_x,pos_y,"温度",RED,WHITE,24,1);
	//框
	LCD_DrawRectangle(pos_x+55,pos_y+5,pos_x+55+100,pos_y+15,GRAY);
	LCD_Fill(pos_x+55+3,pos_y+5+3,(pos_x+55+100-2)-(95-(uint8_t)(i)),pos_y+15-2,RED);

	/* 温度显示 */
	if(currweather->cur_tem>=0)
	{
		LCD_ShowIntNum(pos_x+55+100+6,pos_y+3,(u16)(currweather->cur_tem),2,RED,WHITE,16);
		LCD_ShowChinese16x16(pos_x+55+100+6+16,pos_y+3,degc,RED,WHITE,16,1);
	}
	else
	{
		currweather->cur_tem=-currweather->cur_tem;
		LCD_ShowChar(pos_x+55+100+6,pos_y+3,'-',RED,WHITE,16,1);
		LCD_ShowIntNum(pos_x+55+100+6+8,pos_y+3,(u16)(currweather->cur_tem),2,RED,WHITE,16);
		LCD_ShowChinese16x16(pos_x+55+100+6+8+16,pos_y+3,degc,RED,WHITE,16,1);
	}
	
	/* 湿度显示 */
	LCD_ShowChinese(pos_x,pos_y+30,"湿度",PEABLUE,WHITE,24,1);
	//框
	LCD_DrawRectangle(pos_x+55,pos_y+30+5,pos_x+55+100,pos_y+30+15,GRAY);
	LCD_Fill(pos_x+55+3,pos_y+30+5+3,(pos_x+55+100-2)-(95-(uint8_t)(j)),pos_y+30+15-2,BLUE);
	LCD_ShowIntNum(pos_x+55+100+6,pos_y+30+3,(u16)currweather->cur_humid,2,PEABLUE,WHITE,16);	
	LCD_ShowChinese16x16(pos_x+55+100+6+16,pos_y+30+3,centi,PEABLUE,WHITE,16,1);

}


//Esp8266连接wifi自动获取时间
void Esp8266_Connect_Time(struct rtc_time *tm)
{
	uint8_t buffer=0;
	uint8_t state=0;
	uint8_t i=0;
	char temp[5]={0};
	Esp8266_Recive esp_time;
	
	//显示界面切换
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	LCD_Display_power(0);	
	LCD_ShowString(60,90,"Getting time",BLACK,WHITE,16,0);
	LCD_ShowString(60,106,"is connecting...",BLACK,WHITE,16,0);

	Esp8266_SendCmd("AT+CWMODE=1\r\n");	
	Esp8266_SendCmd("AT+RST\r\n");
	//等待系统重置
	Delay_ms(500);
	//连接wifi
	Esp8266_SendCmd("AT+CWJAP=\"oppo yfc\",\"18781260236\"\r\n");
	//连接网站
	Esp8266_SendCmd("AT+CIPSTART=\"TCP\",\"www.beijing-time.org\",80\r\n");
	Esp8266_SendCmd("AT+CIPMODE=1\r\n");
	Esp8266_SendCmd("AT+CIPSEND\r\n");
	Esp8266_SendString(ESP_USARTx,"GET\r\n");
	while(i<30)
	{		
		//Date: Tue, 03 Jan 2023 11:04:31 GMT
		if(USART_GetFlagStatus(ESP_USARTx,USART_FLAG_RXNE)==SET)
		{
			buffer=USART_ReceiveData(ESP_USARTx);
			USART_ClearFlag(ESP_USARTx,USART_FLAG_RXNE);
			Usart_SendByte(DEBUG_USARTx,buffer);
			
			if(state==0 && buffer=='D')
			{
				state=1;
			}	
			else if(state==1 && buffer=='a')
			{
				state=2;
			}
			else if(state==2 && buffer=='t')
			{
				state=3;
			}
			else if(state==3 && buffer=='e')
			{
				state=4;
			}
			else if(state==4 && buffer==':')
			{
				state=5;			
				while(i<30)
				{
					if(USART_GetFlagStatus(ESP_USARTx,USART_FLAG_RXNE)==SET)
					{
						esp_time.rec_time[i]=USART_ReceiveData(ESP_USARTx);
						USART_ClearFlag(ESP_USARTx,USART_FLAG_RXNE);
						Usart_SendByte(DEBUG_USARTx,esp_time.rec_time[i]);
						i++;
					}		
				}
			}	
			else
			{
				state=0;
			}	
						
		}	
	}
	Esp8266_SendString(ESP_USARTx,"+++");//退出数据透传
	printf("\r\n");
	
	//星期
	temp[0]=esp_time.rec_time[1];
	temp[1]=esp_time.rec_time[2];
	temp[2]=esp_time.rec_time[3];
	if(strcmp(temp,"Sun")==0)
	{
		tm->tm_wday=0;
	}
	else if(strcmp(temp,"Mon")==0)
	{
		tm->tm_wday=1;
	}
	else if(strcmp(temp,"Tue")==0)
	{
		tm->tm_wday=2;
	}
	else if(strcmp(temp,"Wed")==0)
	{
		tm->tm_wday=3;
	}
	else if(strcmp(temp,"Thu")==0)
	{
		tm->tm_wday=4;
	}
	else if(strcmp(temp,"Fri")==0)
	{
		tm->tm_wday=5;
	}
	else if(strcmp(temp,"Sat")==0)
	{
		tm->tm_wday=6;
	}
	for(i=0;i<5;i++)
	{
		temp[i]=0;
	}
	
	//年月日
	//日
	temp[0]=esp_time.rec_time[6];
	temp[1]=esp_time.rec_time[7];
	uint8_t temp1=(temp[0]-48)*10+(temp[1]-48);
	tm->tm_mday=temp1;
	//月
	temp[0]=esp_time.rec_time[9];
	temp[1]=esp_time.rec_time[10];
	temp[2]=esp_time.rec_time[11];
	if(strcmp(temp,"Jan")==0)
	{
		tm->tm_mon=1;
	}
	else if(strcmp(temp,"Feb")==0)
	{
		tm->tm_mon=2;
	}
	else if(strcmp(temp,"Mar")==0)
	{
		tm->tm_mon=3;
	}
	else if(strcmp(temp,"Apr")==0)
	{
		tm->tm_mon=4;
	}
	else if(strcmp(temp,"May")==0)
	{
		tm->tm_mon=5;
	}
	else if(strcmp(temp,"Jun")==0)
	{
		tm->tm_mon=6;
	}
	else if(strcmp(temp,"Jul")==0)
	{
		tm->tm_mon=7;
	}
	else if(strcmp(temp,"Aug")==0)
	{
		tm->tm_mon=8;
	}
	else if(strcmp(temp,"Sep")==0)
	{
		tm->tm_mon=9;
	}
	else if(strcmp(temp,"Oct")==0)
	{
		tm->tm_mon=10;
	}
	else if(strcmp(temp,"Nov")==0)
	{
		tm->tm_mon=11;
	}
	else if(strcmp(temp,"Dec")==0)
	{
		tm->tm_mon=12;
	}
	for(i=0;i<5;i++)
	{
		temp[i]=0;
	}	
	//年
	temp[0]=esp_time.rec_time[13];
	temp[1]=esp_time.rec_time[14];
	temp[2]=esp_time.rec_time[15];
	temp[3]=esp_time.rec_time[16];
	uint16_t temp2=(temp[0]-48)*1000+(temp[1]-48)*100+(temp[2]-48)*10+(temp[3]-48);
	tm->tm_year=temp2;
	
	//时分秒
	//时
	temp[0]=esp_time.rec_time[18];
	temp[1]=esp_time.rec_time[19];
	uint8_t temp3=(temp[0]-48)*10+(temp[1]-48)+8;
	if(temp3>=24)
	{
		temp3-=24;
	}
	tm->tm_hour=temp3;
	//分
	temp[0]=esp_time.rec_time[21];
	temp[1]=esp_time.rec_time[22];
	temp3=(temp[0]-48)*10+(temp[1]-48);
	tm->tm_min=temp3;
	//秒
	temp[0]=esp_time.rec_time[24];
	temp[1]=esp_time.rec_time[25];
	temp3=(temp[0]-48)*10+(temp[1]-48);
	tm->tm_sec=temp3;
	
	//显示界面切换
	LCD_Display_power(1);
	LCD_Fill(0,60,LCD_W,150,WHITE);
	LCD_ShowString(20,90,"Data acquisition succeeded.",BLACK,WHITE,16,0);
	Delay_ms(500);
	LCD_Fill(0,60,LCD_W,150,WHITE);
	
	//设置时间
	Time_Adjust(tm);				
	//向备份寄存器写入标志
	BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);	
}

//Esp8266连接wifi自动获取天气
void Esp8266_Connect_Weather(struct cur_weather *currweather)
{
	//循环位置变量
	uint8_t i=0;
	//数据缓存区
	uint8_t buffer=0;
	//状态切换
	uint8_t rev_state=0;
	//Esp8266天气数据接收定义
	Esp8266_Recive esp_weather;
	
	//显示界面切换
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	LCD_Display_power(0);	
	LCD_ShowString(60,90,"Getting weather",BLACK,WHITE,16,0);
	LCD_ShowString(60,106,"is connecting...",BLACK,WHITE,16,0);

	//0、测试指令(可略)
	Esp8266_SendCmd("AT\r\n");
	//1、设置STA模式
	Esp8266_SendCmd("AT+CWMODE=1\r\n");	
	//2、等待系统重置
	Esp8266_SendCmd("AT+RST\r\n");
	//等待系统重置
	Delay_ms(500);
	//3、开启但连接模式
	Esp8266_SendCmd("AT+CIPMUX=0\r\n");
	//4、连接wifi
	Esp8266_SendCmd("AT+CWJAP=\"oppo yfc\",\"18781260236\"\r\n");
	//5、连接心知天气网络
	Esp8266_SendCmd("AT+CIPSTART=\"TCP\",\"api.seniverse.com\",80\r\n");
	//6、设置esp8266工作在数据穿透模式
	Esp8266_SendCmd("AT+CIPMODE=1\r\n");
	//7、开启数据穿透
	Esp8266_SendCmd("AT+CIPSEND\r\n");
	//8.1、连接到个人网络站点、接收当前天气及温度
	Esp8266_SendString(ESP_USARTx,"GET https://api.seniverse.com/v3/weather/now.json?key=SEiJPSGoiKojJEp-x&location=jiaxing&language=en&unit=c\r\n");
	while(rev_state<13)
	{
		if(USART_GetFlagStatus(ESP_USARTx,USART_FLAG_RXNE)==SET)
		{
			buffer=USART_ReceiveData(ESP_USARTx);
			USART_ClearFlag(ESP_USARTx,USART_FLAG_RXNE);
			Usart_SendByte(DEBUG_USARTx,buffer);
			if(buffer==':')
			{
				rev_state++;

				//1、接收name
				if(rev_state==4)
				{
					i=0;
					while(buffer!=',')
					{					
						if(USART_GetFlagStatus(ESP_USARTx,USART_FLAG_RXNE) == SET)
						{
							buffer=USART_ReceiveData(ESP_USARTx);
							USART_ClearFlag(ESP_USARTx,USART_FLAG_RXNE);
							Usart_SendByte(DEBUG_USARTx,buffer);
							esp_weather.Dat_weather.name[i]=buffer;
							i++;
						}
					}
				}
				
				//2、接收text
				else if(rev_state==11)
				{
					i=0;
					while(buffer!=',')
					{					
						if(USART_GetFlagStatus(ESP_USARTx,USART_FLAG_RXNE) == SET)
						{
							buffer=USART_ReceiveData(ESP_USARTx);
							USART_ClearFlag(ESP_USARTx,USART_FLAG_RXNE);
							Usart_SendByte(DEBUG_USARTx,buffer);
							esp_weather.Dat_weather.text[i]=buffer;
							i++;
						}
					}				
				}
				
				//3、接收wea_code
				else if(rev_state==12)
				{
					i=0;
					while(buffer!=',')
					{					
						if(USART_GetFlagStatus(ESP_USARTx,USART_FLAG_RXNE) == SET)
						{
							buffer=USART_ReceiveData(ESP_USARTx);
							USART_ClearFlag(ESP_USARTx,USART_FLAG_RXNE);
							Usart_SendByte(DEBUG_USARTx,buffer);
							esp_weather.Dat_weather.wea_code[i]=buffer;
							i++;
						}
					}				
				}

				//4、接收temperature
				else if(rev_state==13)
				{
					i=0;
					while(buffer!=',')
					{					
						if(USART_GetFlagStatus(ESP_USARTx,USART_FLAG_RXNE) == SET)
						{
							buffer=USART_ReceiveData(ESP_USARTx);
							USART_ClearFlag(ESP_USARTx,USART_FLAG_RXNE);
							Usart_SendByte(DEBUG_USARTx,buffer);
							esp_weather.Dat_weather.temperature[i]=buffer;
							i++;
						}
					}
				}
			}
		}
	}
	//等待多余数据发送完毕
	Delay_ms(500);

	rev_state=0;//状态清零
	//8.2、连接到个人网络站点、接收当天最高温度最低温度及湿度
	Esp8266_SendString(ESP_USARTx,"GET https://api.seniverse.com/v3/weather/daily.json?key=SEiJPSGoiKojJEp-x&location=guangyuan&language=en&unit=c&start=0&days=3\r\n");
	while(rev_state<24)
	{
		if(USART_GetFlagStatus(ESP_USARTx,USART_FLAG_RXNE)==SET)
		{
			buffer=USART_ReceiveData(ESP_USARTx);
			USART_ClearFlag(ESP_USARTx,USART_FLAG_RXNE);
			Usart_SendByte(DEBUG_USARTx,buffer);
			if(buffer==':')
			{
				rev_state++;

				//1、接收tem_high
				if(rev_state==16)
				{
					i=0;
					while(buffer!=',')
					{					
						if(USART_GetFlagStatus(ESP_USARTx,USART_FLAG_RXNE) == SET)
						{
							buffer=USART_ReceiveData(ESP_USARTx);
							USART_ClearFlag(ESP_USARTx,USART_FLAG_RXNE);
							Usart_SendByte(DEBUG_USARTx,buffer);
							esp_weather.Dat_weather.tem_high[i]=buffer;
							i++;
						}
					}
				}
				
				//2、接收tem_low
				else if(rev_state==17)
				{
					i=0;
					while(buffer!=',')
					{					
						if(USART_GetFlagStatus(ESP_USARTx,USART_FLAG_RXNE) == SET)
						{
							buffer=USART_ReceiveData(ESP_USARTx);
							USART_ClearFlag(ESP_USARTx,USART_FLAG_RXNE);
							Usart_SendByte(DEBUG_USARTx,buffer);
							esp_weather.Dat_weather.tem_low[i]=buffer;
							i++;
						}
					}				
				}
				
				//3、接收humidity
				else if(rev_state==24)
				{
					i=0;
					while(buffer!=',')
					{					
						if(USART_GetFlagStatus(ESP_USARTx,USART_FLAG_RXNE) == SET)
						{
							buffer=USART_ReceiveData(ESP_USARTx);
							USART_ClearFlag(ESP_USARTx,USART_FLAG_RXNE);
							Usart_SendByte(DEBUG_USARTx,buffer);
							esp_weather.Dat_weather.humidity[i]=buffer;
							i++;
						}
					}				
				}
			}
		}
	}
	//等待多余数据发送完毕
	Delay_ms(500);
	//9、退出数据透穿模式
	Esp8266_SendString(ESP_USARTx,"+++");//退出数据透传
	
	//city获取
	for(i=1;esp_weather.Dat_weather.name[i]!='\"';i++)
	{
		currweather->cur_city[i-1]=esp_weather.Dat_weather.name[i];
	}
	currweather->cur_city[i-1]='\0';

	//天气获取
	for(i=1;esp_weather.Dat_weather.text[i]!='\"';i++)
	{
		currweather->cur_wea[i-1]=esp_weather.Dat_weather.text[i];
	}
	currweather->cur_wea[i-1]='\0';
	
	//天气代码获取
	currweather->cur_wea_code=0;
	for(i=1;esp_weather.Dat_weather.wea_code[i]!='\"';i++)
	{
		currweather->cur_wea_code = currweather->cur_wea_code*10 + (esp_weather.Dat_weather.wea_code[i]-48);
	}

	//温度获取
	currweather->cur_tem=0;
	if(esp_weather.Dat_weather.temperature[1]!='-')
	{
		for(i=1;esp_weather.Dat_weather.temperature[i]!='\"';i++)
		{
			currweather->cur_tem=currweather->cur_tem*10+(esp_weather.Dat_weather.temperature[i]-48);
		}
	}
	else
	{
		for(i=2;esp_weather.Dat_weather.temperature[i]!='\"';i++)
		{
			currweather->cur_tem=currweather->cur_tem*10+(esp_weather.Dat_weather.temperature[i]-48);
		}
		currweather->cur_tem=-currweather->cur_tem;
	}

	//最高最低温获取
	//最高温
	currweather->tem_high=0;
	if(esp_weather.Dat_weather.tem_high[1]!='-')
	{
		for(i=1;esp_weather.Dat_weather.tem_high[i]!='\"';i++)
		{
			currweather->tem_high=currweather->tem_high*10+(esp_weather.Dat_weather.tem_high[i]-48);
		}
	}
	else
	{
		for(i=2;esp_weather.Dat_weather.tem_high[i]!='\"';i++)
		{
			currweather->tem_high=currweather->tem_high*10+(esp_weather.Dat_weather.tem_high[i]-48);
		}
		currweather->tem_high=-currweather->tem_high;
	}
	//最低温
	currweather->tem_low=0;
	if(esp_weather.Dat_weather.tem_low[1]!='-')
	{
		for(i=1;esp_weather.Dat_weather.tem_low[i]!='\"';i++)
		{
			currweather->tem_low=currweather->tem_low*10+(esp_weather.Dat_weather.tem_low[i]-48);
		}
	}
	else
	{
		for(i=2;esp_weather.Dat_weather.tem_low[i]!='\"';i++)
		{
			currweather->tem_low=currweather->tem_low*10+(esp_weather.Dat_weather.tem_low[i]-48);
		}
		currweather->tem_low=-currweather->tem_low;
	}

	//湿度获取
	currweather->cur_humid=0;
	for(i=1;esp_weather.Dat_weather.humidity[i]!='\"';i++)
	{
		currweather->cur_humid = currweather->cur_humid*10 + (esp_weather.Dat_weather.humidity[i]-48);
	}

	//显示界面切换
	LCD_Display_power(1);
	LCD_Fill(0,60,LCD_W,150,WHITE);
	LCD_ShowString(20,90,"Data acquisition succeeded.",BLACK,WHITE,16,0);

	printf("\r\n已获取成功数据如下:\r\n");
	printf("city:%s\r\n",currweather->cur_city);
	printf("weather:%s\r\n",currweather->cur_wea);
	printf("weather code:%d\r\n",currweather->cur_wea_code);
	printf("temperature:%d\r\n",currweather->cur_tem);
	printf("tem_high:%d\r\n",currweather->tem_high);
	printf("tem_low:%d\r\n",currweather->tem_low);
	printf("humidity:%d\r\n",currweather->cur_humid);

	Delay_ms(500);
	LCD_Fill(0,60,LCD_W,150,WHITE);
}




