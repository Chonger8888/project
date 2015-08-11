#include "WeaponFactory.h"
#include "weapon.h"
#include "../../GameCommon/GameConfig.h"
#include "../../GameCommon/GameDefine.h"
#include "../BasePlane/BasePlane.h"
#include "../Layer/UIController.h"
#include "../Layer/RollingBg.h"
#include "../../GameCommon/GameData.h"
#include "../../../cocos2d/cocos/editor-support/spine/extension.h"
#include "Config/WeaponConfig.h"



//---------------------------------------
WeaponFactory* WeaponFactory::create()
{
	WeaponFactory* wf = new (std::nothrow) WeaponFactory;
	if (wf && wf->initFactory())
	{
		wf->retain();
		wf->autorelease();
		return wf;
	}
	else
	{
		CC_SAFE_DELETE(wf);
		return nullptr;
	}
}

bool WeaponFactory::initFactory()
{
	do 
	{
	} while (0);

	return true;
}

void WeaponFactory::purge()
{
	this->release();
}

void WeaponFactory::AddEnemyWp( const WeaponPro& weapon, BasePlane* plane )
{
	if (!plane)
	{
		return;
	}
	int wpId = weapon.m_weaponId;
	ResTag res= weapon.m_res;
	float userdata = weapon.m_userdata;

	if (wpId < 1)
	{
		return;
	}

	int ammoHurts = plane->getContactPro().hurts;

	if (res >= ResTag::Bullet_EnemyNor1 && res <= ResTag::Bullet_EnemyNor4)
	{

	}
	else
	{
		//log("--- ammo res out of range---");
	}
	switch (wpId)
	{
	case 1:
		EnemyNorPlWeapon1(wpId, plane, res, ammoHurts);
		break;
	case 2:
		EnemyNorPlWeapon2(wpId, plane, res, ammoHurts);
		break;
	case 3:
		EnemyNorPlWeapon3(wpId, plane, res, ammoHurts);
		break;
	case 4:
		EnemyNorPlWeapon4(wpId, plane, res, ammoHurts);
		break;
	case 5:
		EnemyNorPlWeapon5(wpId, plane, res, ammoHurts);
		break;
	case 6:
		EnemyNorPlWeapon6(wpId, plane, res, ammoHurts);
		break;
	case 7:
		EnemyNorPlWeapon7(wpId, plane, res, ammoHurts);
		break;
	case 8:
		EnemyNorPlWeapon8(wpId, plane, res, ammoHurts);
		break;
	case 9:
		EnemyNorPlWeapon9(wpId, plane, res, ammoHurts);
		break;
	case 10:
		EnemyNorPlWeapon10(wpId, plane, res, ammoHurts);
		break;
	case 11:
		EnemyNorPlWeapon11(wpId, plane, res, ammoHurts);
		break;
	case 12:
		EnemyNorPlWeapon12(wpId, plane, res, ammoHurts);
		break;
	case 13:
		EnemyNorPlWeapon13(wpId, plane, res, ammoHurts);
		break;
	case 14:
		EnemyNorPlWeapon14(wpId, plane, res, ammoHurts);
		break;
	case 15:
		EnemyNorPlWeapon15(wpId, plane, res, ammoHurts);
		break;
	case 16:
		EnemyNorPlWeapon16(wpId, plane, res, ammoHurts);
		break;
	case 17:
		EnemyNorPlWeapon17(wpId, plane, res, ammoHurts);
		break;
	case 18:
		EnemyNorPlWeapon18(wpId, plane, res, ammoHurts);
		break;

	default:
		break;
	}
}

void WeaponFactory::AddMainPlaneWp(int planeType, int plLvl, BasePlane* plane,bool isTemp )
{
	int hurt = -NewDataMgr::getInstance()->getRoleAtkWithId(planeType);

	//log("----- planeType = %d,   planeLvl = %d,   hurts = %d -----", planeType, plLvl, hurt);
	plLvl = 1;
	switch (planeType)
	{
	case 1:
		{
			if (plLvl == 1)
			{
				MainPl1_Wp_1(plane, hurt, 0.08, isTemp);
			}								    
			else if(plLvl == 2)				    
			{								    
				MainPl1_Wp_2(plane, hurt, 0.15f, isTemp);
											    
			}								    
			else if(plLvl == 3)				    
			{								    
				MainPl1_Wp_3(plane, hurt, 0.15f, isTemp);
			}								    
			else if(plLvl == 4)				    
			{								    
				MainPl1_Wp_4(plane, hurt, 0.12f, isTemp);
			}								    
			else if(plLvl == 5)				    
			{								    
				//MainPl1_Wp_5(plane, hurt, 0.1f, isTemp);
			}
		}
		break;
	case 2:
		{
			if (plLvl == 1)
			{
				MainPl2_Wp_1(plane, hurt, 0.1f, isTemp);
			}
			else if(plLvl == 2)
			{
				MainPl2_Wp_2(plane, hurt, 0.15f,isTemp);
			}
			else if(plLvl == 3)
			{
				MainPl2_Wp_3(plane, hurt, 0.12f, isTemp);
			}
			else if(plLvl == 4)
			{
				MainPl2_Wp_4(plane, hurt, 0.11f, isTemp);
			}
			else if(plLvl == 5)
			{
				//MainPl2_Wp_5(plane, hurt, 0.2f, isTemp);
			}
		}
		break;
	case 3:
		{
			if (plLvl == 1)
			{
				MainPl3_Wp_1(plane, hurt, 0.1f, isTemp);
			}
			else if(plLvl == 2)
			{
				MainPl3_Wp_2(plane, hurt, 0.12f, isTemp);

			}
			else if(plLvl == 3)
			{
				MainPl3_Wp_3(plane, hurt, 0.13f, isTemp);
			}
			else if(plLvl == 4)
			{
				MainPl3_Wp_4(plane, hurt, 0.05f, isTemp);
			}
			else if(plLvl == 5)
			{
				MainPl3_Wp_5(plane, hurt, 0.1f, isTemp);
			}
		}
		break;
	default:
		break;
	}
}

