#include "CloudLayer.h"
#include "GameCommon\GameData.h"

void CloudLayer::onEnter()
{
	Layer::onEnter();
	m_isCloudSpeedUp = false;
	m_ws = Director::getInstance()->getWinSize();
	addOneCloud();
}

void CloudLayer::addOneCloud()
{
	int randId = random(1,5);
	auto file = String::createWithFormat("Bg/cloud/cloud_%d.png", randId)->getCString();
	auto cloud = Sprite::create(file);
	cloud->setPosition(m_ws.width* (0.5 + rand_minus1_1() * 0.3), m_ws.height * 2.0);
	addChild(cloud);
	cloud->setName("cloud");

	int Zorder = random(1, 3);
	cloud->setOpacity(100 + Zorder * 50);
	cloud->setZOrder(Zorder);
	cloud->setScale(1.0f + rand_0_1() * 6.0f);
	//cloud->setRotation(rand_minus1_1() * 180);

	float mvTime = 15.0f - Zorder*2;
	float nextTime = 0.5f + rand_0_1() * 10.0f;
	bool isSpeedUp = GameData::getInstance()->getisSpeedUp();

	//如果在冲刺，加速出现云朵
	if (isSpeedUp)
	{
		mvTime = Zorder;
		nextTime = 0.5f + rand_0_1() * 2.0f;

		SpeedUp();
	}
	//移动
	{
		auto mv = MoveTo::create(mvTime, Vec2(m_ws.width * (0.5 + rand_minus1_1() * 0.5), -m_ws.height*0.7));
		auto rm = RemoveSelf::create(true);
		auto seq = Sequence::create(mv, rm, nullptr);
		cloud->runAction(seq);
	}
	
	{
		//下一朵云
		auto node = Node::create();
		addChild(node);

		auto delay = DelayTime::create(nextTime);
		auto next = CallFunc::create(this, callfunc_selector(CloudLayer::addOneCloud));
		auto rm = RemoveSelf::create(true);
		auto seq = Sequence::create(delay, next, rm, nullptr);
		node->runAction(seq);
	}

}

void CloudLayer::SpeedUp()
{
	if (m_isCloudSpeedUp)
	{
		return;
	}
	m_isCloudSpeedUp = true;
	int num = getChildrenCount();
	for (int i = 0; i < num; i++)
	{
		Node* child = getChildByName("cloud");
		if (child)
		{
			child->stopAllActions();

			auto mv = MoveBy::create(1.0f, Vec2(0, -m_ws.height * 1.5));
			auto rm = RemoveSelf::create(true);
			auto seq = Sequence::create(mv, rm, nullptr);
			child->runAction(seq);
		}
	}
}

