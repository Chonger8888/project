/********************************************************************
������:		2014/11/12  18:02	
�ļ���: 	E:\Cocos3.3_Project\MyGame\Classes\Bullet.h 
��  ��:		alex(alex)
�޸��ڣ�	2014/11/12  18:02	

Ŀ  ��:		�ӵ���
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
	@���ܣ�Ϊ�ӵ�����������������
	@������Node:�����ڵ�
	@���أ���
	*/
	void FixAngleWithWeapon(Weapon* weapon);

	//������ײ˧ѡ
	void setBitmask(const int& cate,const int& collision,const int& test);

	void resetHurt(int hurts);

	virtual void InitEdgeLine();

	void ListeningStopWeapon();
	void ListeningBossDead();			//����BOSS����
	void ListeningProtectRemove();		//������������

	void setWeaponDelegate(WeaponDelegate* wpDlgt);

	//������ӵ��������������䴴������ôֻ���������ø����Բ���Ч���ӵ��ڲ������Իᱻ�������õ������滻
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

//��������Ѱ�����Ŀ�꣩,����ʵʱ�������Ŀ��
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

//������Ѱ��Ѫ��������Ŀ�꣩
class MissileForBloodlimit : public Bullet
{
public:
	MissileForBloodlimit();
	static MissileForBloodlimit* create(const std::string& frameName, const float& AngleOffet);
	virtual void onEnter();
	void FlyToTarget(float dt);

	//��������ʱ�ĽǶ�
	void FixDirect();

	virtual void OtherDealWhenContact(const ContactPro& rs);

	virtual void addParticle();

	//����ʱ�����Զ���ը����
	virtual void Explosion();

protected:
	float				m_AngleOffset;
};

//Ʈ�����ӵ�
class FallingBullet : public Bullet
{
public:
	static FallingBullet* create(const std::string& frameName);
	virtual void onEnter();

	void newFalling(float dt);
};

//Ʈ�����ӵ���ǿ��
class FallingBulletEX : public Bullet
{
public:
	static FallingBulletEX* create(const std::string& frameName);
	virtual void onEnter();

	//������
	void QuickShoot();
	void shootCallback();

	void newFalling(float dt);
	virtual void initBody();
};

//�����ӵ�
class CurveBullet : public Bullet
{
public:
	static CurveBullet* create(const std::string& frameName);
	virtual void onEnter();

	void newCurve(float dt);
	virtual void initBullet();

};

//�����ӵ�2
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

// �����ӵ�
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
	int			m_MaxTimes;		//������������
	int			_collisionTimes;
};

// ��
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

//�����ӵ�
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

//����+���
class HelixBulletEx : public HelixBullet
{
public:
	static HelixBulletEx* create(const std::string& frameName);
	virtual void onEnter();
	virtual void initBody();

	//ֹͣ��ת
	void StopHelix();

	//����Ŀ��
	void MoveToPl();

};

//�����ӵ�
class DartBullet : public Bullet
{
public:
	
	static DartBullet* create(const std::string& frameName);
	virtual void onEnter();
	virtual void initBullet();

	//�ӵ�ǰλ�ã��ƶ�����Ļ���ˣ�ͣ��2�룬���ƶ�����Ļ�¶���ʧ
	void PlayAction();

	void addParticle();

	void addForce();
	
};

//С�����ӵ�,��������ǽ�ڷ���һ��
class DartBulletRebounce : public Bullet
{
public:
	static DartBulletRebounce* create(const std::string& frameName, int direct);
	virtual void onEnter();
	virtual void initBody();

	//�ӵ�ǰλ�ã��ƶ�����Ļ���ˣ�ͣ��2�룬���ƶ�����Ļ�¶���ʧ
	void PlayAction();
	void fixAngle();

	virtual void OtherDealWhenContact(const ContactPro& rs);
	virtual void InitEdgeLine();

private:
	int			m_direct;
};

//С����
class DartBulletSmall : public Bullet
{
public:
	static DartBulletSmall* create(const std::string& framename);
	virtual void initBody();
	virtual void onEnter();
};

//���ٷ���
class DartBulletTrace : public Bullet
{
public:
	static DartBulletTrace* create(const std::string& framename);
	virtual void initBody();
	virtual void onEnter();

	//ʵʱ����״̬
	void UpdatePos(float dt);
	void fixPos();

	//��������body,ÿ������body��������˺�
	void Hurt(float dt); 
	void addParticle();
};

//�����ӵ�
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

	//�����о
	void addLaserCore();

	//���ⷢ���
	void addShootPoint();

	ActionInterval* getCommonAction(float scaleX = 1.0f, float scaleY = 1.0f);

	void removeLaser();
	void removeLaserO(Ref* obj);

	//������ʱ���������֡
	void addExtraLight();

	//�������ϳ�Ĺ⻷
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

