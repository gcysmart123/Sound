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
/* 硬件特性 */

#define	CH375_MAX_DATA_LEN	0x40			/* 最大数据包的长度, 缓冲区的长度 */

/* ********************************************************************************************************************* */
/* 命令代码 */

#define	CMD_GET_IC_VER		0x01			/* 获取芯片及固件版本 */
/* 输出: 版本号( 位7为1, 位6为0, 位5~位0为版本号 ) */
/*           CH375B返回版本号的值为0B7H即版本号为37H */

#define	CMD_ENTER_SLEEP		0x03			/* 进入睡眠状态 */

#define	CMD_SET_USB_SPEED	0x04			/* 设置USB总线速度, 在每次CMD_SET_USB_MODE设置USB工作模式时会自动恢复到12Mbps全速 */
/* 输入: 总线速度代码 */
/*           00H=12Mbps全速FullSpeed（默认值）, 01H=1.5Mbps（仅修改频率）, 02H=1.5Mbps低速LowSpeed */
#define	CMD_SET_SYS_FREQ	CMD_SET_USB_SPEED

#define	CMD_RESET_ALL		0x05			/* 执行硬件复位 */

#define	CMD_CHECK_EXIST		0x06			/* 测试工作状态 */
/* 输入: 任意数据 */
/* 输出: 输入数据的按位取反 */

#define	CMD_GET_TOGGLE		0x0A			/* 获取OUT事务的同步状态 */
/* 输入: 数据1AH */
/* 输出: 同步状态 */
/*           位4为1则OUT事务同步, 否则OUT事务不同步 */

#define	CMD_CHK_SUSPEND		0x0B			/* 设备方式: 设置检查USB总线挂起状态的方式 */
/* 输入: 数据10H, 检查方式 */
/*                    00H=不检查USB挂起, 04H=以50mS为间隔检查USB挂起, 05H=以10mS为间隔检查USB挂起 */

#define	CMD_DELAY_100US		0x0F			/* 并口方式: 延时100uS */
/* 输出: 延时期间输出0, 延时结束输出非0 */

#define	CMD_SET_USB_ID		0x12			/* 设备方式: 设置USB厂商VID和产品PID */
/* 输入: 厂商ID低字节, 厂商ID高字节, 产品ID低字节, 产品ID高字节 */

#define	CMD_SET_USB_ADDR	0x13			/* 设置USB地址 */
/* 输入: 地址值 */

#define	CMD_SET_USB_MODE	0x15			/* 设置USB工作模式 */
/* 输入: 模式代码 */
/*       00H=未启用的设备方式, 01H=已启用的设备方式并且使用外部固件模式, 02H=已启用的设备方式并且使用内置固件模式 */
/*       04H=未启用的主机方式, 05H=已启用的主机方式, 06H=已启用的主机方式并且自动产生SOF包, 07H=已启用的主机方式并且复位USB总线 */
/* 输出: 操作状态( CMD_RET_SUCCESS或CMD_RET_ABORT, 其它值说明操作未完成 ) */

#define	CMD_SET_ENDP2		0x18			/* 设备方式: 设置USB端点0的接收器 */
/* 输入: 工作方式 */
/*           位7为1则位6为同步触发位, 否则同步触发位不变 */
/*           位3~位0为事务响应方式:  0000-就绪ACK, 1110-正忙NAK, 1111-错误STALL */

#define	CMD_SET_ENDP3		0x19			/* 设备方式: 设置USB端点0的发送器 */
/* 输入: 工作方式 */
/*           位7为1则位6为同步触发位, 否则同步触发位不变 */
/*           位3~位0为事务响应方式:  0000~1000-就绪ACK, 1110-正忙NAK, 1111-错误STALL */

#define	CMD_SET_ENDP4		0x1A			/* 设备方式: 设置USB端点1的接收器 */
/* 输入: 工作方式 */
/*           位7为1则位6为同步触发位, 否则同步触发位不变 */
/*           位3~位0为事务响应方式:  0000-就绪ACK, 1110-正忙NAK, 1111-错误STALL */

