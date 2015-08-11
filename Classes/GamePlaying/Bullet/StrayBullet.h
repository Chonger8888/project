/********************************************************************
������:		2015/01/13  14:46	
�ļ���: 	E:\Cocos3.3_Project\PlaneGame\Classes\Bullet\StrayBullet.h 
��  ��:		alex(alex)
�޸��ڣ�	2015/01/13  14:46	

Ŀ  ��:		�����㣬�ӵײ���������ӵ�������������
*********************************************************************/
#ifndef __StrayBullet_H__
#define __StrayBullet_H__


#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;


class StrayBulletLayer : public Layer
{
public:
	static StrayBulletLayer* create(const float& t);
	CREATE_FUNC(StrayBulletLayer);
	virtual bool init();
	virtual void onEnter();

	//���ó���ʱ��
	void setDuration(const float& t);
	void shootStrayBullet(float t);
	void removeWhenTimeout();

private:
	float		m_duration;
	Size		m_winSize;
};

#endif // __StrayBullet_H__