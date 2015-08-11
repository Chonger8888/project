/********************************************************************
创建于:		2015/01/21  15:56	
文件名: 	E:\Cocos3.3_Project\PlaneGame\Classes\Common\AI.h 
作  者:		alex(alex)
修改于：	2015/01/21  15:56	

目  的:		AI类
*********************************************************************/
#ifndef __AI_H__
#define __AI_H__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "../editor-support/cocostudio/CCArmature.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocostudio;

#define CREATE_AI(__TYPE__) \
static __TYPE__* create( BasePlane* pl ) \
{ \
	__TYPE__* plAi = new (std::nothrow) __TYPE__; \
	if (plAi) \
	{ \
		plAi->setTarget(pl); \
		plAi->init(); \
		plAi->autorelease(); \
		return plAi; \
	} \
	else \
	{ \
		CC_SAFE_DELETE(plAi); \
		return nullptr; \
	} \
} \

class BasePlane;

//飞机基本AI
class PlaneAI : public Node
{
public:
	PlaneAI()
	{
		m_ShootInterval = 4.5f + rand_minus1_1() * 0.2f;
	}
	void setSpeed(float speed){m_speed = speed;}
	virtual void StartAI(int lvl = 1) = 0;		//外部调用AI的开始接口
	virtual void DeathAction(){}
	virtual void DeadArmature(){}

	float getShootInterval();

	//发射子弹
	void StartShoot();
	void shoot();

protected:
	virtual bool init();

protected:
	CC_SYNTHESIZE(BasePlane*, m_target, Target);
	Size		m_winSize;
	float		m_speed;
	int			m_AIlvl;		//AI等级
	float		m_ShootInterval;	//射击间隔
};

//敌方普通飞机AI NO.1：向下飘落
class EnemyAiOfMoveDown : public PlaneAI
{
public:
	CREATE_AI(EnemyAiOfMoveDown);

	virtual bool init();
	virtual void StartAI(int lvl);
	void FloatDown(float dt);

private:
	float			m_direc;
};

//敌方普通飞机AI NO.2：朝着主飞机飞行
class EnemyAiOfFlyToTarget : public PlaneAI
{
public:
	CREATE_AI(EnemyAiOfFlyToTarget);
	virtual bool init();
	virtual void StartAI(int lvl);
	void FlyToTarget( float t );
};

//敌方普通飞机AI NO.3:悬停+攻击
class EnemyAiOfHoverAttack : public PlaneAI
{
public:
	CREATE_AI(EnemyAiOfHoverAttack);
	virtual bool init();
	virtual void StartAI(int lvl);

	//悬停微动
	void Hover();

	//瞄准镜移动
	void AimMove();

	void AimMoveCallback();

	//冲向目标
	void MoveToMainPl();

private:
	Sprite*			m_aimSp;
	Point			m_aimPos;
};

//敌方普通飞机AI NO.4:悬停左右随机飞行
class EnemyAiOfFloat : public PlaneAI
{
public:
	CREATE_AI(EnemyAiOfFloat);
	virtual bool init();
	virtual void StartAI(int lvl);

	void RandFloat();
private:
	Vec2		m_OriginPos;
};

//敌方普通飞机AI NO.5：静止不动，呼吸
class EnemyAiofStay : public PlaneAI
{
public:
	CREATE_AI(EnemyAiofStay);
	virtual bool init();
	virtual void StartAI(int lvl );

	void Stay();
};

//敌方普通飞机AI NO.5:在屏幕上方随意飞行
class EnemyAiofFree : public PlaneAI
{
public:
	CREATE_AI(EnemyAiofFree);
	virtual bool init();
	virtual void StartAI(int lvl );

	void MoveFree();

};

//BOSS的AI基类
class BossAI : public PlaneAI
{
public:
	//左右随机移动
	virtual ActionInterval* randMove();

	//移动到中间偏右
	virtual ActionInterval* MoveToMidRight();

	//移动到中间
	virtual ActionInterval* MoveToMid();

	//从一端移动到另一端
	virtual ActionInterval* MoveToEdge();

	//冲向主角
	virtual ActionInterval* MoveToMainPl();

	//向下冲
	virtual ActionInterval* MoveDown();

	//得到目标点与BOOS点的距离
	float getDis(const Vec2& pos);

	void playArmat(int idx, float speedScale = 1.0f);
	void playArmat(int idx,Ref *target, SEL_FrameEventCallFunc callFunc,float speedScale = 1.0f);

	//方向不定时改变
	void changeDirect();
	void changeDirCB();

	//死亡动作
	virtual void DeathAction();
	virtual void DeadArmature();
	int getBossHurts();
protected:
	float						m_direct;
	std::vector<int>			m_ActionVec;
	std::vector<int>::iterator	m_it;
};

