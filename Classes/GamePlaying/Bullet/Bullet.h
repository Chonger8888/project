/********************************************************************
创建于:		2014/11/12  18:02	
文件名: 	E:\Cocos3.3_Project\MyGame\Classes\Bullet.h 
作  者:		alex(alex)
修改于：	2014/11/12  18:02	

目  的:		子弹类
*********************************************************************/
#ifndef _BULLET_H_ 
#define _BULLET_H_



#include "cocos2d.h"
#include "../PhysicsUnit.h"
#include "../Weapon/WeaponDelegate.h"
#include "BulletFactory.h"

USING_NS_CC;

#define CREATE_BULLET(__TYPE__) \
static __TYPE__* create( WeaponDelegate* wpDlgt ) \
{ \
	__TYPE__* bullet = new (std::nothrow) __TYPE__; \
	std::string fileName = BulletFactory::getResByTag( wpDlgt->getBltResTag() ); \
	if (bullet && bullet->initWithSpriteFrameName(fileName)) \
	{ \
		bullet->autorelease(); \
		bullet->setWeaponDelegate(wpDlgt); \
		bullet->initBody(); \
		return bullet; \
	} \
	CC_SAFE_DELETE(bullet); \
	return nullptr; \
}

class Weapon;
class Bullet : public PhysicsUnit
{
public:
	Bullet();
	static Bullet* create(const std::string& frameName);
	CREATE_BULLET(Bullet);

	virtual void initBullet(){}
	virtual void onEnter();

	/**
	@功能：为子弹设置其依赖的武器
	@参数：Node:武器节点
	@返回：无
	*/
	void FixAngleWithWeapon(Weapon* weapon);

	//设置碰撞帅选
	void setBitmask(const int& cate,const int& collision,const int& test);

	void resetHurt(int hurts);

	virtual void InitEdgeLine();

	void ListeningStopWeapon();
	void ListeningBossDead();			//监听BOSS死亡
	void ListeningProtectRemove();		//监听护盾破裂

	void setWeaponDelegate(WeaponDelegate* wpDlgt);

	//如果该子弹是由武器来发射创建，那么只有武器设置该属性才有效，子弹内部的属性会被武器设置的属性替换
	void setBltFlySpeed(int speed);
	int getBltFlySpeed();

protected:
	virtual void initBody();
	void setBodyPro();
	void BossDead(Ref* obj);
	void StopBullet(Ref* obj);
	void ProtectRemove(Ref* obj);
	CC_SYNTHESIZE(Weapon*, m_weapon, Weapon);
	int			m_FlySpeed;
	Vec2		m_OriginVelocity;
	WeaponDelegate*			m_WeaponDelegate;
};

//冰导弹（寻找最近目标）,并且实时更新最近目标
class MissileForDis : public Bullet
{
public:
	MissileForDis()
	{
		m_particle1 = nullptr;
		m_particle2 = nullptr;
	}
	static MissileForDis* create(const std::string& frameName);
	static MissileForDis* create();

	virtual void onEnter();

	void FlyToTarget(float dt);

	virtual void initBody();
	virtual void OtherDealWhenContact(const ContactPro& rs);
	virtual void DoSomethingBeforeDead();
	void addParticle();
	void Explosion();

	void removeExpSp(Node* node);
	void removeme();

	void Dying();
	virtual void InitEdgeLine();

	void setBody(float t);
private:
	ParticleSystemQuad*		m_particle1;
	ParticleSystemQuad*		m_particle2;

	Vec2					m_targetPos;
};

//导弹（寻找血上限最大的目标）
class MissileForBloodlimit : public Bullet
{
public:
	MissileForBloodlimit();
	static MissileForBloodlimit* create(const std::string& frameName, const float& AngleOffet);
	virtual void onEnter();
	void FlyToTarget(float dt);

	//修正发射时的角度
	void FixDirect();

	virtual void OtherDealWhenContact(const ContactPro& rs);

	virtual void addParticle();

	//给定时间内自动爆炸消亡
	virtual void Explosion();

protected:
	float				m_AngleOffset;
};

//飘落型子弹
class FallingBullet : public Bullet
{
public:
	static FallingBullet* create(const std::string& frameName);
	virtual void onEnter();

	void newFalling(float dt);
};

//飘落型子弹加强版
class FallingBulletEX : public Bullet
{
public:
	static FallingBulletEX* create(const std::string& frameName);
	virtual void onEnter();

