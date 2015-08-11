#include "PlaneFactory.h"
#include "BasePlane.h"
#include "../PhysicsUnit.h"
#include "../Weapon/WeaponFactory.h"
#include "../Weapon/weapon.h"
#include "../Layer/UIController.h"
#include "../../GameCommon/GameData.h"
#include "../../GameCommon/GameConfig.h"
#include "../../GameCommon/EffectsManager.h"
#include "../../GameCommon/GameDefine.h"
#include "../Layer/RollingBg.h"
#include "../Layer/PhysicsLayer.h"

//------------------------
PlaneFactory* PlaneFactory::create()
{
	PlaneFactory* pf = new (std::nothrow) PlaneFactory;
	if (pf && pf->init())
	{
		pf->retain();
		pf->autorelease();
		return pf;
	}
	else
	{
		CC_SAFE_DELETE(pf);
		return nullptr;
	}
}

void PlaneFactory::purge()
{
	this->release();
}

bool PlaneFactory::init()
{
	do 
	{

	} while (0);

	return true;
}

void PlaneFactory::CreateMainPlane(const PlaneType& tp)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	MainPlane* plane = MainPlane::create(tp);
	plane->setUnitId(UnitId::eMainPlane);
	plane->setTouchAble(true);
	plane->setInvincibleAtLine();
	plane->setBitmask(SELF_PLANE_CATEGORYBITMASK,SELF_PLANE_COLLISIONBITMASK,SELF_PLANE_CONTACTTESTBITMASK);
	plane->setPosition(Vec2(size.width*0.5, -80));
	plane->getHpBar()->SetHpBarVisible(false);

	switch (tp)
	{
	case PlaneType::PlaneType_Wind:
		plane->setShootMusic(Music_shootAmmo1);
		break;
	case PlaneType::PlaneType_Fire:
		plane->setShootMusic(Music_shootAmmo2);
		break;
	case PlaneType::PlaneType_Electric:
		plane->setShootMusic(Music_shootAmmo3);
		break;
	default:
		break;
	}

	ActionInterval* moveTo = MoveTo::create(1.0f,Vec2(size.width * 0.5,size.height * 0.3));
	plane->runAction(moveTo);

	//���ɻ�Ѫ��
	int hp = NewDataMgr::getInstance()->getRoleHpWithId((int)tp);
	plane->setMaxHP(hp);

	//���ɻ����˺�
	int allHurts = -NewDataMgr::getInstance()->getRoleAtkWithId((int)tp);
	plane->setBltHurts(allHurts);

	//�������ɻ����
	int defense = NewDataMgr::getInstance()->getRoleDefenseWithId((int)tp);
	plane->setDefense(defense);

	//���ɻ������
	{
		int planeLvl = NewDataMgr::getInstance()->getRoleGradeWithId( (int)tp );
		planeLvl = (planeLvl-1)/12 + 1;
		auto weaponFact = GameData::getInstance()->getWeaponFactory();
		weaponFact->AddMainPlaneWp((int)tp, planeLvl, plane);
	}
	//plane->setShootAble(false);

	GameData::getInstance()->setMainPlane(plane);
	UIController::getInstance()->getPhysicsLayer()->addChild(plane,1000);
}

