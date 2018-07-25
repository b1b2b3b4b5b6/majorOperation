
#include "i2c.h"


//��ʼ��i2c�ӿ�
//CHECK OK
void i2c_init(void)
{		
	I2C_SCL_OUT;
	I2C_SCL_RESET;	
	I2C_SDA_OUT;
	I2C_SDA_SET;
	i2c_stop();
}			 

//i2c��ʼ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĸߵ���,Ϊi2c��ʼ�ź�
//�ڼ���״̬��,SDA��SCL��Ϊ�͵�ƽ
void i2c_start(void)
{
   I2C_SDA_SET;     //�����߸ߵ�ƽ	   
   I2C_SCL_SET;	    //��ʱ���߸ߵ�ʱ���������ɸ�����
    SCL_DELAY;  
   I2C_SDA_RESET;
    SCL_DELAY;	 
   I2C_SCL_RESET;	    //�����߻ָ��͵�ƽ��������������Ҫ	  
}

//i2cֹͣ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĵ͵���,Ϊi2cֹͣ�ź�
//����״����,SDA,SCL��Ϊ�ߵ�ƽ
void i2c_stop(void)
{
   I2C_SDA_RESET;
    SCL_DELAY;	 
   I2C_SCL_SET;	
    SCL_DELAY; 
   I2C_SDA_SET;	
    SCL_DELAY;
}  

//i2c,д��һ���ֽ�
//����ֵ:1,�ɹ�;0,ʧ��. 
u8 i2c_write_byte(u8 dat)
{
	u8 j,res;	 
	for(j=0;j<8;j++) //ѭ��8�η�������
	{
		if(dat&0x80)I2C_SDA_SET;	
		else
		I2C_SDA_RESET;
		dat<<=1;
		SCL_DELAY;
		I2C_SCL_SET;	
		SCL_DELAY;
		I2C_SCL_RESET;		   
	}			 
	I2C_SDA_IN;		//����SDAΪ���� 
	SCL_DELAY;
	I2C_SCL_SET;			//���յھ�λ,���ж��Ƿ��ͳɹ�
	SCL_DELAY;
	if(I2C_SDA_R) res=0;  //SDA=1����ʧ�ܣ�����0
	else res=1;         //SDA=0���ͳɹ�������1
	I2C_SCL_RESET;		 
	I2C_SDA_OUT;		//����SDAΪ���  
	SCL_DELAY;	  
	return res;  
}	 
//i2c ��ȡһ���ֽ�
//��SCL��������,��������
//����ֵ:����������
u8 i2c_read_byte(u8 ack)
{
	u8 temp=0,j;    
	I2C_SDA_IN;		//����SDAΪ����  
	for(j=8;j>0;j--) 	//ѭ��8�ν�������
	{		     	  
		SCL_DELAY;
		I2C_SCL_SET;
		temp=temp<<1;
		if(I2C_SDA_R)temp++;   
		SCL_DELAY;
		I2C_SCL_RESET;
	}	
	I2C_SDA_OUT;		//����SDAΪ���  
	SCL_DELAY;  
	
	(ack==0)?(I2C_SDA_SET):(I2C_SDA_RESET);
	I2C_SCL_SET;
	SCL_DELAY;
	I2C_SCL_RESET;
	SCL_DELAY;
	I2C_SDA_RESET;
	SCL_DELAY;
	return temp;
} 							    
//д�Ĵ���
//����ֵ:0,�ɹ�;1,ʧ��.
u8 i2c_write_reg(u8 slave, u8 reg,u8 byte)
{
	u8 res=1;
	i2c_start(); 					//����i2c����
	if(!i2c_write_byte(slave))res=0;	//д����ID	  
	if(!i2c_write_byte(reg))		res=0;		//д�Ĵ�����ַ	  
	if(!i2c_write_byte(byte))		res=0; 	//д����	 
  	i2c_stop();	  
  	return	res;
}		  					    
//���Ĵ���
//����ֵ:�����ļĴ���ֵ
u8 i2c_read_reg(u8 slave, u8 reg)
{
	u8 date=0;
	i2c_start(); 				//����i2c����
	i2c_write_byte(slave);		//д����ID	  
  i2c_write_byte(reg);			//д�Ĵ�����ַ	  
 
	RESTART_DELAY;	   
	//���üĴ�����ַ�󣬲��Ƕ�
	i2c_start();
	i2c_write_byte(slave|0X01);	//���Ͷ�����	  
  date=i2c_read_byte(0);		 	//��ȡ����
  i2c_stop();
  return date;
}
uchar i2c_read_buf(u8 slave, uchar reg,uchar *date,uchar bytes)		//����һ������	���ز������
{
	u8 n,res=1;
	i2c_start(); 				//����i2c����
	if(!i2c_write_byte(slave))	res=0;		//д����ID
	if(!i2c_write_byte(reg))	res=0;			//д�Ĵ�����ַ
	
	RESTART_DELAY;
	//���üĴ�����ַ�󣬲��Ƕ�
	i2c_start();
	if(!i2c_write_byte(slave|0X01))	res=0;	//���Ͷ�����
	for(n=0;n<bytes;n++)
	{
		*(date+n)=i2c_read_byte(bytes-n-1);		 	//��ȡ����	
	}
  i2c_stop();	
	return res;
}

uchar i2c_write_buf(u8 slave, uchar reg, uchar *date, uchar bytes)	//����һ������	���ز������
{
	u8 n,res=1;
	i2c_start(); 										//����i2c����
	if(!i2c_write_byte(slave))	res=0;	//д����ID
	if(!i2c_write_byte(reg))		res=0;		//д�Ĵ�����ַ
	for(n=0;n<bytes;n++)
	{
		if(i2c_write_byte(*(date+n))) res =0;		 	//д����
	}
	i2c_stop();
	return	res;
}















