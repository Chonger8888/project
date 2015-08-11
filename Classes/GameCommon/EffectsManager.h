/********************************************************************
������:		2014/11/27  11:10	
�ļ���: 	E:\Cocos3.3_Project\MyGame\Classes\EffectsManager.h 
��  ��:		alex(alex)
�޸��ڣ�	2014/11/27  11:10	

Ŀ  ��:		��Ч��������
*********************************************************************/
#ifndef _EFFECTS_MANAGER_H_
#define _EFFECTS_MANAGER_H_


#include "cocos2d.h"
#include "cocos-ext.h"
#include "../GamePlaying/PhysicsUnit.h"
#include "spine/extension.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define REPEAT_FORVER 10000000
//�ӵ�����Ŀ����Ч����
enum class BulletEffectType
{
	TYPE1,
	TYPE2,
	TYPE3
};

class BasePlane;
class EffectsManager : public Layer
{
public:
	EffectsManager()
	{
		m_MeteorNum = 0;
	}
	CREATE_FUNC(EffectsManager);

	//�ӵ�����Ŀ��󲥷ŵ���Ч
	void displayEffectsHitTarget(const Vec2& displayPos);

	//��Χ��ը��Ч
	void displayScopeAmmo(const Vec2& displayPos);

	//ȼ�ձ�ը��Ч
	void displayBurningExplosion(const Vec2& pos);

	//��Ȧ��Ч
	void displayEletricCircle(Sprite* node);

	//���򼤹���Ч
	void displayHorizontalLaser();

	//˲�������Ϸ������������
	void displaySlowdown(float duration = 1.2f, float timescale = 0.1);

	//��������֡����
	void displayAnimation(Node* parent,const char* plist, const char* animationName, bool isRepeatForever = false);

	//����������ը����
	void displayDeathExplosion(const Point& pos);

	//ȫ��������Ч
	void displaySlowDownEffect(const Vec2& displayPos);

	//���߼��������֡
	void displayQuxianLaserAnimation(Node* node);

	//��Ļ����
	void ScreenShake(Node* node, float t = -0.1f);

	//���鶶��
	void NodeShake(Node* node, float t = -0.1f);

	//��è
	void displayAniOfWhiteCat(Node* node);

	//���������ܻ�����֡
	void displayLightingEff(Node* node);

public:
	//������ͨ��������֡
	void playLightAnimation(const Point& startPos, const Point& endPos);

	//���Ŵ�����������֡
	void playSpecialLightAnimation(const Point& startPos, const Point& endPos);

	//���ż�������֡2
	void playLaserAnimation1(Sprite* sp); 

	//���ż�������֡2
	void playLaserAnimation2(Sprite* sp); 

	//��Χ��ը����֡
	void playExplosionAnimation(Sprite* sp);

	//��Ȧ����֡
	void playElectricAnimation(Sprite* sp);

	//����ǽǰ������֡
	void playBounceWallBeginAnimation(Sprite* sp);

	//����ǽ�������֡
	void playBounceWallRepeatAnimation(Sprite* sp);

	//���յ�������֡
	void playRecyclePadAnimation(Sprite* sp);

	//����
	void playFireBirdAnimation(Sprite* sp);

	//������л���ײЧ��
	void playWindEnemyContactAnimation(Node* node);

	//�����
	void playTornadoAnimation(Node* node);

	//ȼ�յ�ѪЧ��
	void playBurningAnimation(Node* node);

	//��������ģʽ����֡����
	void playFireBallNormalAnimation(Node* node);

	//����������ģʽ����֡����
	void playFireBallEnergyFullAnimation(Node* node);

	//������ͨ״̬����֡����
	void playElectricBallNormalAnimation(Node* node);

	//����������״̬����֡����
	void playElectricBallEnergyFullAnimation(Node* node);

	//������л���ײ����֡����
	void playElectricContactAnimation(Node* node);

	//����1��ͨ����֡
	Animate* getMainPl1Animation_N();

	//����1��������֡
	Animate* getMainPl1Animation_S();

	//����2��ͨ����֡
	Animate* getMainPl2Animation_N();

	//����2��������֡
	Animate* getMainPl2Animation_S();

