#ifndef WeaponDelegate_h__
#define WeaponDelegate_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameCommon\GameDefine.h"
#include "GameCommon\GameConfig.h"

USING_NS_CC;
USING_NS_CC_EXT;

class WeaponDelegate
{
public:
	WeaponDelegate()
	{
		m_BltResTag = ResTag::defaultType,				
		m_BltScaleX = 1.0f,				
		m_BltScaleY = 1.0f,				
		m_ConfigWeaponAngle = 0.0f,			
		m_BltSpeed = 500.0f,				
		m_ConfigShootInterval = 0.15f,			
		m_ConfigWeaponDuration = -1.0f,			
		m_ConfigWeaponOffsetX = 0.0f,			
		m_ConfigWeaponOffsetY = 0.0f,			
		m_ConfigWeaponRotationEachShoot = 0.0f,
		m_ConfigWeaponMaxRotattion = 0.0f,		


		m_BltMass = 0.00001;
		m_BltLinearDamping = 0.0f;
		m_BltZorder = EnemyBulletZorder;
		m_BltHurts = 0;
		m_BltGravityAble = false;
		m_BltIsToPlane = false;
		m_BltBelongTo = UnitId::DefaultIdx;
		m_BltOtherData = nullptr;
		m_BltCategorybitmask = ENEMY_AMMO_CATEGORYBITMASK;
		m_BltCollisionbitmask =	ENEMY_AMMO_COLLISIONBITMASK;
		m_BltContacttestbitmask = ENEMY_AMMO_CONTACTTESTBITMASK;
		m_OriginVelocity = Vec2(0,1);

		m_ConfigWpNum = 1;
		m_ConfigMinDis = 30;	
		m_ConfigExDis = 0;
		m_ConfigZhenFu = 0;
		m_ConfigIsAmmi = false;
	}
	CC_SYNTHESIZE(ResTag,	m_BltResTag,					BltResTag);			 //�ӵ���ԴID
	CC_SYNTHESIZE(float,	m_BltScaleX,					BltScaleX);			 //�ӵ�X�����ţ�Ĭ��1.0��
	CC_SYNTHESIZE(float,	m_BltScaleY,					BltScaleY);			 //�ӵ�Y�����ţ�Ĭ��1.0��
	CC_SYNTHESIZE(float,	m_ConfigWeaponAngle,			ConfigWeaponAngle);			 //��������ʱ�ĽǶ�
	CC_SYNTHESIZE(float,	m_ConfigShootInterval,			ConfigShootInterval);		 //�����ӵ���ʱ����
	CC_SYNTHESIZE(float,	m_ConfigWeaponDuration,			ConfigWeaponDuration);		 //���ﵽ�������ʱ��ʱ������ֹͣ�����ӵ�
	CC_SYNTHESIZE(float,	m_ConfigWeaponOffsetX,			ConfigWeaponOffsetX);		 //������ɻ���ƫ������X�ᣩ
	CC_SYNTHESIZE(float,	m_ConfigWeaponOffsetY,			ConfigWeaponOffsetY);		 //������ɻ���ƫ������Y�ᣩ
	CC_SYNTHESIZE(float,	m_ConfigWeaponRotationEachShoot,	ConfigWeaponRotationEachShoot);	//һ�ο����ı�ĽǶ�
	CC_SYNTHESIZE(float,	m_ConfigWeaponMaxRotattion,		ConfigWeaponMaxRotattion);	//�ı�ĽǶ����ֵ

	CC_SYNTHESIZE(float,	m_BltSpeed,			BltSpeed);				//�ӵ��ķ����ٶ�
	CC_SYNTHESIZE(float,	m_BltMass,			BltMass);				//�ӵ�����
	CC_SYNTHESIZE(float,	m_BltLinearDamping, BltLinearDamping);		//�ӵ�����
	CC_SYNTHESIZE(int,		m_BltZorder,		BltZorder);				//�ӵ�Zorder
	CC_SYNTHESIZE(int,		m_BltHurts,			BltHurts);				//�ӵ��˺�
	CC_SYNTHESIZE(bool,		m_BltGravityAble,	BltGravityAble);		//�ӵ��Ƿ�������
	CC_SYNTHESIZE(bool,		m_BltIsToPlane,		BltIsToPlane);			//�Ƿ�addchild�ڷɻ���
	CC_SYNTHESIZE(UnitId,	m_BltBelongTo,		BltBelongTo);			//�ӵ�����
	CC_SYNTHESIZE(void*,	m_BltOtherData,		BltOtherData);			//�����ӵ�����
	CC_SYNTHESIZE_PASS_BY_REF(Vec2, m_OriginVelocity,BltOriginVelocity);//��ʼ�ٶ�����

	CC_SYNTHESIZE(int,		m_BltCategorybitmask,	BltCategorybitmask);
	CC_SYNTHESIZE(int,		m_BltCollisionbitmask,	BltCollisionbitmask);
	CC_SYNTHESIZE(int,		m_BltContacttestbitmask,	BltContacttestbitmask);

	CC_SYNTHESIZE(int,		m_ConfigWpNum,		ConfigWpNum,);
	CC_SYNTHESIZE(float,	m_ConfigMinDis,		ConfigMinDis);
	CC_SYNTHESIZE(float,	m_ConfigExDis,		ConfigExDis);
	CC_SYNTHESIZE(float,	m_ConfigZhenFu,		ConfigZhenFu);

	CC_SYNTHESIZE(float,	m_ConfigIsAmmi,		ConfigIsAmmi);
};

#endif // WeaponDelegate_h__