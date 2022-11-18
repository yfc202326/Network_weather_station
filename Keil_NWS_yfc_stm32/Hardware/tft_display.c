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
	LCD_GPIO_RES_PIN|LCD_GPIO_DC_PIN|LCD_GPIO_CS_PIN|LCD_GPIO_BLK_PIN); 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_GPIO_SCL_PORT,&GPIO_InitStructure);
	
	//所有端口默认为高电平
	GPIO_SetBits(LCD_GPIO_SCL_PORT,(LCD_GPIO_SCL_PIN|LCD_GPIO_SDA_PIN| \
	LCD_GPIO_RES_PIN|LCD_GPIO_DC_PIN|LCD_GPIO_CS_PIN|LCD_GPIO_BLK_PIN));
}

/************************************************************************
 @brief  LCD串行数据写入函数
 @param  dat  要写入的串行数据
 @retval 无
************************************************************************/
void LCD_Write_Bus(u8 dat) 
{	
	u8 i;
	LCD_CS_Clr();
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
	LCD_CS_Set();	
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
		LCD_WR_DATA16(x1+2);
		LCD_WR_DATA16(x2+2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA16(y1+1);
		LCD_WR_DATA16(y2+1);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA16(x1+2);
		LCD_WR_DATA16(x2+2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA16(y1+3);
		LCD_WR_DATA16(y2+3);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA16(x1+1);
		LCD_WR_DATA16(x2+1);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA16(y1+2);
		LCD_WR_DATA16(y2+2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA16(x1+3);
		LCD_WR_DATA16(x2+3);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA16(y1+2);
		LCD_WR_DATA16(y2+2);
		LCD_WR_REG(0x2c);//储存器写
	}
}

/************************************************************************
 @brief  LCD初始化(注意:SPI传输所有颜色数据高位在前)
 @param  无
 @retval 无
************************************************************************/
void LCD_Init(void)
{
	LCD_GPIO_Config();//初始化GPIO
	
	LCD_RES_Clr();//复位
	Delay_ms(100);
	LCD_RES_Set();
	Delay_ms(100);
	
	LCD_BLK_Set();//打开背光
	Delay_ms(100);
	
	LCD_WR_REG(0x11); //Sleep out
	Delay_ms(120); //Delay 120ms
	//------------------------------------ST7735S Frame rate-------------------------------------------------//
	LCD_WR_REG(0xB1); //Frame rate 80Hz
	LCD_WR_DATA8(0x02);
	LCD_WR_DATA8(0x35);
	LCD_WR_DATA8(0x36);
	LCD_WR_REG(0xB2); //Frame rate 80Hz
	LCD_WR_DATA8(0x02);
	LCD_WR_DATA8(0x35);
	LCD_WR_DATA8(0x36);
	LCD_WR_REG(0xB3); //Frame rate 80Hz
	LCD_WR_DATA8(0x02);
	LCD_WR_DATA8(0x35);
	LCD_WR_DATA8(0x36);
	LCD_WR_DATA8(0x02);
	LCD_WR_DATA8(0x35);
	LCD_WR_DATA8(0x36);
	//------------------------------------End ST7735S Frame rate-------------------------------------------//
	LCD_WR_REG(0xB4); //Dot inversion
	LCD_WR_DATA8(0x03);
	//------------------------------------ST7735S Power Sequence-----------------------------------------//
	LCD_WR_REG(0xC0);
	LCD_WR_DATA8(0xA2);
	LCD_WR_DATA8(0x02);
	LCD_WR_DATA8(0x84);
	LCD_WR_REG(0xC1);
	LCD_WR_DATA8(0xC5);
	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0x2A);
	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0xEE);
	//---------------------------------End ST7735S Power Sequence---------------------------------------//
	LCD_WR_REG(0xC5); //VCOM
	LCD_WR_DATA8(0x0a);
	LCD_WR_REG(0x36);
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x08);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC8);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x78);
	else LCD_WR_DATA8(0xA8);
	//------------------------------------ST7735S Gamma Sequence-----------------------------------------//
	LCD_WR_REG(0XE0);
	LCD_WR_DATA8(0x12);
	LCD_WR_DATA8(0x1C);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x2C);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x2F);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x10);
	LCD_WR_REG(0XE1);
	LCD_WR_DATA8(0x12);
	LCD_WR_DATA8(0x1C);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x2D);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x23);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x2F);
	LCD_WR_DATA8(0x3B);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x10);
	//------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
	LCD_WR_REG(0x3A); //65k mode
	LCD_WR_DATA8(0x05);
	LCD_WR_REG(0x29); //Display on 
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
	u16 k;
	u16 HZnum;//汉字数目
	u16 TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
	TypefaceNum=sizey/8*sizey;//此算法只适用于字宽等于字高，且字高是8的倍数的字，
	                          //也建议用户使用这样大小的字,否则显示容易出问题！
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont16[k].Msk[i]&(0x01<<j))
							LCD_WR_DATA16(fc);
						else 
							LCD_WR_DATA16(bc);
					}
					else//叠加方式
					{
						if(tfont16[k].Msk[i]&(0x01<<j))
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
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
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
	u16 k;
	u16 HZnum;//汉字数目
	u16 TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
	TypefaceNum=sizey/8*sizey;//此算法只适用于字宽等于字高，且字高是8的倍数的字，
	                          //也建议用户使用这样大小的字,否则显示容易出问题！
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont24[k].Msk[i]&(0x01<<j))LCD_WR_DATA16(fc);
						else LCD_WR_DATA16(bc);
					}
					else//叠加方式
					{
						if(tfont24[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
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
	u16 k;
	u16 HZnum;//汉字数目
	u16 TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
	TypefaceNum=sizey/8*sizey;//此算法只适用于字宽等于字高，且字高是8的倍数的字，
	                          //也建议用户使用这样大小的字,否则显示容易出问题！
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont32[k].Msk[i]&(0x01<<j))LCD_WR_DATA16(fc);
						else LCD_WR_DATA16(bc);
					}
					else//叠加方式
					{
						if(tfont32[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
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
void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 temp,sizex,t;
	u16 i,TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
	sizex=sizey/2;//sizex:字宽为高的一半
	TypefaceNum=sizex/8*sizey;
	num=num-' ';    //得到偏移后的值
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //设置光标位置 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==16)temp=ascii_1608[num][i];		       //调用8x16字体
		else if(sizey==32)temp=ascii_3216[num][i];		 //调用16x32字体
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)//非叠加模式
			{
				if(temp&(0x01<<t))LCD_WR_DATA16(fc);
				else LCD_WR_DATA16(bc);
			}
			else//叠加模式
			{
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//画一个点
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
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
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode)
{         
	while(*p!='\0')
	{       
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
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
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);//非叠加模式
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
			LCD_WR_DATA8(pic[k*2]);
			LCD_WR_DATA8(pic[k*2+1]);
			k++;
		}
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

//电量显示
void LCD_Display_power(void)
{
	//电池边框
	LCD_DrawRectangle(3,3,30,10,WHITE);
	LCD_Fill(31,5,32,9,WHITE);
	//电池填充
	LCD_Fill(5,5,29,9,GREEN);
	
	//wifi图标
	LCD_ShowIcon(34,1,16,12,wifi,WHITE,BLACK);
//	LCD_DrawLine(35,1,48,11,RED);	
}

//天气显示
void LCD_Display_weather(void)
{
	LCD_ShowPicture(85,3,32,32,gImage_TTT);
}

//城市
void LCD_Display_City(void)
{
	u8 city_sx=20,city_sy=18;
	u8 air_sx=60,air_sy=18;
	LCD_ShowChinese(city_sx,city_sy,"嘉兴",YELLOW,WHITE,16,1);
	LCD_ShowChinese(air_sx,air_sy,"优",GREEN,WHITE,16,1);
}

//时间
void LCD_Display_Time(u8 hour,u8 min)
{	
	//时钟显示坐标
	u8 time_hour_sx=24,time_hour_sy=35;
	u8 colon_sx=time_hour_sx+32,colon_sy=time_hour_sy;
	u8 time_min_sx=colon_sx+16,time_min_sy=colon_sy;
	
	//时钟
	LCD_ShowIntNum(time_hour_sx,time_hour_sy,(u16)hour,2,WHITE,BLACK,32);
	LCD_ShowChar(colon_sx,colon_sy,':',WHITE,BLACK,32,1);
	LCD_ShowIntNum(time_min_sx,time_min_sy,(u16)min,2,WHITE,BLACK,32);
	
	//日期
	LCD_ShowIntNum(12,time_hour_sy+33,(u16)11,2,WHITE,BLACK,16);
	LCD_ShowChinese(28,time_hour_sy+33,"月",BRED,WHITE,16,1);
	LCD_ShowIntNum(44,time_hour_sy+33,(u16)26,2,WHITE,BLACK,16);
	LCD_ShowChinese(60,time_hour_sy+33,"日",BRED,WHITE,16,1);
	
	//星期
	LCD_ShowChinese(84,time_hour_sy+33,"周六",BRED,WHITE,16,1);
}

//温度及湿度
void LCD_Display_Tem_Hum(void)
{
	//温度坐标
	u8 tem_sx=35,tem_sy=95;
	u8 tem_ex=tem_sx+45,tem_ey=tem_sy+5;
	
	//湿度坐标
	u8 hum_sx=35,hum_sy=112;
	u8 hum_ex=hum_sx+45,hum_ey=hum_sy+5;	
	
	//温度
	LCD_DrawRectangle(tem_sx,tem_sy,tem_ex,tem_ey,WHITE);
	LCD_Fill(tem_sx+2,tem_sy+2,tem_ex-1,tem_ey-1,RED);	
	LCD_ShowIntNum(tem_ex+5,tem_ey-10,(u16)26,2,RED,BLACK,16);
	LCD_ShowIcon(tem_ex+23,tem_sy-5,16,16,Centigrade,RED,BLACK);
	
	//湿度
	LCD_DrawRectangle(hum_sx,hum_sy,hum_ex,hum_ey,WHITE);
	LCD_Fill(hum_sx+2,hum_sy+2,hum_ex-1,hum_ey-1,BLUE);
	LCD_ShowIntNum(hum_ex+5,hum_ey-10,(u16)64,2,GREEN,BLACK,16);	
	LCD_ShowChar(hum_ex+23,hum_sy-5,'%',GREEN,BLACK,16,1);
}