//��Χ����
class OutsideLaser : public LaserBullet
{
public:
	OutsideLaser();
	static OutsideLaser* create(const Vec2& pos, int zorder);
	virtual void onEnter();

	//�����ƶ�����spriteEaseCircleInOut����
	void MoveCircle();

	void addDipan();
	virtual Vec2 getLaserCoreOffset();
	virtual Vec2 getShootPointOffset();
private:
	Vec2			m_pos;
	int				m_zorder;
};

//è
class CatBullet : public Bullet
{
public:
	/*
	1. ÿ����Ծ�̶�����
	2. ��������è���������Ŀ���λ�ã���è��λ��ȷ��
	3. è���κεл���ײ��צ��֮
	4. ��Ŀ��ʱ���������������Ծ
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
	void FirstJump();		//��һ��

	//��Ծ״̬
	void JumpState();

	//ͣ��״̬
	void StayState();

	//��һ������λ��
	void NextPos();

	//ת��
	void TurnDirect();

	//����è�ĽǶ�
	void AdjustCatAngle();

	//�������
	void CatRun();

	virtual void OtherDealWhenContact(const ContactPro& rs);

	//Ŀ���Ƿ�����Ļ��
	bool isTargetInScreen(const Vec2& pos);

	//è����һ��ʱ��
	void CatLeave();
	void LeaveCB();

	//�޶���Ծ�ĽǶ�(б45��)
	void FixJumpPos();

private:
	//�����л�������èצ
	void addClawL();
	void addClawR();

private:
	CatType		m_catType;	
	Vec2		m_contactPos;
	Vec2		m_NextRunPos;		//��ǰ���������
	Vec2		m_JumpVec;			//��Ծ����

	Sprite*		m_catStay;			//ͣ��״̬è
	Sprite*		m_catJump;			//��Ծ״̬è

	int			m_direct;			//����-1��+1�ң�
	bool		m_timeToLeave;		//�뿪
	float		m_ExDis;			//��BOSSʱ��Ծ��������

};

//èצ
class CatClaw : public Bullet
{
public:

	static CatClaw* create(const Vec2& direc);

	virtual void onEnter();
	
	virtual void initBody();

	//ִ�ж�������С��΢�ƣ�����

	void Catch();

private:
	Vec2			m_MvDirect;
};

//BOSS����
class BossDart : public Bullet
{
public:
	static BossDart* create(const std::string& frameName);
	virtual void initBody();
	virtual void initBullet();
	
	void addParticle();
};

//����
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
	//��������body�γɳ����˺�
	void setBody(float dt);
	void start();

	//ִ�ж������ƶ����Ϸ�����ת���Ŵ�
	void FLowerAction();
	void Eff1Action();
	void Eff2Action();

	//���ӱ�ը
	void Explosion();

	//��������
	void DropFlower();

private:
	Sprite*		m_Flower;
	Sprite*		m_eff1;
	Sprite*		m_eff2;
	float		m_radius;
};

//����
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

	//�ӳټ��ػ�
	void DelayShowSparks();
	void DelayShowFireCircle();

	//��
	void Sparks();

	//���Ȧ
	void FlamCircle();

private:
	ParticleSystemQuad* m_particle;
	FireType			m_fireType;
	Size				m_FireSize;
};

//�����
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

	//���������ƶ�
	void MoveUp();

	//���
	void ShootFire();

	void addArmature();
	
private:
	Node*				m_node;
	Flaming::FireType	m_fireType;
};

//����������̻�
class FireWorks : public Bullet
{
public:
	static FireWorks* create(const std::string& frameName);

	virtual void initBody();

	virtual void onEnter();

	//��N���������������
	void resetLinearDamping();
};

//�����ӵ�
class EneRebounceBullet : public Bullet
{
public:
	static EneRebounceBullet* create(const std::string& frameName);
	virtual void initBody();
	virtual void OtherDealWhenContact( const ContactPro& rs );
};

//����з�Ŀ����ӵ�
class BulletToTarget : public Bullet
{
public:
	static BulletToTarget* create(const std::string& frameName, bool isSelf = true);
	virtual void initBody();

	virtual void onEnter();

	/****************�з�**************/
	void FixAngleToMainplane();		//�������ɻ�

	/****************����**************/
	//�����Ƕ�
	void FixAngleOfSelf();

	//��Ŀ��
	void FindAmiOfSelf(int direct);

private:
	bool			m_isSelf;
};

//Χ�����ɻ�������ת
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
	int			m_Level;		//���ܵȼ�
	float		m_radius;		//��ת�뾶
	float		m_curBodyRadius;//����뾶
	float		m_dur;			//����ʱ��
};

class SurroundBullet : public Sprite
{
public:
	static SurroundBullet* create(const std::string& file, float dur, bool isflyout = false);
	virtual void onEnter();
	virtual void onExit();

	void addParticle();

	//�ɳ�ȥ
	void FlyOut();

	//��ת�Ƕ�
	void FixRotate();
	void setOffset(const Vec2& pos);

