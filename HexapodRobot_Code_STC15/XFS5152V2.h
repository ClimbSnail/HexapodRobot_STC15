#ifndef __XFS5152V2_H
#define __XFS5152V2_H

#include "stc15w.h"
#include "config.h"

//��������ʽ  0x00~0x03
#define GB2312	0x00
#define GBK	0x01
#define BIG5	0x02
#define Unicode 	0x03


/**************оƬ��������*********************/
code uchar XFS_StopCom[];//ֹͣ�ϳ�
code uchar XFS_SuspendCom[];//��ͣ�ϳ�
code uchar XFS_RecoverCom[];//�ָ��ϳ�
code uchar XFS_ChackCom[];//״̬��ѯ  оƬ���з���0x4F
code uchar XFS_PowerDownCom[];//����POWER DOWN ״̬����

extern void XFS_FrameInfo(uchar *dat);//�ı��ϳɺ���
extern void YSOrder(unsigned char order);//������������



#endif