	//����3��ͨ����֡
	Animate* getMainPl3Animation_N();

	//����3��������֡
	Animate* getMainPl3Animation_S();

	//�������ȥǰ������֡
	ActionInterval* playHoodleAnimation();

	static ParticleSystemQuad* addParticle(const std::string& file, Node* parent,const Vec2& pos = Vec2(0,0),bool autoRM = true, float delay = -1.0f);

	//��������
	void playActionOfMainPlDead(BasePlane* node, ActionInterval* func);

	//���ǽӵ�����ʱ����Ч
	void playEffGetRewards(int tp);

	//����֡
	static Animate* getFramesAnimation( const std::string& qianzui, int nums, float dt, int start = 0, int loop = 1,bool iscache = true);
	static Animate* getFramesAnimation( const std::string& qianzui, float dt, int loop = 1,bool iscache = true);

	//�ɻ���ֹ�ƶ�
	void PlaneCantTouchAble(float t);
	void PlaneResumeTouch();

private:
	//��Χ��ը�ص�
	void removeScopeAmmo(Node* sender);

	//���򼤹�ص�
	void removeHorizontalLaser(Node* sender);

	//������ص����ָ������ٶ�
	void resumeScheduler();

	//����֡����ɾ��
	void removeAnimation(Node* node);

	//����֡����ʱ�滻�ľ���
	Sprite* AnimationSprite(Node* node);
private:
	int			m_MeteorNum;	//����������
};

//��Χ��ը
class ScopeEffect : public PhysicsUnit
{
public:
	static ScopeEffect* create(const char* file,const Size& size = Size(90,90));
	virtual void initBody();
	void initSelfPro();
	virtual void FuncAfterEnter();

private:
	Size			m_expSize;
};

//Ѫ�����ּ�����ʾ
class BloodDecreaseTTF : public Node
{
public:
	static BloodDecreaseTTF* create(Node* targert, int bloodNum);

	void addTTF(Node* targert,const int& num);

	void removeTTF();
};

//��������
class BigLighting : public PhysicsUnit
{
public:
	static BigLighting* create();
	virtual void initBody();
	void initSelfPro();
	virtual void FuncAfterEnter();

	//����ɾ���Լ�
	void removeCallback();
};


//����������д���࣬������Ч
class LightingLayer : public Layer
{
public:
	CREATE_FUNC(LightingLayer);
	virtual void onEnter();

	void Lighting(const int& times);

private:
	//������Ч
	void addEffect();
	void effectCallback(Node* node);

	//�������
	void addLight();
	void lightCallback();

private:
	float		m_fadeIntime;
};

//��ʯ
class FallingStone : public PhysicsUnit
{
public:
	static FallingStone* create();

	virtual void initBody();
	virtual void FuncAfterEnter();
	void initSelfPro();

	//��ʾ
	void warning();

	//����
	void falling();

	virtual void InitEdgeLine();

};

//BOSS��ը
class ExplosionSprite : public Node
{
public:
	CREATE_FUNC(ExplosionSprite);

	virtual void onEnter();

	//void addParticle();

	void displayParticle();

	void RemovePartical(Node* node);

	void PartEff1();
	void PartEff2();
	void PartEff3();
	void shake();

	//ȫ�����
	void clearAllEnemy();

private:
};


//BOSS���־���
class WarningOfBoss : public Layer
{
public:
	CREATE_FUNC(WarningOfBoss);
	virtual void onEnter();

	//��������
	void RollSp(bool isUp);

	//�м�����
	void MidWenzi();

	//ȫ�����
	void ScreenRed();
	void removeWarning();

};

//��Բ�˶�
class  EllipticFunc
{
public:
	// ����һ���ṹ������ȷ����Բ�Ĳ���
	typedef struct _EllipticConfig {
		Vec2 conterPos;
		//��Բa��������б��
		float aLength;
		//��Բc�������ǵױ�
		float cLength;

		_EllipticConfig(const Vec2& pos, float aL, float cL)
		{
			conterPos = pos;
			aLength = aL;
			cLength = cL;
		}
	}EllipticConfig;


