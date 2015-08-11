/********************************************************************
创建于:		2014/11/26  11:05	
文件名: 	E:\Cocos3.3_Project\MyGame\Classes\PhysicsUnit.h 
作  者:		alex(alex)
修改于：	2014/11/26  11:05	

目  的:		所有附带物理属性的精灵的基类
*********************************************************************/

#ifndef _PHYSICS_UNIT_H_
#define _PHYSICS_UNIT_H_

#include "cocos2d.h"
#include "Buff.h"
#include "../GameCommon/GameDefine.h"

USING_NS_CC;

//BUFF结构体
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

//物品掉落
struct DropReward
{
	float					HpPercent;	//血量百分比
	int						nums;			//掉落数量
	DropGoodsType			DropTp;			//掉落物品类型
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

//刚体精灵属性
struct SelfPro
{
	UnitId				idx;			//类别
	UnitId				belongTo;		//属于谁，来自于谁
	int					defense;		//防御
	int					curHP;			//当前血量
	int					maxHP;			//血上限
	int					isInvincible;	//是否无敌	
	int					score;			//死亡给出的分数
	int					goldsNum;		//死亡给出的金币数
	int					powerNum;		//能量
	std::vector<DropReward>	rewardVec;	//在什么血量掉落什么物品

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

//该单元能送给对方的奖励或BUFF
struct ContactPro
{
	UnitId			idx;				//ID与SelfPro的ID对应
	UnitId			belongTo;			//属于谁，来自于谁
	int				hurts;				//伤害
	int				effectsid;			//触发特效id（0表示无特效）
	float			ActionSpeed;		//减缓动作执行速度的百分比(0.0 - 1.0)
	bool			isTarget;			//是否被导弹锁定
	BuffStruct		buffPro;	//BUFF
	Vec2			pos;		//碰撞时的位置
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
	virtual void PlayMusicWhenContact(){}		//碰撞音效
	virtual void PlayMusicWhenDead(){}			//死亡音效
	virtual void AnimationWhenDead(){}			//死亡特效

	void setUnitId(const UnitId& idx);
	void setBelongTo(const UnitId& idx);
	void setCantContact();

	//实时判断死亡
	void CheckDead(float dt);
	void setDirectDeath();						//设置强制死亡
	void setHpEmpty();							//设置血量为0
	void setHpEmptyAndInvincible();				//设置血量为空，并且无敌(主要用于boss)

	//从内存中移除自己
	virtual void RemoveMyself();						//唯一的死亡函数(防止多次删除导致崩溃)
	virtual void DoSomethingBeforeDead(){}		//死亡之前要做的事

	//判断是否进入了屏幕
	virtual void CheckIsInScreen(float dt);

	//检测是否在屏幕之外
	virtual void CheckIsOutScreen(float dt);
	
	//基本属性操作
	void setInvincible(int invincible);
	void setMaxBlood(int max);
	
	//当自己发生碰撞时,改变自己的属性
	void ChangeProperty(const ContactPro& rs);

	/************************************************************************/

	//碰撞属性影响
	void ImpactOfHp(const ContactPro& rs);						//血量影响
	void ImpactOfSpecialEffects(const int& idx);				//触发特效
	
	virtual void ImpactOfBuff(const ContactPro& rs);	//BUFF影响

	void DeathImpactOfDrops();							//死亡得分
	void DeathImpactOfDropReward();						//当血量少于指定比例时，掉落指定物品
	virtual void OtherDealWhenContact(const ContactPro& rs){}	//其他碰撞影响
	virtual void FuncHpEmpty(){}
	/************************************************************************/
	virtual void FuncWhenHpChanged(){}

	float getHpPercent();		//得到当前血量的百分比
	bool isLiving();				//血量是否为0

	float getEdgeLine(const int& idx);		//删除线（各单元在由4条线包围的框外会被删除）

	void setContactPro(const ContactPro& rs){m_ContactPro = rs;}
	ContactPro getContactPro(){return m_ContactPro;}

	void setSelfPro(const SelfPro& ua){m_SelfPro = ua;}
	SelfPro getSelfPro(){return m_SelfPro;}

	void CanRemoveAtLine();
	void setInvincibleAtLine(bool b = true);

	virtual void InitEdgeLine();

	//击毁敌机统计
	virtual void EnemyPlaneStatistics(){}

	//游戏暂停与恢复
	void GameIsPause(Ref* obj);
	void GameIsResume(Ref* obj);

	virtual void deadLog(){}
//
//	//<<*****************设置色调*********************/
//public:
//	float getHue();
//	void setHue(float hue);	//0~2π
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
//	//*****************设置色调*********************>>/

protected:
	Size				m_WinSize;
	bool				m_isInvincibleAtLine;	//在删除线上是否无敌
	bool				m_isNaturalDead;		//是否自然死亡
	bool				m_isDirectDeath;		//强制死亡（用于特效）
	bool				m_isOutScreen;			//是否出了屏幕

	std::vector<float>	m_EdgeLineVec;
	ContactPro			m_ContactPro;		//奖励信息
	SelfPro				m_SelfPro;			//自己的属性

	UnitId				m_Killer;			//击杀者
	bool				m_isRemoving;
};


#endif