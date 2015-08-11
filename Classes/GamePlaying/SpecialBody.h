/********************************************************************
������:		2014/12/15  15:21	
�ļ���: 	E:\Cocos3.3_Project\MyGame\Classes\SpecialBody.h 
��  ��:		alex(alex)
�޸��ڣ�	2014/12/15  15:21	

Ŀ  ��:		һЩ����ĸ��徫��
*********************************************************************/
#ifndef __SpecialBody_h__
#define __SpecialBody_h__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "PhysicsUnit.h"
#include "../GameCommon/GameDefine.h"

USING_NS_CC;
USING_NS_CC_EXT;

//Χǽ
class Wall : public PhysicsUnit
{
public:
	static Wall* create(const Size& size,const PhysicsMaterial& pm = PhysicsMaterial(0.0f,1.0f,0.0f));
	void initBody(const Size& size,const PhysicsMaterial& pm );
};


//ȫ����ը
class ExplisonScreen : public PhysicsUnit
{
public:
	ExplisonScreen();
	static ExplisonScreen* create();
	virtual void initBody();
	void initPro();

	virtual void onEnter();

	void setBody(float dt);

	//��N��֮��ɾ��
	void removeExplosion();

	//��ը������Ч
	void StartExp();
	void AddParticle();

	//��ը��Ч
	void PlayExpMusic();
	void StartExpMusic();

	void Zhenlie();

private:
	float					m_duration;
	int						m_idx;
};


//����
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


//��̻���
class SpeedUpProtect : public PhysicsUnit
{
public:
	static SpeedUpProtect* create();
	virtual void initBody();
	virtual void onEnter();
	virtual void onExit();

	//���ֶ���
	void addProtect();

	//β������֡�仯
	void addTail(Node* parent);

	//������Ч
	void addParticle();

private:
	ActionInterval*		m_Tail1;	//β����һ�׶�
	ActionInterval*		m_Tail2;	//β���ڶ��׶�

	Sprite*				m_protect1;	//����1
	Sprite*				m_protect2;	//����2
	Sprite*				m_protect3;	//����3

	ParticleSystemQuad*	m_partc1;
	ParticleSystemQuad*	m_partc2;


};

#endif // SpecialBody_h__