	static inline float tuoyuanXat( float a, float c, float t, bool isleft )//����X����
	{
		//��������
		if (isleft)
		{
			return a*cos(2*PI*t)-a;
		}
		
		return -a*cos(2*PI*t)+a;
	}
	static inline float tuoyuanYat( float a, float c, float t )//����Y����
	{
		float b = sqrt(powf(a, 2) - powf(c, 2));//��Ϊ֮ǰ����Ĳ����ǽ���c�����Ƕ̰���b��������Ҫ�����b
		//��������
		return b*sin(2*PI*t);
	}
};


//���ǲ�
class MeteorLayer : public Layer
{
public:
	CREATE_FUNC(MeteorLayer);
	virtual void onEnter();

	void StartMeteor();

	void RemoveMe();

private:
	int			m_num;
};

//����
class TalismanLayer : public Node
{
public:
	TalismanLayer()
	{
		m_isStop = false;
	}
	CREATE_FUNC(TalismanLayer);
	virtual void onEnter();
	virtual void onExit();

	//�м��ַ�
	void MidText();

	void RemoveMe();

	void StopText();

	Node* createHalfEff();

	void RotateSurround();

	void StopTalisman(Ref* obj);

private:
	int			m_TextId;
	Size		m_ws;
	Sprite*		m_UpderNode;
	CC_SYNTHESIZE(float, m_dur, duration);
	bool		m_isStop;
};

//����
class Bow : public PhysicsUnit
{
public:
	Bow()
	{
		m_shootTimes = 0;
	}
	static Bow* create();
	virtual void onEnter();

	void InitNode();
	void Start();
	void removeSelf();

	//��������
	Animate* JuqiAnimat();

	//�����⶯��
	Animate* BowShine();

	void ShootOut();
	void Hurt();
	void setBody();

private:
	int					m_shootTimes;
};


//��������˺��ĸ���
class AttackContinuous : public PhysicsUnit
{
public:
	static AttackContinuous* createCircle(float radius, float dur, int hurts, 
		int cateBit, int collisionBit, int contactBit);
	static AttackContinuous* createBox(const Size& size, float dur, int hurts, 
		int cateBit, int collisionBit, int contactBit);
	void UpdateBox(float dt);
	void UpdateCircle(float dt);
	void initBox(const Size& size);
	void initCircle(float radius);

	void RemoveWhenTimeout(float dur);
private:
	float		m_dur;
	float		m_radius;
	Size		m_Size;
	int			m_cateBit;
	int			m_collosionBit;
	int			m_contactBit;
	int			m_hurts;
};

//������ʾ
class SpecialSkillTips : public Layer
{
public:
	static SpecialSkillTips* create(int lvl);

	void addClippingNode();
	void addTips();

	virtual void onEnter();
	Node* getClippingContent();
	void removeMe();

	virtual void update(float delta);

private:
	Sprite*			m_bg1;
	Sprite*			m_bg2;
	Size			m_ws;
	int				m_lvl;
};

//ͨ��
class CompleteGame : public Layer
{
public:
	static CompleteGame* create(int lvl);
	void addClippingNode();
	Node* getClippingContent();
	void addBg();

	virtual void onEnter();

	//��ת���������
	void TurnToResultLayer();

private:
	Size			m_ws;
	int				m_lvl;
};

//��ͼ�л�
class MapSwitchTips : public Layer
{
public:
	static MapSwitchTips* create(int lvl, float restTime, float delay = 0.5f);
	virtual bool init();
	virtual void onEnter();
	void setRestTime(float t);
	void addTips();
	void removeSelf();

private:
	int			m_lvl;
	Size		m_ws;
	float		m_delay;
};

//���о���
class FlyDistance : public Node
{
public:
	static FlyDistance* create();
	virtual void onEnter();
	void refreshDis(float dt);

private:
	int			m_dis;
};


//��˸
ActionInterval* ShanshuoAction(const float& time); 

//��Բ�˶�
void RunEllipticAction(Node* node,const EllipticFunc::EllipticConfig& ec, float time, bool isleft);
cocos2d::Point MoveStep( const Point& Amipos, const Point& myPos,const float& maxPix );

#endif