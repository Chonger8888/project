/********************************************************************
������:		2014/11/13  11:29	
�ļ���: 	E:\Cocos3.3_Project\MyGame\Classes\weapon.h 
��  ��:		alex(alex\)
�޸��ڣ�	2014/11/13  11:29	

Ŀ  ��:		�����ӵ�������
*********************************************************************/
#ifndef _WEAPON_H_
#define _WEAPON_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../PhysicsUnit.h"
#include "WeaponDelegate.h"

USING_NS_CC;
USING_NS_CC_EXT;

class Bullet;
class BasePlane;

enum class WeaponType
{
	Normal,			//����
	Replace,		//�滻	
	Addition,		//�ӳ�
};

enum class CreateType
{
	Native,			//���ش���
	Config,			//���ô���
};

class Weapon : public Node, public WeaponDelegate
{
public:
	Weapon();
	static Weapon* create(Node* pl);
	static Weapon* createByConfig(Node* pl, WeaponDelegate config);
	virtual void onEnter();
	void setOffset(const Vec2& offset);			//����������Էɻ���λ��ƫ��
	void setDelayShoot(float dt = 0.0f);		//�����ӳٷ���ʱ��
	void setShootTimes(int shootTimes);			//���÷������,Ĭ�����޷���
	void setShootSpeed(float newSpeed);			//���÷�������
	void setBullstType(const BulletType& tp);	//���÷�����ӵ�����
	void StartShoot();							

	void ShootCrisscross(float interval, float offsetX);		//������
	void ShootOneBullet();
private:
	void initWeaponWithConfig(WeaponDelegate config);
	void shoot(float dt);						//�ӵ����䶨ʱ��
	void PauseShoot(float pauseTime);			//��ͣ����һ��ʱ��
	void ResumeShoot();							//�ָ�����
	void removeWeapon();						//ɾ������

protected:
	float			m_shootSpeed;
	float			m_delayShoot;		//�ӳ����
	CC_SYNTHESIZE(bool,m_isShootAble,ShootAble);		//�Ƿ���Է����ӵ�
	int				m_ShootTimes;		//Ԥ���������
	int				m_curShootTimes;	//��ǰ�������
	bool			m_isSwing;			//�Ƿ�ִ�к���ڶ�
	CreateType		m_CreateType;
	BulletType		m_bulletType;
	Size			m_winSize;
	CC_SYNTHESIZE(Node*,m_WeaponParent,WeaponParent);
	CC_SYNTHESIZE(WeaponType,m_WeaponType,WeaponType);
};

//�Զ�Ѱ��Ŀ���ӵ�
class AutoWeapon : public Weapon
{
public:
	AutoWeapon();
	static AutoWeapon* create(bool isEnemy = false);
	virtual void onEnter();

	//����Ѱ�����Ŀ��,��ת�Լ��ĽǶ�
	void SeachTarget(float dt);
	void setLeft(bool b){m_isLeft = b;}
private:
	bool		m_isLeft;
	Size		m_winSize;
};





#endif
