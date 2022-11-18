#include "tft_display.h"

/************************************************************************
 @brief  LCD_GPIO����
 @param  ��
 @retval ��
************************************************************************/
void LCD_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//���ж˿ھ�ΪGPIOBʱ��
	RCC_APB2PeriphClockCmd(LCD_GPIO_SCL_CLK,ENABLE);
	//NJTRST��ӳ�䵽PB4���ܣ�JTAG-DP Disabled and SW-DP Enabled��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	
	
	//����GPIO��ʼ��
	GPIO_InitStructure.GPIO_Pin = (LCD_GPIO_SCL_PIN|LCD_GPIO_SDA_PIN| \
	LCD_GPIO_RES_PIN|LCD_GPIO_DC_PIN|LCD_GPIO_CS_PIN|LCD_GPIO_BLK_PIN); 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_GPIO_SCL_PORT,&GPIO_InitStructure);
	
	//���ж˿�Ĭ��Ϊ�ߵ�ƽ
	GPIO_SetBits(LCD_GPIO_SCL_PORT,(LCD_GPIO_SCL_PIN|LCD_GPIO_SDA_PIN| \
	LCD_GPIO_RES_PIN|LCD_GPIO_DC_PIN|LCD_GPIO_CS_PIN|LCD_GPIO_BLK_PIN));
}

/************************************************************************
 @brief  LCD��������д�뺯��
 @param  dat  Ҫд��Ĵ�������
 @retval ��
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
 @brief  LCDд��8λ����
 @param  dat д�������
 @retval ��
************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	LCD_Write_Bus(dat);
}

/************************************************************************
 @brief  LCDд��16λ����
 @param  dat д�������
 @retval ��
************************************************************************/
void LCD_WR_DATA16(u16 dat)
{
	LCD_Write_Bus(dat>>8);
	LCD_Write_Bus(dat);
}

/************************************************************************
 @brief  LCDд������
 @param  cmd д�������
 @retval ��
************************************************************************/
void LCD_WR_REG(u8 cmd)
{
	LCD_DC_Clr();//д����
	LCD_Write_Bus(cmd);
	LCD_DC_Set();//д����
}

/************************************************************************
 @brief  ������ʼ�ͽ�����ַ
 @param  x1,x2 �����е���ʼ�ͽ�����ַ
		 y1,y2 �����е���ʼ�ͽ�����ַ
 @retval ��
************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA16(x1+2);
		LCD_WR_DATA16(x2+2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA16(y1+1);
		LCD_WR_DATA16(y2+1);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA16(x1+2);
		LCD_WR_DATA16(x2+2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA16(y1+3);
		LCD_WR_DATA16(y2+3);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA16(x1+1);
		LCD_WR_DATA16(x2+1);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA16(y1+2);
		LCD_WR_DATA16(y2+2);
		LCD_WR_REG(0x2c);//������д
	}
	else
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA16(x1+3);
		LCD_WR_DATA16(x2+3);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA16(y1+2);
		LCD_WR_DATA16(y2+2);
		LCD_WR_REG(0x2c);//������д
	}
}

/************************************************************************
 @brief  LCD��ʼ��(ע��:SPI����������ɫ���ݸ�λ��ǰ)
 @param  ��
 @retval ��
************************************************************************/
void LCD_Init(void)
{
	LCD_GPIO_Config();//��ʼ��GPIO
	
	LCD_RES_Clr();//��λ
	Delay_ms(100);
	LCD_RES_Set();
	Delay_ms(100);
	
	LCD_BLK_Set();//�򿪱���
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
 @brief  ��ָ�����������ɫ
 @param  xsta,ysta   ��ʼ����
		 xend,yend   ��ֹ����
		 color       Ҫ������ɫ
 @retval ��
************************************************************************/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//������ʾ��Χ
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			LCD_WR_DATA16(color);
		}
	} 					  	    
}

