#include "Loading.h"
#include "../editor-support/cocostudio/CCArmatureDataManager.h"
#include "GameSceneMgr.h"
#include "GameCommon/GameData.h"



using namespace cocostudio;

Scene* LoadingLayer::createScene()
{
	auto scene = Scene::create();
	
	//loading��
	auto layer = LoadingLayer::create();
	scene->addChild(layer);

	return scene;
}

 
bool LoadingLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	m_curPreloadNum = 0;
	m_totalNums = 0;

	auto ws =Director::getInstance()->getWinSize();

	auto imgGuang = Sprite::create("loadRes/imgRoleGuang.png");
	imgGuang->setPosition(Vec2(ws.width*0.5 - 5+ 120 ,ws.height*0.6-5 ));
	imgGuang->setScale( 2.3f );
	addChild(imgGuang);

	auto role = Sprite::create("roleImg/roleImg3.png");
	role->setPosition(Vec2(ws.width*0.5+ 120,ws.height*0.6 ));
	role->setScale( 0.8f );
	addChild(role);


 	initProgress();
 	initFile();


	return true;
}

void LoadingLayer::initFile()
{
	//�з�С���ӵ�
	addFrameRes("ammo/EnemyNorBullet.plist",
		"ammo/EnemyNorBullet.png");

	//С��������ը
	addFrameRes("animation/PlaneDeadExpl/PlaneDeadExpl.plist",
		"animation/PlaneDeadExpl/PlaneDeadExpl.png");

	//�����ӵ�
	addFrameRes("ammo/SelfBullet.plist",
		"ammo/SelfBullet.png");
	
	//BOSS�ӵ�
	addFrameRes("ammo/BossBullet.plist",
		"ammo/BossBullet.png");

	//�����ӵ�
	addFrameRes("ammo/CardBullet.plist",
		"ammo/CardBullet.png");
	
	//���ѵ�ǰ��
	addFrameRes("animation/ExplosionBullet/ExplBult_1.plist",
		"animation/ExplosionBullet/ExplBult_1.png");

	//���ѵ����
	addFrameRes("animation/ExplosionBullet/ExplBult_2.plist",
		"animation/ExplosionBullet/ExplBult_2.png");

	//������
	addFrameRes("Drops/DropResList.plist",
		"Drops/DropResList.png");

	//�л���Դ
	addFrameRes("plane/enemy/enemyPlane.plist",
		"plane/enemy/enemyPlane.png");

	//����1��ͨ
	addFrameRes("animation/MainPl1N/MainPl1.plist",
		"animation/MainPl1N/MainPl1.png");

	//����1���ƶ�
	addFrameRes("animation/MainPl1N/Plane1MoveAnimL.plist",
		"animation/MainPl1N/Plane1MoveAnimL.png");

	//����1���ƶ�
	addFrameRes("animation/MainPl1N/Plane1MoveAnimR.plist",
		"animation/MainPl1N/Plane1MoveAnimR.png");

	//����2��ͨ
	addFrameRes("animation/MainPl2N/MainPl2.plist",
		"animation/MainPl2N/MainPl2.png");

	//����2���ƶ�
	addFrameRes("animation/MainPl2N/Pl2MoveL.plist",
		"animation/MainPl2N/Pl2MoveL.png");

	//����2���ƶ�
	addFrameRes("animation/MainPl2N/Pl2MoveR.plist",
		"animation/MainPl2N/Pl2MoveR.png");

	//����3��ͨ
	addFrameRes("animation/MainPl3N/MainPl3.plist",
		"animation/MainPl3N/MainPl3.png");

	//����3���ƶ�
	addFrameRes("animation/MainPl3N/Pl3MoveL.plist",
		"animation/MainPl3N/Pl3MoveL.png");

	//����3���ƶ�
	addFrameRes("animation/MainPl3N/Pl3MoveR.plist",
		"animation/MainPl3N/Pl3MoveR.png");

	//�����ܻ�
	addFrameRes("animation/LightingEff/LightingEff.plist",
		"animation/LightingEff/LightingEff.png");

	//����β��1
	addFrameRes("animation/SpeedUpProtect/ProtectTailFirst.plist",
		"animation/SpeedUpProtect/ProtectTailFirst.png");

	//����β��2
	addFrameRes("animation/SpeedUpProtect/ProtectTailSec.plist",
		"animation/SpeedUpProtect/ProtectTailSec.png");

	//����
	addFrameRes("animation/PlaneProtect/PlaneProtect.plist",
		"animation/PlaneProtect/PlaneProtect.png");

	//��������
	addFrameRes("animation/FireSurround/FireExpl.plist",
		"animation/FireSurround/FireExpl.png");

	//������������֡
	addFrameRes("animation/Laser3/Laser3.plist",
		"animation/Laser3/Laser3.png");

	//����������Ч
	addFrameRes("animation/GiftEff/GiftEffMagnetAni.plist",
		"animation/GiftEff/GiftEffMagnetAni.png");

	//����������Ч
	addFrameRes("animation/GiftEff/GiftEffWpUpgradeAni.plist",
		"animation/GiftEff/GiftEffWpUpgradeAni.png");

	//��Ѫ��Ч
	addFrameRes("animation/GiftEff/GiftEffAddHpAni.plist",
		"animation/GiftEff/GiftEffAddHpAni.png");

	//��
	addFrameRes("animation/arrow/arrowAnimat.plist",
		"animation/arrow/arrowAnimat.png");
	
	//��
	addFrameRes("animation/bow/bowAnimat.plist",
		"animation/bow/bowAnimat.png");

	//BOSS1
	addArmatureRes("animation/boss1/kaijia0.png",
		"animation/boss1/kaijia0.plist",
		"animation/boss1/kaijia.ExportJson");

	//BOSS2
	addArmatureRes("animation/boss2/boss-siling0.png",
		"animation/boss2/boss-siling0.plist",
		"animation/boss2/boss-siling.ExportJson");
	
	//BOSS3
	addArmatureRes("animation/boss3/renzheboss-20.png",
		"animation/boss3/renzheboss-20.plist",
		"animation/boss3/renzheboss-2.ExportJson");

	//BOSS4
	addArmatureRes("animation/boss4/boss-meinv0.png",
		"animation/boss4/boss-meinv0.plist",
		"animation/boss4/boss-meinv.ExportJson");

	//����1��
	addArmatureRes("plane/armat/fireDragon/xiaolong/xiaolong0.png",
		"plane/armat/fireDragon/xiaolong/xiaolong0.plist",
		"plane/armat/fireDragon/xiaolong/xiaolong.ExportJson");

	//����2��
	addArmatureRes("plane/armat/fireDragon/zhonglong/zhonglong0.png",
		"plane/armat/fireDragon/zhonglong/zhonglong0.plist",
		"plane/armat/fireDragon/zhonglong/zhonglong.ExportJson");

	//����3��
	addArmatureRes("plane/armat/fireDragon/julong/julong0.png",
		"plane/armat/fireDragon/julong/julong0.plist",
		"plane/armat/fireDragon/julong/julong.ExportJson");

	//������ʽ1
	//addArmatureRes("","","animation/Role1/shijiu.ExportJson");

	//������ʽ2
	//addArmatureRes("","","animation/Role2/shuanglong.ExportJson");

	//������ʽ3
	//addArmatureRes("","","animation/Role3/huolong.ExportJson");

	//��ͨ�л���������
	addArmatureRes("plane/armat/plane_4/plane_40.png",
		"plane/armat/plane_4/plane_40.plist",
		"plane/armat/plane_4/plane_4.ExportJson");

	addArmatureRes("plane/armat/plane_5/plane_50.png",
		"plane/armat/plane_5/plane_50.plist",
		"plane/armat/plane_5/plane_5.ExportJson");

	addArmatureRes("plane/armat/plane_10/plane_100.png",
		"plane/armat/plane_10/plane_100.plist",
		"plane/armat/plane_10/plane_10.ExportJson");

	addArmatureRes("plane/armat/plane_15/plane_150.png",
		"plane/armat/plane_15/plane_150.plist",
		"plane/armat/plane_15/plane_15.ExportJson");

	addArmatureRes("plane/armat/plane_20/plane_200.png",
		"plane/armat/plane_20/plane_200.plist",
		"plane/armat/plane_20/plane_20.ExportJson");


	LoadResources();

}

