/********************************************************************
������:		2015/02/04  14:54	
�ļ���: 	E:/Cocos3.3_Project/PlaneGame/Classes/Common/BossTimeOut.h 
��  ��:		alex(alex)
�޸��ڣ�	2015/02/04  14:54	

Ŀ  ��:		BOSS��ʱû�д���
*********************************************************************/
#ifndef __BossTimeOut_H__
#define __BossTimeOut_H__


#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

//BOSS��ʱ������ʯ
class BossTimeOut : public Node
{
public:
	BossTimeOut();
	CREATE_FUNC(BossTimeOut);

	void setMaxTime(const float& time);
	void setInterval(const float& time);

	virtual void onEnter();

	void StartTiming();

	//����ʱ
	void TimeCutdown();

	//ʱ�䵽�ˣ�ִ�е�����ʯ����
	void TimeOut();

	//������ʯ
	void Fallingstone();

	Vec2 randPos();

	void speedUp(float dt);

protected:
	Node*		m_delegate;
	float		m_maxTime;
	float		m_interval;
	int			m_curRandPos;
};

#endif // __BossTimeOut_H__