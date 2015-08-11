
/********************************************************************
������:		2015/01/22  14:41	
�ļ���: 	E:\Cocos3.3_Project\PlaneGame\Classes\UserInfo\DropGoods.h 
��  ��:		alex(alex)
�޸��ڣ�	2015/01/22  14:41	

Ŀ  ��:		��������
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
���ٷɻ���õ��Ľ���
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

//����ģʽ
enum DropMod
{
	Place_Mod,		//�ڷ�ģʽ
	Drop_Mod		//����ģʽ
};

//������
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
	virtual void PlayMusicWhenDead();			//������Ч

	void setUpSpeed(float speed);

	virtual void InitSpeed();
	//������Ե÷֡�������Ӱ��
	virtual void Rewards(){}
	void updateRewards(const int& rewardtype);

	void ActionCallback(Node* node);

	//ͳ�ƽӵ�������
	void StatictisNum();

	//������ɻ��Ƿ��д���״̬
	void IsMainPlHasMagnet(float dt);

	//�������ɻ�
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
	virtual void delayGive();		//�ӳٷ���ӵ�������
	virtual void PlayMusicWhenDead();			//������Ч

	//������
	void addFlower(int lvl,int hurts);

	//��������
	void addFeibiao(int lvl, int hurts);

	//��Ȧ����
	void addFireCircle(int lvl,int hurts);

	//��������
	void addMissile(int lvl,int hurts);

	//è����
	void addCat(int lvl, int hurts);

	//��׶����
	void addIcePiton(int lvl,int hurts);

	//������
	void addDragon(int lvl,int hurts);

	//��������
	void addLaser(int lvl,int hurts);

	//��������
	void addLighting(int lvl,int hurts);

};

//��ɫ����������
class DropRedWp : public WpDrops
{
public:
	CREATE_DROP(DropRedWp);
	virtual void onEnter();

	//�ӳٷ�������
	virtual void delayGive();
	virtual void OtherDealWhenContact(const ContactPro& rs);
};

//��ɫ����������
class DropYellowWp : public WpDrops
{
public:
	CREATE_DROP(DropYellowWp);

	virtual void onEnter();

	virtual void delayGive();
	virtual void OtherDealWhenContact(const ContactPro& rs);

};

//��ɫ����������
class DropBlueWp : public WpDrops
{
public:
	CREATE_DROP(DropBlueWp);
	virtual void onEnter();

	virtual void delayGive();
	virtual void OtherDealWhenContact(const ContactPro& rs);
};

//��������
class WeaponUpgradeDrop : public DropGold
{
public:
	CREATE_DROP(WeaponUpgradeDrop);
	void setBuff(const BuffStruct& buff);
	virtual void OtherDealWhenContact(const ContactPro& rs);
};

//����
class MainPlaneMagnet : public DropGold
{
public:
	CREATE_DROP(MainPlaneMagnet);
	virtual void onEnter();
	virtual void OtherDealWhenContact(const ContactPro& rs);

	static void GiftEff(const std::string& qianzhui, int num, const std::string& pic, const Vec2& pos = Vec2(0,0));
};

//��Ѫ
class HpAdditionDrop : public DropGold
{
public:
	CREATE_DROP(HpAdditionDrop);
	virtual void OtherDealWhenContact(const ContactPro& rs);
	void setAddHpNums(int hp);		//���ü�Ѫ��

};


//�����﹤��
class DropsFactory : public Ref
{
public:
	static DropsFactory* create();
	void purge();
	bool init();

	void AddDrops(const Point& pos, DropGoodsType tp, int nums = 1);

	//���1
	void DropGoldsType1(const Point& pos);

	//���2
	void DropGoldsType2(const Point& pos);

	//���3
	void DropGoldsType3(const Point& pos);

	//��ɫ����
	void DropWithRedWp(const Point& pos);

	//��ɫ����
	void DropWithBlueWp(const Point& pos);

	//��ɫ����
	void DropWithYellowWp(const Point& pos);

	//����BUFF1
	void DropWithWpBuff1(const Vec2& pos);
	//����BUFF2
	void DropWithWpBuff2(const Vec2& pos);
	//����BUFF3
	void DropWithWpBuff3(const Vec2& pos);

	//�ظ�Ѫ��100
	void AddHp100(const Vec2& pos);
	
	//�ظ�Ѫ��500
	void AddHp500(const Vec2& pos);

	//����
	void DropMagnet(const Vec2& pos);

	static std::string getResByTag(ResTag tag);

	Vec2 getRandPos(const Vec2& centerPos);

private:
	Vector<SpriteBatchNode*>	m_BatchNodeVec;


};



#endif
