/********************************************************************
������:		2014/11/17  16:08	
�ļ���: 	E:\Cocos3.3_Project\MyGame\Classes\BaseClass.h 
��  ��:		alex(alex)
�޸��ڣ�	2014/11/17  16:08	

Ŀ  ��:		
*********************************************************************/
#ifndef _GAME_DEFINE_H_
#define _GAME_DEFINE_H_

#include "cocos2d.h"

USING_NS_CC;


/************************************************************************/
/* 
//��ײɸѡ
*/
/************************************************************************/

#define SELF_AMMO_CATEGORYBITMASK		0x00000010		//������ҩ�ķ�������CategoryBitmask
#define SELF_AMMO_COLLISIONBITMASK		0x00010001		//������ҩ����ײ����collisionBitmask
#define SELF_AMMO_COLLISIONBITMASK_P	0x00000000		//��͸
#define SELF_AMMO_CONTACTTESTBITMASK	0x00010001		//������ҩ����ײ��������contacttestbitmask

#define SELF_PLANE_CATEGORYBITMASK		0x00000100		//�����ɻ��ķ�������CategoryBitmask
#define SELF_PLANE_COLLISIONBITMASK		0x00101021		//�����ɻ�����ײ����collisionBitmask
#define SELF_PLANE_CONTACTTESTBITMASK	0x00111021		//�����ɻ�����ײ��������contacttestbitmask

#define ENEMY_AMMO_CATEGORYBITMASK		0x00001000		//�з���ҩ�ķ�������CategoryBitmask
#define ENEMY_AMMO_COLLISIONBITMASK		0x10000101		//�з���ҩ����ײ����collisionBitmask
#define ENEMY_AMMO_CONTACTTESTBITMASK	0x10000101		//�з���ҩ����ײ��������contacttestbitmask

#define ENEMY_PLANE_CATEGORYBITMASK		0x00010000		//�з��ɻ��ķ�������CategoryBitmask
#define ENEMY_PLANE_COLLISIONBITMASK	0x10000010		//�з��ɻ�����ײ����collisionBitmask
#define ENEMY_PLANE_COLLISIONBITMASK_P	0x00000000		//�з��ɻ�����ײ����collisionBitmask
#define ENEMY_PLANE_CONTACTTESTBITMASK	0x10000110		//�з��ɻ�����ײ��������contacttestbitmask

#define REWARD_CATEGORYBITMASK			0x00100000		//�����ķ�������CategoryBitmask
#define REWARD_COLLISIONBITMASK			0x00000100		//��������ײ����collisionBitmask
#define REWARD_CONTACTTESTBITMASK		0x00000100		//��������ײ��������contacttestbitmask

//�������и���ɸѡ���ȿ���з��ɻ���ײ��Ҳ����з��ӵ���ײ��
#define SELF_SCOPE_CATEGORYBITMASK		0x00000110		//
#define SELF_SCOPE_COLLISIONBITMASK		0x00011021		//
#define SELF_SCOPE_CONTACTTESTBITMASK	0x00011021		//

//������ը����ɸѡ��ֻ��л�������ײ���Ҵ�Խ
#define SELF_EXPLOSION_CATEGORYBITMASK		0x00000010		//
#define SELF_EXPLOSION_COLLISIONBITMASK		0x00000000		//
#define SELF_EXPLOSION_CONTACTTESTBITMASK	0x00010001		//

//����Χǽ��ֻ���뵯�鷢��ʵ����ײ
#define WALL_CATEGORYBITMASK			0x01000000		
#define WALL_COLLISIONBITMASK			0x10000020		
#define WALL_CONTACTTESTBITMASK			0x10000020	

//������壬��л�\�з��ӵ���Χǽ����ʵ����ײ
#define HOODLE_CATEGORYBITMASK			0x10000000		
#define HOODLE_COLLISIONBITMASK			0x11011002		
#define HOODLE_COLLISIONBITMASK_P		0x11000002		//��л�������͸��ײ
#define HOODLE_CONTACTTESTBITMASK		0x11011002		

//���壬ֻ�뵯�鷢����ײ
#define PADDLE_CATEGORYBITMASK			0x00000002		
#define PADDLE_COLLISIONBITMASK			0x10000000		
#define PADDLE_CONTACTTESTBITMASK		0x10000000		

