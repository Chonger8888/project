
/********************************************************************
创建于:		2015/01/22  14:41	
文件名: 	E:\Cocos3.3_Project\PlaneGame\Classes\UserInfo\DropGoods.h 
作  者:		alex(alex)
修改于：	2015/01/22  14:41	

目  的:		凋落物类
*********************************************************************/
#ifndef __DropGoods_H__
#define __DropGoods_H__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "PhysicsUnit.h"
#include "GameCommon\GameDefine.h"

USING_NS_CC;
USING_NS_CC_EXT;


/************************************************************************/
/* 
击毁飞机后得到的奖励
*/
/************************************************************************/
#define CREATE_DROP(_ClassName_) \
static _ClassName_* create( const std::string& frameName, float UpSpeed = 200 ) \
{ \
	_ClassName_* s = new (std::nothrow) _ClassName_; \
	if (s && s->initWithSpriteFrameName(frameName)) \
	{ \
		s->autorelease(); \
		s->setUpSpeed(UpSpeed); \
		s->initBody(); \
		return s; \
	} \
	else \
	{ \
		CC_SAFE_DELETE(s); \
		return nullptr; \
	} \
} \

//掉落模式
enum DropMod
{
	Place_Mod,		//摆放模式
	Drop_Mod		//掉落模式
};

//掉落物
class DropGold : public PhysicsUnit
{
public:
	DropGold()
		:m_DropMod(DropMod::Drop_Mod)
		,m_MagnetDis(150)
		,m_UpSpeed(200.0f)
	{

	}
	CREATE_DROP(DropGold);
	static DropGold* create(const Vec2& pos, ResTag res = ResTag::Drops_Golds1,  int glod  = 0, int score = 0);
	virtual void onEnter();
	virtual void initBody();
	virtual void DoSomethingBeforeDead();			
	virtual void InitEdgeLine();
	virtual void PlayMusicWhenDead();			//死亡音效

	void setUpSpeed(float speed);

	virtual void InitSpeed();
	//死亡后对得分、奖励的影响
	virtual void Rewards(){}
	void updateRewards(const int& rewardtype);

	void ActionCallback(Node* node);

	//统计接到的数量
	void StatictisNum();

	//检测主飞机是否有磁铁状态
	void IsMainPlHasMagnet(float dt);

	//飞向主飞机
	void MoveToMainPl(float dt);
	void setRewardsInfo(int gold = 0, int score = 0);
protected:
	CC_SYNTHESIZE_PASS_BY_REF(DropMod,m_DropMod,DropMod);
	float			m_MagnetDis;
	float			m_UpSpeed;
	Vec2			m_StartPos;
};

class WpDrops : public DropGold
{
public:
	virtual void Rewards();
	virtual void delayGive();		//延迟发射接到的武器
	virtual void PlayMusicWhenDead();			//死亡音效

	//花类型
	void addFlower(int lvl,int hurts);

	//飞镖类型
	void addFeibiao(int lvl, int hurts);

	//火圈类型
	void addFireCircle(int lvl,int hurts);

	//导弹类型
	void addMissile(int lvl,int hurts);

	//猫类型
	void addCat(int lvl, int hurts);

	//冰锥类型
	void addIcePiton(int lvl,int hurts);

	//龙类型
	void addDragon(int lvl,int hurts);

	//激光类型
	void addLaser(int lvl,int hurts);

	//闪电类型
	void addLighting(int lvl,int hurts);

};

//红色武器掉落物
class DropRedWp : public WpDrops
{
public:
	CREATE_DROP(DropRedWp);
	virtual void onEnter();

	//延迟发射武器
	virtual void delayGive();
	virtual void OtherDealWhenContact(const ContactPro& rs);
};

//黄色武器掉落物
class DropYellowWp : public WpDrops
{
public:
	CREATE_DROP(DropYellowWp);

	virtual void onEnter();

	virtual void delayGive();
	virtual void OtherDealWhenContact(const ContactPro& rs);

};

//蓝色武器掉落物
class DropBlueWp : public WpDrops
{
public:
	CREATE_DROP(DropBlueWp);
	virtual void onEnter();

	virtual void delayGive();
	virtual void OtherDealWhenContact(const ContactPro& rs);
};

//武器升级
class WeaponUpgradeDrop : public DropGold
{
public:
	CREATE_DROP(WeaponUpgradeDrop);
	void setBuff(const BuffStruct& buff);
	virtual void OtherDealWhenContact(const ContactPro& rs);
};

//磁铁
class MainPlaneMagnet : public DropGold
{
public:
	CREATE_DROP(MainPlaneMagnet);
	virtual void onEnter();
	virtual void OtherDealWhenContact(const ContactPro& rs);

	static void GiftEff(const std::string& qianzhui, int num, const std::string& pic, const Vec2& pos = Vec2(0,0));
};

//加血
class HpAdditionDrop : public DropGold
{
public:
	CREATE_DROP(HpAdditionDrop);
	virtual void OtherDealWhenContact(const ContactPro& rs);
	void setAddHpNums(int hp);		//设置加血量

};


//凋落物工厂
class DropsFactory : public Ref
{
public:
	static DropsFactory* create();
	void purge();
	bool init();

	void AddDrops(const Point& pos, DropGoodsType tp, int nums = 1);

	//金币1
	void DropGoldsType1(const Point& pos);

	//金币2
	void DropGoldsType2(const Point& pos);

	//金币3
	void DropGoldsType3(const Point& pos);

	//红色武器
	void DropWithRedWp(const Point& pos);

	//蓝色武器
	void DropWithBlueWp(const Point& pos);

	//黄色武器
	void DropWithYellowWp(const Point& pos);

	//武器BUFF1
	void DropWithWpBuff1(const Vec2& pos);
	//武器BUFF2
	void DropWithWpBuff2(const Vec2& pos);
	//武器BUFF3
	void DropWithWpBuff3(const Vec2& pos);

	//回复血量100
	void AddHp100(const Vec2& pos);
	
	//回复血量500
	void AddHp500(const Vec2& pos);

	//磁铁
	void DropMagnet(const Vec2& pos);

	static std::string getResByTag(ResTag tag);

	Vec2 getRandPos(const Vec2& centerPos);

private:
	Vector<SpriteBatchNode*>	m_BatchNodeVec;


};



#endif
