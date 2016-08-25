/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
* �ļ���       ��uart.c
* ����         �����ں���
* ��ע         ���ο����ݴ�ѧ�����̺ͷ�˼�����ٷ�������
*
* ʵ��ƽ̨     ��Ұ��kinetis������
* ��汾       ��
* Ƕ��ϵͳ     ��
*
* ����         ��
* �Ա���       ��http://firestm32.taobao.com
* ����֧����̳ ��http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

#include "common.h"
#include "uart.h"
#include "assert.h"
#include "include.h"

volatile struct UART_MemMap *UARTx[6] = {UART0_BASE_PTR, UART1_BASE_PTR, UART2_BASE_PTR, UART3_BASE_PTR, UART4_BASE_PTR, UART5_BASE_PTR}; //�������ָ�����鱣�� UARTx �ĵ�ַ

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_init
*  ����˵������ʼ�����ڣ����ò�����
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*            baud        �����ʣ���9600��19200��56000��115200��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ڹٷ��������޸�
*************************************************************************/
void uart_init (UARTn uratn, u32 baud)
{
    register uint16 sbr, brfa;
    uint8 temp;
    u32 sysclk;     //ʱ��

    /* ���� UART���ܵ� GPIO �ӿ� ����ʱ�� */
    switch(uratn)
    {
      case UART0:
        if(UART0_RX == PTA1)
            PORTA_PCR1 = PORT_PCR_MUX(0x2);      //��PTA1��ʹ��UART0_RXD
        else if(UART0_RX == PTA15)
            PORTA_PCR15 = PORT_PCR_MUX(0x3);     //��PTA15��ʹ��UART0_RXD
        else if(UART0_RX == PTB16)
            PORTB_PCR16 = PORT_PCR_MUX(0x3);     //��PTB16��ʹ��UART0_RXD
        else if(UART0_RX == PTD6)
            PORTD_PCR6 = PORT_PCR_MUX(0x3);      //��PTD6��ʹ��UART0_RXD
        else
            assert_failed(__FILE__, __LINE__);   //���ùܽ�����

        if(UART0_TX == PTA2)
            PORTA_PCR2 = PORT_PCR_MUX(0x2);     //��PTA2��ʹ��UART0_RXD
        else if(UART0_TX == PTA14)
            PORTA_PCR14 = PORT_PCR_MUX(0x3);     //��PTA14��ʹ��UART0_RXD
        else if(UART0_TX == PTB17)
            PORTB_PCR17 = PORT_PCR_MUX(0x3);     //��PTB17��ʹ��UART0_RXD
        else if(UART0_TX == PTD7)
            PORTD_PCR7 = PORT_PCR_MUX(0x3);     //��PTD7��ʹ��UART0_RXD
        else
            assert_failed(__FILE__, __LINE__);  //���ùܽ�����


        SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      //ʹ�� UARTn ʱ��
        break;

      case UART1:
        if(UART1_RX == PTC3)
            PORTC_PCR3 = PORT_PCR_MUX(0x3);     //��PTC3��ʹ��UART1_RXD
        else if(UART1_RX == PTE1)
            PORTE_PCR1 = PORT_PCR_MUX(0x3);     //��PTE1��ʹ��UART1_RXD
        else
            assert_failed(__FILE__, __LINE__);  //���ùܽ�����

        if(UART1_TX == PTC4)
            PORTC_PCR4 = PORT_PCR_MUX(0x3);     //��PTC4��ʹ��UART1_RXD
        else if(UART1_TX == PTE0)
            PORTE_PCR0 = PORT_PCR_MUX(0x3);     //��PTE0��ʹ��UART1_RXD
        else
            assert_failed(__FILE__, __LINE__);  //���ùܽ�����

        SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
        break;

      case UART2:
        PORTD_PCR3 = PORT_PCR_MUX(0x3);         //��PTD3��ʹ��UART2_TXD����
        PORTD_PCR2 = PORT_PCR_MUX(0x3);         //��PTD2��ʹ��UART2_RXD
        SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
        break;

      case UART3:
        if(UART3_RX == PTB10)
            PORTB_PCR10 = PORT_PCR_MUX(0x3);     //��PTB10��ʹ��UART3_RXD
        else if(UART3_RX == PTC16)
            PORTC_PCR16 = PORT_PCR_MUX(0x3);     //��PTC16��ʹ��UART3_RXD
        else if(UART3_RX == PTE5)
            PORTE_PCR5 = PORT_PCR_MUX(0x3);      //��PTE5��ʹ��UART3_RXD
        else 
            assert_failed(__FILE__, __LINE__);                   //���ùܽ�����

        if(UART3_TX == PTB11)
            PORTB_PCR11 = PORT_PCR_MUX(0x3);     //��PTB11��ʹ��UART3_RXD
        else if(UART3_TX == PTC17)
            PORTC_PCR17 = PORT_PCR_MUX(0x3);     //��PTC17��ʹ��UART3_RXD
        else if(UART3_TX == PTE4)
            PORTE_PCR4 = PORT_PCR_MUX(0x3);     //��PTE4��ʹ��UART3_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //���ùܽ�����

        SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;
        break;

      case UART4:
        if(UART4_RX == PTC14)
            PORTC_PCR14 = PORT_PCR_MUX(0x3);     //��PTC14��ʹ��UART4_RXD
        else if(UART4_RX == PTE25)
            PORTE_PCR25 = PORT_PCR_MUX(0x3);     //��PTE25��ʹ��UART4_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //���ùܽ�����

        if(UART4_TX == PTC15)
            PORTC_PCR15 = PORT_PCR_MUX(0x3);     //��PTC15��ʹ��UART4_RXD
        else if(UART4_TX == PTE24)
            PORTE_PCR24 = PORT_PCR_MUX(0x3);     //��PTE24��ʹ��UART4_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //���ùܽ�����

        SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;
        break;

      case UART5:
        if(UART5_RX == PTD8)
            PORTD_PCR8 = PORT_PCR_MUX(0x3);     //��PTD8��ʹ��UART5_RXD
        else if(UART5_RX == PTE9)
            PORTE_PCR9 = PORT_PCR_MUX(0x3);     //��PTE9��ʹ��UART5_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //���ùܽ�����

        if(UART5_TX == PTD9)
            PORTD_PCR9 = PORT_PCR_MUX(0x3);     //��PTD9��ʹ��UART5_RXD
        else if(UART5_TX == PTE8)
            PORTE_PCR8 = PORT_PCR_MUX(0x3);     //��PTE8��ʹ��UART5_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //���ùܽ�����

        SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;
        break;
      default:
        break;
    }


    //���õ�ʱ��Ӧ�ý�ֹ���ͽ���
    UART_C2_REG(UARTx[uratn]) &= ~(UART_C2_TE_MASK  | UART_C2_RE_MASK );

    //���ó�8λ��У��ģʽ
    //���� UART ���ݸ�ʽ��У�鷽ʽ��ֹͣλλ����ͨ������ UART ģ����ƼĴ��� C1 ʵ�֣�
    UART_C1_REG(UARTx[uratn]) = 0;	// ȫ��ֱ��ʹ��Ĭ�����þ��У�����ֱ����0

    //���㲨���ʣ�����0��1ʹ���ں�ʱ�ӣ���������ʹ������ʱ��
    if ((uratn == UART0) | (uratn == UART1))
        sysclk = core_clk_khz * 1000;            //�ں�ʱ��
    else
        sysclk = bus_clk_khz * 1000;  //����ʱ��

    //���� UART ����ͨѶ�����ʡ�ͨ������ UART ģ��Ĳ����ʼĴ���
    sbr = (u16)(sysclk / (baud << 4));

    /* Save off the current value of the UARTx_BDH except for the SBR field */
    temp = UART_BDH_REG(UARTx[uratn]) & ~(UART_BDH_SBR(0x1F));

    UART_BDH_REG(UARTx[uratn]) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
    UART_BDL_REG(UARTx[uratn]) = (u8)(sbr & UART_BDL_SBR_MASK);

    //brfa = (((sysclk*32)/(baud * 16)) - (sbr * 32));
    brfa = (((sysclk << 5) / (baud << 4)) - (sbr << 5));

    /* Save off the current value of the UARTx_C4 register except for the BRFA field */
    temp = UART_C4_REG(UARTx[uratn]) & ~(UART_C4_BRFA(0x1F));

    UART_C4_REG(UARTx[uratn]) = temp |  UART_C4_BRFA(brfa);

    /* �������ͺͽ��� */
    UART_C2_REG(UARTx[uratn]) |= (UART_C2_TE_MASK | UART_C2_RE_MASK );

    //�����Ƿ��������պͷ����жϡ�ͨ������ UART ģ��� C2 �Ĵ�����
    //RIE �� TIE λʵ�֡����ʹ���жϣ���������ʵ���жϷ������
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_getchar
*  ����˵��������ʱ��ȴ����ڽ���һ���ֽ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ����յ����ֽ�
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ٷ�����
*************************************************************************/
char uart_getchar (UARTn uratn)
{
    /* Wait until character has been received */
    while (!(UART_S1_REG(UARTx[uratn]) & UART_S1_RDRF_MASK));

    /* Return the 8-bit data from the receiver */
    return UART_D_REG(UARTx[uratn]);
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_pendchar
*  ����˵��������ʱ��ȴ����ڽ���һ���ֽ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ����յ����ֽ�
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
char uart_pendchar (UARTn uratn, char *ch)
{
    u32 i = 0;

    while(++i < 0xffffff)                                         //ʱ������
    {
        if(UART_S1_REG(UARTx[uratn]) & UART_S1_RDRF_MASK)         //��ѯ�Ƿ���ܵ�����
        {
            *ch  =   UART_D_REG(UARTx[uratn]);                    //���ܵ�8λ������
            return  1;                                            //���� 1 ��ʾ���ճɹ�
        }
    }

    *ch = 0;                                                     //���ղ�����Ӧ������˽�����
    return 0;                                                    //����0��ʾ����ʧ��
}


/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_pendstr
*  ����˵��������ʱ��ȴ����ڽ����ַ���
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ����յ����ֽ�
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
char uart_pendstr (UARTn uratn, char *str)
{
    u32 i = 0;
    while(uart_pendchar(uratn, str + i++));

    return (i <= 1 ? 0 : 1);
}



/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_putchar
*  ����˵�������ڷ���һ���ֽ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ٷ����̣�printf������⺯��
*************************************************************************/
void uart_putchar (UARTn uratn, char ch)
{
    //�ȴ����ͻ�������
    while(!(UART_S1_REG(UARTx[uratn]) & UART_S1_TDRE_MASK));
    //��������
    UART_D_REG(UARTx[uratn]) = (u8)ch;
}



/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_query
*  ����˵������ѯ�Ƿ���ܵ�һ���ֽ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ�1           ���յ�һ���ֽ���
*            0           û�н��յ�
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ٷ�����
*************************************************************************/
int uart_query (UARTn uratn)
{
    return (UART_S1_REG(UARTx[uratn]) & UART_S1_RDRF_MASK);
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_sendN
*  ����˵�������� ����ָ��len���ֽڳ����ַ��� ������ NULL Ҳ�ᷢ�ͣ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*            buff        ���ͻ�����
*            len         ���ͳ���
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void uart_sendN (UARTn uratn, uint8 *buff, uint16 len)
{
    int i;
    for(i = 0; i < len; i++)
    {
        uart_putchar(uratn, buff[i]);
    }
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_sendStr
*  ����˵�������з����ַ���
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*            str         �ַ���
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void uart_sendStr (UARTn uratn, const u8 *str)
{
    while(*str)
    {
        uart_putchar(uratn, *str++);
    }
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_irq_EN
*  ����˵���������ڽ����ж�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void uart_irq_EN(UARTn uratn)
{
    UART_C2_REG(UARTx[uratn]) |= UART_C2_RIE_MASK;    //����UART�����ж�
    enable_irq((uratn << 1) + 45);			        //���������ŵ�IRQ�ж�
}


/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_irq_DIS
*  ����˵�����ش��ڽ����ж�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void uart_irq_DIS(UARTn uratn)
{
    UART_C2_REG(UARTx[uratn]) &= ~UART_C2_RIE_MASK;   //��ֹUART�����ж�
    disable_irq((uratn << 1) + 45);			        //�ؽ������ŵ�IRQ�ж�
}


/************************************************
*  �������ƣ�SCISend_UnsignedInt
*  ����˵����SCI����UnsignedInt����
*  ����˵����UARTnΪ�ڼ������ڣ�pnumΪ����ֵ
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void SCISend_UnsignedInt(UARTn uratn,unsigned int pnum)
{
    if(pnum<10)
    {  uart_putchar(uratn,pnum+'0');}
    else if((10<=pnum)&&(pnum<100))
    {
        uart_putchar(uratn,pnum/10+'0');
        uart_putchar(uratn,pnum%10+'0');
    }
    else if(pnum<=999)
    {
        uart_putchar(uratn,(unsigned char)(pnum/100+'0'));
        uart_putchar(uratn,(unsigned char)(pnum/10%10+'0'));
        uart_putchar(uratn,(unsigned char)(pnum%10+'0'));
    }
    else if(pnum<=9999)
    {
        uart_putchar(uratn,pnum/1000+'0');uart_putchar(uratn,pnum/100%10+'0');
        uart_putchar(uratn,pnum/10%10+'0');uart_putchar(uratn,pnum%10+'0');
    }
    else if(pnum<=99999)
    {
        uart_putchar(uratn,pnum/10000+48);uart_putchar(uratn,pnum/1000%10+48);
        uart_putchar(uratn,pnum/100%10+48);uart_putchar(uratn,pnum/10%10+48);
        uart_putchar(uratn,pnum%10+48);
    }
}


/************************************************
*  �������ƣ�SCISend_Int
*  ����˵����SCI����SCISend_Int����
*  ����˵����UARTnΪ�ڼ������ڣ�pnumΪ����ֵ
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void SCISend_Int(UARTn uratn,int pnum)
{
    if(pnum<0)
    { pnum=-pnum; uart_putchar(uratn,'-');}
    if(pnum<10)
    {  uart_putchar(uratn,pnum+'0');}
    else if((10<=pnum)&&(pnum<100))
    {
        uart_putchar(uratn,pnum/10+'0');uart_putchar(uratn,pnum%10+'0');}
    else if(pnum<=999)
    {
        uart_putchar(uratn,(unsigned char)(pnum/100+'0'));
        uart_putchar(uratn,(unsigned char)(pnum/10%10+'0'));
        uart_putchar(uratn,(unsigned char)(pnum%10+'0'));
    }
    else if(pnum<=9999)
    {
        uart_putchar(uratn,pnum/1000+'0');uart_putchar(uratn,pnum/100%10+'0');
        uart_putchar(uratn,pnum/10%10+'0');uart_putchar(uratn,pnum%10+'0');
    }
    else if(pnum<=99999)
    {
        uart_putchar(uratn,pnum/10000+48);uart_putchar(uratn,pnum/1000%10+48);
        uart_putchar(uratn,pnum/100%10+48);uart_putchar(uratn,pnum/10%10+48);
        uart_putchar(uratn,pnum%10+48);
    }
}

/************************************************
*  �������ƣ�SCISendFloat
*  ����˵����SCI����float����
*  ����˵����UARTnΪ�ڼ������ڣ�fΪ����ֵ
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void SCISendFloat(UARTn uratn,float f)
{				
    if(f<0)
    {
        uart_putchar(uratn,'-');
        f=(-1)*f;
    }
    uart_putchar(uratn,((int)(f))/100%10+'0');
    uart_putchar(uratn,((int)(f))/10%10+'0');
    uart_putchar(uratn,((int)(f))%10+'0');
    uart_putchar(uratn,'.');
    uart_putchar(uratn,((int)(f*10))%10+'0');
    uart_putchar(uratn,((int)(f*100))%10+'0');
    uart_putchar(uratn,' ');
}


/************************************************
*  �������ƣ�SCISend_to_PIDDebug
*  ����˵����SCI���͵�����ʾ������ѧ������λ��
*  ����˵����UARTnΪ�ڼ�������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void SCISend_to_PIDDebug(UARTn uratn)
{ 		
    uart_putchar(uratn,'I');	 		            	
    SCISend_Int(uratn,(int)(0));		//red,m_angle
    uart_putchar(uratn,'|');	
    SCISend_Int(uratn,(int)(0));		//blue,	
    uart_putchar(uratn,'|');
    SCISend_Int(uratn,(int)(0));			//Gyro_valuelight blue
    uart_putchar(uratn,'\r');
    uart_putchar(uratn,'\n');
}




/************************************************
*  �������ƣ�SCI_Send_Datas
*  ����˵����SCI���͵�����ʾ�������Լ�����λ��,AT+UART=115200,1,2,\r\n
*  ����˵����UARTnΪ�ڼ�������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void SCI_Send_Datas(UARTn uratn)
{
  static unsigned short int send_data[3][8] = { { 0 }, { 0 }, { 0 } };
  
  send_data[0][0] = (unsigned short int)(Angle.NowH);//Angle.rotation
  send_data[0][1] = (unsigned short int)(Angle.GoalH);//Angle.pitch
  send_data[0][2] = (unsigned short int)(Angle.G_angleH);//Angle.roll
  send_data[0][3] = (unsigned short int)(Angle.m_angleH);//Angle.yaw
  send_data[0][4] = (unsigned short int)(0);//Angle.rotation);
  send_data[0][5] = (unsigned short int)(0);
  send_data[0][6] = (unsigned short int)(0);
  send_data[0][7] = (unsigned short int)(0);
  
  send_data[1][0] = (unsigned short int)(Gyro_ADC.x);
  send_data[1][1] = (unsigned short int)(Gyro_ADC.y);
  send_data[1][2] = (unsigned short int)(Gyro_ADC.z);
  send_data[1][3] = (unsigned short int)(0);
  send_data[1][4] = (unsigned short int)(0);
  send_data[1][5] = (unsigned short int)(0);
  send_data[1][6] = (unsigned short int)(0);
  send_data[1][7] = (unsigned short int)(0);
  
  send_data[2][0] = (unsigned short int)(Acc_ADC_Data.x);
  send_data[2][1] = (unsigned short int)(Acc_ADC_Data.y);
  send_data[2][2] = (unsigned short int)(Acc_ADC_Data.z);//.z
  send_data[2][3] = (unsigned short int)(0);
  send_data[2][4] = (unsigned short int)(0);
  send_data[2][5] = (unsigned short int)(0);
  send_data[2][6] = (unsigned short int)(0);
  send_data[2][7] = (unsigned short int)(0);
  
  uart_putchar(uratn, 'S');
  uart_putchar(uratn, 'T');
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 8; j++)
    {
      uart_putchar(uratn, (unsigned char)(send_data[i][j] & 0x00ff));
      uart_putchar(uratn, (unsigned char)(send_data[i][j] >> 8u));
    }
  
}


/************************************************
*  �������ƣ�SCI_Send_CCD
*  ����˵����SCI���͵�����ʾ�������Լ���λ����ccd��ʾ
*  ����˵����UARTnΪ�ڼ�������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void SCI_Send_CCD(UARTn uratn, int ccdNum)
{
//  if (ccdNum != 1 && ccdNum != 2)
//    return;
//  uart_putchar(uratn, 255);
//  uart_putchar(uratn, 255);
//  uart_putchar(uratn, ccdNum);//number
//  for (int i = 0; i < 128; i++)
//    if (ccdData1[i] == 255)
//      uart_putchar(uratn, 254);
//    else
//      uart_putchar(uratn, ccdData1[i]);
//    if (ccdNum == 2)
//      for (int i = 0; i < 128; i++)
//	if (ccdData2[i] == 255)
//          uart_putchar(uratn, 254);
//	else
//          uart_putchar(uratn, ccdData2[i]);
}

/************************************************
*  �������ƣ�SCI_Send_fireImage
*  ����˵����//send fireImage camera Data
*  ����˵����UARTnΪ�ڼ�������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void SCI_Send_fireImage(UARTn uratn)
{
	int H = 240, L = 320;
	//send "CAMERA"
	uart_putchar(uratn, 'C');
	uart_putchar(uratn, 'A');
	uart_putchar(uratn, 'M');
	uart_putchar(uratn, 'E');
	uart_putchar(uratn, 'R');
	uart_putchar(uratn, 'A');
	uart_putchar(uratn, (unsigned char)(H >> 8));//send Hang
	uart_putchar(uratn, (unsigned char)(H & 0x00FF));
	uart_putchar(uratn, (unsigned char)(L >> 8));//send Lie
	uart_putchar(uratn, (unsigned char)(L & 0x00FF));
	//send all data
	for (int i = 0; i < H; i++)
	for (int j = 0; j < L / 8; j++)
		uart_putchar(uratn, Image_fire[i][j]);
}


/************************************************
*  �������ƣ�Send_to_DataScope
*  ����˵����SCI���͵�����ʾ������DataScope 
*  ����˵����UARTnΪ�ڼ�������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
float OutData[4] = {0};
void Send_to_DataScope(UARTn uratn)
{
//    delayms(5);
//    OutData[0]=Speed.Car;
//    OutData[1]=Speed.PWM_Integral;
//    OutData[2]=Speed.Goal+500;
//    OutData[3]=Speed.Goal-500;
//    OutPut_Data(uratn);
}
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}
void OutPut_Data(UARTn uratn)
{
    int temp[4] = {0};
    unsigned int temp1[4] = {0};
    unsigned char databuf[10] = {0};
    unsigned char i;
    unsigned short CRC16 = 0;
    for(i=0;i<4;i++)
    {
        temp[i]  = (int)OutData[i];
        temp1[i] = (unsigned int)temp[i];
    }
    for(i=0;i<4;i++) 
    {
        databuf[i*2]   = (unsigned char)(temp1[i]%256);
        databuf[i*2+1] = (unsigned char)(temp1[i]/256);
    }
    CRC16 = CRC_CHECK(databuf,8);
    databuf[8] = CRC16%256;
    databuf[9] = CRC16/256;
    for(i=0;i<10;i++)
        uart_putchar(uratn,databuf[i]);
}