Weapon* WeaponFactory::SpecialWP_Laser_S( BasePlane* appendNode,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/, float wpLife /*= -1.0f*/ )
{
	if (!appendNode)
	{
		return nullptr;
	}
	Weapon* wp = Weapon::create(appendNode);
	wp->setBullstType(BulletType::Self_LaserBullet);
	wp->setBltIsToPlane(true);
	wp->setShootTimes(1);
	wp->setBltHurts(hurts);
	appendNode->AddOneWp(wp, WeaponType::Addition);					//加上新的武器（不影响原有的武器）
	return wp;
}

Weapon* WeaponFactory::SpecialWP_Cat_S( BasePlane* appendNode,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/, float wpLife /*= -1.0f*/ )
{
	return nullptr;
}

Weapon* WeaponFactory::SpecialWP_IceMisile_S( BasePlane* appendNode,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/, float wpLife /*= -1.0f*/ )
{
	if (!appendNode)
	{
		return nullptr;
	}
	Weapon* wp = Weapon::create(appendNode);
	wp->setBullstType(BulletType::Self_MissleForDis);
	wp->setShootTimes(1);
	wp->setBltHurts(hurts);
	appendNode->AddOneWp(wp, WeaponType::Addition);				//加上新的武器（不影响原有的武器）
	return wp;
}

Weapon* WeaponFactory::SpecialWp_SmallDart_S( BasePlane* appendNode,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/, float wpLife /*= -1.0f*/ )
{
	if (!appendNode)
	{
		return nullptr;
	}
	Weapon* wp = Weapon::create(appendNode);
	wp->setBullstType(BulletType::Self_SmallDartBullet);
	wp->setShootTimes(1);
	wp->setBltHurts(hurts);
	appendNode->AddOneWp(wp, WeaponType::Addition);				//加上新的武器（不影响原有的武器）
	return wp;
}

void WeaponFactory::EnemyNorPlWeapon1(int wpId, BasePlane* plane, const ResTag& res, int hurts )
{
	//发射1颗飘弹
	Weapon* wp = Weapon::create(plane);
	wp->setBullstType(BulletType::Enemy_FallingExBullet);
	wp->setBltResTag(res);
	wp->setShootTimes(2);
	wp->setBltHurts(hurts);	
	wp->setRotation(180);
	wp->setPosition( Vec2(0,-50));	
	plane->AddOneWp(wp,WeaponType::Normal);

}

void WeaponFactory::EnemyNorPlWeapon2(int wpId, BasePlane* plane, const ResTag& res, int hurts )
{
	//同时发射2颗飘弹
	std::vector<Weapon*> vec;
	for (int i = 0; i < 2; i++)
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Enemy_FallingExBullet);
		wp->setShootTimes(3);
		wp->setBltResTag(res);
		wp->setBltHurts(hurts);	
		wp->setPosition(Vec2(0,-50));	
		wp->setRotation(5 - 10 * i + 180);
		vec.push_back(wp);
	}
	plane->AddWpVec(vec,WeaponType::Normal);

}

void WeaponFactory::EnemyNorPlWeapon3(int wpId, BasePlane* plane, const ResTag& res, int hurts )
{
	//同时发射6颗飘弹
	int randNum = random(0,1);
	int tag = -1;
	if (randNum == 1)
	{
		tag = 1;
	}
	std::vector<Weapon*> vec;
	for (int i = 1; i <= 6; i++)
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Enemy_FallingExBullet);
		wp->setShootTimes(1);
		wp->setDelayShoot(0.1*i);
		wp->setBltResTag(res);
		wp->setBltHurts(hurts);	
		wp->setPosition(Vec2(0,-50));
		wp->setRotation(tag * (i - 3.5) * 20 + 180);
		vec.push_back(wp);
	}	
	plane->AddWpVec(vec,WeaponType::Normal);
}

void WeaponFactory::EnemyNorPlWeapon4(int wpId, BasePlane* plane, const ResTag& res, int hurts )
{
	//朝主角方向发射1列子弹，每列1颗子弹
	Weapon* wp = Weapon::create(plane);
	wp->setBullstType(BulletType::Enemy_default);
	wp->setShootTimes(3);
	wp->setBltResTag(res);
	wp->setBltHurts(hurts);
	wp->setBltSpeed(500);
	plane->AddOneWp(wp,WeaponType::Normal);

	auto mainpl = GameData::getInstance()->getMainPlane();
	if (mainpl)
	{
		auto plPos = mainpl->convertToWorldSpaceAR(Vec2::ZERO);
		auto myPos = wp->convertToWorldSpaceAR(Vec2::ZERO);
		auto newVec = plPos - myPos;
		auto angle = newVec.getAngle() * RAD_DEG;
		wp->setRotation(-angle + 90);		//由于子弹原本是向上发射的，角度为-90，所以要加上90来抵消
	}
}

void WeaponFactory::EnemyNorPlWeapon5(int wpId, BasePlane* plane, const ResTag& res, int hurts )
{
	auto mainpl = GameData::getInstance()->getMainPlane();
	auto plPos = Vec2(360,0);
	if (mainpl)
	{
		plPos = mainpl->convertToWorldSpaceAR(Vec2::ZERO);
	}
	float angleOffset = 0.0f;

	std::vector<Weapon*> vec;

	//朝主角方向发射2列子弹，每列1颗子弹
	for (int i = 0; i < 3; ++i)
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Enemy_default);
		wp->setShootTimes(1);
		wp->setDelayShoot(0.1f * i);
		wp->setBltResTag(res);
		wp->setBltHurts(hurts);
		wp->setBltSpeed(700 + (i * 50));
		wp->setBltLinearDamping(0.5f);

		if (mainpl)
		{
			auto parentPos = plane->convertToWorldSpaceAR(Vec2::ZERO);
			auto myPos = wp->convertToWorldSpaceAR(Vec2::ZERO);
			myPos = myPos + parentPos;

			auto newVec = plPos - myPos;
			angleOffset = newVec.getAngle() * RAD_DEG;
		}
		wp->setRotation(3 *i - angleOffset+ 90);
		vec.push_back(wp);
	}
	for (int i = 0; i < 3; ++i)
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Enemy_default);
		wp->setShootTimes(1);
		wp->setDelayShoot(0.1f * i);
		wp->setBltResTag(res);
		wp->setBltHurts(hurts);
		wp->setBltSpeed(700 + (i * 50));
		wp->setBltLinearDamping(0.5f);

		if (mainpl)
		{
			auto parentPos = plane->convertToWorldSpaceAR(Vec2::ZERO);
			auto myPos = wp->convertToWorldSpaceAR(Vec2::ZERO);
			myPos = myPos + parentPos;
			auto newVec = plPos - myPos;
			angleOffset = newVec.getAngle() * RAD_DEG;
		}
		wp->setRotation(-3 *i - angleOffset+ 90);
		vec.push_back(wp);
	}
	plane->AddWpVec(vec,WeaponType::Normal);

}