#define	CMD_SET_ENDP5		0x1B			/* 设备方式: 设置USB端点1的发送器 */
/* 输入: 工作方式 */
/*           位7为1则位6为同步触发位, 否则同步触发位不变 */
/*           位3~位0为事务响应方式:  0000~1000-就绪ACK, 1110-正忙NAK, 1111-错误STALL */

#define	CMD_SET_ENDP6		0x1C			/* 设置USB端点2/主机端点的接收器 */
/* 输入: 工作方式 */
/*           位7为1则位6为同步触发位, 否则同步触发位不变 */
/*           位3~位0为事务响应方式:  0000-就绪ACK, 1101-就绪但不返回ACK, 1110-正忙NAK, 1111-错误STALL */

#define	CMD_SET_ENDP7		0x1D			/* 设置USB端点2/主机端点的发送器 */
/* 输入: 工作方式 */
/*           位7为1则位6为同步触发位, 否则同步触发位不变 */
/*           位3~位0为事务响应方式:  0000-就绪ACK, 1101-就绪但无须应答, 1110-正忙NAK, 1111-错误STALL */

#define	CMD_GET_STATUS		0x22			/* 获取中断状态并取消中断请求 */
/* 输出: 中断状态 */

#define	CMD_UNLOCK_USB		0x23			/* 设备方式: 释放当前USB缓冲区 */

#define	CMD_RD_USB_DATA0	0x27			/* 从当前USB中断的端点缓冲区读取数据块 */
/* 输出: 长度, 数据流 */

#define	CMD_RD_USB_DATA		0x28			/* 从当前USB中断的端点缓冲区读取数据块, 并释放缓冲区, 相当于 CMD_RD_USB_DATA0 + CMD_UNLOCK_USB */
/* 输出: 长度, 数据流 */

#define	CMD_WR_USB_DATA3	0x29			/* 设备方式: 向USB端点0的发送缓冲区写入数据块 */
/* 输入: 长度, 数据流 */

#define	CMD_WR_USB_DATA5	0x2A			/* 设备方式: 向USB端点1的发送缓冲区写入数据块 */
/* 输入: 长度, 数据流 */

#define	CMD_WR_USB_DATA7	0x2B			/* 向USB端点2或者主机端点的发送缓冲区写入数据块 */
/* 输入: 长度, 数据流 */

/* ********************************************************************************************************************* */
/* 操作状态 */

#define	CMD_RET_SUCCESS		0x51			/* 命令操作成功 */
#define	CMD_RET_ABORT		0x5F			/* 命令操作失败 */

/* ********************************************************************************************************************* */
/* USB中断状态 */

#ifndef	USB_INT_EP0_SETUP

/* 以下状态代码为特殊事件中断, 如果通过CMD_CHK_SUSPEND启用USB总线挂起检查, 那么必须处理USB总线挂起和睡眠唤醒的中断状态 */
#define	USB_INT_USB_SUSPEND	0x05			/* USB总线挂起事件 */
#define	USB_INT_WAKE_UP		0x06			/* 从睡眠中被唤醒事件 */

/* 以下状态代码0XH用于USB设备方式 */
/*   位3-位2指示当前事务, 00=OUT, 10=IN, 11=SETUP */
/*   位1-位0指示当前端点, 00=端点0, 01=端点1, 10=端点2, 11=USB总线复位 */
#define	USB_INT_EP1_OUT		0x01			/* USB端点1的OUT */
#define	USB_INT_EP1_IN		0x09			/* USB端点1的IN */
#define	USB_INT_EP2_OUT		0x02			/* USB端点2的OUT */
#define	USB_INT_EP2_IN		0x0A			/* USB端点2的IN */
/* USB_INT_BUS_RESET	0x0000XX11B */		/* USB总线复位 */
#define	USB_INT_BUS_RESET1	0x03			/* USB总线复位 */
#define	USB_INT_BUS_RESET2	0x07			/* USB总线复位 */
#define	USB_INT_BUS_RESET3	0x0B			/* USB总线复位 */
#define	USB_INT_BUS_RESET4	0x0F			/* USB总线复位 */

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