BasePlane* PlaneFactory::createEnemyPlane( int id,float userdata, int gamelvl )
{
    int curMapId = UIController::getInstance()->getGameBgMgr()->getCurBgId();

    //ÑªÁ¿¹«Ê½
    float val1 = NewDataMgr::getInstance()->getEnemyPlaneWithKey()->HpCoe1;
    float val2 = NewDataMgr::getInstance()->getEnemyPlaneWithKey()->HpCoe2;
    int PlHp = pow((userdata * curMapId * gamelvl), val1) + val2 * userdata;

    //·ÖÊý¹«Ê½
    val1 = NewDataMgr::getInstance()->getEnemyPlaneWithKey()->ScoreCoe1;
    val2 = NewDataMgr::getInstance()->getEnemyPlaneWithKey()->ScoreCoe2;
    int score = pow((userdata * curMapId * gamelvl), val1) + val2 * userdata;

    //ÉËº¦
    int hurt = GameData::getInstance()->getBulletHurtsOfEnemyPl(userdata);

    BasePlane* pl = nullptr;

    //4,5,10,15,20 Îª¹Ç÷À¶¯»­
    if(id < 1000)
    {
        if(	id != 4 &&
                id != 5 &&
                id != 10 &&
                id != 15 &&
                id != 20 )
        {
            //Ö»ÓÐÍ¼Æ¬Ð¡¹Ö
            std::string file = String::createWithFormat("plane_%d.png", id)->getCString();
            pl = EnemyPlane::createWithPicture(file);
            if (id == 16)	//ÐèÐèÒªË®Æ½·­×ª
            {
                EnemyPlane* enePl = (EnemyPlane*)pl;
                enePl->setFlipXonEnter();
            }
        }
        else
        {
            //¹Ç÷À¶¯»­Ð¡¹Ö
            std::string name = String::createWithFormat("plane_%d", id)->getCString();
            pl = EnemyPlane::createWithArmature(name);
        }
        pl->setBltHurts(hurt);
        pl->setMaxHP(PlHp);
        pl->setUnitId(UnitId(id + (int)UnitId::ePlane_1));
        pl->setBitmask(
                ENEMY_PLANE_CATEGORYBITMASK,
                ENEMY_PLANE_COLLISIONBITMASK,
                ENEMY_PLANE_CONTACTTESTBITMASK);
        auto selfpro = pl->getSelfPro();
        selfpro.score = score;
        pl->setSelfPro(selfpro);
        pl->setNeedFlyToMainPl(true);
    }
    else
    {
        //ÑªÁ¿¹«Ê½
        val1 = NewDataMgr::getInstance()->getEnemyPlaneWithKey(id)->HpCoe1;
        val2 = NewDataMgr::getInstance()->getEnemyPlaneWithKey(id)->HpCoe2;
        PlHp = pow((userdata * curMapId * gamelvl), val1) + val2 * userdata;

        //·ÖÊý¹«Ê½
        val1 = NewDataMgr::getInstance()->getEnemyPlaneWithKey(id)->ScoreCoe1;
        val2 = NewDataMgr::getInstance()->getEnemyPlaneWithKey(id)->ScoreCoe2;
        score = pow((userdata * curMapId * gamelvl), val1) + val2 * userdata;

        //ÉËº¦
        //¹«Ê½
        val1 = NewDataMgr::getInstance()->getEnemyPlaneWithKey(id)->AtkCoe1;
        val2 = NewDataMgr::getInstance()->getEnemyPlaneWithKey(id)->AtkCoe2;
        hurt = -(pow((userdata * gamelvl * curMapId), val1) + val2 * userdata);
        switch (id)
        {
            case 1001:
            {
                //ÈÌÕßBOSS
                pl = IronManBoss::create("kaijia",1);
                pl->setHpEff(HpEff(1.0f,100.0f));		//ËÀÍö¼ÓÑª

                pl->addRewards(0.0f, 10, DropGoodsType::DropGoods_Gold1);
                pl->addRewards(0.0f, 3, DropGoodsType::DropGoods_Gold2);
                pl->addRewards(0.0f, 1, DropGoodsType::DropGoods_Gold3);
            }
                break;
            case 1002:
            {
                //ÈÌÕßBOSS
                pl = IronManBoss::create("kaijia",2);
                pl->setHpEff(HpEff(1.0f,100.0f));		//ËÀÍö¼ÓÑª

                pl->addRewards(0.0f, 13, DropGoodsType::DropGoods_Gold1);
                pl->addRewards(0.0f, 5, DropGoodsType::DropGoods_Gold2);
                pl->addRewards(0.0f, 2, DropGoodsType::DropGoods_Gold3);
            }
                break;
            case 1003:
            {
                //ÈÌÕßBOSS
                pl = IronManBoss::create("kaijia",3);
                pl->setHpEff(HpEff(1.0f,100.0f));		//ËÀÍö¼ÓÑª

                pl->addRewards(0.0f, 16, DropGoodsType::DropGoods_Gold1);
                pl->addRewards(0.0f, 7, DropGoodsType::DropGoods_Gold2);
                pl->addRewards(0.0f, 3, DropGoodsType::DropGoods_Gold3);
            }
                break;
            case 1004:
            {
                pl = BoneWarriorBoss::create("boss-siling",1);
                pl->setHpEff(HpEff(1.0f,100.0f));		//ËÀÍö¼ÓÑª
                pl->addRewards(0.0f, 19, DropGoodsType::DropGoods_Gold1);
                pl->addRewards(0.0f, 9, DropGoodsType::DropGoods_Gold2);
                pl->addRewards(0.0f, 4, DropGoodsType::DropGoods_Gold3);
            }
                break;
            case 1005:
            {
                pl = BoneWarriorBoss::create("boss-siling",2);
                pl->setHpEff(HpEff(1.0f,100.0f));		//ËÀÍö¼ÓÑª

                pl->addRewards(0.0f, 22, DropGoodsType::DropGoods_Gold1);
                pl->addRewards(0.0f, 11, DropGoodsType::DropGoods_Gold2);
                pl->addRewards(0.0f, 5, DropGoodsType::DropGoods_Gold3);
            }
                break;
            case 1006:
            {
                pl = BoneWarriorBoss::create("boss-siling",3);
                pl->setHpEff(HpEff(1.0f,100.0f));		//ËÀÍö¼ÓÑª

                pl->addRewards(0.0f, 25, DropGoodsType::DropGoods_Gold1);
                pl->addRewards(0.0f, 13, DropGoodsType::DropGoods_Gold2);
                pl->addRewards(0.0f, 6, DropGoodsType::DropGoods_Gold3);
            }
                break;
            case 1007:
            {
                pl = RenzheBoss::create("renzheboss-2",1);
                pl->setHpEff(HpEff(1.0f,100.0f));		//ËÀÍö¼ÓÑª

                pl->addRewards(0.0f, 28, DropGoodsType::DropGoods_Gold1);
                pl->addRewards(0.0f, 15, DropGoodsType::DropGoods_Gold2);
                pl->addRewards(0.0f, 7, DropGoodsType::DropGoods_Gold3);
            }
                break;
            case 1008:
            {
                pl = RenzheBoss::create("renzheboss-2",2);
                pl->setHpEff(HpEff(1.0f,100.0f));		//ËÀÍö¼ÓÑª

                pl->addRewards(0.0f, 31, DropGoodsType::DropGoods_Gold1);
                pl->addRewards(0.0f, 17, DropGoodsType::DropGoods_Gold2);
                pl->addRewards(0.0f, 8, DropGoodsType::DropGoods_Gold3);
            }
                break;
            case 1009:
            {
                pl = RenzheBoss::create("renzheboss-2",3);
                pl->setHpEff(HpEff(1.0f,100.0f));		//ËÀÍö¼ÓÑª

                pl->addRewards(0.0f, 34, DropGoodsType::DropGoods_Gold1);
                pl->addRewards(0.0f, 19, DropGoodsType::DropGoods_Gold2);
                pl->addRewards(0.0f, 9, DropGoodsType::DropGoods_Gold3);
            }
                break;
            case 1010:
            {
                pl = PoseidonBoss::create("boss-meinv",1);
                pl->setHpEff(HpEff(1.0f,100.0f));		//ËÀÍö¼ÓÑª
                BaseBoss* boss = (BaseBoss*)pl;
                boss->SwitchBgWhenDead(2);

                pl->addRewards(0.0f, 37, DropGoodsType::DropGoods_Gold1);
                pl->addRewards(0.0f, 21, DropGoodsType::DropGoods_Gold2);
                pl->addRewards(0.0f, 10, DropGoodsType::DropGoods_Gold3);
            }
                break;
            case 1011:
            {
                pl = PoseidonBoss::create("boss-meinv",2);
                pl->setHpEff(HpEff(1.0f,100.0f));		//ËÀÍö¼ÓÑª
                BaseBoss* boss = (BaseBoss*)pl;
                boss->SwitchBgWhenDead(3);

                pl->addRewards(0.0f, 40, DropGoodsType::DropGoods_Gold1);
                pl->addRewards(0.0f, 23, DropGoodsType::DropGoods_Gold2);
                pl->addRewards(0.0f, 11, DropGoodsType::DropGoods_Gold3);
            }
                break;
            case 1012:
            {
                pl = PoseidonBoss::create("boss-meinv",3);
                pl->setHpEff(HpEff(1.0f,100.0f));		//ËÀÍö¼ÓÑª

                pl->addRewards(0.0f, 43, DropGoodsType::DropGoods_Gold1);
                pl->addRewards(0.0f, 25, DropGoodsType::DropGoods_Gold2);
                pl->addRewards(0.0f, 12, DropGoodsType::DropGoods_Gold3);
            }
                break;
            default:
                break;
        }
        pl->setDeadMusic(Music_mainPlDead);
        pl->setMaxHP(PlHp);
        pl->setUnitId(UnitId::ePlaneBoss);
        pl->setBltHurts(hurt);
        auto selfpro = pl->getSelfPro();
        selfpro.score = score;
        pl->setSelfPro(selfpro);
    }

    GameData::getInstance()->AddEnemyPlane(pl);
    return pl;
}