/************************************************************************
 @brief  ��ָ��λ�û���
 @param  x,y ��������
		 color �����ɫ
 @retval ��
************************************************************************/
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_Address_Set(x,y,x,y);//���ù��λ�� 
	LCD_WR_DATA16(color);
} 

/************************************************************************
 @brief  ����
 @param  x1,y1   ��ʼ����
		 x2,y2   ��ֹ����
		 color   �ߵ���ɫ
 @retval ��
************************************************************************/
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	//������������ 
	delta_x=x2-x1;
	delta_y=y2-y1;
	//�����������
	uRow=x1;
	uCol=y1;
	//���õ�������x 
	if(delta_x>0)incx=1; 
	else if (delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;}
	//���õ�������y
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//ˮƽ�� 
	else {incy=-1;delta_y=-delta_x;}
	//ѡȡ�������������� 
	if(delta_x>delta_y)distance=delta_x; 
	else distance=delta_y;
	//���������㷨
	for(t=0;t<distance+2;t++)//distance+1+1
	{
		LCD_DrawPoint(uRow,uCol,color);//����
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
 @brief  ������
 @param  x1,y1   ��ʼ����
		 x2,y2   ��ֹ����
		 color   ���ε���ɫ
 @retval ��
************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}

/************************************************************************
 @brief  ��Բ
 @param  x0,y0   Բ������
		 r       �뾶
		 color   Բ����ɫ
 @retval ��
************************************************************************/
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
	a=0;b=r;	
	//8�㻭Բ�㷨
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
		if((a*a+b*b)>(r*r))//�ж�Ҫ���ĵ��Ƿ��Զ
		{
			b--;
		}
	}
}


