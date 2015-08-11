/********************************************************************
创建于:		2014/12/15  15:21	
文件名: 	E:\Cocos3.3_Project\MyGame\Classes\SpecialBody.h 
作  者:		alex(alex)
修改于：	2014/12/15  15:21	

目  的:		一些特殊的刚体精灵
*********************************************************************/
#ifndef __SpecialBody_h__
#define __SpecialBody_h__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "PhysicsUnit.h"
#include "../GameCommon/GameDefine.h"

USING_NS_CC;
USING_NS_CC_EXT;

//围墙
class Wall : public PhysicsUnit
{
public:
	static Wall* create(const Size& size,const PhysicsMaterial& pm = PhysicsMaterial(0.0f,1.0f,0.0f));
	void initBody(const Size& size,const PhysicsMaterial& pm );
};


//全屏爆炸
class ExplisonScreen : public PhysicsUnit
{
public:
	ExplisonScreen();
	static ExplisonScreen* create();
	virtual void initBody();
	void initPro();

	virtual void onEnter();

	void setBody(float dt);

	//在N秒之后删除
	void removeExplosion();

	//爆炸粒子特效
	void StartExp();
	void AddParticle();

	//爆炸音效
	void PlayExpMusic();
	void StartExpMusic();

	void Zhenlie();

private:
	float					m_duration;
	int						m_idx;
};


//护盾
class PlaneProtect : public PhysicsUnit
{
public:
	static PlaneProtect* create(float t);
	virtual void initBody();
	virtual void onEnter();
	virtual void onExit();

	virtual void OtherDealWhenContact(const ContactPro& rs);
	void removeEff(Node* node);

	static void ContactEff(Node* node, float radius, const Vec2& pos, float scele = 1.0f, const Vec2& offset = Vec2(0,0));

	void HpLoadbar();

	static void ClearScreen();

private:
	float		m_time;
	ProgressTimer* m_HpLoadbar;
};


//冲刺护盾
class SpeedUpProtect : public PhysicsUnit
{
public:
	static SpeedUpProtect* create();
	virtual void initBody();
	virtual void onEnter();
	virtual void onExit();

	//护罩抖动
	void addProtect();

	//尾部序列帧变化
	void addTail(Node* parent);

	//粒子特效
	void addParticle();

private:
	ActionInterval*		m_Tail1;	//尾部第一阶段
	ActionInterval*		m_Tail2;	//尾部第二阶段

	Sprite*				m_protect1;	//护罩1
	Sprite*				m_protect2;	//护罩2
	Sprite*				m_protect3;	//护罩3

	ParticleSystemQuad*	m_partc1;
	ParticleSystemQuad*	m_partc2;


};

#endif // SpecialBody_h__