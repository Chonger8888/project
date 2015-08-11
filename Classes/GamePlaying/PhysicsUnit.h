/********************************************************************
������:		2014/11/26  11:05	
�ļ���: 	E:\Cocos3.3_Project\MyGame\Classes\PhysicsUnit.h 
��  ��:		alex(alex)
�޸��ڣ�	2014/11/26  11:05	

Ŀ  ��:		���и����������Եľ���Ļ���
*********************************************************************/

#ifndef _PHYSICS_UNIT_H_
#define _PHYSICS_UNIT_H_

#include "cocos2d.h"
#include "Buff.h"
#include "../GameCommon/GameDefine.h"

USING_NS_CC;

//BUFF�ṹ��
typedef struct BuffStruct
{
	BuffId	buffId;
	float	durarion;
	BuffStruct()
	{
		buffId = BuffId::Buff_default;
		durarion = 0.0;
	}
	BuffStruct(BuffId id, float t)
	{
		buffId = id;
		durarion = t;
	}
}BuffStruct;

//��Ʒ����
struct DropReward
{
	float					HpPercent;	//Ѫ���ٷֱ�
	int						nums;			//��������
	DropGoodsType			DropTp;			//������Ʒ����
	DropReward()
	{
		HpPercent = 0.0f;
		nums = 1;
		DropTp = DropGoodsType::DropGoods_Default;
	}

	DropReward(float p, int num, const DropGoodsType& tp)
	{
		HpPercent = p;
		nums = num;
		DropTp = tp;
	}
};

//���徫������
struct SelfPro
{
	UnitId				idx;			//���
	UnitId				belongTo;		//����˭��������˭
	int					defense;		//����
	int					curHP;			//��ǰѪ��
	int					maxHP;			//Ѫ����
	int					isInvincible;	//�Ƿ��޵�	
	int					score;			//���������ķ���
	int					goldsNum;		//���������Ľ����
	int					powerNum;		//����
	std::vector<DropReward>	rewardVec;	//��ʲôѪ������ʲô��Ʒ

	SelfPro()
	{
		idx = UnitId::DefaultIdx;			
		belongTo = UnitId::DefaultIdx;
		defense = 0;
		score = 0;
		goldsNum = 0;
		powerNum = 0;
		maxHP = 1;		
		curHP = maxHP;			
		isInvincible = 0;	
	}
};

//�õ�Ԫ���͸��Է��Ľ�����BUFF
struct ContactPro
{
	UnitId			idx;				//ID��SelfPro��ID��Ӧ
	UnitId			belongTo;			//����˭��������˭
	int				hurts;				//�˺�
	int				effectsid;			//������Чid��0��ʾ����Ч��
	float			ActionSpeed;		//��������ִ���ٶȵİٷֱ�(0.0 - 1.0)
	bool			isTarget;			//�Ƿ񱻵�������
	BuffStruct		buffPro;	//BUFF
	Vec2			pos;		//��ײʱ��λ��
	ContactPro()
	{
		idx = UnitId::DefaultIdx;
		belongTo = UnitId::DefaultIdx;
		hurts = 0;
		effectsid = 0;
		ActionSpeed = 0.0f;
		isTarget = false;
	}
};

class PhysicsUnit : public Sprite,public BuffManager
{
public:
	PhysicsUnit();
	
	void setEdgeLine();
	virtual void onEnter();
	virtual void onExit();

	virtual void initBody(){}
	virtual void FuncAfterEnter(){}
	virtual void PlayMusicWhenContact(){}		//��ײ��Ч
	virtual void PlayMusicWhenDead(){}			//������Ч
	virtual void AnimationWhenDead(){}			//������Ч

	void setUnitId(const UnitId& idx);
	void setBelongTo(const UnitId& idx);
	void setCantContact();