void WeaponFactory::EnemyNorPlWeapon6(int wpId, BasePlane* plane, const ResTag& res, int hurts )
{
	auto mainpl = GameData::getInstance()->getMainPlane();
	auto plPos = Vec2(360,0);
	if (mainpl)
	{
		plPos = mainpl->convertToWorldSpaceAR(Vec2::ZERO);
	}
	float angleOffset = 0.0f;
	
	std::vector<Weapon*> vec;

	//朝主角方向发射4列子弹，每列1颗子弹
	for (int i = 0; i < 3; ++i)
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Enemy_default);
		wp->setShootTimes(4);
		wp->setBltResTag(res);
		wp->setBltHurts(hurts);	
		wp->setBltSpeed(300);
		wp->setPosition(Vec2(0,0));

		auto parentPos = plane->convertToWorldSpaceAR(Vec2::ZERO);
		auto myPos = wp->convertToWorldSpaceAR(Vec2::ZERO);
		myPos = myPos + parentPos;
		auto newVec = plPos - myPos;
		angleOffset = newVec.getAngle() * RAD_DEG;
		wp->setRotation( (i - 1) * 20 + 90 - angleOffset);

		{
			auto rot = RotateBy::create(1.0f,-30);
			auto rep = RepeatForever::create(rot);
			wp->runAction(rep);
		}
		vec.push_back(wp);
	}	
	plane->AddWpVec(vec,WeaponType::Normal);
}

void WeaponFactory::EnemyNorPlWeapon7(int wpId, BasePlane* plane, const ResTag& res, int hurts )
{
	auto mainpl = GameData::getInstance()->getMainPlane();
	auto plPos = Vec2(360,0);
	if (mainpl)
	{
		plPos = mainpl->convertToWorldSpaceAR(Vec2::ZERO);
	}
	float angleOffset = 0.0f;

	std::vector<Weapon*> vec;

	for (int i = 0; i < 2; ++i)
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Enemy_default);
		wp->setShootTimes(6);
		wp->setShootSpeed(0.1f);
		wp->setBltResTag(ResTag::Bullet_EnemyNor4);
		wp->setBltHurts(hurts);	
		wp->setBltSpeed(500);
		auto parentPos = plane->convertToWorldSpaceAR(Vec2::ZERO);
		auto myPos = wp->convertToWorldSpaceAR(Vec2::ZERO);
		myPos = myPos + parentPos;
		auto newVec = plPos - myPos;
		angleOffset = newVec.getAngle() * RAD_DEG;
		wp->setRotation(15 - 30 *i - angleOffset+ 90);
		
		{
			auto rot = RotateBy::create(1.0f,30);
			auto rep = RepeatForever::create(rot);
			wp->runAction(rep);
		}
		vec.push_back(wp);
	}
	plane->AddWpVec(vec, WeaponType::Normal);

}

void WeaponFactory::EnemyNorPlWeapon8(int wpId, BasePlane* plane, const ResTag& res, int hurts )
{
	//朝主角方向发射3列子弹，每列4颗子弹
	auto mainpl = GameData::getInstance()->getMainPlane();
	auto plPos = Vec2(360,0);
	if (mainpl)
	{
		plPos = mainpl->convertToWorldSpaceAR(Vec2::ZERO);
	}
	float angleOffset = 0.0f;

	std::vector<Weapon*> vec;
	for (int i = 0; i < 3; ++i)
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Enemy_default);
		wp->setShootSpeed(0.15f);
		wp->setShootTimes(4);
		wp->setBltResTag(res);
		wp->setBltHurts(hurts);	
		wp->setPosition(Vec2(0,-50));
		wp->setBltSpeed(500);

		auto parentPos = plane->convertToWorldSpaceAR(Vec2::ZERO);
		auto myPos = wp->convertToWorldSpaceAR(Vec2::ZERO);
		myPos = myPos + parentPos;

		auto newVec = plPos - myPos;
		angleOffset = newVec.getAngle() * RAD_DEG;
		wp->setRotation((i - 1) * 15 - angleOffset + 90);

		vec.push_back(wp);
	}
	plane->AddWpVec(vec, WeaponType::Normal);
}

void WeaponFactory::EnemyNorPlWeapon9(int wpId, BasePlane* plane, const ResTag& res, int hurts )
{
	//朝主角方向发射1组三角形子弹
	auto mainpl = GameData::getInstance()->getMainPlane();
	auto plPos = Vec2(360,0);
	if (mainpl)
	{
		plPos = mainpl->convertToWorldSpaceAR(Vec2::ZERO);
	}
	float angleOffset = 0.0f;
	std::vector<Weapon*> vec;

	int count = 0;
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Enemy_default);
		wp->setDelayShoot(count * 0.1);
		wp->setShootTimes(1);
		wp->setBltResTag(res);
		wp->setBltHurts(hurts);	
		wp->setBltSpeed(500);
		wp->setPosition(Vec2(0,-50));

		auto parentPos = plane->convertToWorldSpaceAR(Vec2::ZERO);
		auto myPos = wp->convertToWorldSpaceAR(Vec2::ZERO);
		myPos = myPos + parentPos;

		auto newVec = plPos - myPos;
		angleOffset = newVec.getAngle() * RAD_DEG;
		wp->setRotation(0 - angleOffset + 90);
		vec.push_back(wp);
	}
	count++;

	for (int i = 0; i < 2; i++)
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Enemy_default);
		wp->setBltResTag(res);
		wp->setBltHurts(hurts);	
		wp->setDelayShoot(count * 0.1);
		wp->setShootTimes(1);
		wp->setBltSpeed(500);
		wp->setPosition(Vec2(0,-50));
		wp->setRotation(-angleOffset + 3 * (i-0.5) + 90);
		vec.push_back(wp);
	}
	count++;

	for (int i = 0; i < 3; i++)
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Enemy_default);
		wp->setDelayShoot(count * 0.1);
		wp->setShootTimes(1);
		wp->setBltResTag(res);
		wp->setBltHurts(hurts);	
		wp->setBltSpeed(500);
		wp->setPosition(Vec2(0,-50));
		wp->setRotation(-angleOffset + 4 * (i-1) + 90);
		vec.push_back(wp);
	}
	plane->AddWpVec(vec, WeaponType::Normal);
}

