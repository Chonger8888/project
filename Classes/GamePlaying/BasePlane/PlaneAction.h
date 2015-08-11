/********************************************************************
������:		2014/12/08  11:13	
�ļ���: 	E:\Cocos3.3_Project\MyGame\Classes\EnterAction.h 
��  ��:		alex(alex)
�޸��ڣ�	2014/12/08  11:13	

Ŀ  ��:		�з��ɻ��볡�Ķ���
*********************************************************************/
#ifndef _PLANE_ACTION_H_
#define _PLANE_ACTION_H_


#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

extern float LinePos[][2];

class BasePlane;
class EnterAction : public Ref
{
public:
	static EnterAction* create();
	void purge();

	bool init();

	ActionInterval* AddEnemyActionByIdx(int ActionId,BasePlane* pl);

	//��Ԥ���趨�Ĺ켣����(��������)
	ActionInterval* CreateCardinalSplineL2U(BasePlane* pl);

	//��Ԥ���趨�Ĺ켣����(��������)
	ActionInterval* CreateCardinalSplineL2B(BasePlane* pl);

	//��Ԥ���趨�Ĺ켣����(��������)
	ActionInterval* CreateCardinalSplineR2U(BasePlane* pl);

	//��Ԥ���趨�Ĺ켣����(��������)
	ActionInterval* CreateCardinalSplineR2B(BasePlane* pl);
	
	//���·�����Ļ�����Ե
	ActionInterval* CreateMoveToUpEdge(BasePlane* pl);

	//���·��е���Ļ�ϲ������λ��
	ActionInterval* CreateMoveToScreen(BasePlane* pl);

	//����/�ұ߳���ֵ��Ļ���λ��
	ActionInterval* CreateHorizonMoveToScreen(BasePlane* pl);

	//����/�ҳ�ƽ����Ļ�Ĺ̶�λ��
	ActionInterval* CreateMoveToFasteningPos(BasePlane* pl);

	/************************************/
	//���ұ߳��������������ƻ���
	ActionInterval* CreateBezierFromUpToBackAtRight(BasePlane* pl);
	
	//���ұ߳��������������ƻ���
	ActionInterval* CreateBezierFromBottomToBackAtRight(BasePlane* pl);

	//����߳��������������ƻ���
	ActionInterval* CreateBezierFromUpToBackAtLeft(BasePlane* pl);

	//����߳��������������ƻ���
	ActionInterval* CreateBezierFromBottomToBackAtLeft(BasePlane* pl);

	//���ұ߳����������˶������
	ActionInterval* CreateBezierRight2Left(BasePlane* pl);

	//����߳����������˶����ұ�
	ActionInterval* CreateBezierLeft2Right(BasePlane* pl);

	//�������˶����м䣬�ص�����
	ActionInterval* CreateBezierFromLP2RP(BasePlane* pl);

	//�������˶����м䣬�ص�����
	ActionInterval* CreateBezierFromRP2LP(BasePlane* pl);

	//���ұ߳������߲�����
	ActionInterval* CreateBezierWaveAtRight(BasePlane* pl);

	//����߳������߲�����
	ActionInterval* CreateBezierWaveAtLeft(BasePlane* pl);

private:
	Size		m_winSize;
};


#endif