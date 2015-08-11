#include "ContactManager.h"
#include "PhysicsUnit.h"

void ContactManager::onEnter()
{
	Layer::onEnter();

	//Ìí¼ÓÅö×²¼àÌý
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(ContactManager::onContactBegin,this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener,this);
}

void ContactManager::onExit()
{
	Layer::onExit();
}

bool ContactManager::onContactBegin( const PhysicsContact& contact )
{
	PhysicsBody* bodyA = contact.getShapeA()->getBody();
	PhysicsBody* bodyB = contact.getShapeB()->getBody();
	if (!bodyA || !bodyB)
	{
		return true;
	}
	PhysicsUnit* puA = (PhysicsUnit*)(bodyA->getNode());
	PhysicsUnit* puB = (PhysicsUnit*)(bodyB->getNode());
	if (!puA || !puB)
	{
		return true;
	}
	Vec2 posA = puA->convertToWorldSpaceAR(Vec2::ZERO);
	Vec2 posB = puB->convertToWorldSpaceAR(Vec2::ZERO);

	ContactPro rsA = puA->getContactPro();
	ContactPro rsB = puB->getContactPro();
	rsA.pos = posA;
	rsB.pos = posB;

	puA->ChangeProperty(rsB);
	puB->ChangeProperty(rsA);

	return true;
}