/************************************************************************
 @brief  ��ʾ����16x16����
 @param  x,y��ʾ����
		 *s Ҫ��ʾ�ĺ���
         fc �ֵ���ɫ
         bc �ֵı���ɫ
         sizey �ֺ�
         mode:  0�ǵ���ģʽ  1����ģʽ
 @retval ��
************************************************************************/
void LCD_ShowChinese16x16(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;//������Ŀ
	u16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0=x;
	TypefaceNum=sizey/8*sizey;//���㷨ֻ�������ֿ�����ָߣ����ָ���8�ı������֣�
	                          //Ҳ�����û�ʹ��������С����,������ʾ���׳����⣡
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont16[k].Msk[i]&(0x01<<j))
							LCD_WR_DATA16(fc);
						else 
							LCD_WR_DATA16(bc);
					}
					else//���ӷ�ʽ
					{
						if(tfont16[k].Msk[i]&(0x01<<j))
							LCD_DrawPoint(x,y,fc);//��һ����
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
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 

/************************************************************************
 @brief  ��ʾ����24x24����
 @param  x,y��ʾ����
		 *s Ҫ��ʾ�ĺ���
         fc �ֵ���ɫ
         bc �ֵı���ɫ
         sizey �ֺ�
         mode:  0�ǵ���ģʽ  1����ģʽ
 @retval ��
************************************************************************/
void LCD_ShowChinese24x24(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;//������Ŀ
	u16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0=x;
	TypefaceNum=sizey/8*sizey;//���㷨ֻ�������ֿ�����ָߣ����ָ���8�ı������֣�
	                          //Ҳ�����û�ʹ��������С����,������ʾ���׳����⣡
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont24[k].Msk[i]&(0x01<<j))LCD_WR_DATA16(fc);
						else LCD_WR_DATA16(bc);
					}
					else//���ӷ�ʽ
					{
						if(tfont24[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//��һ����
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
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 

/************************************************************************
 @brief  ��ʾ����32x32����
 @param  x,y��ʾ����
		 *s Ҫ��ʾ�ĺ���
         fc �ֵ���ɫ
         bc �ֵı���ɫ
         sizey �ֺ�
         mode:  0�ǵ���ģʽ  1����ģʽ
 @retval ��
************************************************************************/
void LCD_ShowChinese32x32(u16 x,u16 y,char *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;//������Ŀ
	u16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0=x;
	TypefaceNum=sizey/8*sizey;//���㷨ֻ�������ֿ�����ָߣ����ָ���8�ı������֣�
	                          //Ҳ�����û�ʹ��������С����,������ʾ���׳����⣡
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont32[k].Msk[i]&(0x01<<j))LCD_WR_DATA16(fc);
						else LCD_WR_DATA16(bc);
					}
					else//���ӷ�ʽ
					{
						if(tfont32[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//��һ����
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
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
}

/************************************************************************
 @brief  ��ʾ���ִ�
 @param  x,y��ʾ����
         *s Ҫ��ʾ�ĺ��ִ�
         fc �ֵ���ɫ
         bc �ֵı���ɫ
         sizey �ֺ� ��ѡ 16 24 32
         mode:  0�ǵ���ģʽ  1����ģʽ
 @retval ��
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
 @brief  ��ʾ�����ַ�
 @param  x,y��ʾ����
         num Ҫ��ʾ���ַ�
         fc �ֵ���ɫ
         bc �ֵı���ɫ
         sizey �ֺ� 16 32
         mode:  0�ǵ���ģʽ  1����ģʽ
 @retval ��
************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 temp,sizex,t;
	u16 i,TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0=x;
	sizex=sizey/2;//sizex:�ֿ�Ϊ�ߵ�һ��
	TypefaceNum=sizex/8*sizey;
	num=num-' ';    //�õ�ƫ�ƺ��ֵ
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //���ù��λ�� 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==16)temp=ascii_1608[num][i];		       //����8x16����
		else if(sizey==32)temp=ascii_3216[num][i];		 //����16x32����
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)//�ǵ���ģʽ
			{
				if(temp&(0x01<<t))LCD_WR_DATA16(fc);
				else LCD_WR_DATA16(bc);
			}
			else//����ģʽ
			{
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//��һ����
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
 @brief  ��ʾ�ַ���
 @param  x,y��ʾ����
         *p Ҫ��ʾ���ַ���
         fc �ֵ���ɫ
         bc �ֵı���ɫ
		 sizey �ֺ� 16 32
         mode:  0�ǵ���ģʽ  1����ģʽ
 @retval ��
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
 @brief  ��m^n����
 @param  m������nָ��
 @retval ��
************************************************************************/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;
	return result;
}

/************************************************************************
 @brief  ��ʾ��������
 @param  x,y��ʾ����
         num Ҫ��ʾ��������
         len Ҫ��ʾ��λ��
         fc �ֵ���ɫ
         bc �ֵı���ɫ
         sizey �ֺ� 16 32
 @retval ��
************************************************************************/
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp;
	u8 enshow=0;
	u8 sizex=sizey/2;//sizex:�ֿ�Ϊ�ߵ�һ��
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);//�ǵ���ģʽ
				continue;
			}
			else 
				enshow=1;//��ʼ��ʾ�������� 
		 	 
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);//�ǵ���ģʽ,"temp+48"=="temp+'0'"
	}
} 

/************************************************************************
 @brief  ��ʾ��λС������
 @param  x,y��ʾ����
         num Ҫ��ʾС������
         len Ҫ��ʾ��λ��
         fc �ֵ���ɫ
         bc �ֵı���ɫ
         sizey �ֺ� 16 32
 @retval ��
************************************************************************/
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp,sizex;
	u16 num1;
	sizex=sizey/2;//sizex:�ֿ�Ϊ�ߵ�һ��
	num1=num*100;//����������100��
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);//�ǵ���ģʽ
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);//�ǵ���ģʽ,"temp+48"=="temp+'0'"
	}
}

/************************************************************************
 @brief  ��ʾͼƬ
 @param  x,y�������
         length ͼƬ����
         width  ͼƬ���
         pic[]  ͼƬ����   
 @retval ��
************************************************************************/
void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[])
{
	u16 i,j,k=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);//����ʵ��,����Ҫ��1
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
 @brief  ��ʾͼ��
 @param  x,y�������
         length ͼ�곤��
         width  ͼ����
         icon[]  ͼ������   
         fc ͼ�����ɫ
         bc ͼ��ı���ɫ
 @retval ��
************************************************************************/
void LCD_ShowIcon(u16 x,u16 y,u16 length,u16 width,const u8 icon[],u16 fc,u16 bc)
{
	u16 i,j,k,temp=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);//����ʵ��,����Ҫ��1
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

//������ʾ
void LCD_Display_power(void)
{
	//��ر߿�
	LCD_DrawRectangle(3,3,30,10,WHITE);
	LCD_Fill(31,5,32,9,WHITE);
	//������
	LCD_Fill(5,5,29,9,GREEN);
	
	//wifiͼ��
	LCD_ShowIcon(34,1,16,12,wifi,WHITE,BLACK);
//	LCD_DrawLine(35,1,48,11,RED);	
}

//������ʾ
void LCD_Display_weather(void)
{
	LCD_ShowPicture(85,3,32,32,gImage_TTT);
}

//����
void LCD_Display_City(void)
{
	u8 city_sx=20,city_sy=18;
	u8 air_sx=60,air_sy=18;
	LCD_ShowChinese(city_sx,city_sy,"����",YELLOW,WHITE,16,1);
	LCD_ShowChinese(air_sx,air_sy,"��",GREEN,WHITE,16,1);
}

//ʱ��
void LCD_Display_Time(u8 hour,u8 min)
{	
	//ʱ����ʾ����
	u8 time_hour_sx=24,time_hour_sy=35;
	u8 colon_sx=time_hour_sx+32,colon_sy=time_hour_sy;
	u8 time_min_sx=colon_sx+16,time_min_sy=colon_sy;
	
	//ʱ��
	LCD_ShowIntNum(time_hour_sx,time_hour_sy,(u16)hour,2,WHITE,BLACK,32);
	LCD_ShowChar(colon_sx,colon_sy,':',WHITE,BLACK,32,1);
	LCD_ShowIntNum(time_min_sx,time_min_sy,(u16)min,2,WHITE,BLACK,32);
	
	//����
	LCD_ShowIntNum(12,time_hour_sy+33,(u16)11,2,WHITE,BLACK,16);
	LCD_ShowChinese(28,time_hour_sy+33,"��",BRED,WHITE,16,1);
	LCD_ShowIntNum(44,time_hour_sy+33,(u16)26,2,WHITE,BLACK,16);
	LCD_ShowChinese(60,time_hour_sy+33,"��",BRED,WHITE,16,1);
	
	//����
	LCD_ShowChinese(84,time_hour_sy+33,"����",BRED,WHITE,16,1);
}

//�¶ȼ�ʪ��
void LCD_Display_Tem_Hum(void)
{
	//�¶�����
	u8 tem_sx=35,tem_sy=95;
	u8 tem_ex=tem_sx+45,tem_ey=tem_sy+5;
	
	//ʪ������
	u8 hum_sx=35,hum_sy=112;
	u8 hum_ex=hum_sx+45,hum_ey=hum_sy+5;	
	
	//�¶�
	LCD_DrawRectangle(tem_sx,tem_sy,tem_ex,tem_ey,WHITE);
	LCD_Fill(tem_sx+2,tem_sy+2,tem_ex-1,tem_ey-1,RED);	
	LCD_ShowIntNum(tem_ex+5,tem_ey-10,(u16)26,2,RED,BLACK,16);
	LCD_ShowIcon(tem_ex+23,tem_sy-5,16,16,Centigrade,RED,BLACK);
	
	//ʪ��
	LCD_DrawRectangle(hum_sx,hum_sy,hum_ex,hum_ey,WHITE);
	LCD_Fill(hum_sx+2,hum_sy+2,hum_ex-1,hum_ey-1,BLUE);
	LCD_ShowIntNum(hum_ex+5,hum_ey-10,(u16)64,2,GREEN,BLACK,16);	
	LCD_ShowChar(hum_ex+23,hum_sy-5,'%',GREEN,BLACK,16,1);
}