	//先喷射
	void QuickShoot();
	void shootCallback();

	void newFalling(float dt);
	virtual void initBody();
};

//曲线子弹
class CurveBullet : public Bullet
{
public:
	static CurveBullet* create(const std::string& frameName);
	virtual void onEnter();

	void newCurve(float dt);
	virtual void initBullet();

};

//曲线子弹2
class CurveBullet2 : public Bullet
{
public:
	typedef enum BullDirection
	{
		Dirct_Left,
		Dirct_Right
	}BullDirection;

	static CurveBullet2* create(const std::string& frameName,const BullDirection& dir);
	virtual void onEnter();

	void newCurve(float dt);
	virtual void initBullet();

private:
	BullDirection _dir;

};

// 反弹子弹
class ReboundBullect : public Bullet
{
public:
	ReboundBullect()
	:_collisionTimes(1)
	,m_MaxTimes(2)
	{

	};

	static ReboundBullect* create(const std::string& frameName );
	virtual void onEnter();
	virtual void initBody();
	virtual void OtherDealWhenContact(const ContactPro& rs);

private:
	int			m_MaxTimes;		//反弹次数上限
	int			_collisionTimes;
};

// 榴弹
class GrenadeBullect : public Bullet
{
public:
	GrenadeBullect()
	{

	};

	static GrenadeBullect* create(const std::string& frameName);
	virtual void onEnter();
	virtual void initBullet();
	void RandPos();

	virtual void InitEdgeLine();
	virtual void OtherDealWhenContact(const ContactPro& rs);

private:

};

//螺旋子弹
class HelixBullet : public Bullet
{
public:
	
	static HelixBullet* create(const std::string& frameName);
	virtual void onEnter();

	void Helix(float dt);
	virtual void initBullet();
private:
	Vec2	m_centerPos;
	Vec2	m_beginPos;
	float	m_randNun;
};

//螺旋+冲击
class HelixBulletEx : public HelixBullet
{
public:
	static HelixBulletEx* create(const std::string& frameName);
	virtual void onEnter();
	virtual void initBody();

	//停止旋转
	void StopHelix();

	//冲向目标
	void MoveToPl();

};

//飞镖子弹
class DartBullet : public Bullet
{
public:
	
	static DartBullet* create(const std::string& frameName);
	virtual void onEnter();
	virtual void initBullet();

	//从当前位置，移动到屏幕顶端，停留2秒，再移动到屏幕下端消失
	void PlayAction();

	void addParticle();

	void addForce();
	
};

//小飞镖子弹,碰到左右墙壁反弹一次
class DartBulletRebounce : public Bullet
{
public:
	static DartBulletRebounce* create(const std::string& frameName, int direct);
	virtual void onEnter();
	virtual void initBody();

	//从当前位置，移动到屏幕顶端，停留2秒，再移动到屏幕下端消失
	void PlayAction();
	void fixAngle();

	virtual void OtherDealWhenContact(const ContactPro& rs);
	virtual void InitEdgeLine();

private:
	int			m_direct;
};

//小飞镖
class DartBulletSmall : public Bullet
{
public:
	static DartBulletSmall* create(const std::string& framename);
	virtual void initBody();
	virtual void onEnter();
};

//跟踪飞镖
class DartBulletTrace : public Bullet
{
public:
	static DartBulletTrace* create(const std::string& framename);
	virtual void initBody();
	virtual void onEnter();

	//实时运行状态
	void UpdatePos(float dt);
	void fixPos();

	//不断设置body,每次设置body都会造成伤害
	void Hurt(float dt); 
	void addParticle();
};

//激光子弹
class LaserBullet : public Bullet
{
public:
	LaserBullet()
		:m_dur(3.0f)
		,m_scaleY(2000)
		,m_scaleX(2.0f)
		,m_timeIn(1.0f)
		,m_TimeToScale(2.0f)
		,m_TimeToBack(2.0f)
	{

	}
	static LaserBullet* create(int lvl = 1);
	virtual void initBody();
	virtual void initBullet();
	virtual void onEnter();
	virtual void onExit();

	void setBody(float dt);

	//激光光芯
	void addLaserCore();

	//激光发射点
	void addShootPoint();

	ActionInterval* getCommonAction(float scaleX = 1.0f, float scaleY = 1.0f);

	void removeLaser();
	void removeLaserO(Ref* obj);

	//三级的时候加上序列帧
	void addExtraLight();