void WeaponFactory::EnemyNorPlWeapon10(int wpId, BasePlane* plane, const ResTag& res, int hurts )
{
	//大绕圈弹，向下绕大圈。
	Weapon* wp = Weapon::create(plane);
	wp->setBullstType(BulletType::Enemy_HelixBullet);
	wp->setShootTimes(5);
	wp->setBltResTag(res);
	wp->setBltHurts(hurts);	
	wp->setPosition(Vec2(0,-50));
	plane->AddOneWp(wp, WeaponType::Normal);
}

void WeaponFactory::EnemyNorPlWeapon11(int wpId, BasePlane* plane, const ResTag& res, int hurts )
{
	//小绕圈弹，向下绕2圈，然后子弹朝主角位置射击。
	Weapon* wp = Weapon::create(plane);
	wp->setBullstType(BulletType::Enemy_HelixExBullet);
	wp->setShootTimes(3);
	wp->setBltResTag(res);
	wp->setBltHurts(hurts);	
	wp->setPosition(Vec2(0,-50));
	plane->AddOneWp(wp, WeaponType::Normal);
}

void WeaponFactory::EnemyNorPlWeapon12(int wpId, BasePlane* plane, const ResTag& res, int hurts )
{
	float randNum = random(0,1) - 0.5;
	randNum *= 2;

	std::vector<Weapon*> vec;
	for (int i = 0; i < 3; ++i)
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Enemy_Gravity);
		wp->setShootTimes(5);
		wp->setShootSpeed(0.05);
		wp->setBltResTag(res);
		wp->setBltSpeed(500);
		wp->setBltHurts(hurts);	
		wp->setBltLinearDamping(0.2);
		wp->setRotation(120 * i);
		
		{
			auto rot = RotateBy::create(1.0f,180 * randNum);
			auto rep = RepeatForever::create(rot);
			wp->runAction(rep);
		}
		vec.push_back(wp);
	}
	plane->AddWpVec(vec, WeaponType::Normal);
}

void WeaponFactory::EnemyNorPlWeapon13(int wpId, BasePlane* plane, const ResTag& res, int hurts )
{
	std::vector<Weapon*> vec;

	//左
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Enemy_Gravity);
		wp->setShootTimes(4);
		wp->setBltResTag(ResTag::Bullet_EnemyNor4);
		wp->setBltHurts(hurts);	
		wp->setShootSpeed(0.05);
		wp->setOffset(Vec2(-30,0));
		wp->setRotation(175);
		auto rot = RotateBy::create(1.0f,-30);
		auto rep = RepeatForever::create(rot);
		wp->runAction(rep);
		vec.push_back(wp);
	}
	//右
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Enemy_Gravity);
		wp->setShootSpeed(0.05);
		wp->setShootTimes(4);
		wp->setBltResTag(ResTag::Bullet_EnemyNor4);
		wp->setBltHurts(hurts);	
		wp->setOffset(Vec2(30,0));
		wp->setRotation(185);

		auto rot = RotateBy::create(1.0f,30);
		auto rep = RepeatForever::create(rot);
		wp->runAction(rep);

		vec.push_back(wp);
	}

	plane->AddWpVec(vec, WeaponType::Normal);
}

void WeaponFactory::EnemyNorPlWeapon14(int wpId, BasePlane* plane, const ResTag& res, int hurts )
{
	std::vector<Weapon*> vec;

	//弹射弹，向两边弹射2颗反弹子弹。
	for (int i = 1; i <= 2; ++i)
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Enemy_Rebounce);
		wp->setShootTimes(5);
		wp->setBltResTag(res);
		wp->setBltHurts(hurts);
		wp->setBltGravityAble(true);
		wp->setRotation(-90 + i * 180);
		vec.push_back(wp);
	}
	plane->AddWpVec(vec, WeaponType::Normal);
}

void WeaponFactory::EnemyNorPlWeapon15(int wpId, BasePlane* plane, const ResTag& res, int hurts )
{
	std::vector<Weapon*> vec;

	//弹射弹，向四周弹射6颗反弹子弹。
	for (int i = 0; i < 6; ++i)
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Enemy_Rebounce);
		wp->setShootTimes(5);
		wp->setBltResTag(res);
		wp->setBltGravityAble(true);
		wp->setBltHurts(hurts);	
		wp->setPosition(Vec2(0,-50));
		wp->setRotation(60 * i);
		vec.push_back(wp);
	}	
	plane->AddWpVec(vec, WeaponType::Normal);
}

void WeaponFactory::EnemyNorPlWeapon16(int wpId, BasePlane* plane, const ResTag& res, int hurts )
{
	auto mainpl = GameData::getInstance()->getMainPlane();
	auto plPos = Vec2(360,0);
	if (mainpl)
	{
		plPos = mainpl->convertToWorldSpaceAR(Vec2::ZERO);
	}

	float angleOffset = 0.0f;
	//弹射弹，向四周弹射6颗反弹子弹。
	Weapon* wp = Weapon::create(plane);
	wp->setBullstType(BulletType::Enemy_default);
	wp->setShootTimes(20);
	wp->setShootSpeed(0.05);
	wp->setBltResTag(ResTag::Bullet_EnemyNor3);
	//wp->setBltGravityAble(true);
	wp->setBltHurts(hurts);	

	auto parentPos = plane->convertToWorldSpaceAR(Vec2::ZERO);
	auto myPos = wp->convertToWorldSpaceAR(Vec2::ZERO);
	myPos = myPos + parentPos;

	auto newVec = plPos - myPos;
	angleOffset = -newVec.getAngle() * RAD_DEG + 90;
	wp->setRotation(angleOffset);

	{
		auto rot = RotateTo::create(0.5,angleOffset + 45);
		auto back = RotateTo::create(0.5,angleOffset - 45);
		auto seq = Sequence::create(rot, back, nullptr);
		auto rep = RepeatForever::create(seq);
		wp->runAction(rep);
	}
	plane->AddOneWp(wp, WeaponType::Normal);
}

