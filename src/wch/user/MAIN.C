/********************************** (C) COPYRIGHT *******************************
* File Name          : MAIN.C
* Author             : WCH
* Version            : V1.0
* Date               : 2016/03/18
* Description        : FOR Taifatech Inc.  
*******************************************************************************/



/******************************************************************************/
/* INCLUDE HEAD FILE  */
#include <string.h>
#include "EXKM.h"	
#include "CH559.H"
#include <stdio.h>

#define DEBUG 1

/******************************************************************************
 * global flag
 */
bit  InitFlag;
/******************************************************************************
 * global  function
 */

#define   FREQ_SYS    48000000                       // ϵͳ��Ƶ12MHz

/*******************************************************************************
* Function Name  : mDelayus(UNIT16 n)
* Description    : us��ʱ����
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/
void    mDelayuS( UINT16 n )                           // ��uSΪ��λ��ʱ
{
	while ( n ) {  // total = 12~13 Fsys cycles, 1uS @Fsys=12MHz
		++ SAFE_MOD;  // 2 Fsys cycles, for higher Fsys, add operation here
#ifdef	FREQ_SYS
#if		FREQ_SYS >= 14000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 16000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 18000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 20000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 22000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 24000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 26000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 28000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 30000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 32000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 34000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 36000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 38000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 40000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 42000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 44000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 46000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 48000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 50000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 52000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 54000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 56000000
		++ SAFE_MOD;
#endif
#endif
		-- n;
	}
}

/*******************************************************************************
* Function Name  : mDelayms(UNIT16 n)
* Description    : ms��ʱ����
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/
void    mDelaymS( UINT16 n )                              // ��mSΪ��λ��ʱ
{
    while ( n )
    {
        mDelayuS( 1000 );
        -- n;
    }
}

/*********************************************************************
 * @fn     ��KM_Timer0Init
 *
 * @brief  ��initialize timer(2ms)
 *
 * @param  ��none
 *
 * @return ��none
 */
void KM_Timer0Init( void )
{
	TMOD |= 0x01;
	TL0 = 0x40;
	TH0 = 0xA2;
	TR0 = 1;
}

/*******************************************************************************
* Function Name  : mInitSTDIO
* Description    : Ϊprintf��getkey���������ʼ������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void	mInitSTDIO( )
{
    UINT32 x;
    UINT8 x2; 

    SM0 = 0;
    SM1 = 1;
    SM2 = 0;                                                                   //����0ʹ��ģʽ1
                                                                               //ʹ��Timer1��Ϊ�����ʷ�����
    RCLK = 0;                                                                  //UART0����ʱ��
    TCLK = 0;                                                                  //UART0����ʱ��
    PCON |= SMOD;
    x = 10 * FREQ_SYS / 57600 / 16;                                            //���������Ƶ��ע��x��ֵ��Ҫ���                            
    x2 = x % 10;
    x /= 10;
    if ( x2 >= 5 ) x ++;                                                       //��������

    TMOD = TMOD & ~ bT1_GATE & ~ bT1_CT & ~ MASK_T1_MOD | bT1_M1;              //0X20��Timer1��Ϊ8λ�Զ����ض�ʱ��
    T2MOD = T2MOD | bTMR_CLK | bT1_CLK;                                        //Timer1ʱ��ѡ��
    TH1 = 0-x;                                                                 //12MHz����,buad/12Ϊʵ�������ò�����
    TR1 = 1;                                                                   //������ʱ��1
    TI = 1;
    REN = 1;                                                                   //����0����ʹ��
}

/*******************************************************************************
* Function Name  : CfgFsys( )
* Description    : CH559ʱ��ѡ������ú���,Ĭ��ʹ���ڲ�����12MHz�����������FREQ_SYS����
                   ����PLL_CFG��CLOCK_CFG���õõ�����ʽ���£�
                   Fsys = (Fosc * ( PLL_CFG & MASK_PLL_MULT ))/(CLOCK_CFG & MASK_SYS_CK_DIV);
                   ����ʱ����Ҫ�Լ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void	CfgFsys( )  
{
    SAFE_MOD = 0x55;                                                           //������ȫģʽ
    SAFE_MOD = 0xAA;                                                 
//  CLOCK_CFG |= bOSC_EN_XT;                                                   //ʹ���ⲿ����                                         
//  CLOCK_CFG &= ~bOSC_EN_INT;                                              
	  CLOCK_CFG &= ~MASK_SYS_CK_DIV;
    CLOCK_CFG |= 6;                                                            //����ϵͳʱ��48MHz
//  CLOCK_CFG |= 8;                                                            //����ϵͳʱ��36MHz
//  CLOCK_CFG |= 10;                                                           //����ϵͳʱ��28.8MHz
//  CLOCK_CFG |= 12;                                                           //����ϵͳʱ��24MHz
//  CLOCK_CFG |= 16;                                                           //����ϵͳʱ��18MHz  	
    SAFE_MOD = 0xFF;                                                           //�رհ�ȫģʽ  
//  ����޸���Ƶ��Ҫͬʱ�޸�FREQ_SYS������������ʱ������׼
}

/*********************************************************************
 * @fn     ��main
 *
 * @brief  ��main function
 *
 * @param  ��none
 *
 * @return ��none
 */	  
void main( void ) 
{
	u8  buf[10];
	u8  Sourcebuf[10];
	u8  len,s,cnt1,cnt2,cnt3,SourceLen;
  CfgFsys();
  mDelaymS(20);//wait FSYS

	mInitSTDIO( );
	KM_Timer0Init( );
	KM_LibInit( );  // initialize usb host
  printf("satr��������n\n");
	while ( 1 ){ 
		KM_ProcessSystem();
		if( TF0 ){
			TF0 = 0;
	    TL0 = 0x30;
	    TH0 = 0x7B;//2ms
			cnt1++;
			cnt2++;
      cnt3++;
		}
		if(( cnt1 >= 1 )){
			cnt1 = 0;
			s = KM_HostGetData( 0,buf,&len,Sourcebuf, &SourceLen);	// get key data
			if( s == 0 ){
// ��ȡ���ݳɹ� buf ������ݣ�len���ݳ��ȡ�	
      for(s=0;s<len;s++){				
      printf("%02X  ",(UINT16)buf[s]);
			}
      printf( "\n" );			
			}
		}
		if(( cnt2 >= 1 )){
			cnt2= 0;
			s = KM_HostGetData( 1,buf,&len,Sourcebuf, &SourceLen );	// get key data
			if( s == 0 ){
// ��ȡ���ݳɹ� buf ������ݣ�len���ݳ��ȡ�
      printf("res %02X \n",(UINT16)s);				
      for(s=0;s<len;s++){				
      printf("%02X  ",(UINT16)buf[s]);
			}		
      printf( "\n" );			
			}
		}
		if( cnt3 >= 250){
      cnt3 = 0;
      KM_TypeQuery( 1,buf,&len );
		  for(s=0;s<len;s=s+2){	
       if(buf[s+1]== DEC_KEY){			
		     printf("ifc %02x key\n ",(UINT16)buf[s]);
		   }	 
       if(buf[s+1]== DEC_MOUSE){			
		     printf("ifc %02x mouse\n ",(UINT16)buf[s]);
		   }
       if(buf[s+1]== DEC_MOUSE_BIOS){			
		     printf("ifc %02x mouse-bios\n ",(UINT16)buf[s]);
		   }	
	    }	
    }
	}
}