	//三级向上冲的光环
	void ExtraCircle();
	void addExtraCircle();

	virtual Vec2 getLaserCoreOffset();
	virtual Vec2 getShootPointOffset();

protected:
	Sprite*			m_ShootSp;
	float			m_timeIn;
	float			m_TimeToScale;
	float			m_TimeToBack;
	float			m_dur;
	float			m_scaleY;
	float			m_scaleX;
	int				m_level;
};

//外围激光
class OutsideLaser : public LaserBullet
{
public:
	OutsideLaser();
	static OutsideLaser* create(const Vec2& pos, int zorder);
	virtual void onEnter();

	//左右移动，做spriteEaseCircleInOut动作
	void MoveCircle();

	void addDipan();
	virtual Vec2 getLaserCoreOffset();
	virtual Vec2 getShootPointOffset();
private:
	Vec2			m_pos;
	int				m_zorder;
};

//猫
class CatBullet : public Bullet
{
public:
	/*
	1. 每次跳跃固定距离
	2. 方向由离猫距离最近的目标的位置，与猫的位置确定
	3. 猫与任何敌机碰撞，爪击之
	4. 无目标时，随机左右向上跳跃
	*/
	CatBullet()
	{
		m_timeToLeave = false;
		m_ExDis = 0.0f;
	}
	enum CatType
	{
		cat,
		wolf,
		tiger
	};
	
	static CatBullet* create(bool isleft, CatType ct = cat);
	virtual void initBullet();
	virtual void initBody();
	virtual void onEnter();
	void initCat();
	void FirstJump();		//第一跳

	//跳跃状态
	void JumpState();

	//停留状态
	void StayState();

	//下一个奔跑位置
	void NextPos();

	//转向
	void TurnDirect();

	//调整猫的角度
	void AdjustCatAngle();

	//组合起来
	void CatRun();

	virtual void OtherDealWhenContact(const ContactPro& rs);

	//目标是否在屏幕内
	bool isTargetInScreen(const Vec2& pos);

	//猫存在一段时间
	void CatLeave();
	void LeaveCB();

	//限定跳跃的角度(斜45度)
	void FixJumpPos();

private:
	//碰到敌机，创建猫爪
	void addClawL();
	void addClawR();

private:
	CatType		m_catType;	
	Vec2		m_contactPos;
	Vec2		m_NextRunPos;		//当前奔跑坐标点
	Vec2		m_JumpVec;			//跳跃方向

	Sprite*		m_catStay;			//停留状态猫
	Sprite*		m_catJump;			//跳跃状态猫

	int			m_direct;			//方向（-1左，+1右）
	bool		m_timeToLeave;		//离开
	float		m_ExDis;			//打BOSS时跳跃距离扩大

};

//猫爪
class CatClaw : public Bullet
{
public:

	static CatClaw* create(const Vec2& direc);

	virtual void onEnter();
	
	virtual void initBody();

	//执行动作：缩小，微移，渐隐

	void Catch();

private:
	Vec2			m_MvDirect;
};

//BOSS飞镖
class BossDart : public Bullet
{
public:
	static BossDart* create(const std::string& frameName);
	virtual void initBody();
	virtual void initBullet();
	
	void addParticle();
};

//花瓣
class FlowerBullet : public Bullet
{
public:
	FlowerBullet()
		:m_radius(50)
	{

	}
	static FlowerBullet* create();
	virtual void initBody();
	virtual void onEnter();
	void initFLower();
	//不断设置body形成持续伤害
	void setBody(float dt);
	void start();

	//执行动作：移动到上方，旋转，放大
	void FLowerAction();
	void Eff1Action();
	void Eff2Action();

	//粒子爆炸
	void Explosion();

	//掉落粒子
	void DropFlower();

private:
	Sprite*		m_Flower;
	Sprite*		m_eff1;
	Sprite*		m_eff2;
	float		m_radius;
};

//龙火
class Flaming : public PhysicsUnit
{
public:

	enum FireType
	{
		eSmall,
		eMiddle,
		eBig,
	};

	static Flaming* create(FireType ft = eBig);
	virtual void initBody();
	virtual void onEnter();
	void setBody(float dt);

	//延迟加载火花
	void DelayShowSparks();
	void DelayShowFireCircle();

	//火花
	void Sparks();

	//喷火圈
	void FlamCircle();

private:
	ParticleSystemQuad* m_particle;
	FireType			m_fireType;
	Size				m_FireSize;
};

