/********************************************************************
������:		2015/02/15  10:11	
�ļ���: 	E:\Cocos3.3_Project\PlaneGame\Classes\Common\GameConfig.h 
��  ��:		alex(alex)
�޸��ڣ�	2015/02/15  10:11	

Ŀ  ��:		��Ϸ��������
*********************************************************************/
#ifndef __GameConfig_H__
#define __GameConfig_H__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "NewDataMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

//Ѫ��
#define Ene_Plane_Hp_A		120	
#define Ene_Plane_Hp_B		230	
#define Ene_Plane_Hp_C		500	
#define Ene_Plane_Hp_D		1200	
#define Ene_Plane_Hp_E		800	
#define Ene_Plane_Hp_6		250	
#define Ene_Plane_Hp_7		1300	
#define Ene_Plane_Hp_8		1000	
#define Ene_Plane_Hp_9		2000	
#define Ene_Plane_Hp_10		1500	
#define Ene_Plane_Hp_max	2000

#define Boss1_MaxBlood		25000
#define Boss2_MaxBlood		28000
#define Boss3_MaxBlood		30000
#define Boss4_MaxBlood		35000

#define Protect_Hp			10000

//�˺�
#define Ene_Blt_hurt		-30
#define Ene_Pl_hurt			-30
#define FallSton_hurt		-1000
#define Bow_hurt			-10000


//����1��������
#define SelfMainPl_1_NorBlt_hurt	-120							//��ͨ��׼ֵ
#define SelfMainPl_1_AddBlt_hurt	SelfMainPl_1_NorBlt_hurt * 1.8	//��ǿ��ֵ

//����2��������
#define SelfMainPl_2_NorBlt_hurt	SelfMainPl_1_NorBlt_hurt * 1.6				
#define SelfMainPl_2_AddBlt_hurt	SelfMainPl_1_AddBlt_hurt * 1.6

//����3��������
#define SelfMainPl_3_NorBlt_hurt	SelfMainPl_2_NorBlt_hurt * 1.8				
#define SelfMainPl_3_AddBlt_hurt	SelfMainPl_2_AddBlt_hurt * 1.8

//��ɱ�л��÷�
#define Score_Pl1			100
#define Score_Pl2			300
#define Score_Pl3			500
#define Score_Pl4			700
#define Score_Pl5			1200

//����Ѫ��
#define MainPl_1_Hp			100
#define MainPl_2_Hp			100 * 1.38
#define MainPl_3_Hp			100 * 1.38 * 1.38

#define MainPlBltSpeed		1500

#define	EnemyBulletZorder	-100
#define	SelfBulletZorder	-200

class DataConfig
{
public:
	//��ǰ���ǵ���ͨ�˺�
	static int getCurMainPlHurt()
	{
		int curPlIdx = NewDataMgr::getInstance()->mRoleConfig.roleIndex;
		int lvl = NewDataMgr::getInstance()->getRoleGradeWithId(curPlIdx);
		int hurt = NewDataMgr::getInstance()->getRoleAtkWithId(curPlIdx, lvl);

		return hurt;
	}

	static int getCardHurts(int cardTp, int lvl)
	{
		int curPlhurt = getCurMainPlHurt();
		int add = NewDataMgr::getInstance()->getCardAtk(cardTp, lvl);
		int eachHurt = curPlhurt * (1 + add * 1.0f / 100);
		return -eachHurt;
	}

	//����1����ͨ�ӵ��˺�
	static int getMainPl1NorHurt(float rate, int WeaponNum)
	{
		return SelfMainPl_1_NorBlt_hurt * rate/WeaponNum;
	}
	
	//����1�ļ�ǿ�ӵ��˺�
	static int getMainPl1AddHurt(float rate, int WeaponNum)
	{
		return SelfMainPl_1_AddBlt_hurt * rate/WeaponNum;
	}

	//����2����ͨ�ӵ��˺�
	static int getMainPl2NorHurt(float rate, int WeaponNum)
	{
		return SelfMainPl_2_NorBlt_hurt * rate/WeaponNum;
	}
	
	//����2�ļ�ǿ�ӵ��˺�
	static int getMainPl2AddHurt(float rate, int WeaponNum)
	{
		return SelfMainPl_2_NorBlt_hurt * rate/WeaponNum;
	}

	//����3����ͨ�ӵ��˺�
	static int getMainPl3NorHurt(float rate, int WeaponNum)
	{
		return SelfMainPl_3_NorBlt_hurt * rate/WeaponNum;
	}

	//����3�ļ�ǿ�ӵ��˺�
	static int getMainPl3AddHurt(float rate, int WeaponNum)
	{
		return SelfMainPl_3_NorBlt_hurt * rate/WeaponNum;
	}
};

#endif // __GameConfig_H__