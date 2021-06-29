#ifndef __XFS5152V2_H
#define __XFS5152V2_H

#include "stc15w.h"
#include "config.h"

//定义编码格式  0x00~0x03
#define GB2312 0x00
#define GBK 0x01
#define BIG5 0x02
#define Unicode 0x03

/**************芯片设置命令*********************/
code uchar XFS_StopCom[];      //停止合成
code uchar XFS_SuspendCom[];   //暂停合成
code uchar XFS_RecoverCom[];   //恢复合成
code uchar XFS_ChackCom[];     //状态查询  芯片空闲返回0x4F
code uchar XFS_PowerDownCom[]; //进入POWER DOWN 状态命令

extern void XFS_FrameInfo(uchar *dat);    //文本合成函数
extern void YSOrder(unsigned char order); //语音设置命令

#endif