void LoadingLayer::LoadResources()
{
	for (auto it = m_FrameResVec.begin(); it != m_FrameResVec.end(); it++)
	{
		std::string pngFile = (*it).pngFile;
		Director::getInstance()->getTextureCache()->addImageAsync(pngFile.c_str(),
			CC_CALLBACK_1(LoadingLayer::LoadResCallback,this));
	}
	
}

void LoadingLayer::LoadResCallback( cocos2d::Texture2D* texture )
{
	std::string plistFile = m_FrameResVec.at(m_curPreloadNum).plistFile;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistFile.c_str(), texture); 
	m_curPreloadNum++;

	refreshProgress();

	if (m_curPreloadNum >= m_FrameResVec.size())
	{
		if (m_ArmatureResVec.empty())
		{
			HideDraw();
			return;
		}
		//����������ͨ��Դ
		for (auto it = m_ArmatureResVec.begin(); it != m_ArmatureResVec.end(); it++)
		{
			ResStruct rs = (*it);
			if (rs.plistFile.empty())
			{
				ArmatureDataManager::getInstance()->
					addArmatureFileInfoAsync(rs.configFile,
					this,schedule_selector(LoadingLayer::LoadArmatureCallback)
					);
			}
			else
			{
				ArmatureDataManager::getInstance()->
					addArmatureFileInfoAsync(rs.pngFile,rs.plistFile,rs.configFile,
					this,schedule_selector(LoadingLayer::LoadArmatureCallback)
					);
			}
		}
	}
}