void WeaponFactory::EnemyNorPlWeapon17(int wpId, BasePlane* plane, const ResTag& res, int hurts )
{
	std::vector<Weapon*> vec;

	//向周围发射圆形扩散弹12颗
	int num = 1;
	float angle = 360.0f/5;
	for (int i = 0; i < 5; ++i)
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Enemy_default);
		wp->setShootTimes(3 + num);
		wp->setBltResTag(ResTag::Bullet_EnemyNor3);
		wp->setBltHurts(hurts);	
		wp->setBltSpeed(300);
		wp->setRotation(angle * i);
		if (num == 1)
		{
			num = 0;
		}
		else if(num == 0)
		{
			num = 1;
		}
		{
			auto rot = RotateBy::create(1.0f,90);
			auto rep = RepeatForever::create(rot);
			wp->runAction(rep);
		}
		vec.push_back(wp);
	}	

	plane->AddWpVec(vec, WeaponType::Normal);

}

void WeaponFactory::EnemyNorPlWeapon18(int wpId, BasePlane* plane, const ResTag& res, int hurts )
{
	std::vector<Weapon*> vec;

	//向周围发射圆形扩散弹12颗,射两次
	int num = 1;
	float angle = 360.0f/8;
	for (int i = 0; i < 8; ++i)
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Enemy_default);
		wp->setShootTimes(3 + num);
		wp->setBltResTag(ResTag::Bullet_EnemyNor3);
		wp->setBltHurts(hurts);	
		wp->setBltSpeed(300);
		wp->setRotation(angle * i);
		if (num == 1)
		{
			num = 0;
		}
		else if(num == 0)
		{
			num = 1;
		}
		{
			auto rot = RotateBy::create(1.0f,90);
			auto rep = RepeatForever::create(rot);
			wp->runAction(rep);
		}
		vec.push_back(wp);
	}	
	/*for (int i = 0; i < 8; ++i)
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Enemy_default);
		wp->setShootTimes(3 + num);
		wp->setBltResTag(ResTag::Bullet_EnemyNor3);
		wp->setBltHurts(hurts);	
		wp->setDelayShoot(0.5f);
		wp->setBltSpeed(300);
		wp->setRotation(angle * i);
		if (num == 1)
		{
			num = 0;
		}
		else if(num == 0)
		{
			num = 1;
		}
		{
			auto rot = RotateBy::create(1.0f,-90);
			auto rep = RepeatForever::create(rot);
			wp->runAction(rep);
		}
		vec.push_back(wp);
	}	*/
	plane->AddWpVec(vec, WeaponType::Normal);

}

void WeaponFactory::MainPl1_Wp_1( BasePlane* plane,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/,bool isTemp )
{
	if (!plane)
	{
		return;
	}

	std::vector<Weapon*> wpVec;

	for (int i = 0; i < 2; i++)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 2, false);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setBltZorder(SelfBulletZorder);
		wp->setShootSpeed(Shootspeed);
		wp->setOffset(Vec2(-45 + 90 * i, 0));
		wpVec.push_back(wp);
		wp->ShootCrisscross(Shootspeed, 20 - 40 * i);
	}
	/*for (int i = 0; i < 3; ++i)
	{
		Weapon* wp = Weapon::create(plane);
		wp->setBullstType(BulletType::Convolution);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setBltZorder(SelfBulletZorder);
		wp->setBltCategorybitmask(SELF_AMMO_CATEGORYBITMASK);
		wp->setBltCollisionbitmask(SELF_AMMO_COLLISIONBITMASK);
		wp->setBltContacttestbitmask(SELF_AMMO_CONTACTTESTBITMASK);
		wp->setShootSpeed(0.05f);
		wp->setBltSpeed(700);
		wp->setBltResTag(ResTag::Bullet_Self_14);
		wp->setRotation(120 * i);
		wp->setShootTimes(5);
		wpVec.push_back(wp);

		auto rot = RotateBy::create(0.5f, 120);
		wp->runAction(rot);
	}*/
	
	if (isTemp)
	{
		plane->AddWpVec(wpVec, WeaponType::Replace);
	}
	else
	{
		plane->AddWpVec(wpVec, WeaponType::Normal);
	}
}

void WeaponFactory::MainPl1_Wp_2( BasePlane* plane,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/, bool isTemp )
{
	if (!plane)
	{
		return;
	}
	std::vector<Weapon*> wpVec;

	Weapon* wp = getWeaponDataWithConfig(plane, 2, false);
	wp->setBltSpeed(MainPlBltSpeed);
	wp->setBltZorder(SelfBulletZorder);
	wp->setShootSpeed(Shootspeed);
	wp->setOffset(Vec2(0, 40));
	wpVec.push_back(wp);

	for (int i = 0; i < 2; i++)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 1, false);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setBltZorder(SelfBulletZorder);
		wp->setShootSpeed(Shootspeed);
		wp->setOffset(Vec2(-30 + 60 * i, 20));
		wpVec.push_back(wp);
	}
	if (isTemp)
	{
		plane->AddWpVec(wpVec, WeaponType::Replace);
	}
	else
	{
		plane->AddWpVec(wpVec, WeaponType::Normal);
	}
}

void WeaponFactory::MainPl1_Wp_3( BasePlane* plane,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/, bool isTemp )
{
	if (!plane)
	{
		return;
	}
	std::vector<Weapon*> wpVec;

	Weapon* wp = getWeaponDataWithConfig(plane, 4, false);
	wp->setBltSpeed(MainPlBltSpeed);
	wp->setBltZorder(SelfBulletZorder);
	wp->setShootSpeed(Shootspeed);
	wp->setOffset(Vec2(0, 40));
	wpVec.push_back(wp);

	for (int i = 0; i < 2; i++)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 3, false);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setBltZorder(SelfBulletZorder);
		wp->setShootSpeed(Shootspeed);
		wp->setOffset(Vec2(-45 + 90 * i, 0));
		wpVec.push_back(wp);
	}
	if (isTemp)
	{
		plane->AddWpVec(wpVec, WeaponType::Replace);
	}
	else
	{
		plane->AddWpVec(wpVec, WeaponType::Normal);
	}
}