	void removeMe();
	void setDur(float t);
private:
	bool			m_isFlyOut;
	Vec2			m_offset;
	float			m_dur;

};

//�ɳ�ȥ���ӵ�
class FlyOutBullet : public Bullet
{
public:
	static FlyOutBullet* create();
	virtual void initBody();
};

//����һ����������������ɵ�������ת��ֱ���Ϸɳ���Ļ
class ThreePetalsBullet : public Bullet
{
public:
	static ThreePetalsBullet* create(const std::string& file);
	virtual void initBody();

	virtual void onEnter();
	void addParticle();
	virtual void DoSomethingBeforeDead();
};

//�ɽ��ӵ�
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

	//����Ԥ���Ƕȳ���
	void appear();

	//Ѱ��Ŀ�꣬��ת��Ӧ�Ƕ�
	void FixAngle();

	//�ɳ�ȥ
	void FlyOut();

	//������β
	void AddTail();

	virtual void DoSomethingBeforeDead();
	virtual void InitEdgeLine();

private:
	float			m_rotate;		//��ת�Ƕ�
	bool			m_isPierce;		//�Ƿ�͸
	Vec2			m_flyDirect;	//���з���
	MotionStreak*	m_tail;
};

//���絥������
class DrawLighting : public Node
{
public:
	DrawLighting();
	static DrawLighting* create(const Vec2& startPos,const Vec2& endPos);
	virtual void onEnter();

	//��ʾ����ͼƬ
	void RefreshPicture();
	void removeMe();
private:
	Vec2			m_startPos;
	Vec2			m_endPos;
	Sprite*			m_Sp;
};

//�����
class NewLightingLayer : public Layer
{
public:
	NewLightingLayer();
	static NewLightingLayer* create(int num, int hurt);
	virtual void onEnter();
	virtual void onExit();
	void setHurt(int hurt);

	//�ҳ�����Ŀ���
	void FindAllTargets();
	bool FindTarget(Vec2& startPos);	//������

	bool isFound(PhysicsUnit* target);

	bool isInScreen(const Vec2& pos);

	//��Ⱦ����ͼƬ
	void DrawLight();

	//��ʼ���磬����ʱ��
	void StartLighting(float dur = -1.0f);

	void removeMe();
	void removeMeO(Ref* obj);

private:
	std::vector<PhysicsUnit*>	m_targetList;	//���ҵ���Ŀ���б�
	std::vector<Vec2>			m_posList;		//����ĵ��б�
	int							m_lightingNums;	//��������
	float						m_EdgeL;
	float						m_EdgeR;
	float						m_EdgeD;
	float						m_EdgeU;
	int							m_hurt;
};

//���繥����
class LightingAttackPoint : public Bullet
{
public:
	static LightingAttackPoint* create();
	virtual void initBody();

	virtual void onEnter();
};

//���ѵ�
class ExplosionBullet : public Bullet
{
public:
	static ExplosionBullet* create( float angle);
	virtual void initBody();
	virtual void onEnter();

	//�����Ƕ��ӵ�
	void FixBulletAngle();

	//����һ��ʱ����ٶȼ���Ϊ�㣬����8������
	void Explosion();

	//�ӷ��䵽��ը�ڼ�ִ�����ɸ���������������֡
	void ExplAction();

private:
	float			m_angle;
};

//Boss3�ķ���
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

//Boss4���ӵ�1,��ͣһ��ʱ�䣬��Ȧ���������ɻ�
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

	//���¸���
	void FloatUpDown();

	void Helix(float dt);

	//ֹͣ��ת
	void StopHelix();

	//����Ŀ��
	void MoveToPl();

	//һ��ʱ���ʼ
	void Start();

private:
	float		m_StayTime;
};

//Boss4���ӵ�2�����չ켣�˶�����Ʈ��
class Bullet2OfBoss4 : public Bullet
{
public:
	static Bullet2OfBoss4* create();
	virtual void initBody();
	virtual void onEnter();

	//�߱���������
	void MoveBezier();

	//��ʼƯ��
	void StartFloat();
	void newFalling(float dt);
};

//Boss4���ӵ�3������Ļ�ײ�����һ���������˶���ָ��λ�ã�����һȦˮ���ӵ��������������˶�
class Bullet3OfBoss4 : public Bullet
{
public:
	static Bullet3OfBoss4* create();
	virtual void initBody();
	virtual void onEnter();
	void expolsion();

	//�������
	static void addWp(Node* node, int hurt);
};

//Boss4���ӵ�4�������ӵ�3��ֻ�������ɵ�λ�ã����ڵײ���Ҳ�����˶�
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

//����
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

//������
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

//�����ӵ�
class CrossBullet : public Bullet
{
public:
	static CrossBullet* create(const std::string& file, Weapon* weapon);
	virtual void initBody();

	virtual void onEnter();

	void cross(float dt);

};

#endif