BasePlane* PlaneFactory::createEnemyPlane(LevelPlaneConfigData* configData)
{
    int id = configData->name();
    int PlHp = configData->health() * 10;
    int score = configData->score();
    int hurt = -configData->attack();
	BasePlane* pl = nullptr;

	id = 1001;

	if(id < 1000)
	{
		if(	id != 4 && 
			id != 5 &&
			id != 10 &&
			id != 15 &&
			id != 20 )
		{
			//ֻ��ͼƬС��
			std::string file = String::createWithFormat("plane_%d.png", id)->getCString();
			pl = EnemyPlane::createWithPicture(file,configData->trajectoryID());
			if (id == 16)	//����Ҫˮƽ��ת
			{
				EnemyPlane* enePl = (EnemyPlane*)pl;
				enePl->setFlipXonEnter();
			}
		}
		else 
		{
			//��������С��
			std::string name = String::createWithFormat("plane_%d", id)->getCString();
			pl = EnemyPlane::createWithArmature(name,configData->trajectoryID());
		}
		dynamic_cast<EnemyPlane*>(pl)->startTrack();
		pl->setBltHurts(hurt);
		pl->setMaxHP(PlHp);
		pl->setUnitId(UnitId(id + (int)UnitId::ePlane_1));
		pl->setBitmask(
			ENEMY_PLANE_CATEGORYBITMASK,
			ENEMY_PLANE_COLLISIONBITMASK,
			ENEMY_PLANE_CONTACTTESTBITMASK);
		auto selfpro = pl->getSelfPro();
		selfpro.score = score;
		pl->setSelfPro(selfpro);
		pl->setNeedFlyToMainPl(true);
		pl->setFireConfig(atoi(configData->barrageId().c_str()),configData->barrageInterval());
	}
	else
	{
		switch (id)
		{
		case 1001:
			{
				//����BOSS
				pl = IronManBoss::create("kaijia",1);
				pl->setHpEff(HpEff(1.0f,100.0f));		//������Ѫ
				pl->getHpBar()->setLifeNum(5);
				pl->addRewards(0.0f, 10, DropGoodsType::DropGoods_Gold1);
				pl->addRewards(0.0f, 3, DropGoodsType::DropGoods_Gold2);
				pl->addRewards(0.0f, 1, DropGoodsType::DropGoods_Gold3);
			}
			break;
		case 1002:
			{
				//����BOSS
				pl = IronManBoss::create("kaijia",2);
				pl->setHpEff(HpEff(1.0f,100.0f));		//������Ѫ
				
				pl->addRewards(0.0f, 13, DropGoodsType::DropGoods_Gold1);
				pl->addRewards(0.0f, 5, DropGoodsType::DropGoods_Gold2);
				pl->addRewards(0.0f, 2, DropGoodsType::DropGoods_Gold3);
			}
			break;
		case 1003:
			{
				//����BOSS
				pl = IronManBoss::create("kaijia",3);
				pl->setHpEff(HpEff(1.0f,100.0f));		//������Ѫ

				pl->addRewards(0.0f, 16, DropGoodsType::DropGoods_Gold1);
				pl->addRewards(0.0f, 7, DropGoodsType::DropGoods_Gold2);
				pl->addRewards(0.0f, 3, DropGoodsType::DropGoods_Gold3);
			}
			break;
		case 1004:
			{
				pl = BoneWarriorBoss::create("boss-siling",1);
				pl->setHpEff(HpEff(1.0f,100.0f));		//������Ѫ
				pl->addRewards(0.0f, 19, DropGoodsType::DropGoods_Gold1);
				pl->addRewards(0.0f, 9, DropGoodsType::DropGoods_Gold2);
				pl->addRewards(0.0f, 4, DropGoodsType::DropGoods_Gold3);
			}
			break;
		case 1005:
			{
				pl = BoneWarriorBoss::create("boss-siling",2);
				pl->setHpEff(HpEff(1.0f,100.0f));		//������Ѫ

				pl->addRewards(0.0f, 22, DropGoodsType::DropGoods_Gold1);
				pl->addRewards(0.0f, 11, DropGoodsType::DropGoods_Gold2);
				pl->addRewards(0.0f, 5, DropGoodsType::DropGoods_Gold3);
			}
			break;
		case 1006:
			{
				pl = BoneWarriorBoss::create("boss-siling",3);
				pl->setHpEff(HpEff(1.0f,100.0f));		//������Ѫ

				pl->addRewards(0.0f, 25, DropGoodsType::DropGoods_Gold1);
				pl->addRewards(0.0f, 13, DropGoodsType::DropGoods_Gold2);
				pl->addRewards(0.0f, 6, DropGoodsType::DropGoods_Gold3);
			}
			break;
		case 1007:
			{
				pl = RenzheBoss::create("renzheboss-2",1);
				pl->setHpEff(HpEff(1.0f,100.0f));		//������Ѫ

				pl->addRewards(0.0f, 28, DropGoodsType::DropGoods_Gold1);
				pl->addRewards(0.0f, 15, DropGoodsType::DropGoods_Gold2);
				pl->addRewards(0.0f, 7, DropGoodsType::DropGoods_Gold3);
			}
			break;
		case 1008:
			{
				pl = RenzheBoss::create("renzheboss-2",2);
				pl->setHpEff(HpEff(1.0f,100.0f));		//������Ѫ

				pl->addRewards(0.0f, 31, DropGoodsType::DropGoods_Gold1);
				pl->addRewards(0.0f, 17, DropGoodsType::DropGoods_Gold2);
				pl->addRewards(0.0f, 8, DropGoodsType::DropGoods_Gold3);
			}
			break;
		case 1009:
			{
				pl = RenzheBoss::create("renzheboss-2",3);
				pl->setHpEff(HpEff(1.0f,100.0f));		//������Ѫ

				pl->addRewards(0.0f, 34, DropGoodsType::DropGoods_Gold1);
				pl->addRewards(0.0f, 19, DropGoodsType::DropGoods_Gold2);
				pl->addRewards(0.0f, 9, DropGoodsType::DropGoods_Gold3);
			}
			break;
		case 1010:
			{
				pl = PoseidonBoss::create("boss-meinv",1);
				pl->setHpEff(HpEff(1.0f,100.0f));		//������Ѫ
				BaseBoss* boss = (BaseBoss*)pl;
				boss->SwitchBgWhenDead(2);

				pl->addRewards(0.0f, 37, DropGoodsType::DropGoods_Gold1);
				pl->addRewards(0.0f, 21, DropGoodsType::DropGoods_Gold2);
				pl->addRewards(0.0f, 10, DropGoodsType::DropGoods_Gold3);
			}
			break;
		case 1011:
			{
				pl = PoseidonBoss::create("boss-meinv",2);
				pl->setHpEff(HpEff(1.0f,100.0f));		//������Ѫ
				BaseBoss* boss = (BaseBoss*)pl;
				boss->SwitchBgWhenDead(3);

				pl->addRewards(0.0f, 40, DropGoodsType::DropGoods_Gold1);
				pl->addRewards(0.0f, 23, DropGoodsType::DropGoods_Gold2);
				pl->addRewards(0.0f, 11, DropGoodsType::DropGoods_Gold3);
			}
			break;
		case 1012:
			{
				pl = PoseidonBoss::create("boss-meinv",3);
				pl->setHpEff(HpEff(1.0f,100.0f));		//������Ѫ

				pl->addRewards(0.0f, 43, DropGoodsType::DropGoods_Gold1);
				pl->addRewards(0.0f, 25, DropGoodsType::DropGoods_Gold2);
				pl->addRewards(0.0f, 12, DropGoodsType::DropGoods_Gold3);
			}
			break;
		default:
			break;
		}
		pl->setDeadMusic(Music_mainPlDead);		
		pl->setMaxHP(PlHp);
		pl->setUnitId(UnitId::ePlaneBoss);
		pl->setBltHurts(hurt);
		auto selfpro = pl->getSelfPro();
		selfpro.score = score;
		pl->setSelfPro(selfpro);
	}
	
	GameData::getInstance()->AddEnemyPlane(pl);
	return pl;
}