void WeaponFactory::MainPl1_Wp_4( BasePlane* plane,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/, bool isTemp )
{
	if (!plane)
	{
		return;
	}

	std::vector<Weapon*> wpVec;

	Weapon* wp = getWeaponDataWithConfig(plane, 6, false);
	wp->setBltSpeed(MainPlBltSpeed);
	wp->setBltZorder(SelfBulletZorder);
	wp->setShootSpeed(Shootspeed);
	wp->setOffset(Vec2(0, 40));
	wpVec.push_back(wp);

	for (int i = 0; i < 2; i++)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 5, false);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setBltZorder(SelfBulletZorder);
		wp->setShootSpeed(Shootspeed);
		wp->setOffset(Vec2(-45 + 90 * i, 20));
		wpVec.push_back(wp);
		wp->setRotation(-3 + 6 * i);
	}
	if (isTemp)
	{
		plane->AddWpVec(wpVec, WeaponType::Replace);
	}
	else
	{
		plane->AddWpVec(wpVec, WeaponType::Normal);
	}
}

void WeaponFactory::MainPl1_Wp_5( BasePlane* plane,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/, bool isTemp )
{
	if (!plane)
	{
		return;
	}
	std::vector<Weapon*> wpVec;

	int eachHurts = hurts * MainAttack;
	for (int i = 0; i < 5; i++)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 3, false);
		wp->setRotation((i - 2) * 1);
		wp->setOffset(Vec2((i - 2) * 15, 50 - fabsf(i-2) * 10));
		wp->setBltZorder(SelfBulletZorder + fabsf(i-2) * 1);
		wp->setShootSpeed(Shootspeed);
		wp->setBltSpeed(MainPlBltSpeed);
		wpVec.push_back(wp);
	}
	//左边3个散开
	for (int i = 0; i < 3; i++)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 6, false);
		wp->setBltSpeed(MainPlBltSpeed*0.8);
		wp->setShootSpeed(Shootspeed * 5);
		wp->setOffset(Vec2(-55 -i * 10, 60*i));		
		wp->setBltZorder(SelfBulletZorder);
		wp->setRotation( -9 + i * 3);
		wpVec.push_back(wp);
	}

	//右边3个散开
	for (int i = 0; i < 3; i++)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 6, false);
		wp->setBltSpeed(MainPlBltSpeed*0.8);
		wp->setShootSpeed(Shootspeed * 5);
		wp->setOffset(Vec2(55 + i * 10, 60*i));
		wp->setRotation( 9 - i * 3);		
		wp->setBltZorder(SelfBulletZorder);
		wpVec.push_back(wp);
	}
	if (isTemp)
	{
		plane->AddWpVec(wpVec, WeaponType::Replace);
	}
	else
	{
		plane->AddWpVec(wpVec, WeaponType::Normal);
	}
}

void WeaponFactory::MainPl2_Wp_1( BasePlane* plane,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/, bool isTemp )
{
	if (!plane)
	{
		return;
	}
	std::vector<Weapon*> wpVec;

	for (int i = 0; i < 3; i++)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 7, false);
		wp->setShootSpeed(Shootspeed);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setBltZorder(SelfBulletZorder - fabsf(i - 1));
		wp->setOffset(Vec2(0, 20));
		wp->setRotation((i - 1) * 5);
		wpVec.push_back(wp);
	}
	if (isTemp)
	{
		plane->AddWpVec(wpVec, WeaponType::Replace);
	}
	else
	{
		plane->AddWpVec(wpVec, WeaponType::Normal);
	}
}

void WeaponFactory::MainPl2_Wp_2( BasePlane* plane,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/, bool isTemp )
{
	if (!plane)
	{
		return;
	}
	std::vector<Weapon*> wpVec;

	Weapon* wp = getWeaponDataWithConfig(plane, 8, false);
	wp->setBltSpeed(MainPlBltSpeed);
	wp->setBltZorder(SelfBulletZorder);
	wp->setShootSpeed(Shootspeed);
	wp->setOffset(Vec2(0, 40));
	wpVec.push_back(wp);

	for (int i = 0; i < 2; i++)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 9, false);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setBltZorder(SelfBulletZorder);
		wp->setShootSpeed(Shootspeed);
		wp->setOffset(Vec2(-45 + 90 * i, 20));
		wpVec.push_back(wp);
	}
	if (isTemp)
	{
		plane->AddWpVec(wpVec, WeaponType::Replace);
	}
	else
	{
		plane->AddWpVec(wpVec, WeaponType::Normal);
	}
}

void WeaponFactory::MainPl2_Wp_3( BasePlane* plane,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/, bool isTemp )
{
	if (!plane)
	{
		return;
	}
	std::vector<Weapon*> wpVec;

	for (int i = 0; i < 3; i++)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 10, false);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setShootSpeed(Shootspeed);
		wp->setBltZorder(SelfBulletZorder);
		wp->setOffset(Vec2((i - 1) * 60, 60 - fabsf(i-1) * 60));
		wpVec.push_back(wp);
	}

	if (isTemp)
	{
		plane->AddWpVec(wpVec, WeaponType::Replace);
	}
	else
	{
		plane->AddWpVec(wpVec, WeaponType::Normal);
	}
}

void WeaponFactory::MainPl2_Wp_4( BasePlane* plane,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/, bool isTemp )
{
	if (!plane)
	{
		return;
	}
	std::vector<Weapon*> wpVec;

	for (int i = 0; i < 2; ++i)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 11, false);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setShootSpeed(Shootspeed);
		wp->setBltZorder(SelfBulletZorder);
		wp->setOffset(Vec2(-15 + 30 * i, 60));
		wpVec.push_back(wp);
	}
	for (int i = 0; i < 2; ++i)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 12, false);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setShootSpeed(Shootspeed);
		wp->setBltZorder(SelfBulletZorder);
		wp->setOffset(Vec2(-55 + 110 * i, 0));
		wpVec.push_back(wp);
	}
	if (isTemp)
	{
		plane->AddWpVec(wpVec, WeaponType::Replace);
	}
	else
	{
		plane->AddWpVec(wpVec, WeaponType::Normal);
	}
}

