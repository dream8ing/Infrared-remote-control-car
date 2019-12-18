/*********************************************************************************
* ����дʱ�䡿�� 2014��3��5��
* ����    �ߡ��� �������:03
* ����    ������ 1.0
* ����    վ���� http://www.qxmcu.com/ 
* ���Ա����̡��� http://qxmcu.taobao.com/ (ֱ����)  http://qx-mcu.taobao.com/  ���ܵ꣩
* ��ʵ��ƽ̨���� QX-MCS51 ��Ƭ��������
* ���ⲿ���񡿣� 11.0592mhz	
* ������оƬ���� STC89C52RC
* �����뻷������ Keil ��Visio4	
* �������ܡ��� �������ʾ�������ֵ0-9 			   			            			    
* ��ʹ��˵������ ʹ�ú������ʱ��Ӧ���������ͷ�嵽������ָ��λ�ã�ע�ⷽ��
				 ���������л�����ʾ	 ,�������ͷԲ�������򿪷����ⲿ
**********************************************************************************/

/*Ԥ��������*/
#include<reg52.h>    //������Ƭ���Ĵ�����ͷ�ļ�
#include<intrins.h> //����_nop_()���������ͷ�ļ�
#define uchar unsigned char
#define uint  unsigned int
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};

sbit IRIN=P3^2;         //���������������


sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;

sbit OUT1=P1^0;
sbit OUT2=P1^1;
sbit OUT3=P1^2;
sbit OUT4=P1^3;
sbit OUT5=P1^4;
sbit OUT6=P1^5;
sbit OUT7=P1^6;
sbit OUT8=P1^7;

uchar code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,
                        0xf8,0x80,0x90};
uchar IRCOM[7];
uchar m;


void delay1ms();
void delay(unsigned char x) ;
void display();
void delay1(int ms);



/*******************************************************************/
void main()
{
	 unsigned int  a;
   
	IRIN=1;                    //I/O�ڳ�ʼ��	
	IE=0x81;                 //�������ж��ж�,ʹ�� INT0 �ⲿ�ж�
	TCON=TCON|0x01;               //������ʽΪ���帺���ش���
	
	 while(1)
	 {
		
	    for(a=100;a>0;a--)
   {
     display();
   }
	  }
	        	

} 
/**********************************************************/
void IR_IN() interrupt 0 using 0
{
	unsigned char j,k,N=0;
	unsigned char q=0;
	EX0 = 0;   
	delay(15);
	if (IRIN==1) 
	{ 
		EX0 =1;
		return;
	} 
	           //ȷ��IR�źų���
	while (!IRIN)            //��IR��Ϊ�ߵ�ƽ������9ms��ǰ���͵�ƽ�źš�
		delay(1);
	
	for (j=0;j<4;j++)         //�ռ���������
	{ 
		for (k=0;k<8;k++)        //ÿ��������8λ
		{
			while (IRIN)            //�� IR ��Ϊ�͵�ƽ������4.5ms��ǰ���ߵ�ƽ�źš�
			{delay(1);}
			while (!IRIN)          //�� IR ��Ϊ�ߵ�ƽ
				delay(1);
			while (IRIN)           //����IR�ߵ�ƽʱ��
			{
				delay(1);
				N++;           
				if (N>=30)
				{ 
					EX0=1;
					return;
				}                  //0.14ms���������Զ��뿪��
			}                        //�ߵ�ƽ�������                
			IRCOM[j]=IRCOM[j] >> 1;     //�������λ����0��
			if(N>=8)
			{
				IRCOM[j] = IRCOM[j] | 0x80;		  //�������λ����1��
			}  
			N=0;
		}
	}
	
	if(IRCOM[2]!=~IRCOM[3])
	{ 
		EX0=1;
		return; 
	}
	
	IRCOM[5]=IRCOM[2] & 0x0F;     //ȡ����ĵ���λ
	IRCOM[6]=IRCOM[2] >> 4;       //����4�Σ�����λ��Ϊ����λ
	
	if(IRCOM[5]>9)
	{ 
		IRCOM[5]=IRCOM[5]+0x37;
	}
	else
	IRCOM[5]=IRCOM[5]+0x30;
	
	if(IRCOM[6]>9)
	{ 
		IRCOM[6]=IRCOM[6]+0x37;
	}
	else
	IRCOM[6]=IRCOM[6]+0x30;

	q= (((IRCOM[6]&0x0f)<<4) + (IRCOM[5]&0x0f));
    

	switch(q)    //�жϰ�������ֵ
	{
		case 0x16:m=0;break; //���ڷ���0
		case 0x03:m=1;break;  //���ڷ���01
		case 0x18:m=15;
			OUT1=1;		  
	  		OUT2=0;
	  		OUT3=1;
	  		OUT4=0;
	  		OUT5=1;
	  		OUT6=0;
	  		OUT7=1;
	  		OUT8=0;
			 delay1(20);
				
			 break;  //���ڷ���02
		case 0x55:m=3;break;  //���ڷ���03
		case 0x08:m=14;
			OUT1=1;
	 		 OUT2=0;
	 		 OUT3=0;
	 		 OUT4=0;
			  OUT5=1;
			  OUT6=0;
			  OUT7=0;
			  OUT8=0; 
			  delay1(20);
			  break;  //���ڷ���04
		case 0x13:m=5;break;  //���ڷ���05
		case 0x51:m=13;
			OUT1=0;
	 		 OUT2=0;
	 		 OUT3=1;
	 		 OUT4=0;
			  OUT5=0;
			  OUT6=0;
			  OUT7=1;
			  OUT8=0; 
			  delay1(20);
			  break;  //���ڷ���06
		case 0x42:m=7;break;  //���ڷ���07
		case 0x52:m=12;
			OUT1=0;		  
	  		OUT2=1;
	  		OUT3=0;
	  		OUT4=1;
	  		OUT5=0;
	  		OUT6=1;
	  		OUT7=0;
	  		OUT8=1;
			 delay1(20);
			 break;  //���ڷ���08
		case 0x41:m=9;break;  //���ڷ���09
	
		default :break; 
	}

	 EX0 = 1; 
}


/***************��ʱ����*****************************/
void delay(unsigned char x)    //x*0.14MS
{
	unsigned char i;
	while(x--)
	{
		for (i = 0; i<13; i++) {}
	}
}


void display()
{
   
   P0=table[m];
//            while(m==15)
//			{}
//			 while(m==8)
//			{} 
//			while(m==11)
//	  		{
//	  			}
   P2_3 = 0;
   delay(5);
   P2_3 = 1;
   
}

void delay1(int ms)
{
	unsigned char y;
	while(ms--)
	{
		for(y = 0; y<250; y++)
		{
			_nop_();
			_nop_();
			_nop_();
			_nop_();
		}
	}
}


//************************************************************
void delay1ms()
{
 unsigned int i;
 for(i=8024;i>0;i--);  //��ʱ124*8+10=1002us
}

