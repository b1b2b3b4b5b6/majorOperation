#include "adc.h"

u16 tpm;
u16	temperature;
u16 power;

void adc_gpio_init(void)
{
	gpio_init_pin(PIN_TEMP,GPIO_Mode_AIN);
	gpio_init_pin(PIN_POWER,GPIO_Mode_AIN);
}
void adc_init()
{ 	
	adc_gpio_init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	ADC_DeInit(ADC1);  //��λADC1 
	
	ADC_InitTypeDef ADC_InitStructure; 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 
}	

u16 ch_sample(u8 channel)
{
	ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_239Cycles5 );			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));

	return ADC_GetConversionValue(ADC1);	
}
void adc_sample(void)   
{
	temperature = ch_sample(CH_TEMP)*100/0xfff;
	power = ch_sample(CH_POWER)*100/0xfff;
}