// boss������ ����Χǽ��0x00000100  ��л���0x00000002
#define ENEMY_AMMO2_CATEGORYBITMASK		0x00000020		//�з�������ҩ�ķ�������CategoryBitmask
#define ENEMY_AMMO2_COLLISIONBITMASK	0x01000100		//�з�������ҩ����ײ����collisionBitmask
#define ENEMY_AMMO2_COLLISIONBITMASK_P	0x01000000		//�з�������ҩ����ײ����collisionBitmask
#define ENEMY_AMMO2_CONTACTTESTBITMASK	0x01000100		//�з�������ҩ����ײ��������contacttestbitmask

#define PixelToMeter	5			//�������׵�ת���ʣ��磺10px = 1�ף�

#define Pi2Angle 180/M_PI
#define Angle2pi M_PI/180


//��������徫�������ID
enum class UnitId
{
	DefaultIdx = -1,

	eBullet_1,
	eBullet_2,
	eBullet_3,
	eBullet_4,
	eBullet_5,
	eBullet_6,
	eBullet_7,
	eBullet_8,
	eBullet_9,
	eBullet_10,
	eBullet_11,
	eBullet_12,
	eBullet_13,
	eBullet_14,
	eBullet_15,
	eBullet_16,
	eBullet_17,
	eBullet_18,
	eBullet_19,
	eBullet_20,
	eBullet_21,
	eBullet_22,
	eBullet_23,
	eBullet_24,
	eBullet_25,
	eBullet_26,
	eBullet_27,
	eBullet_28,
	eBullet_29,
	eBullet_30,
	eBullet_31,
	eBullet_32,
	eBullet_33,
	eBullet_34,
	eBullet_35,
	eBullet_36,
	eBullet_37,
	eBullet_38,
	eBullet_max,

	ePlaneBoss,

	eBottomWall,		//4��ǽ
	eLeftWall,			
	eRightWall,			
	eUpWall,			
	eScreenExplosion,	//ȫ����ը
	eFanghudun,			//������
	eScopeExplosion,	//��Χ��ը

	eRedCard,			//3�ֿ���
	eBlueCard,
	eYellowCard,

	ePlane_1,
	ePlane_2,
	ePlane_3,
	ePlane_4,
	ePlane_5,
	ePlane_6,
	ePlane_7,
	ePlane_8,
	ePlane_9,
	ePlane_10,
	ePlane_11,
	ePlane_12,
	ePlane_13,
	ePlane_14,
	ePlane_15,
	ePlane_16,
	ePlane_17,
	ePlane_18,
	ePlane_19,
	ePlane_20,
	ePlane_21,
	ePlane_22,
	ePlane_max,

	eMainPlane,

};

//BUFF��ID
enum class BuffId
{
	Buff_default = 0,		//Ĭ����Ч
	Buff_Sandan,			//ɢ��BUFF
	Buff_Burning,			//ȼ��BUFF
	Buff_TempNorWp1,		//������ͨ�ӵ�BUFF1
	Buff_TempNorWp2,		//������ͨ�ӵ�BUFF2
	Buff_TempNorWp3,		//������ͨ�ӵ�BUFF3
	Buff_LightingLvl1,		//����buffһ��
	Buff_LightingLvl2,		//����buff����
	Buff_LightingLvl3,		//����buff����
	Buff_Magnet,			//����
};


//�ӵ���Դ����
enum class ResTag
{
	defaultType,
	
	//С���ӵ�
	Bullet_EnemyNor1,
	Bullet_EnemyNor2,
	Bullet_EnemyNor3,
	Bullet_EnemyNor4,

	//BOSS�ӵ�
	Bullet_BossFireAmmo,
	Bullet_BossShuiPaoBig,
	Bullet_BossShuiPaoSmall,
	Bullet_BossKulou,
	Bullet_BossBigDart,
	Bullet_BossSmallDart,

	//�����ӵ�
	Bullet_Self_0,
	Bullet_Self_1,
	Bullet_Self_2,
	Bullet_Self_3,
	Bullet_Self_4,
	Bullet_Self_5,
	Bullet_Self_6,
	Bullet_Self_7,
	Bullet_Self_8,
	Bullet_Self_9,
	Bullet_Self_10,
	Bullet_Self_11,
	Bullet_Self_12,
	Bullet_Self_13,
	Bullet_Self_14,
	Bullet_Self_15,
	Bullet_Self_16,
	Bullet_Self_17,
	Bullet_Self_18,
	Bullet_Self_19,
	Bullet_Self_20,
	Bullet_Self_21,
	Bullet_Self_22,

	//�����ӵ�
	Bullet_Card_BigDart,
	Bullet_Card_SmallDart,

