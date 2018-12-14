
#include "N76E003.h"
#include "WS281X.h"
#include "SFR_Macro.h"
#include "define.h"
#include "intrins.h"
#include "string.h"
#include "I2C.h"
#include "buzzer.h"
/* Private variables ---------------------------------------------------------*/

bit BIT_TMP;

#define I2C_CLOCK               13									//iic��������
#define EEPROM_SLA              0x40								//�ӻ���ַ	

U8 xdata data_received[64];

U8 xdata data_store[256];
U8 ptr;
U8 data_num = 0,num = 0,data_flag = 0,ll = 0;
U8 Rxflag=0;
U8 begin_flag = 0;
U8 led_number = 0;
U8 led_lck = 0;

void Init_I2C(void)
{
	P13_OpenDrain_Mode;
	P14_OpenDrain_Mode;		
    SDA = 1;                                //set SDA and SCL pins high
    SCL = 1;   	
    set_P0SR_6;                             //set SCL (P06) is  Schmitt triggered input select. 	
    set_EI2C;                               //enable I2C interrupt by setting IE1 bit 0
    set_EA;
    I2ADDR = EEPROM_SLA;                    //define own slave address
    set_I2CEN;                              //enable I2C circuit
    set_AA;

	memset(data_received,0x00,64);//���㣬��ֹ���洦������ʱ��ȡ�����ݳ���
	memset(data_store,0x00,256);	

}

void i2c_task(void)
{	
	if( begin_flag == 1)	 //�����������ݽ�����־
	{			
				led_lck = data_received[1];	
/*******************************************************************************
				�������ж��������жϣ���ִ�к���������˳���������ʼ���յ�����
*******************************************************************************/
				if(data_received[1]==0xa1 && data_received[2]==0x01)
				{
						if( data_received[3]>=0x04 )
							data_received[3]=0x04;
						buzzer_on();
						data_num = 0;
						memset(data_received,0x00,64);		
						begin_flag = 0;	
						return ;
				}
				if(data_received[1]==0xa1 && data_received[2]==0x00)
				{
						buzzer_off();
						data_num = 0;
						memset(data_received,0x00,64);		
						begin_flag = 0;	
						return ;
				}
/*******************************************************************************
				�����ݽ���
*******************************************************************************/
				data_num = 0;
				for( num=0;num<22;num++ )
				{
					data_store[ptr] = data_received[num+3];	
					ptr++;	
				}	
#if 0
				for( num=0;num<22;num++ )
				{
					Send_Data_To_UART0(data_store[num]);		
				}
#endif				
/*******************************************************************************
				�����ݽ��ս���������Ϊdata_received[2]-0x80
*******************************************************************************/
				if(data_received[2]>0x80)
				{
					Rxflag = 1;
					led_number = data_received[2]-0x80;
					ptr = 0;
				}
				memset(data_received,0x00,64);		
				begin_flag = 0;	
	}																			
}

	
void I2C_ISR(void) interrupt 6
{
    switch (I2STAT)
    {
#if 0
        case 0x00:
            STO = 1;
            break;
#endif
        case 0x60://ADDR ACK
//						memset(data_received,0x00,sizeof(data_received));
			systatus = 1;
            AA = 1;
            break;
#if 0				
        case 0x68:
						P02 = 0;
						
            while(1);
            break;
#endif
        case 0x80:	//DATA ACK																			//��������
            data_received[data_num] = I2DAT;
						data_num++;					
            AA = 1;
            break;
#if 0
        case 0x88:	//DATA ERROR																			//����ʱ����
            data_received[data_num] = I2DAT;
            data_num = 0;
						clr_GPIO1;
            AA = 1;
            break;
#endif				       
/*******************************************************************************
��������ֹͣλ������ÿ�ζ���ִ�е�����ʱ���жϲ��������Ը�Ϊ�ж�����ֵ
*******************************************************************************/
				case 0xA0:	//REC END		
						begin_flag = 1;	
            AA = 1;						
            break;
#if 0 
				case 0xA8:	//ADDR BACK ACK											
						ll = 0;
						I2DAT = data_store[ll];   
						ll++;
            AA = 1;
            break;
#endif	  
#if 0				
        case 0xB8:  //DATA BACK ACK 
				
						I2DAT = data_store[ll];
            ll++;
            AA = 1;
            break;
#endif
        case 0xC0:	//NACK						
						data_num = 0;		//Ϊ��һ��������׼��
						memset(data_received,0x00,sizeof(data_received));	
						memset(data_store,0x00,sizeof(data_store));				
						ptr = 0;
						ll = 0;
            AA = 1;
            break; 
#if 0
        case 0xC8:	
            AA = 1;
            break;
#endif
		default:		
						set_STO;
//((void(code*)(void))0x0000)();	
		   break;		
    }
    SI = 0;   
    while(SI != 0)
    {
				if(I2STAT == 0x00)
        {
            set_STO;
        }
        SI = 0;
        if(SI != 0)
        {
            clr_I2CEN;
            set_I2CEN;
            clr_SI;
            //clr_I2CEN
        } 
										
    }   
    while(STO);
}







/************************ (C) COPYRIGHT QITAS ******************END OF FILE****/


