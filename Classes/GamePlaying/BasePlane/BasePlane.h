/********************************************************************
������:		2014/11/13  16:19	
�ļ���: 	E:\Cocos3.3_Project\MyGame\Classes\MyPlane.h 
��  ��:		alex(alex)
�޸��ڣ�	2014/11/13  16:19	

Ŀ  ��:		�ɻ�
*********************************************************************/
#ifndef _BASEPLANE_H_
#define _BASEPLANE_H_


#include "cocos2d.h"
#include "../PhysicsUnit.h"
#include "GameCommon/NewAudio.h"
#include "../../../cocos2d/cocos/editor-support/cocostudio/CCArmature.h"
#include "../../GameCommon/GameDefine.h"
#include "../Weapon/WeaponFactory.h"
#include "PlaneSkin.h"
#include "../Weapon/weapon.h"
#include "Config/TrackConfig.h"

USING_NS_CC;
using namespace cocostudio;

//����֡�ɻ�
#define CREATE_PLANE_FRAME(__TYPE__) \
static __TYPE__* createWithAnimation( Animate* animat ) \
{ \
	__TYPE__* plane = new (std::nothrow) __TYPE__; \
	if (plane && plane->initWithFile("weapon.png")) \
	{ \
		plane->InitSkinWithAimation(animat); \
		plane->initBody(); \
		plane->ShowHpBar(); \
		plane->autorelease(); \
		return plane; \
	} \
	else \
	{ \
		CC_SAFE_DELETE(plane); \
		return nullptr; \
	} \
} \

class BossType1AI;
class Weapon;
class BossType2AI;
class BossType3AI;
class PlaneAI;
class BasePlane;
class TrackManager;

class HpBar : public Sprite
{
public:
	HpBar();
	static HpBar* create(BasePlane* plane);
	virtual void onEnter();

	//ˢ��Ѫ��
	void RefreshHpBar(float percent);

	//����λ��
	void setHpBarOffset(const Vec2& offset);

	//����Ѫ����ʾ/����
	void SetHpBarVisible(bool visible);

	//����Ѫ������
	void setHpBarLengthScale(float scale);

	void setLifeNum(int lifeNum);

	void NextLife(int nuwNum);

	void changeColor();

private:
	//��ʾѪ��
	void ShowHpBar();

private:
	ProgressTimer*		m_HpBar;			//Ѫ��������
	BasePlane*			m_Plane;			//Ҫ��ʾѪ���ķɻ�
	CC_SYNTHESIZE(int,	m_MaxLife, MaxLife);//��������
	int					m_LeftLife;			//ʣ������
	Color3B				m_BgColor;
	Color3B				m_BarColor;
};

class BasePlane : public PhysicsUnit 
{
public:
	BasePlane();
	virtual void onExit();
	virtual void RemoveMyself();

	//�л�ͳ��
	virtual void EnemyPlaneStatistics();

	//ˢ��Ѫ��
	virtual void FuncWhenHpChanged();

	//������ײ˧ѡ
	void setBitmask(const int& cate,const int& collision,const int& test);

	//����Ѫ����
	void setMaxHP(int hp);

	//�����˺�
	void setBltHurts(int hurts);

	//���÷���
	void setDefense(int defense);

	//����������Ч
	void setDeadMusic(const std::string& file);

	//3.�������������빥������
	void setSelfAndAttact(const SelfPro& myPro, const ContactPro& attactPro);

	//���ϵ���
	void AddOneWp(Weapon* weapon,WeaponType wt);

	//���϶��
	void AddWpVec(std::vector<Weapon*> wpVec,WeaponType wt);

	//ɾ��ĳ������
	void RemoveOneWeapon(Weapon* weapon);

	//�ָ�ԭ����ͨ����
	void resumeNorWp();

	//�Ƴ���������
	void clearWeapon();

	void setShootAble(bool b);
	void setNeedFlyToMainPl(bool b);
	virtual void AnimationWhenDead();
	virtual void OtherDealWhenContact(const ContactPro& rs);
	virtual void PlayMusicWhenDead();			//������Ч
	virtual void DoSomethingBeforeDead();

	void SetFuncAfterEnterWin(Ref* obj, SEL_CallFunc func);			//�볡��ִ�еĹ���

	void FlyToMainPlaneWhenSpeedUp();
	void setShootMusicAble(bool b);

	virtual Vec2 getArmatPosOffset();
	virtual Size getSkinSize();

	void addRewards(float HpPct, int wardNum, DropGoodsType dgt);
	void runSelfAction();

	//�������÷���
	void setFireConfig(int danmuId, float interval);
	void StartFire();

protected:

	void ShowHpBar();
	void StartShoot();
	virtual void CheckIsInScreen(float dt);

	//�ö�ʱ���������޵н���ʱ�䣨�ö������ܻᱻֹͣstopallActions��
	void WudiOver(float dt);

	void runEnterAnimation();

	virtual void initBody();

	virtual void FuncAfterEnter();

	void removePartical(Node* node);

	void CheckIsSpeedUp(float dt);
	void FlyToMainPlane(float dt);

protected:
	std::vector<Weapon*>			m_SpecialWpVec;							//�����������ɵ���
	std::vector<Weapon*>			m_NorWpVec;								//��ͨ�������ڽӵ�����buff��ʱ�򣬻���ͣ����
	std::vector<Weapon*>			m_TempWpVec;							//����Ψһbuff�����ص���ֻ����ʱ��

	CC_SYNTHESIZE(ActionInterval*, m_EnterAnimation, EnterAnimation);		//�볡����
	CC_SYNTHESIZE(PlaneAI*, m_SelfAi, SelfAi);								//�Լ���AI
	CC_SYNTHESIZE_PASS_BY_REF(HpEff, m_HpEff, HpEff);						//���������ɻ�HPӰ��

	std::string						m_Musicfile;
	bool							m_ShootMusicAble;
	bool							m_isNeedFlyToMainPl;					//�ڳ�̵�ʱ���Ƿ���Ҫ�������ɻ�
	CC_SYNTHESIZE(bool, m_IsShootAble, IsShootAble);			
	Ref*							m_enterObj;
	SEL_CallFunc					m_enterFunc;
	CC_SYNTHESIZE_PASS_BY_REF(Size, m_SkinSize, SkinSize);
	CC_SYNTHESIZE(HpBar*, m_HpBar, HpBar);
	
	CC_SYNTHESIZE(int, m_ConfigDanmuId,			ConfigDanmuId);
	CC_SYNTHESIZE(float, m_ConfigFIreInterval,	ConfigFIreInterval);
};

//���ɻ� 
class MainPlane : public BasePlane
{
public:
	MainPlane();
	static MainPlane* create( PlaneType pt );
	
	void InitSkin();
	virtual bool onTouchBegan( Touch *touch, Event *unused_event );
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	//�ƶ�����ʱ���ж���ָ�Ƿ��ƶ�
	void checkIsMoveing(float dt);

	virtual void onEnter();

	virtual void DoSomethingBeforeDead();
	virtual void AnimationWhenDead();
	virtual void OtherDealWhenContact(const ContactPro& rs);

	virtual void initBody();

	//����Ч��
	void addSpeedUpEffect(float dur = -1.0f);
	void removeSpeedUpEffect();

	virtual void InitEdgeLine();

	//����Ϸ����ʱ�������ʤ������ô���ɻ����Ϸϳ���Ļ��
	void FlyUp();
	void FlyUpCallback();

	//�볡���һ��ʱ�����޵�
	void Revive();

	//�޵н���
	void WudiOver(Node* node);

	//���ϻ���
	void AddProtect();

	//�����Ч
	void ShootMusic();
	void setShootMusic(const std::string& musicfile);
	void PlayShootMusic();
	void ShootMusicCB(Node* node);

	//�������˺���˸
	void TintWhenHurt(float dur = 1.0f);
	void resumeColor();

	//���ϴ���
	void addMagnet();
	void removeMagnet();

	//�����л�
	void playMoveAnimationL();
	void playMoveAnimationBackL();
	void playMoveAnimationR();
	void playMoveAnimationBackR();
	void playNorAnimation();
	void AnimatFinished();

	ActionInterval* getMoveAnimationL(bool isHead = true);
	ActionInterval* getMoveAnimationR(bool isHead = true);
	ActionInterval* getNorAnimation();

	void setHaveProtect(bool b);
	void WeaponUpgrade();
	void WeaponDowngrade();

protected:
	Node*			m_SpeedUpNode;
	Node*			m_SpeedUpTail;
	bool			m_isProtect;
	std::string		m_shootMusic;
	Sprite*			m_NorAnimationNode;
	Sprite*			m_MoveAnimationNodeL;
	Sprite*			m_MoveAnimationNodeR;
	CC_SYNTHESIZE(bool, m_IsHasMagnet, IsHasMagnet);
	CC_SYNTHESIZE(bool, m_isTouchEnabled, TouchAble);
	CC_SYNTHESIZE(PlaneType,m_PlaneType, PlaneType);

	bool			m_isMoveAniPlayingL;
	bool			m_isMoveAniPlayingR;
	bool			m_isFinished;
	float			m_TempPosX;			//��ָ�ƶ�����֮��ľ���
	Vec2			m_PrePos;
	Vec2			m_CurPos;
	int				m_WpLvl;			//�����ȼ�
};