//BOSS1的AI
class IronManBossAI : public BossAI
{
public:
	CREATE_AI(IronManBossAI);
	virtual void StartAI(int lvl);

	void NewAction();
	
	//预定动作
	void initAction();

	//不发射子弹的动作
	void NextAction();

	//动作1:连续发一个子弹
	void Action1();
	
	//动作2：相继发5个子弹
	void Action2();

	//动作3：同时发3个子弹
	void Action3();

	//动作4：朝目标砍杀
	void Action4();
	void Action4_1();

	//动作5：朝下砍杀
	void Action5();

	//动作6：发射中心爆裂弹
	void Action6();

	//动作7：发射随机爆裂弹
	void Action7();

private:
	//向下发射一个手里剑
	void shoot1Sword();

	//向下发射三个散状飞镖
	void shoot3Dart();

	//向下发射2个散状飞镖
	void shoot2Dart();

	//向下发射一组3个的散状手里剑
	void shoot3Sword();

	//朝屏幕中心发射一个爆裂弹
	void shootExplosionInMid();

	//在屏幕的随机一个点发射一个爆裂弹
	void shootExplosionRand();

	//播放某次骨骼动画
	void PlayArmatWith1Sword();		//发射一个手里剑
	void PlayArmatWith3Sword();		//发射3个手里剑
	void PlayArmatWith3Dart();		//发射3个飞镖
	void PlayArmatWith2Dart();		//发射2个飞镖
	void PlayArmatWithReady();		//发射前的准备
	void PlayArmatWithMvAttack1();	//朝向主角冲刺1
	void PlayArmatWithMvAttack2();	//朝向主角冲刺2
	void PlayArmatWithMvDown();		//向下冲刺
	void PlayArmatWithMidExpl();	//发射一个中心爆裂弹
	void PlayArmatWithRandExpl();	//发射一个随机爆裂弹


	
	void onFrameEventOfIdex0(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	//帧事件：发射飞镖
	void onFrameEventOf3Dart(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);
	void onFrameEventOf2Dart(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	//帧事件：发射手里剑
	void onFrameEventOf1Sword(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);
	void onFrameEventOf3Sword(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	void onFrameEventOfAttack1(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);
	void onFrameEventOfAttack2(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	//向下冲刺攻击
	void onFrameEventOfMoveDown(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	//帧事件：发射爆裂弹,中心位置
	void onFrameEventOfMidExpl(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	//帧事件：发射爆裂弹,随机位置
	void onFrameEventOfRandExpl(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	void setGunRotate(float r);
	Vec2 getShootPos();
	Vec2 getGunPos();
private:

	float						m_OriginRotate;
	Sprite*						m_Huohua;		//火花
	int							m_level;		//AI等级
};

//骷髅BOSS的AI
class BoneWarriorAI : public BossAI
{
public:
	CREATE_AI(BoneWarriorAI);
	virtual void StartAI(int lvl);

	void FixPos();
	//左右随机移动
	virtual ActionInterval* randMove();

	//发射2列曲线子弹
	void shoot2BezierBullet();

	//发射1个跳弹
	void shoot1JumpBullet();

	void ActionRest();
	void Action1();
	void Action2();
	void Action3();
	void Action4();
	void Action5();
	void NextAction();
	void initAction();

	//流弹
	void addLiudan();		//打BOSSS时间超时
	void addLiudanCB();

	//死亡动作
	//virtual void DeathAction();
	//virtual void DeadArmature();

	//在左右移动的同时，同时向两边发射一个反弹子弹
	void ShootReboundDown();

	//随机一个方向上发射一颗反弹子弹
	void ShootReboundRandom();

	//在左右两边各发射三个反弹子弹
	void Shoot6Rebound();

	//朝主角发射一颗反弹子弹
	void ShootReboundToMainPlane();

	//从左/右边发射一个流弹
	void ShootLiudanL();
	void ShootLiudanR();

private:
	void changeDirect();
	void changeDirCB();


	int		m_times;			//动作重复次数
};

//第三个BOSS的AI
class PhysicsUnit;
class RenzheBossAI : public BossAI
{
public:
	RenzheBossAI()
	{
		m_Dart = nullptr;
		m_shootDirect = -1;
		m_sandanCount = 0;
	}
	CREATE_AI(RenzheBossAI);
	virtual void StartAI(int lvl);
	void initAction();
	void NewAction();

	void ActionRest();
	void ActionShootDart1();
	void ActionShootDart2();
	void ActionShootDart3();
	void ActionShootDart4();
	void ActionShootExecration();

	virtual ActionInterval* MoveToMidRight();
	virtual ActionInterval* MoveToMid();

	//确定飞镖的初始位置
	Vec2 getDartLocalPos();
	Vec2 getDartWorldPos();

	//创建飞镖
	PhysicsUnit* CreateDart(bool isWudi = false);

	//飞镖轨迹的点
	PointArray* getPointArray1();		//飞镖的位置到主飞机的轨迹点（都转化为世界坐标）
	PointArray* getPointArray2();

	//轨迹1
	void DartAction1();

	//轨迹2
	void DartAction2();

	//轨迹3
	void DartAction3();

	//慢慢生成2个飞镖，一段时间后走贝赛尔曲线
	void DartAction4();

	//发射散弹
	void ShootSandan();

	//生成一片云
	void addCloud();
	void setArmatFunc(cocos2d::Ref *target, SEL_FrameEventCallFunc callFunc, float speed = 1.0);

	//念咒时的吸气
	void ExecrationParticle();
	void removeExecrationParticle();

	void pauseArmat();
	void resumeArmat();
	Vec2 getReadyOffset();

	void Ready(Ref* obj, SEL_CallFunc func);				//准备动作
	void Ready1();
	void Ready2();
	void Ready3();
	void Ready4();

	void PlayArmatRest();		//休息动画
	void PlayArmatKeep(float speed = 1.0f);		//保持动作动画

	void PlayArmatShoot1();		//发射轨迹1飞镖动画
	void PlayArmatShoot2();		//发射轨迹2飞镖动画
	void PlayArmatShoot3();		//发射轨迹3飞镖动画
	void PlayArmatCall2Darts();	//召唤2个飞镖动画
	void PlayArmatExecration();	//念咒动画
	void PlayArmatHuishou();	//回收飞镖动画

	//各种帧事件
	void onFrameEventOfShoot1(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);		//轨迹1
	void onFrameEventOfShoot2(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);		//轨迹2
	void onFrameEventOfShoot3(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);		//轨迹3
	void onFrameEventOfCall2Darts(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);	//召唤2个飞镖
	void onFrameEventOfExecration(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);	//念咒
	void onFrameEventOfHuishou(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);	//回收
	void onFrameEventOfKeep(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);		//保持姿势
	void onFrameEventOfRest(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);		//休息
	void onFrameEventOfReady(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);		//准备

	virtual void DeathAction();
	Vec2 getExecrationPos();

private:
	PhysicsUnit*				m_Dart;
	int					m_shootDirect;
	float				m_action1Time;
	float				m_action2Time;
	float				m_action3Time;
	float				m_action4Time;
	float				m_sandanSpeed;
	float				m_sandanNum;		//散弹数量
	int					m_sandanCount;		//散弹发射次数
	int					m_sandanCountMax;	//散弹发射次数上限
};

//海神AI
class PoseidonBossAI : public BossAI
{
public:
	CREATE_AI(PoseidonBossAI);
	virtual void StartAI(int lvl);
	void initAction();
	void NewAction();

	void Action0();
	void Action1();
	void Action2();
	void Action3();
	void Action4();
	void Action5();
	void Action6();

	/******发射子弹*****/

	//一横排子弹
	void ShootOneLineBullet();

	//射2列贝赛尔曲线
	void ShootTwoBezierBulletL();
	void ShootTwoBezierBulletR();

	//从底部飘出一个水泡，爆炸
	void ShootOneShuiPaoBullet();

	//在BOSS周边生成3个水泡
	void Shoot3ShuiPaoBullet();

	//发射旋转头晕子弹
	void ShootRotateBullet();

	//发射三角形子弹
	void SHootTriangleBullet(float angle);

	void ShootTriangBulletSomeTimes();

	/***************播放骨骼动画**************/
	void PlayAnimation1();
	void PlayAnimation2L();
	void PlayAnimation2R();
	void PlayAnimation3();
	void PlayAnimation4();
	void PlayAnimation5();
	void PlayAnimation6();
	void PlayAnimationRest();
	void PlayAnimationRest(float dur);

	/***************骨骼动画帧动画*************/
	void onFrameEventOfShoot1(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);		
	void onFrameEventOfShoot2L(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);		
	void onFrameEventOfShoot2R(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);		
	void onFrameEventOfShoot3(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);		
	void onFrameEventOfShoot4(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);		
	void onFrameEventOfShoot5(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);		
	void onFrameEventOfShoot6(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);		

	void PauseDur(float dt);
	void pauseArmat();
	void resumeArmat();
	void changeSpeed(float scale);

	Vec2 getLeftHandPos();
	Vec2 getRightHandPos();
private:
	int			m_bezierTimes;		//发射贝塞尔曲线子弹次数
};

//AI工厂
class PlaneAIFactory : public Ref
{
public:
	static PlaneAIFactory* create();
	void purge();

	bool init();

	PlaneAI* AddEnemyAiByIdx(BasePlane* pl, int id);


};

#endif // __AI_H__