void WeaponFactory::MainPl2_Wp_5( BasePlane* plane,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/, bool isTemp )
{
	if (!plane)
	{
		return;
	}
	std::vector<Weapon*> wpVec;

	for (int i = 0; i < 3; ++i)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 11, false);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setShootSpeed(Shootspeed);
		wp->setBltZorder(SelfBulletZorder);
		wp->setOffset(Vec2((i - 1) * 20, 50 - fabsf(i-1) * 20));
		wpVec.push_back(wp);
	}
	for (int i = 0; i < 2; ++i)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 12, false);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setShootSpeed(Shootspeed);
		wp->setBltZorder(SelfBulletZorder);
		wp->setOffset(Vec2(-45 + 90 * i, 20));
		wpVec.push_back(wp);
	}
	for (int i = 0; i < 3; ++i)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 10, true);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setShootSpeed(Shootspeed * 3);
		wp->setBltZorder(SelfBulletZorder);
		wp->setOffset(Vec2(-100 + 20 * i, 50 * i));
		wp->setRotation(-20 + 5 * i);
		wpVec.push_back(wp);
	}
	for (int i = 0; i < 3; ++i)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 10, true);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setShootSpeed(Shootspeed * 3);
		wp->setBltZorder(SelfBulletZorder);
		wp->setOffset(Vec2(100 - 20 * i, 50 * i));
		wp->setRotation(20 - 5 * i);
		wpVec.push_back(wp);
	}
	if (isTemp)
	{
		plane->AddWpVec(wpVec, WeaponType::Replace);
	}
	else
	{
		plane->AddWpVec(wpVec, WeaponType::Normal);
	}
}

void WeaponFactory::MainPl3_Wp_1( BasePlane* plane,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/, bool isTemp )
{
	if (!plane)
	{
		return;
	}
	std::vector<Weapon*> wpVec;

	for (int i = 0; i < 2; ++i)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 14, false);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setShootSpeed(Shootspeed);
		wp->setBltZorder(SelfBulletZorder);
		wp->setOffset(Vec2(-15 + 30 * i, 80));
		wpVec.push_back(wp);
	}
	for (int i = 0; i < 2; ++i)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 13, false);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setShootSpeed(Shootspeed);
		wp->setBltZorder(SelfBulletZorder);
		wp->setOffset(Vec2(-45 + 90 * i, 0));
		wpVec.push_back(wp);
	}
	if (isTemp)
	{
		plane->AddWpVec(wpVec, WeaponType::Replace);
	}
	else
	{
		plane->AddWpVec(wpVec, WeaponType::Normal);
	}
}

void WeaponFactory::MainPl3_Wp_2( BasePlane* plane,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/, bool isTemp )
{
	if (!plane)
	{
		return;
	}
	std::vector<Weapon*> wpVec;

	Weapon* wp = getWeaponDataWithConfig(plane, 16, false);
	wp->setShootSpeed(Shootspeed);
	wp->setBltSpeed(MainPlBltSpeed);
	wp->setBltZorder(SelfBulletZorder - 1);
	wp->setOffset(Vec2(0, 60));
	wpVec.push_back(wp);

	for (int i = 0; i < 2; i++)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 15, false);
		wp->setShootSpeed(Shootspeed);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setBltZorder(SelfBulletZorder + 1);
		wp->setOffset(Vec2(-55 + 110 * i, 0));
		wpVec.push_back(wp);
	}

	if (isTemp)
	{
		plane->AddWpVec(wpVec, WeaponType::Replace);
	}
	else
	{
		plane->AddWpVec(wpVec, WeaponType::Normal);
	}
}

void WeaponFactory::MainPl3_Wp_3( BasePlane* plane,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/, bool isTemp )
{
	if (!plane)
	{
		return;
	}
	std::vector<Weapon*> wpVec;

	Weapon* wp = getWeaponDataWithConfig(plane, 17, false);
	wp->setBltSpeed(MainPlBltSpeed);
	wp->setBltZorder(SelfBulletZorder);
	wp->setShootSpeed(Shootspeed);
	wp->setOffset(Vec2(0, 65));
	wpVec.push_back(wp);

	for (int i = 0; i < 2; i++)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 15, false);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setBltZorder(SelfBulletZorder);
		wp->setShootSpeed(Shootspeed);
		wp->setOffset(Vec2(-40 + 80 * i, 0));
		wpVec.push_back(wp);
	}
	if (isTemp)
	{
		plane->AddWpVec(wpVec, WeaponType::Replace);
	}
	else
	{
		plane->AddWpVec(wpVec, WeaponType::Normal);
	}
}

void WeaponFactory::MainPl3_Wp_4( BasePlane* plane,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/, bool isTemp )
{
	if (!plane)
	{
		return;
	}
	std::vector<Weapon*> wpVec;

	Weapon* wp = getWeaponDataWithConfig(plane, 18, false);
	wp->setBltSpeed(MainPlBltSpeed);
	wp->setBltZorder(SelfBulletZorder);
	wp->setShootSpeed(0.15);
	wp->setOffset(Vec2(0, 40));
	wpVec.push_back(wp);

	for (int i = 0; i < 2; i++)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 19, false);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setBltZorder(SelfBulletZorder);
		wp->setShootSpeed(Shootspeed);
		wp->setOffset(Vec2(-45 + 90 * i, 20));
		wpVec.push_back(wp);
		wp->setRotation(-10 + 20 * i);

		auto rot = RotateBy::create(0.5, 15 - 30 * i);
		auto back = rot->reverse();
		auto seq = Sequence::create(rot, back, nullptr);
		auto rep = RepeatForever::create(seq);
		wp->runAction(rep);
	}
	if (isTemp)
	{
		plane->AddWpVec(wpVec, WeaponType::Replace);
	}
	else
	{
		plane->AddWpVec(wpVec, WeaponType::Normal);
	}
}

