#ifndef __WCH372_H
#define __WCH372_H

#include "STC12C5A60S2.H"
#include "timer.h"

#define CH372_CMD_PORT P2
#define CH372_DAT_PORT P2


sbit CH372_A0 = P4 ^ 3;
sbit CH372_INT = P3 ^ 2;
sbit CH372_WR = P3 ^ 6;
sbit CH372_RD = P3 ^ 7;
sbit CH372_CS = P4 ^ 4;

/* ********************************************************************************************************************* */
/* Ӳ������ */

#define	CH375_MAX_DATA_LEN	0x40			/* ������ݰ��ĳ���, �������ĳ��� */

/* ********************************************************************************************************************* */
/* ������� */

#define	CMD_GET_IC_VER		0x01			/* ��ȡоƬ���̼��汾 */
/* ���: �汾��( λ7Ϊ1, λ6Ϊ0, λ5~λ0Ϊ�汾�� ) */
/*           CH375B���ذ汾�ŵ�ֵΪ0B7H���汾��Ϊ37H */

#define	CMD_ENTER_SLEEP		0x03			/* ����˯��״̬ */

#define	CMD_SET_USB_SPEED	0x04			/* ����USB�����ٶ�, ��ÿ��CMD_SET_USB_MODE����USB����ģʽʱ���Զ��ָ���12Mbpsȫ�� */
/* ����: �����ٶȴ��� */
/*           00H=12Mbpsȫ��FullSpeed��Ĭ��ֵ��, 01H=1.5Mbps�����޸�Ƶ�ʣ�, 02H=1.5Mbps����LowSpeed */
#define	CMD_SET_SYS_FREQ	CMD_SET_USB_SPEED

#define	CMD_RESET_ALL		0x05			/* ִ��Ӳ����λ */

#define	CMD_CHECK_EXIST		0x06			/* ���Թ���״̬ */
/* ����: �������� */
/* ���: �������ݵİ�λȡ�� */

#define	CMD_GET_TOGGLE		0x0A			/* ��ȡOUT�����ͬ��״̬ */
/* ����: ����1AH */
/* ���: ͬ��״̬ */
/*           λ4Ϊ1��OUT����ͬ��, ����OUT����ͬ�� */

#define	CMD_CHK_SUSPEND		0x0B			/* �豸��ʽ: ���ü��USB���߹���״̬�ķ�ʽ */
/* ����: ����10H, ��鷽ʽ */
/*                    00H=�����USB����, 04H=��50mSΪ������USB����, 05H=��10mSΪ������USB���� */

#define	CMD_DELAY_100US		0x0F			/* ���ڷ�ʽ: ��ʱ100uS */
/* ���: ��ʱ�ڼ����0, ��ʱ���������0 */

#define	CMD_SET_USB_ID		0x12			/* �豸��ʽ: ����USB����VID�Ͳ�ƷPID */
/* ����: ����ID���ֽ�, ����ID���ֽ�, ��ƷID���ֽ�, ��ƷID���ֽ� */

#define	CMD_SET_USB_ADDR	0x13			/* ����USB��ַ */
/* ����: ��ֵַ */

#define	CMD_SET_USB_MODE	0x15			/* ����USB����ģʽ */
/* ����: ģʽ���� */
/*       00H=δ���õ��豸��ʽ, 01H=�����õ��豸��ʽ����ʹ���ⲿ�̼�ģʽ, 02H=�����õ��豸��ʽ����ʹ�����ù̼�ģʽ */
/*       04H=δ���õ�������ʽ, 05H=�����õ�������ʽ, 06H=�����õ�������ʽ�����Զ�����SOF��, 07H=�����õ�������ʽ���Ҹ�λUSB���� */
/* ���: ����״̬( CMD_RET_SUCCESS��CMD_RET_ABORT, ����ֵ˵������δ��� ) */

#define	CMD_SET_ENDP2		0x18			/* �豸��ʽ: ����USB�˵�0�Ľ����� */
/* ����: ������ʽ */
/*           λ7Ϊ1��λ6Ϊͬ������λ, ����ͬ������λ���� */
/*           λ3~λ0Ϊ������Ӧ��ʽ:  0000-����ACK, 1110-��æNAK, 1111-����STALL */

#define	CMD_SET_ENDP3		0x19			/* �豸��ʽ: ����USB�˵�0�ķ����� */
/* ����: ������ʽ */
/*           λ7Ϊ1��λ6Ϊͬ������λ, ����ͬ������λ���� */
/*           λ3~λ0Ϊ������Ӧ��ʽ:  0000~1000-����ACK, 1110-��æNAK, 1111-����STALL */

#define	CMD_SET_ENDP4		0x1A			/* �豸��ʽ: ����USB�˵�1�Ľ����� */
/* ����: ������ʽ */
/*           λ7Ϊ1��λ6Ϊͬ������λ, ����ͬ������λ���� */
/*           λ3~λ0Ϊ������Ӧ��ʽ:  0000-����ACK, 1110-��æNAK, 1111-����STALL */