void LoadingLayer::addFrameRes( const char* plist, const char* pngfile)
{
	ResStruct rs;
	rs.plistFile = plist;
	rs.pngFile = pngfile;
	m_FrameResVec.push_back(rs);
}

void LoadingLayer::initProgress()
{
	Size ws = Director::getInstance()->getWinSize();
	//m_loadProgress = Label::create("0%","",50);
	//m_loadProgress->setPosition(ws.width*0.5, ws.height*0.3);
	//addChild(m_loadProgress,1);

	auto load = Sprite::create("loadRes/imgLoading.png");
	load->setPosition(ws.width*0.5, ws.height*0.25);
	addChild(load);

	Sprite* barBg = Sprite::create("loadRes/imgProgressBg.png");
	barBg->setPosition(ws.width*0.5, ws.height*0.3);
	addChild(barBg);

	Sprite* sp = Sprite::create("loadRes/imgProgress.png");
	m_BloodBar = ProgressTimer::create(sp);
	m_BloodBar->setType(kCCProgressTimerTypeBar);  
	m_BloodBar->setBarChangeRate(Vec2(1,0));
	m_BloodBar->setPercentage(20);
	m_BloodBar->setMidpoint(ccp(0,0.5));
	m_BloodBar->setPosition(ccp(barBg->getContentSize().width*0.5,barBg->getContentSize().height*0.5));
	barBg->addChild(m_BloodBar);
	barBg->setScaleY(0.9);

	m_CurProgress = Sprite::create("loadRes/imgCurPr.png");
	m_CurProgress->setPosition(ccp(20,barBg->getContentSize().height*0.45));
	barBg->addChild(m_CurProgress);

	auto parcile = ParticleSystemQuad::create("particla/LoadBar/LoadBar.plist");
	parcile->setPosition( Vec2(9,15) );
	m_CurProgress->addChild( parcile );

	ActionInterval* tint = TintBy::create(0.3f,0,0,-255);
	ActionInterval* ease = EaseCircleActionInOut::create(tint);
	ActionInterval* back = ease->reverse();
	ActionInterval* seq = Sequence::create(tint, back, nullptr);
	ActionInterval* rep = RepeatForever::create(seq);
	//m_BloodBar->runAction(rep);

}

void LoadingLayer::refreshProgress()
{
	int totalnum = m_FrameResVec.size() + m_ArmatureResVec.size();

	float percent = m_curPreloadNum*1.0 /totalnum;
	std::string progress = String::createWithFormat("%3.0f%%",percent * 100)->getCString();
	//m_loadProgress->setString(progress);
	m_BloodBar->setPercentage(percent*100);

	float barY = m_BloodBar->getContentSize().width*percent;
	if ( barY < m_BloodBar->getContentSize().width*0.95 )
		m_CurProgress->setPosition( Vec2(barY,m_CurProgress->getPosition().y) );
	else
		m_CurProgress->setPosition( Vec2(m_BloodBar->getContentSize().width*0.95,m_CurProgress->getPosition().y) );
}

void LoadingLayer::HideDraw()
{
	//������ɣ�������Ϸ
	//GameSceneMgr::getInstance()->changeScene( Scene_Type_PlayInterface );
	GameSceneMgr::getInstance()->changeScene(Scene_Type_FristInterface);
}

void LoadingLayer::addArmatureRes( const std::string& imagePath, const std::string& plistPath, const std::string& configFilePath )
{
	ResStruct rs;
	rs.plistFile = plistPath;
	rs.pngFile = imagePath;
	rs.configFile = configFilePath;
	m_ArmatureResVec.push_back(rs);
}

void LoadingLayer::LoadArmatureCallback( float dt )
{
	m_curPreloadNum++;
	refreshProgress();

	int totalnum = m_FrameResVec.size() + m_ArmatureResVec.size();
	if (m_curPreloadNum >= totalnum)
	{
		ActionInterval* delay = DelayTime::create(1.0f);
		CallFunc* func = CallFunc::create(this,callfunc_selector(LoadingLayer::delayDrawScene));
		ActionInterval* seq = Sequence::create(delay, func, nullptr);
		runAction(seq);
	}

}

void LoadingLayer::delayDrawScene()
{
	HideDraw();
}