	//������
	Drops_Golds1,
	Drops_Golds2,
	Drops_Golds3,
	Drops_WpBuff1,

	Drops_RedWp,
	Drops_YellowWp,
	Drops_BlueWp,

	Drops_Magnet,
	Drops_AddHp100,
	Drops_AddHp500,

};


//�ӵ�����
enum class BulletType
{
	Self_Default,

	Self_NorLinear,			//��ͨ�����ӵ�
	Self_ToTarget,			//��ͨ����Ŀ����ӵ�

	Self_DartBullet,		//�����
	Self_SmallDartBullet,	//С����
	Self_LaserBullet,
	Self_MissleForDis,		//����
	Self_Cross,
	
	Enemy_default,
	Enemy_Bullet1,
	Enemy_Gravity,
	Enemy_FallIng,
	Enemy_Bezier,
	Enemy_BezierLeft,		// �����÷��������
	Enemy_BezierRight,		// �����÷��������
	Enemy_BossRebound,			// �����ӵ�
	Enemy_Grenade,			// ��
	Enemy_HelixBullet,		// ��ת���ӵ�
	Enemy_HelixExBullet,	// ��ת���ӵ�WX
	Enemy_FallingExBullet,	// Ʈ���ӵ�
	Enemy_Fireworks,		//�̻�
	Enemy_Rebounce,			//����
	Enemy_BossDart,			//BOSS��������
	Enemy_Boss4Bezier,		//BOSS4�ı������ӵ�

	Boss1_GravityBullet,

	Convolution,			//����
};

//������-����
enum class DropGoodsType
{
	DropGoods_Default = 0,
	DropGoods_RedWp,
	DropGoods_BlueWp,
	DropGoods_YellowWp,

	DropGoods_Gold1,
	DropGoods_Gold2,
	DropGoods_Gold3,
	DropGoods_NorWpEx,			//��ǿ����ͨ�ӵ�
	DropGoods_AddHp100,
	DropGoods_AddHp500,
	DropGoods_Magnet,

};

enum class PlaneType
{
	PlaneType_Wind = 1,
	PlaneType_Fire,
	PlaneType_Electric,
};


//����ͳ�ƽṹ��
typedef struct EnemyStatistics
{
	UnitId		EnePlaneId;		//�����л�����
	UnitId		KillerId;		//��ɱ�õл���Դͷ����
	int			Score;			//��ɱ�÷�

	EnemyStatistics(const UnitId& pId, const UnitId& kId, int score)
	{
		EnePlaneId = pId;
		KillerId = kId;
		Score = score;
	}
}ES;

//�л������������ɻ�Ѫ��
typedef struct HpEff
{
	float		rate;		//����
	float		hp;			//����HP��ֵ
	HpEff()
	{
		rate = -1.0f;		//Ĭ��С��0����ʾ��Ч
		hp = 0.0f;
	}

	HpEff(float r, float h)
	{
		rate = r;		
		hp = h;
	}

}HpEff;

//BOSS�����ȼ�
enum class BossRating
{
	e_SS,
	e_S,
	e_A,
	e_B,
};

//BOSS���ͼ������ȼ�
typedef struct BossGrade
{
	int			bossType;
	BossRating	rateing;
}BossGrade;


//������Ϣ
#define Msg_Shake				"screen shake"				//��Ļ����
#define Msg_BounceWallTimeout	"bounceWall time out"		//����ʱ�䵽��
#define Msg_RecyclePadTimeout	"recyclePad time out"		//���յ���ʱ�䵽��
#define Msg_BossDead			"current boss is dead"		//BOSS����
#define Msg_StopMainPlBullet	"stop all bullet"			//ֹͣ�����ӵ�
#define Msg_OutScreen			"physivsunit is out screen"	//������Ļ
#define Msg_ProtectRemove		"Protect Remove"			//��������

#define Msg_PlayMainSceneAction	"play main scene action"	// �����涯��
#define Msg_BuySucceeEff		"buy succee effect"			// ����ɹ���Ч
#define Msg_ChangeGemGold		"change gem gold"			// ����������������ʯ��ң�
#define Msg_ChangeCard			"change Card option"		// ���Ľ�ɫѡ��
#define Msg_ChangeRole			"change role option"		// ���Ľ�ɫѡ��
#define Msg_ShowButtomBar		"main scene show buttom bar"	// ��ʾ�ײ���
#define Msg_HideButtomBar		"main scene Hide buttom bar"	// ���صײ���
#define Msg_ConsumeLife			"consume once life"			// ����һ������

#endif