//BOSS
class CalculateRating;
class BaseBoss : public BasePlane
{
public:
	BaseBoss()
	{
		m_BgId = 0;
		m_Armat = nullptr;
	}
	void InitSkinWithArmat( const std::string& name );

	virtual void FuncAfterEnter();

	//�ӳٽ���,���ž���
	void BossDelayIn();

	//boss��ը����
	void BossExplosion();
	void displayBossExplosion();
	void shake();
	virtual void DoSomethingBeforeDead();
	virtual void OtherDealWhenContact(const ContactPro& rs);
	void resumeColor();

	//��������
	void RealDead();

	virtual void addAi(){}
	void addRating();
	virtual void addTimeOut(){}
	virtual void FuncHpEmpty();

	virtual float BeHitEffRadius();	//�ܻ���Ч�뾶
	virtual Vec2 BeHitEffOffset();	//�ܻ���Чλ��ƫ��

	//�����л���ͼ
	void SwitchBgWhenDead(int BgId);

	virtual void FuncWhenHpChanged();

protected:
	CalculateRating*	m_ratingNode;
	PlaneAI*			m_Ai;
	CC_SYNTHESIZE(int, m_BossLvl, BossLvl);
	int					m_BgId;
	CC_SYNTHESIZE(Armature*, m_Armat, Armature);							//��������
};

//BOSS1 :������
class IronManBoss : public BaseBoss
{
public:
	static IronManBoss* create(const std::string& artFile, int lvl); 

	virtual void addAi();
	virtual void initBody();

	virtual void addTimeOut();
	virtual void OtherDealWhenContact(const ContactPro& rs);
	virtual Vec2 getArmatPosOffset();

};


//BOSS:����ս��
class BoneWarriorBoss : public BaseBoss
{
public:
	static BoneWarriorBoss* create(const std::string& artFile, int lvl); 
	virtual void onEnter();

	virtual void addAi();
	virtual void initBody();
	virtual void OtherDealWhenContact(const ContactPro& rs);

};

//������BOSS������
class RenzheBoss : public BaseBoss
{
public:
	static RenzheBoss* create(const std::string& artFile, int lvl);

	virtual void addAi();
	virtual void initBody();
	virtual void FuncAfterEnter();
};

//����
class PoseidonBoss : public BaseBoss
{
public:
	static PoseidonBoss* create(const std::string& artFile, int vl);
	virtual void initBody();

	virtual void addAi();
	virtual void FuncAfterEnter();
	virtual Vec2 BeHitEffOffset();	//�ܻ���Чλ��ƫ��

};

//��ͨ�л�
class EnemyPlane : public BasePlane
{
public:
	EnemyPlane()
	{
		m_Picture = nullptr;
		m_Armat = nullptr;
		m_isNeedFlipX = false;
		m_trackMgr = nullptr;
	}
	~EnemyPlane();
	//ͼƬ�ɻ�
	static EnemyPlane* createWithPicture(const std::string& file, const std::string& TrackId = "0");

	//���������ɻ�
	static EnemyPlane* createWithArmature(const std::string& name, const std::string& TrackId = "0");
	void initTrack(const std::string& TrackId);
	void InitSkinWithPicture( const std::string& file );
	void InitSkinWithArmature( const std::string& name );
	virtual void onEnter();
	void addWeapon();
	void setFlipXonEnter();
	void OtherDealWhenContact(const ContactPro& rs);
	void startTrack();
private:
	CC_SYNTHESIZE(Sprite*, m_Picture, Picture);										//ͼƬ
	CC_SYNTHESIZE(PlaneArmature*, m_Armat, PlaneArmature);							//��������
	CC_SYNTHESIZE_PASS_BY_REF(WeaponPro, m_WeaponPro, WeaponPro);
	TrackManager*		m_trackMgr;
	bool				m_isNeedFlipX;
};

//����BOSS����
class CalculateRating : public Node
{
public:
	CalculateRating()
	{
		m_SurvivlaTime = 0.0f;
		m_BossType = 1001;
		m_isShow = false;
	}
	static CalculateRating* create();
	void setBossType(int tp);
	void startRating();

	void updateTime(float dt);

	//�õ�����
	BossRating getRating();

	//��¼��С���ʱ��
	void SurvivalTimeRecord();

	//BOSS��������������ʾ
	void showRating();
	void removeRatingSp(Node* node);

protected:
	float		m_SurvivlaTime;			//���ʱ��
	int			m_BossType;				//BOSS����
	bool		m_isShow;
};

#endif