#define	CMD_SET_ENDP5		0x1B			/* �豸��ʽ: ����USB�˵�1�ķ����� */
/* ����: ������ʽ */
/*           λ7Ϊ1��λ6Ϊͬ������λ, ����ͬ������λ���� */
/*           λ3~λ0Ϊ������Ӧ��ʽ:  0000~1000-����ACK, 1110-��æNAK, 1111-����STALL */

#define	CMD_SET_ENDP6		0x1C			/* ����USB�˵�2/�����˵�Ľ����� */
/* ����: ������ʽ */
/*           λ7Ϊ1��λ6Ϊͬ������λ, ����ͬ������λ���� */
/*           λ3~λ0Ϊ������Ӧ��ʽ:  0000-����ACK, 1101-������������ACK, 1110-��æNAK, 1111-����STALL */

#define	CMD_SET_ENDP7		0x1D			/* ����USB�˵�2/�����˵�ķ����� */
/* ����: ������ʽ */
/*           λ7Ϊ1��λ6Ϊͬ������λ, ����ͬ������λ���� */
/*           λ3~λ0Ϊ������Ӧ��ʽ:  0000-����ACK, 1101-����������Ӧ��, 1110-��æNAK, 1111-����STALL */

#define	CMD_GET_STATUS		0x22			/* ��ȡ�ж�״̬��ȡ���ж����� */
/* ���: �ж�״̬ */

#define	CMD_UNLOCK_USB		0x23			/* �豸��ʽ: �ͷŵ�ǰUSB������ */

#define	CMD_RD_USB_DATA0	0x27			/* �ӵ�ǰUSB�жϵĶ˵㻺������ȡ���ݿ� */
/* ���: ����, ������ */

#define	CMD_RD_USB_DATA		0x28			/* �ӵ�ǰUSB�жϵĶ˵㻺������ȡ���ݿ�, ���ͷŻ�����, �൱�� CMD_RD_USB_DATA0 + CMD_UNLOCK_USB */
/* ���: ����, ������ */

#define	CMD_WR_USB_DATA3	0x29			/* �豸��ʽ: ��USB�˵�0�ķ��ͻ�����д�����ݿ� */
/* ����: ����, ������ */

#define	CMD_WR_USB_DATA5	0x2A			/* �豸��ʽ: ��USB�˵�1�ķ��ͻ�����д�����ݿ� */
/* ����: ����, ������ */

#define	CMD_WR_USB_DATA7	0x2B			/* ��USB�˵�2���������˵�ķ��ͻ�����д�����ݿ� */
/* ����: ����, ������ */

/* ********************************************************************************************************************* */
/* ����״̬ */

#define	CMD_RET_SUCCESS		0x51			/* ��������ɹ� */
#define	CMD_RET_ABORT		0x5F			/* �������ʧ�� */

/* ********************************************************************************************************************* */
/* USB�ж�״̬ */

#ifndef	USB_INT_EP0_SETUP

/* ����״̬����Ϊ�����¼��ж�, ���ͨ��CMD_CHK_SUSPEND����USB���߹�����, ��ô���봦��USB���߹����˯�߻��ѵ��ж�״̬ */
#define	USB_INT_USB_SUSPEND	0x05			/* USB���߹����¼� */
#define	USB_INT_WAKE_UP		0x06			/* ��˯���б������¼� */

/* ����״̬����0XH����USB�豸��ʽ */
/*   λ3-λ2ָʾ��ǰ����, 00=OUT, 10=IN, 11=SETUP */
/*   λ1-λ0ָʾ��ǰ�˵�, 00=�˵�0, 01=�˵�1, 10=�˵�2, 11=USB���߸�λ */
#define	USB_INT_EP1_OUT		0x01			/* USB�˵�1��OUT */
#define	USB_INT_EP1_IN		0x09			/* USB�˵�1��IN */
#define	USB_INT_EP2_OUT		0x02			/* USB�˵�2��OUT */
#define	USB_INT_EP2_IN		0x0A			/* USB�˵�2��IN */
/* USB_INT_BUS_RESET	0x0000XX11B */		/* USB���߸�λ */
#define	USB_INT_BUS_RESET1	0x03			/* USB���߸�λ */
#define	USB_INT_BUS_RESET2	0x07			/* USB���߸�λ */
#define	USB_INT_BUS_RESET3	0x0B			/* USB���߸�λ */
#define	USB_INT_BUS_RESET4	0x0F			/* USB���߸�λ */

#endif

extern void ch372_init();
extern void ch372_wr_cmd_port(unsigned char cmd);
extern void ch372_wr_dat_port(unsigned char dat);
extern unsigned char ch372_rd_dat_port();
extern void wch372_send1byte(unsigned char function, unsigned char uesrdata);
extern void wch372_send2byte(unsigned char function, unsigned char uesrdata1, unsigned char uesrdata2);
extern void wch372_send3byte(unsigned char function, unsigned char uesrdata1, unsigned char uesrdata2, unsigned char uesrdata3);

extern void delay50ms();

#endif
