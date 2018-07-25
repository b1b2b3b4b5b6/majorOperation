
#include "i2c.h"


//初始化i2c接口
//CHECK OK
void i2c_init(void)
{		
	I2C_SCL_OUT;
	I2C_SCL_RESET;	
	I2C_SDA_OUT;
	I2C_SDA_SET;
	i2c_stop();
}			 

//i2c起始信号
//当时钟为高的时候,数据线的高到低,为i2c起始信号
//在激活状态下,SDA和SCL均为低电平
void i2c_start(void)
{
   I2C_SDA_SET;     //数据线高电平	   
   I2C_SCL_SET;	    //在时钟线高的时候数据线由高至低
    SCL_DELAY;  
   I2C_SDA_RESET;
    SCL_DELAY;	 
   I2C_SCL_RESET;	    //数据线恢复低电平，单操作函数必要	  
}

//i2c停止信号
//当时钟为高的时候,数据线的低到高,为i2c停止信号
//空闲状况下,SDA,SCL均为高电平
void i2c_stop(void)
{
   I2C_SDA_RESET;
    SCL_DELAY;	 
   I2C_SCL_SET;	
    SCL_DELAY; 
   I2C_SDA_SET;	
    SCL_DELAY;
}  

//i2c,写入一个字节
//返回值:1,成功;0,失败. 
u8 i2c_write_byte(u8 dat)
{
	u8 j,res;	 
	for(j=0;j<8;j++) //循环8次发送数据
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
	I2C_SDA_IN;		//设置SDA为输入 
	SCL_DELAY;
	I2C_SCL_SET;			//接收第九位,以判断是否发送成功
	SCL_DELAY;
	if(I2C_SDA_R) res=0;  //SDA=1发送失败，返回0
	else res=1;         //SDA=0发送成功，返回1
	I2C_SCL_RESET;		 
	I2C_SDA_OUT;		//设置SDA为输出  
	SCL_DELAY;	  
	return res;  
}	 
//i2c 读取一个字节
//在SCL的上升沿,数据锁存
//返回值:读到的数据
u8 i2c_read_byte(u8 ack)
{
	u8 temp=0,j;    
	I2C_SDA_IN;		//设置SDA为输入  
	for(j=8;j>0;j--) 	//循环8次接收数据
	{		     	  
		SCL_DELAY;
		I2C_SCL_SET;
		temp=temp<<1;
		if(I2C_SDA_R)temp++;   
		SCL_DELAY;
		I2C_SCL_RESET;
	}	
	I2C_SDA_OUT;		//设置SDA为输出  
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
//写寄存器
//返回值:0,成功;1,失败.
u8 i2c_write_reg(u8 slave, u8 reg,u8 byte)
{
	u8 res=1;
	i2c_start(); 					//启动i2c传输
	if(!i2c_write_byte(slave))res=0;	//写器件ID	  
	if(!i2c_write_byte(reg))		res=0;		//写寄存器地址	  
	if(!i2c_write_byte(byte))		res=0; 	//写数据	 
  	i2c_stop();	  
  	return	res;
}		  					    
//读寄存器
//返回值:读到的寄存器值
u8 i2c_read_reg(u8 slave, u8 reg)
{
	u8 date=0;
	i2c_start(); 				//启动i2c传输
	i2c_write_byte(slave);		//写器件ID	  
  i2c_write_byte(reg);			//写寄存器地址	  
 
	RESTART_DELAY;	   
	//设置寄存器地址后，才是读
	i2c_start();
	i2c_write_byte(slave|0X01);	//发送读命令	  
  date=i2c_read_byte(0);		 	//读取数据
  i2c_stop();
  return date;
}
uchar i2c_read_buf(u8 slave, uchar reg,uchar *date,uchar bytes)		//接收一组数据	返回操作结果
{
	u8 n,res=1;
	i2c_start(); 				//启动i2c传输
	if(!i2c_write_byte(slave))	res=0;		//写器件ID
	if(!i2c_write_byte(reg))	res=0;			//写寄存器地址
	
	RESTART_DELAY;
	//设置寄存器地址后，才是读
	i2c_start();
	if(!i2c_write_byte(slave|0X01))	res=0;	//发送读命令
	for(n=0;n<bytes;n++)
	{
		*(date+n)=i2c_read_byte(bytes-n-1);		 	//读取数据	
	}
  i2c_stop();	
	return res;
}

uchar i2c_write_buf(u8 slave, uchar reg, uchar *date, uchar bytes)	//发送一组数据	返回操作结果
{
	u8 n,res=1;
	i2c_start(); 										//启动i2c传输
	if(!i2c_write_byte(slave))	res=0;	//写器件ID
	if(!i2c_write_byte(reg))		res=0;		//写寄存器地址
	for(n=0;n<bytes;n++)
	{
		if(i2c_write_byte(*(date+n))) res =0;		 	//写数据
	}
	i2c_stop();
	return	res;
}















