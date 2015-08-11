/********************************************************************
创建于:		2014/11/13  16:19	
文件名: 	E:\Cocos3.3_Project\MyGame\Classes\MyPlane.h 
作  者:		alex(alex)
修改于：	2014/11/13  16:19	

目  的:		飞机
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

//序列帧飞机
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

	//刷新血条
	void RefreshHpBar(float percent);

	//修正位置
	void setHpBarOffset(const Vec2& offset);

	//设置血量显示/隐藏
	void SetHpBarVisible(bool visible);

	//设置血量长度
	void setHpBarLengthScale(float scale);

	void setLifeNum(int lifeNum);

	void NextLife(int nuwNum);

	void changeColor();

private:
	//显示血条
	void ShowHpBar();

private:
	ProgressTimer*		m_HpBar;			//血量进度条
	BasePlane*			m_Plane;			//要显示血条的飞机
	CC_SYNTHESIZE(int,	m_MaxLife, MaxLife);//生命数量
	int					m_LeftLife;			//剩余生命
	Color3B				m_BgColor;
	Color3B				m_BarColor;
};

class BasePlane : public PhysicsUnit 
{
public:
	BasePlane();
	virtual void onExit();
	virtual void RemoveMyself();

	//敌机统计
	virtual void EnemyPlaneStatistics();

	//刷新血量
	virtual void FuncWhenHpChanged();

	//设置碰撞帅选
	void setBitmask(const int& cate,const int& collision,const int& test);

	//设置血上限
	void setMaxHP(int hp);

	//设置伤害
	void setBltHurts(int hurts);

	//设置防御
	void setDefense(int defense);

	//设置死亡音效
	void setDeadMusic(const std::string& file);

	//3.设置自身属性与攻击属性
	void setSelfAndAttact(const SelfPro& myPro, const ContactPro& attactPro);

	//加上单把
	void AddOneWp(Weapon* weapon,WeaponType wt);

	//加上多把
	void AddWpVec(std::vector<Weapon*> wpVec,WeaponType wt);

	//删除某把武器
	void RemoveOneWeapon(Weapon* weapon);

	//恢复原有普通武器
	void resumeNorWp();

	//移除所有武器
	void clearWeapon();

	void setShootAble(bool b);
	void setNeedFlyToMainPl(bool b);
	virtual void AnimationWhenDead();
	virtual void OtherDealWhenContact(const ContactPro& rs);
	virtual void PlayMusicWhenDead();			//死亡音效
	virtual void DoSomethingBeforeDead();

	void SetFuncAfterEnterWin(Ref* obj, SEL_CallFunc func);			//入场后执行的功能

	void FlyToMainPlaneWhenSpeedUp();
	void setShootMusicAble(bool b);

	virtual Vec2 getArmatPosOffset();
	virtual Size getSkinSize();

	void addRewards(float HpPct, int wardNum, DropGoodsType dgt);
	void runSelfAction();

	//设置配置发射
	void setFireConfig(int danmuId, float interval);
	void StartFire();

protected:

	void ShowHpBar();
	void StartShoot();
	virtual void CheckIsInScreen(float dt);

	//用定时器来设置无敌结束时间（用动作可能会被停止stopallActions）
	void WudiOver(float dt);

	void runEnterAnimation();

	virtual void initBody();

	virtual void FuncAfterEnter();

	void removePartical(Node* node);

	void CheckIsSpeedUp(float dt);
	void FlyToMainPlane(float dt);

protected:
	std::vector<Weapon*>			m_SpecialWpVec;							//触发武器，可叠加
	std::vector<Weapon*>			m_NorWpVec;								//普通武器，在接到武器buff的时候，会暂停发射
	std::vector<Weapon*>			m_TempWpVec;							//武器唯一buff，不重叠，只重置时间

	CC_SYNTHESIZE(ActionInterval*, m_EnterAnimation, EnterAnimation);		//入场动画
	CC_SYNTHESIZE(PlaneAI*, m_SelfAi, SelfAi);								//自己的AI
	CC_SYNTHESIZE_PASS_BY_REF(HpEff, m_HpEff, HpEff);						//死亡对主飞机HP影响

	std::string						m_Musicfile;
	bool							m_ShootMusicAble;
	bool							m_isNeedFlyToMainPl;					//在冲刺的时候是否需要冲向主飞机
	CC_SYNTHESIZE(bool, m_IsShootAble, IsShootAble);			
	Ref*							m_enterObj;
	SEL_CallFunc					m_enterFunc;
	CC_SYNTHESIZE_PASS_BY_REF(Size, m_SkinSize, SkinSize);
	CC_SYNTHESIZE(HpBar*, m_HpBar, HpBar);
	
	CC_SYNTHESIZE(int, m_ConfigDanmuId,			ConfigDanmuId);
	CC_SYNTHESIZE(float, m_ConfigFIreInterval,	ConfigFIreInterval);
};

//主飞机 
class MainPlane : public BasePlane
{
public:
	MainPlane();
	static MainPlane* create( PlaneType pt );
	
	void InitSkin();
	virtual bool onTouchBegan( Touch *touch, Event *unused_event );
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	//移动动画时，判定手指是否移动
	void checkIsMoveing(float dt);

	virtual void onEnter();

	virtual void DoSomethingBeforeDead();
	virtual void AnimationWhenDead();
	virtual void OtherDealWhenContact(const ContactPro& rs);

	virtual void initBody();

	//加速效果
	void addSpeedUpEffect(float dur = -1.0f);
	void removeSpeedUpEffect();

	virtual void InitEdgeLine();

	//在游戏结束时，如果是胜利，那么主飞机向上废除屏幕外
	void FlyUp();
	void FlyUpCallback();

	//入场后的一段时间内无敌
	void Revive();

	//无敌结束
	void WudiOver(Node* node);

	//加上护盾
	void AddProtect();

	//射击音效
	void ShootMusic();
	void setShootMusic(const std::string& musicfile);
	void PlayShootMusic();
	void ShootMusicCB(Node* node);

	//主角受伤害闪烁
	void TintWhenHurt(float dur = 1.0f);
	void resumeColor();

	//加上磁铁
	void addMagnet();
	void removeMagnet();

	//动画切换
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
	float			m_TempPosX;			//手指移动两刻之间的距离
	Vec2			m_PrePos;
	Vec2			m_CurPos;
	int				m_WpLvl;			//武器等级
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

	//延迟进入,播放警告
	void BossDelayIn();

	//boss爆炸动作
	void BossExplosion();
	void displayBossExplosion();
	void shake();
	virtual void DoSomethingBeforeDead();
	virtual void OtherDealWhenContact(const ContactPro& rs);
	void resumeColor();

	//真正死亡
	void RealDead();

	virtual void addAi(){}
	void addRating();
	virtual void addTimeOut(){}
	virtual void FuncHpEmpty();

	virtual float BeHitEffRadius();	//受击特效半径
	virtual Vec2 BeHitEffOffset();	//受击特效位置偏移

	//设置切换地图
	void SwitchBgWhenDead(int BgId);

	virtual void FuncWhenHpChanged();

protected:
	CalculateRating*	m_ratingNode;
	PlaneAI*			m_Ai;
	CC_SYNTHESIZE(int, m_BossLvl, BossLvl);
	int					m_BgId;
	CC_SYNTHESIZE(Armature*, m_Armat, Armature);							//骨骼动画
};

//BOSS1 :钢铁侠
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


//BOSS:幽灵战将
class BoneWarriorBoss : public BaseBoss
{
public:
	static BoneWarriorBoss* create(const std::string& artFile, int lvl); 
	virtual void onEnter();

	virtual void addAi();
	virtual void initBody();
	virtual void OtherDealWhenContact(const ContactPro& rs);

};

//第三个BOSS：忍者
class RenzheBoss : public BaseBoss
{
public:
	static RenzheBoss* create(const std::string& artFile, int lvl);

	virtual void addAi();
	virtual void initBody();
	virtual void FuncAfterEnter();
};

//海神
class PoseidonBoss : public BaseBoss
{
public:
	static PoseidonBoss* create(const std::string& artFile, int vl);
	virtual void initBody();

	virtual void addAi();
	virtual void FuncAfterEnter();
	virtual Vec2 BeHitEffOffset();	//受击特效位置偏移

};

//普通敌机
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
	//图片飞机
	static EnemyPlane* createWithPicture(const std::string& file, const std::string& TrackId = "0");

	//骨骼动画飞机
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
	CC_SYNTHESIZE(Sprite*, m_Picture, Picture);										//图片
	CC_SYNTHESIZE(PlaneArmature*, m_Armat, PlaneArmature);							//骨骼动画
	CC_SYNTHESIZE_PASS_BY_REF(WeaponPro, m_WeaponPro, WeaponPro);
	TrackManager*		m_trackMgr;
	bool				m_isNeedFlipX;
};

//计算BOSS评级
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

	//得到评级
	BossRating getRating();

	//记录最小存活时间
	void SurvivalTimeRecord();

	//BOSS死亡弹出评级显示
	void showRating();
	void removeRatingSp(Node* node);

protected:
	float		m_SurvivlaTime;			//存活时间
	int			m_BossType;				//BOSS类型
	bool		m_isShow;
};

#endif
