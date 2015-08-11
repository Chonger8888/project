#ifndef __GLOBAL_SCHEDULE_H__
#define __GLOBAL_SCHEDULE_H__

#include "cocos2d.h"
USING_NS_CC;

/************************************************************************/
/* ȫ�ֶ�ʱ��                                                           */
/************************************************************************/

#define SCHEDULE CCDirector::getInstance()->getScheduler() 


class GlobalSchedule: public Ref 
{ 
public: 
	// ��ʼȫ�ֶ�ʱ�� fInterval: ʱ���� ; fDelay: �ӳ����� 
	static void start(float fInterval = 0.0f, float fDelay = 0.0f); 
	// ֹͣȫ�ֶ�ʱ�� 
	static float stop(); 
	// ȫ�ֶ�ʱ����ͣ 
	static void pause(); 
	// ȫ�ֶ�ʱ����ͣ�ָ� 
	static void resume(); 

	// ȫ�ֶ�ʱ�����߼�ʵ�� 
	void globalUpdate(float t); 

private: 
	// ���캯��˽�л���ֻ��ͨ�� start ������ȫ�ֶ�ʱ�� 
	GlobalSchedule(float fInterval, float fDelay); 
	~GlobalSchedule(); 

	// ��̬�������ֵ��� 
	static GlobalSchedule* m_pSchedule; 

	static float m_countTime;
	static float m_fInterval;
}; 


#endif