	//ʵʱ�ж�����
	void CheckDead(float dt);
	void setDirectDeath();						//����ǿ������
	void setHpEmpty();							//����Ѫ��Ϊ0
	void setHpEmptyAndInvincible();				//����Ѫ��Ϊ�գ������޵�(��Ҫ����boss)

	//���ڴ����Ƴ��Լ�
	virtual void RemoveMyself();						//Ψһ����������(��ֹ���ɾ�����±���)
	virtual void DoSomethingBeforeDead(){}		//����֮ǰҪ������

	//�ж��Ƿ��������Ļ
	virtual void CheckIsInScreen(float dt);

	//����Ƿ�����Ļ֮��
	virtual void CheckIsOutScreen(float dt);
	
	//�������Բ���
	void setInvincible(int invincible);
	void setMaxBlood(int max);
	
	//���Լ�������ײʱ,�ı��Լ�������
	void ChangeProperty(const ContactPro& rs);

	/************************************************************************/

	//��ײ����Ӱ��
	void ImpactOfHp(const ContactPro& rs);						//Ѫ��Ӱ��
	void ImpactOfSpecialEffects(const int& idx);				//������Ч
	
	virtual void ImpactOfBuff(const ContactPro& rs);	//BUFFӰ��

	void DeathImpactOfDrops();							//�����÷�
	void DeathImpactOfDropReward();						//��Ѫ������ָ������ʱ������ָ����Ʒ
	virtual void OtherDealWhenContact(const ContactPro& rs){}	//������ײӰ��
	virtual void FuncHpEmpty(){}
	/************************************************************************/
	virtual void FuncWhenHpChanged(){}

	float getHpPercent();		//�õ���ǰѪ���İٷֱ�
	bool isLiving();				//Ѫ���Ƿ�Ϊ0

	float getEdgeLine(const int& idx);		//ɾ���ߣ�����Ԫ����4���߰�Χ�Ŀ���ᱻɾ����

	void setContactPro(const ContactPro& rs){m_ContactPro = rs;}
	ContactPro getContactPro(){return m_ContactPro;}

	void setSelfPro(const SelfPro& ua){m_SelfPro = ua;}
	SelfPro getSelfPro(){return m_SelfPro;}

	void CanRemoveAtLine();
	void setInvincibleAtLine(bool b = true);

	virtual void InitEdgeLine();

	//���ٵл�ͳ��
	virtual void EnemyPlaneStatistics(){}

	//��Ϸ��ͣ��ָ�
	void GameIsPause(Ref* obj);
	void GameIsResume(Ref* obj);

	virtual void deadLog(){}
//
//	//<<*****************����ɫ��*********************/
//public:
//	float getHue();
//	void setHue(float hue);	//0~2��
//
//protected:
//	float _hue;
//	GLfloat _mat[3][3];
//
//	bool initWithTexture(cocos2d::Texture2D *texture);
//	bool initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect);
//	virtual bool initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect &rect, bool rotated);
//	virtual bool initWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame);
//
//	void setupDefaultSettings();
//	void initShader();
//	const GLchar* shaderBody();
//	virtual void updateColor();
//	void updateColorMatrix();
//	void updateAlpha();
//	GLfloat getAlpha();
//
//	void hueUniformCallback(cocos2d::GLProgram *p, cocos2d::Uniform *u);
//
//	//*****************����ɫ��*********************>>/

protected:
	Size				m_WinSize;
	bool				m_isInvincibleAtLine;	//��ɾ�������Ƿ��޵�
	bool				m_isNaturalDead;		//�Ƿ���Ȼ����
	bool				m_isDirectDeath;		//ǿ��������������Ч��
	bool				m_isOutScreen;			//�Ƿ������Ļ

	std::vector<float>	m_EdgeLineVec;
	ContactPro			m_ContactPro;		//������Ϣ
	SelfPro				m_SelfPro;			//�Լ�������

	UnitId				m_Killer;			//��ɱ��
	bool				m_isRemoving;
};


#endif