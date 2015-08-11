/********************************************************************
������:		2014/11/14  11:03	
�ļ���: 	E:\Cocos3.3_Project\MyGame\Classes\RollingBg.h 
��  ��:		alex(alex)
�޸��ڣ�	2014/11/14  11:03	

Ŀ  ��:		��������
*********************************************************************/
#ifndef _ROLLING_BG_H_
#define _ROLLING_BG_H_


#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GameBg : public Layer
{
public:
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void update(float delta);
	void refreshSpeed(float dt);

	virtual float SpeedRate();
	virtual bool isFlip();				//�Ƿ�Ҫ��ת,Ĭ�ϲ���Ҫ
	virtual void SpeedUpEase(){};		//����ʱ������Ч��

	void addBgAndRolling();

	void Shake(Ref* obj);

	void SwitchBg(int id);

protected:
	CC_SYNTHESIZE(float, m_curSpeed, CurSpeed);
	CC_SYNTHESIZE(Sprite*, m_Bg1, Bg1);
	CC_SYNTHESIZE(Sprite*, m_Bg2, Bg2);
	CC_SYNTHESIZE(int, m_curBgId, CurBgId);

	Size		m_size;
	Texture2D*	m_BgTexture1;
	Texture2D*	m_BgTexture2;
	Texture2D*	m_BgTexture3;
};

//��������
class RollingBg : public GameBg
{
public:
	CREATE_FUNC(RollingBg)

	virtual float SpeedRate();
	virtual bool isFlip();				//�Ƿ�Ҫ��ת,Ĭ�ϲ���Ҫ
	virtual void SpeedUpEase();		//����ʱ������Ч��

	virtual void onEnter();
};


//�����Ʋ�
class RollingClouds : public GameBg
{
public:
	CREATE_FUNC(RollingClouds);

	virtual float SpeedRate();
	virtual bool isFlip();				//�Ƿ�Ҫ��ת,Ĭ�ϲ���Ҫ
	virtual void SpeedUpEase();;		//����ʱ������Ч��
};

#endif
