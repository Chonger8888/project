#include "Buff.h"
#include "../GameCommon/GameDefine.h"
#include "../GameCommon/GameData.h"
#include "../GameCommon/GameConfig.h"
#include "Weapon/weapon.h"
#include "Weapon/WeaponFactory.h"
#include "Bullet/Bullet.h"
#include "Layer/UIController.h"
#include "Layer/PhysicsLayer.h"

BaseBuff* BaseBuff::create(BuffManager* buffmanager)
{
	auto buff = new (std::nothrow) BaseBuff;
	if (buff && buff->init())
	{
		buff->setManager(buffmanager);
		buff->autorelease();
		return buff;
	}
	else
	{
		CC_SAFE_DELETE(buff);
		return nullptr;
	}
}

void BaseBuff::setManager( BuffManager* buffmanager )
{
	m_buffManager = buffmanager;
}

void BaseBuff::ActiveBuff( Node* node,const BuffId& buffId, const float& duration )
{
	m_buffid = buffId;
	m_maxTime = duration;
	m_LeftTime = duration;
	m_buffNode = node;

	//ִ�м���BUFF����
	bool isActive = m_buffManager->isBuffActive(buffId);
	if (!isActive)
	{
		addBUffById(buffId);
		ActionInterval* delay = DelayTime::create(1.0);
		CallFunc*		func = CallFunc::create(this,callfunc_selector(BaseBuff::CutDown));
		ActionInterval* seq = Sequence::create(delay,func,nullptr);
		ActionInterval* repeat = RepeatForever::create(seq);
		runAction(repeat);
	}
}

void BaseBuff::CutDown()
{
	m_LeftTime -= 1.0;
	//log("--left time = %f",m_LeftTime);
	if (m_LeftTime <= 0.0)
	{
		m_LeftTime = 0.0;
		removeBuff();
	}
}

float BaseBuff::getLeftTime()
{
	return m_LeftTime;
}

BuffId BaseBuff::getBuffId()
{
	return m_buffid;
}

void BaseBuff::removeBuff()
{
	//ִ������BUFF����
	removeBuffById(m_buffid);

	m_buffManager->DelBuff(this);
	this->removeFromParentAndCleanup(true);
}

void BaseBuff::addBUffById(const BuffId& buffid )
{
	switch (buffid)
	{
	case BuffId::Buff_default:
		break;
	case BuffId::Buff_Sandan:
		{
			
			break;
		}
	case BuffId::Buff_Burning:
		{
			
			break;
		}
	case BuffId::Buff_TempNorWp1:
	case BuffId::Buff_TempNorWp2:
	case BuffId::Buff_TempNorWp3:
		{
			//���ɻ�����������
			int planeType = NewDataMgr::getInstance()->mRoleConfig.roleIndex;
			int planeLvl = NewDataMgr::getInstance()->getRoleGradeWithId( planeType );
			auto mainpl = GameData::getInstance()->getMainPlane();
			auto weaponFact = GameData::getInstance()->getWeaponFactory();
			planeLvl = (planeLvl-1)/12 + 1;
			planeLvl++;
			if (planeLvl > 5 )
			{
				planeLvl = 5;
			}
			weaponFact->AddMainPlaneWp(planeType, planeLvl, mainpl, true);
		}
		break;
	case BuffId::Buff_LightingLvl1:
		{
			auto cardType = NewDataMgr::getInstance()->mRoleConfig.cardArr[2].cardID;
			auto cardLvl = NewDataMgr::getInstance()->mRoleConfig.cardArr[2].cardStarLv;
			cardType = cardType%3;
			int hurt = DataConfig::getCardHurts(cardType, cardLvl);
			hurt /= 24;
			NewLightingLayer* layer = NewLightingLayer::create(1, hurt);
			UIController::getInstance()->getPhysicsLayer()->addChild(layer,-2000);
			layer->setTag(5191802);

		}
		break;
	case BuffId::Buff_LightingLvl2:
		{
			auto cardType = NewDataMgr::getInstance()->mRoleConfig.cardArr[2].cardID;
			auto cardLvl = NewDataMgr::getInstance()->mRoleConfig.cardArr[2].cardStarLv;
			cardType = cardType%3;
			int hurt = DataConfig::getCardHurts(cardType, cardLvl);
			hurt /= 24;

			NewLightingLayer* layer = NewLightingLayer::create(2,hurt);
			UIController::getInstance()->getPhysicsLayer()->addChild(layer,-2000);
			layer->setTag(5191802);

		}
		break;
	case BuffId::Buff_LightingLvl3:
		{
			auto cardType = NewDataMgr::getInstance()->mRoleConfig.cardArr[2].cardID;
			auto cardLvl = NewDataMgr::getInstance()->mRoleConfig.cardArr[2].cardStarLv;
			cardType = cardType%3;
			int hurt = DataConfig::getCardHurts(cardType, cardLvl);
			hurt /= 24;
			NewLightingLayer* layer = NewLightingLayer::create(3, hurt);
			UIController::getInstance()->getPhysicsLayer()->addChild(layer,-2000);
			layer->setTag(5191802);

		}
		break;
	case BuffId::Buff_Magnet:
		{
			auto mainpl = GameData::getInstance()->getMainPlane();
			if (mainpl)
			{
				MainPlane* pl = (MainPlane*) mainpl;
				pl->addMagnet();
			}
		}
		break;
	default:
		break;
	}
	m_buffManager->addBuff(this);
}