void WeaponFactory::MainPl3_Wp_5( BasePlane* plane,int hurts /*= -1*/, float Shootspeed /*= -1.0f*/, bool isTemp )
{
	if (!plane)
	{
		return;
	}
	std::vector<Weapon*> wpVec;

	Weapon* wp = getWeaponDataWithConfig(plane, 21, false);
	wp->setBltSpeed(MainPlBltSpeed);
	wp->setShootSpeed(Shootspeed);
	wp->setBltZorder(SelfBulletZorder);
	wp->setOffset(Vec2(0, 60));
	wpVec.push_back(wp);

	for (int i = 0; i < 5; ++i)
	{
		if (i == 2)
		{
			continue;
		}
		Weapon* wp = getWeaponDataWithConfig(plane, 20, false);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setShootSpeed(Shootspeed);
		wp->setBltZorder(SelfBulletZorder);
		wp->setOffset(Vec2((i - 2) * 30, 60 - fabsf(i-2) * 20));
		wpVec.push_back(wp);
	}
	for (int i = 0; i < 2; ++i)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 17, false);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setShootSpeed(Shootspeed * 0.25);
		wp->setBltZorder(SelfBulletZorder);
		wp->setOffset(Vec2(-65 + i * 130, 40));
		wp->setRotation(-5 + 10 * i);
		wpVec.push_back(wp);

		auto rot = RotateBy::create(0.3f, -20 + 40 * i);
		auto back = rot->reverse();
		auto seq = Sequence::create(rot, back, nullptr);
		auto rep = RepeatForever::create(seq);
		wp->runAction(rep);
	}
	for (int i = 0; i < 2; ++i)
	{
		Weapon* wp = getWeaponDataWithConfig(plane, 22, false);
		wp->setBltSpeed(MainPlBltSpeed);
		wp->setShootSpeed(Shootspeed * 0.25);
		wp->setBltZorder(SelfBulletZorder);
		wp->setOffset(Vec2(-50 + i * 100, 40));
		wp->setRotation(-25 + 50 * i);
		wpVec.push_back(wp);

		auto rot = RotateBy::create(0.2f, -15 + 30 * i);
		auto back = rot->reverse();
		auto seq = Sequence::create(rot, back, nullptr);
		auto rep = RepeatForever::create(seq);
		wp->runAction(rep);
	}
	if (isTemp)
	{
		plane->AddWpVec(wpVec, WeaponType::Replace);
	}
	else
	{
		plane->AddWpVec(wpVec, WeaponType::Normal);
	}
}

Weapon* WeaponFactory::getWeaponDataWithConfig(BasePlane* plane, int weaponId, bool isToTarget )
{
	int curRole = NewDataMgr::getInstance()->mRoleConfig.roleIndex;
	int baseHurt = -NewDataMgr::getInstance()->getRoleAtkWithId(curRole);

	auto data = NewDataMgr::getInstance()->getBulletConfigWithId(weaponId);
	auto core = data->attackCoe;	//攻击系数
	auto scaleX = data->scaleX;
	auto scaleY = data->scaleY;
	auto res = (int)ResTag::Bullet_BossSmallDart + data->resId + 1;
	auto bulletType = BulletType::Self_Default;
	if (isToTarget)
	{
		bulletType = BulletType::Self_ToTarget;
	}
	int hurt = baseHurt * core;

	Weapon* wp = Weapon::create(plane);
	wp->setBullstType(bulletType);
	wp->setBltResTag((ResTag)res);
	wp->setBltBelongTo(UnitId::eMainPlane);
	wp->setBltScaleX(scaleX);
	wp->setBltScaleY(scaleY);
	wp->setBltHurts(hurt);
	wp->setBltCategorybitmask(SELF_AMMO_CATEGORYBITMASK);
	wp->setBltCollisionbitmask(SELF_AMMO_COLLISIONBITMASK);
	wp->setBltContacttestbitmask(SELF_AMMO_CONTACTTESTBITMASK);

	return wp;
}

void WeaponFactory::AddWeaponWithConfig( BasePlane* plane, int ConfigId )
{
	int bitMask = plane->getPhysicsBody()->getCategoryBitmask();
	BulletType bt = BulletType::Self_Default;
	switch (bitMask)
	{
	case ENEMY_PLANE_CATEGORYBITMASK:
		bt = BulletType::Enemy_default;
		break;
	case SELF_PLANE_CATEGORYBITMASK:
		bt = BulletType::Self_Default;
		break;
	default:
		break;
	}
	int hurt = -100;
	auto config = WeaponConfig::getWeaponConfigWithId(ConfigId);
	for (int i = 0; i < config.size(); i++)
	{
		auto weaponPro = config.at(i);
		int WpNum = weaponPro.getConfigWpNum();
		if (WpNum > 1)
		{
			float DisInterval = 30;
			float minDis = weaponPro.getConfigMinDis();
			if (minDis < DisInterval)
			{
				minDis = DisInterval;
			}
			float ExDis = weaponPro.getConfigExDis();
			float zhenfu = weaponPro.getConfigZhenFu();
			int eachDis = minDis + ExDis;
			int SideNum = WpNum/2;
			float t = ExDis*0.5/zhenfu * weaponPro.getConfigShootInterval();

			//确定每把武器的位置
			//左边
			for (int i = 0; i < SideNum; ++i)
			{
				Weapon* wp = Weapon::createByConfig(plane,weaponPro);
				auto pos = Vec2(-(DisInterval * (i + 0.5) + ExDis * (i+0.5)), 0);
				wp->setBullstType(bt);
				wp->setBltHurts(hurt);
				plane->AddOneWp(wp, WeaponType::Normal);
				wp->setPosition(pos);

				auto endPos = Vec2(-(minDis * (i + 0.5)), 0);
				auto mvDis = endPos - pos;

				if (zhenfu > 0)
				{
					auto mv = MoveBy::create(t, mvDis);
					auto back = mv->reverse();
					auto seq = Sequence::create(mv, back, nullptr);
					auto rep = RepeatForever::create(seq);
					wp->runAction(rep);
				}
			}

			//右边
			for (int i = 0; i < SideNum; ++i)
			{
				Weapon* wp = Weapon::createByConfig(plane,weaponPro);
				auto pos = Vec2(DisInterval * (i + 0.5) + ExDis * (i+0.5), 0);
				wp->setBullstType(bt);
				wp->setBltHurts(hurt);
				plane->AddOneWp(wp, WeaponType::Normal);
				wp->setPosition(pos);

				auto endPos = Vec2(-(minDis * (i + 0.5)), 0);
				auto mvDis = endPos - pos;

				if (zhenfu > 0)
				{
					auto mv = MoveBy::create(t, mvDis);
					auto back = mv->reverse();
					auto seq = Sequence::create(mv, back, nullptr);
					auto rep = RepeatForever::create(seq);
					wp->runAction(rep);
				}
			}
		}
		else
		{
			Weapon* wp = Weapon::createByConfig(plane,weaponPro);
			wp->setBullstType(bt);
			wp->setBltHurts(hurt);
			plane->AddOneWp(wp, WeaponType::Normal);
		}
	}
}