//喷火龙
class FireDragon : public Bullet
{
public:
	FireDragon()
	{
		m_node = nullptr;
	}
	static FireDragon* create(Flaming::FireType ft);
	virtual void initBody();
	virtual void onEnter();

	//慢慢向上移动
	void MoveUp();

	//喷火
	void ShootFire();

	void addArmature();
	
private:
	Node*				m_node;
	Flaming::FireType	m_fireType;
};

//向上喷射的烟花
class FireWorks : public Bullet
{
public:
	static FireWorks* create(const std::string& frameName);

	virtual void initBody();

	virtual void onEnter();

	//在N秒后重设线性阻尼
	void resetLinearDamping();
};

//反弹子弹
class EneRebounceBullet : public Bullet
{
public:
	static EneRebounceBullet* create(const std::string& frameName);
	virtual void initBody();
	virtual void OtherDealWhenContact( const ContactPro& rs );
};

//射向敌方目标的子弹
class BulletToTarget : public Bullet
{
public:
	static BulletToTarget* create(const std::string& frameName, bool isSelf = true);
	virtual void initBody();

	virtual void onEnter();

	/****************敌方**************/
	void FixAngleToMainplane();		//射向主飞机

	/****************己方**************/
	//修正角度
	void FixAngleOfSelf();

	//找目标
	void FindAmiOfSelf(int direct);

private:
	bool			m_isSelf;
};

//围绕主飞机不断旋转
class BulletSurroundMainPl : public Bullet
{
public:
	BulletSurroundMainPl()
	{
		m_dur = 8.0f;
	}
	static BulletSurroundMainPl* create(int lvl);
	virtual void onEnter();
	virtual void initBody();

	void addSurroundBullet(float angle, bool isflyout = true);

	void setBody(float dt);
	float getDur();

	void removeMeO(Ref* obj);
private:
	int			m_Level;		//技能等级
	float		m_radius;		//旋转半径
	float		m_curBodyRadius;//刚体半径
	float		m_dur;			//持续时间
};

class SurroundBullet : public Sprite
{
public:
	static SurroundBullet* create(const std::string& file, float dur, bool isflyout = false);
	virtual void onEnter();
	virtual void onExit();

	void addParticle();

	//飞出去
	void FlyOut();

	//旋转角度
	void FixRotate();
	void setOffset(const Vec2& pos);

	void removeMe();
	void setDur(float t);
private:
	bool			m_isFlyOut;
	Vec2			m_offset;
	float			m_dur;

};

//飞出去的子弹
class FlyOutBullet : public Bullet
{
public:
	static FlyOutBullet* create();
	virtual void initBody();
};

//花（一级）：三个花瓣组成的整体旋转垂直向上飞出屏幕
class ThreePetalsBullet : public Bullet
{
public:
	static ThreePetalsBullet* create(const std::string& file);
	virtual void initBody();

	virtual void onEnter();
	void addParticle();
	virtual void DoSomethingBeforeDead();
};

//飞剑子弹
class AirazorBullet :  public Bullet
{
public:
	AirazorBullet()
	{
		m_tail = nullptr;
	}
	static AirazorBullet* create(const std::string& file, float r, bool isPierce);
	virtual void onEnter();
	virtual void initBody();

	//按照预定角度出现
	void appear();

	//寻找目标，旋转相应角度
	void FixAngle();

	//飞出去
	void FlyOut();

	//加上拖尾
	void AddTail();

	virtual void DoSomethingBeforeDead();
	virtual void InitEdgeLine();

private:
	float			m_rotate;		//旋转角度
	bool			m_isPierce;		//是否穿透
	Vec2			m_flyDirect;	//飞行方向
	MotionStreak*	m_tail;
};

//闪电单个绘制
class DrawLighting : public Node
{
public:
	DrawLighting();
	static DrawLighting* create(const Vec2& startPos,const Vec2& endPos);
	virtual void onEnter();

	//显示闪电图片
	void RefreshPicture();
	void removeMe();
private:
	Vec2			m_startPos;
	Vec2			m_endPos;
	Sprite*			m_Sp;
};

//闪电层
class NewLightingLayer : public Layer
{
public:
	NewLightingLayer();
	static NewLightingLayer* create(int num, int hurt);
	virtual void onEnter();
	virtual void onExit();
	void setHurt(int hurt);

	//找出所有目标点
	void FindAllTargets();
	bool FindTarget(Vec2& startPos);	//传引用

	bool isFound(PhysicsUnit* target);

