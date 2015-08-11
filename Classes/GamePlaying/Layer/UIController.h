/********************************************************************
������:		2014/11/13  19:51	
�ļ���: 	E:\Cocos3.3_Project\MyGame\Classes\UIController.h 
��  ��:		alex(alex)
�޸��ڣ�	2014/11/13  19:51	

Ŀ  ��:		
*********************************************************************/
#ifndef _UI_CONTROLLER_H_
#define _UI_CONTROLLER_H_


#include "cocos2d.h"

USING_NS_CC;

class MenuLayer;
class PhysicsLayer;
class Bullet;
class EffectsManager;
class DropGold;
class SpecialSkillReward;
class GameMainLayer;
class GameResult;
class RollingBg;
class CloudLayer;

class UIController : public Ref
{
public:
	static UIController* getInstance()
	{
		static UIController instance;
		return &instance;
	}

	void setPhysicsLayer(PhysicsLayer* layer);
	PhysicsLayer* getPhysicsLayer();

	void setMenuLayer(MenuLayer* layer);
	MenuLayer* getMenuLayer();

	void setMainUILayer( GameMainLayer* layer );
	GameMainLayer* getMainUILayer( void );

	void setEffectsLayer(EffectsManager* effectsLayer);
	EffectsManager* getEffectsLayer();

	void setGameBgMgr(RollingBg* layer);
	RollingBg* getGameBgMgr();

	void addBulletsForTarget(Sprite* weapon,Bullet* bullet,const Point& pos, const Vec2& vecDirec, bool isleft);
	void addMissile(Bullet* bullet,const Point& pos);

	//��������
	void addScore(DropGold* sc,const Vec2& pos);

	//�µķ�ʽ
	void addBullets(Bullet* bullet, const Point& pos, const int& Zorder);

	void addBulletStreakEff(Bullet* bullet, const Point& pos, const int& Zorder = -1);


	//��ʾ�������
	void ShowResultLayer(bool isWin = true);

	//��������
	void addEnergyFullTips();		//��������ʾ

	void addCollectModeTips();		//�ռ�ģʽ��ʾ

	void addKillModeTips();			//��ɱģʽ��ʾ

private:
	UIController();
	~UIController();

	Size				m_winSize;

	PhysicsLayer*		m_physicsLayer;
	MenuLayer*			m_menuLayer;
	EffectsManager*		m_EffectsLayer;
	GameResult*			m_GameResoult;
	GameMainLayer*		m_GameMainUiLayer;
	RollingBg*			m_GameBgMgr;
	CC_SYNTHESIZE(CloudLayer*, m_CloudLayer,CloudLayer);
};

#endif