void BaseBuff::removeBuffById(const BuffId& buffid )
{
	switch (buffid)
	{
	case BuffId::Buff_default:
		break;
	case BuffId::Buff_Sandan:
		{
		
			break;
		}
	case BuffId::Buff_Burning:
		{
			
			break;
		}
	case BuffId::Buff_TempNorWp1:
	case BuffId::Buff_TempNorWp2:
	case BuffId::Buff_TempNorWp3:
		{
			MainPlane* pl = (MainPlane*)m_buffNode;
			pl->resumeNorWp();
		}
		break;;
	case BuffId::Buff_LightingLvl1:
	case BuffId::Buff_LightingLvl2:
	case BuffId::Buff_LightingLvl3:
		{
			auto node = UIController::getInstance()->getPhysicsLayer()->getChildByTag(5191802);
			if (node)
			{
				node->removeFromParentAndCleanup(true);
			}
		}
		break;
	case BuffId::Buff_Magnet:
		{
			auto mainpl = GameData::getInstance()->getMainPlane();
			if (mainpl)
			{
				MainPlane* pl = (MainPlane*) mainpl;
				pl->removeMagnet();
			}
		}
		break;
	default:
		break;
	}
}

void BaseBuff::resetTime()
{
	m_LeftTime = m_maxTime;
}





//---------------------------------
void BuffManager::addBuff( BaseBuff* buff )
{
	m_buffVec.push_back(buff);
}

void BuffManager::DelBuff( BaseBuff* buff )
{
	std::vector<BaseBuff*>::iterator it = m_buffVec.begin();

	for (it; it != m_buffVec.end(); it++)
	{
		BaseBuff* bb = (*it);
		if (bb->getBuffId() == buff->getBuffId())
		{
			m_buffVec.erase(it);
			break;
		}
	}
}

bool BuffManager::isBuffActive( const BuffId& buffid )
{
	bool isActive = false;
	std::vector<BaseBuff*>::iterator it = m_buffVec.begin();

	for (it; it != m_buffVec.end(); it++)
	{
		BaseBuff* bb = (*it);
		if (bb->getBuffId() == buffid)
		{
			isActive = true;
			break;
		}
	}

	return isActive;
}

void BuffManager::resetBuff(const BuffId& buffid)
{
	std::vector<BaseBuff*>::iterator it = m_buffVec.begin();

	for (it; it != m_buffVec.end(); it++)
	{
		BaseBuff* bb = (*it);
		if (bb->getBuffId() == buffid)
		{
			bb->resetTime();
			break;
		}
	}

}
