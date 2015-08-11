/********************************************************************
������:		2015/03/11  14:33	
�ļ���: 	E:\Cocos3.3_Project\PlaneGame2\Classes\GamePlaying\Bullet\BulletFactory.h 
��  ��:		alex()
�޸��ڣ�	2015/03/11  14:33	

Ŀ  ��:		
*********************************************************************/
#ifndef __BulletFactory_H__
#define __BulletFactory_H__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "../../GameCommon/GameDefine.h"

USING_NS_CC;
USING_NS_CC_EXT;


//�ӵ���������
class Bullet;
class Weapon;
class BulletFactory : public Ref
{
public:
	static BulletFactory* create();
	void purge();

	//����IDȡ���ӵ�
	Bullet* createBullet(Weapon* weapon,const BulletType& tp, const ResTag& res = ResTag::defaultType);

	//��ͨ�л����ӵ�
	Bullet* CreateBulletOfNorPlane();

	//����ͨ���ӵ�
	Bullet* SelfBullet_Default(Weapon* weapon, ResTag res);

	//�з�ͨ���ӵ�
	Bullet* EnemyBullet_Default(Weapon* weapon, ResTag res);

	//�з��ӵ�
	Bullet* EnemyBullet_1(Weapon* weapon, ResTag res);
	Bullet* EnemyBullet_2(Weapon* weapon, ResTag res);

	//���������ӵ�
	Bullet* EnemyBulletGravity(Weapon* weapon, ResTag res);

	//Boss���������ӵ�
	Bullet* BossBulletGravity(Weapon* weapon);
	// Bossr�����ӵ�
	Bullet* BossBulletRebound(Weapon* weapon);
	// ��
	Bullet* EnemyGrenade( Weapon* weapon, ResTag res );

	//Ʈ���ӵ�
	Bullet* EnemyBulletFalling(Weapon* weapon, ResTag res);

	//�����������ӵ�
	Bullet* EnemyBulletBezier(Weapon* weapon, ResTag res);
	//�����������ӵ������÷���Left
	Bullet* EnemyBulletBezierLeft(Weapon* weapon, ResTag res);
	//�����������ӵ������÷���Right
	Bullet* EnemyBulletBezierRight(Weapon* weapon, ResTag res);
	//��������(�����뵼��)
	Bullet* SelfMissileForDis(Weapon* weapon);

	Bullet* SelfCrossBullet(Weapon* weapon, ResTag res);

	//����������Ŀ��ΪѪ�������
	Bullet* createSelfMissileForMaxHpTarget(const float& AngleOffet );

	//��������
	Bullet* createStrayBullet();

	//�����ӵ�
	Bullet* createHelixBullet(Weapon* weapon, ResTag res);

	//�����ӵ�EX
	Bullet* createHelixExBullet(Weapon* weapon, ResTag res);

	//Ʈ�����ӵ���ǿ��
	Bullet* EnemyBulletFallingEx(Weapon* weapon, ResTag res);

	//�̻�
	Bullet* EnemyFireworks( Weapon* weapon );

	//����
	Bullet* EnemyRebounce(Weapon* weapon, ResTag res);

	//����
	Bullet* CreateDartBullet();		//(��)
	Bullet* CreateSmallDartBullet();//(С)

	//�����ӵ�
	Bullet* CreateLaserBullet();

	//����BOSS�ķ���
	Bullet* BossBulletDart(Weapon* weapon);

	//���ȥ˲�䣬����������Ŀ��
	Bullet* Bullet2Target(Weapon* weapon, ResTag res);

	//Boss4�ı���������
	Bullet* BezierBulletOfBoss4(Weapon* weapon);

	static std::string getResByTag(ResTag tag);

private:
	bool init();
};



#endif // __BulletFactory_H__