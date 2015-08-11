/********************************************************************
������:		2014/12/10  14:49	
�ļ���: 	E:\Cocos3.3_Project\MyGame\Classes\PlaneManager.h 
��  ��:		alex(alex)
�޸��ڣ�	2014/12/10  14:49	

Ŀ  ��:		���ַɻ������ɹ���
*********************************************************************/
#ifndef _PLANE_MANAGER_H_
#define _PLANE_MANAGER_H_


#include "cocos2d.h"
#include "cocos-ext.h"
#include "../../GameCommon/GameDefine.h"

USING_NS_CC;
USING_NS_CC_EXT;

//�����ż�������Ʒ���
class PhysicsUnit;
class MainPlane;
class BasePlane;
class PlaneManager : public Ref
{
public:
	PlaneManager();
	~PlaneManager();
	static PlaneManager* create();
	void purge();

	void InitLvlData(const int& lvl);

	void refreshEnemyPlane();
	void AddGiftPlaneGroup();
	void addNextNormalGroup();
	void AnalyseOneGroup(TMXObjectGroup* group);	//�������е�ĳһ��
	BasePlane* CreateWithPro(const Vec2& pos, int plId, int weaponId, int bulletRes, 
		int actionId, int AIid, int gameLvl, float userdata);

	int getNamePro(TMXObjectGroup* group,const int& idx);
	
	//������һ�ؿ�
	void AddNextLevel();

	//��Ϣһ��ʱ��
	void Rest(float dur);
	void RestCallback();

	void AddOnePlane(PhysicsUnit* pl);
	void DelOnePlane(PhysicsUnit* pl);

	void resetData();
	
private:
	bool init();

	/************TMX����***************/
	Point getPosByTmx(ValueMap& dict);							//�л�λ��
	void AddDropsByTmx(ValueMap& dict,PhysicsUnit* pl);			//�л�������
	bool readGroupTmx(const int& gameLvl);						//��ȡTMX�ļ�

	Vector<TMXObjectGroup*>				m_GruopVec;
	Vector<TMXObjectGroup*>::iterator   m_GruopIt;

	CC_SYNTHESIZE(cocos2d::experimental::TMXTiledMap*,m_GroupTmx,GroupTmx);	//�л����
	CC_SYNTHESIZE(bool, m_IsGroupOver, IsGroupOver);				//��Ϸ�Ƿ����
	CC_SYNTHESIZE(int, m_curLvl, CurLvl);						//��ǰ�ؿ�
	float								m_restTime;				//��Ϣʱ��
	int									m_restCount;			//��Ϣ����
	std::list<PhysicsUnit*>				m_PlaneGroup;
};

class GiftPlaneManager : public Node
{
public:
	CREATE_FUNC(GiftPlaneManager);
	virtual void onEnter();

	//������������
	void addGiftPlaneGroup();
	void addNextGiftGroup();
	void addOneGiftPlane();

private:
	int				m_PlaneId;
	int				m_ActionId;
};


#endif