	bool isInScreen(const Vec2& pos);

	//渲染闪电图片
	void DrawLight();

	//开始闪电，持续时间
	void StartLighting(float dur = -1.0f);

	void removeMe();
	void removeMeO(Ref* obj);

private:
	std::vector<PhysicsUnit*>	m_targetList;	//已找到的目标列表
	std::vector<Vec2>			m_posList;		//闪电的点列表
	int							m_lightingNums;	//闪电数量
	float						m_EdgeL;
	float						m_EdgeR;
	float						m_EdgeD;
	float						m_EdgeU;
	int							m_hurt;
};

//闪电攻击点
class LightingAttackPoint : public Bullet
{
public:
	static LightingAttackPoint* create();
	virtual void initBody();

	virtual void onEnter();
};

//爆裂弹
class ExplosionBullet : public Bullet
{
public:
	static ExplosionBullet* create( float angle);
	virtual void initBody();
	virtual void onEnter();

	//修正角度子弹
	void FixBulletAngle();

	//飞行一段时间后，速度几乎为零，加上8加武器
	void Explosion();

	//从发射到爆炸期间执行若干个动画，包括序列帧
	void ExplAction();

private:
	float			m_angle;
};

//Boss3的飞镖
class DartOfBoss3 : public PhysicsUnit
{
public:
	static DartOfBoss3* create();
	virtual void initBody();
	virtual void onEnter();
	virtual void InitEdgeLine();

	void setBody(float dt);
	void addParticle();
};

//Boss4的子弹1,悬停一段时间，绕圈，冲向主飞机
class Bullet1OfBoss4 : public Bullet
{
public:
	Bullet1OfBoss4()
	{
		m_StayTime = 2.0;
	}
	static Bullet1OfBoss4* create(ResTag res,float stayTime);
	virtual void initBody();
	virtual void onEnter();

	static void BreathAction(Node* node);

	//上下浮动
	void FloatUpDown();

	void Helix(float dt);

	//停止旋转
	void StopHelix();

	//冲向目标
	void MoveToPl();

	//一段时间后开始
	void Start();

private:
	float		m_StayTime;
};

//Boss4的子弹2，按照轨迹运动，再飘落
class Bullet2OfBoss4 : public Bullet
{
public:
	static Bullet2OfBoss4* create();
	virtual void initBody();
	virtual void onEnter();

	//走贝塞尔曲线
	void MoveBezier();

	//开始漂游
	void StartFloat();
	void newFalling(float dt);
};

//Boss4的子弹3，在屏幕底部生成一个，向上运动到指定位置，爆开一圈水泡子弹，做自由落体运动
class Bullet3OfBoss4 : public Bullet
{
public:
	static Bullet3OfBoss4* create();
	virtual void initBody();
	virtual void onEnter();
	void expolsion();

	//添加武器
	static void addWp(Node* node, int hurt);
};

//Boss4的子弹4，类似子弹3，只不过生成的位置，不在底部，也不用运动
class Bullet4OfBoss5: public Bullet
{
public:
	Bullet4OfBoss5()
	{
		m_dur = 0.0f;
	}
	static Bullet4OfBoss5* create(float dur);
	virtual void initBody();
	virtual void onEnter();
	void expolsion();

private:
	float			m_dur;
};

//流星
class BulletMeteor : public Bullet
{
public:
	static BulletMeteor* create(const std::string& file, float v);
	virtual void initBody();
	virtual void onEnter();
	void addParticle();
	void randRemove();
	void explosion();
	virtual void InitEdgeLine();
	virtual void DoSomethingBeforeDead();

private:
	float			m_velocity;
};

//回旋弹
class BulletConvolution  : public Bullet
{
public:
	BulletConvolution()
	{
		m_CurCount = 0;
		m_ConvolutionDur = 0.5f;
		m_ConvolutionTimes = 2;
	}
	CREATE_BULLET(BulletConvolution);
	virtual void onEnter();

	void BulletReverse();

	CC_SYNTHESIZE(float,	m_ConvolutionDur,		ConvolutionDur);
	CC_SYNTHESIZE(int,		m_ConvolutionTimes,		ConvolutionTimes);

private:
	int				m_CurCount;
};

//交叉子弹
class CrossBullet : public Bullet
{
public:
	static CrossBullet* create(const std::string& file, Weapon* weapon);
	virtual void initBody();

	virtual void onEnter();

	void cross(float dt);

};

#endif
