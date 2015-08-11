#include "PhysicsUnit.h"
#include "DropGoods.h"
#include "Layer/UIController.h"
#include "../GameCommon/GameData.h"
#include "../GameCommon/EffectsManager.h"
#include "BasePlane/PlaneManager.h"


using namespace experimental;

//#define UP_LINE			1.08
//#define BOTTOM_LINE		-0.08
//#define LEFT_LINE		-0.12
//#define RIGHT_LINE		1.12

#define UP_LINE			1.5
#define BOTTOM_LINE		0.0
#define LEFT_LINE		-2.0
#define RIGHT_LINE		2.0

const GLchar* colorRotationShaderBody();
void xRotateMat(float mat[3][3], float rs, float rc);
void yRotateMat(float mat[3][3], float rs, float rc);
void zRotateMat(float mat[3][3], float rs, float rc);
void matrixMult(float a[3][3], float b[3][3], float c[3][3]);
void hueMatrix(GLfloat mat[3][3], float angle);
void premultiplyAlpha(GLfloat mat[3][3], float alpha);

PhysicsUnit::PhysicsUnit()
	:m_isInvincibleAtLine(false)
	,m_isNaturalDead(false)
	,m_isDirectDeath(false)
	,m_isOutScreen(false)
	,m_isRemoving(false)
	,m_Killer(UnitId::DefaultIdx)
{
	m_WinSize = Director::getInstance()->getWinSize();
}

void PhysicsUnit::onEnter()
{
	Sprite::onEnter();
	setEdgeLine();

	schedule(schedule_selector(PhysicsUnit::CheckIsInScreen));
	schedule(schedule_selector(PhysicsUnit::CheckDead));
	FuncAfterEnter();
}

void PhysicsUnit::onExit()
{
	Sprite::onExit();
	NotificationCenter::getInstance()->removeAllObservers(this);
}

void PhysicsUnit::RemoveMyself()
{
	unschedule(schedule_selector(PhysicsUnit::CheckDead));
	m_isRemoving = true;

	//死亡之前，留下遗言
	DoSomethingBeforeDead();

	deadLog();

	this->removeFromParentAndCleanup(true);
}

void PhysicsUnit::CheckIsInScreen(float dt)
{
	CCPoint pos = this->convertToWorldSpaceAR(Point::ZERO);

	//是否超出删除线，是则删除（在可以删除的前提下）
	if (pos.y < getEdgeLine(3) && pos.y > getEdgeLine(2) && 
		pos.x < getEdgeLine(1) && pos.x > getEdgeLine(0))
	{
		//进入了屏幕,在实时检测超出屏幕
		unschedule(schedule_selector(PhysicsUnit::CheckIsInScreen));
		schedule(schedule_selector(PhysicsUnit::CheckIsOutScreen));
	}
}

void PhysicsUnit::CheckIsOutScreen( float dt )
{
	CCPoint pos = this->convertToWorldSpaceAR(Point::ZERO);

	//是否超出删除线，是则删除（在可以删除的前提下）
	if (pos.y < getEdgeLine(3) && pos.y > getEdgeLine(2) && 
		pos.x < getEdgeLine(1) && pos.x > getEdgeLine(0))
	{
		
	}
	else
	{
		//不受死亡线影响
		if (!m_isInvincibleAtLine)
		{
			m_isNaturalDead = true;
		}
		m_isOutScreen = true;
		unschedule(schedule_selector(PhysicsUnit::CheckIsOutScreen));
	}
}

float PhysicsUnit::getEdgeLine(const int& idx)
{
	return m_EdgeLineVec.at(idx);
}

void PhysicsUnit::setEdgeLine()
{
	InitEdgeLine();

	//执行一个动作，在10S后设置在删除线可以删除（另外在更新函数中，还有判断是否进入屏幕，如果是，也会被设置为可以删除）
	//此处是为了保证任何单元出了屏幕外存活时间不超过10S（不受死亡线影响的除外）
	ActionInterval* delay = DelayTime::create(10.0);
	CallFunc* func = CallFunc::create(this,callfunc_selector(PhysicsUnit::CanRemoveAtLine));
	ActionInterval* seq = Sequence::create(delay, func, nullptr);
	runAction(seq);
}

void PhysicsUnit::setMaxBlood(int max )
{
	m_SelfPro.maxHP = max;
	m_SelfPro.curHP = max;
}

void PhysicsUnit::ImpactOfHp(const ContactPro& rs)
{
	if (m_SelfPro.isInvincible || m_SelfPro.curHP <= 0)
	{
		return;
	}
	m_SelfPro.curHP += (m_SelfPro.defense + rs.hurts);

	FuncWhenHpChanged();
	DeathImpactOfDropReward();
	DeathImpactOfDrops();
	if (m_SelfPro.curHP <= 0)
	{
		m_Killer = rs.belongTo;	//由谁杀死
		FuncHpEmpty();
	}
}

float PhysicsUnit::getHpPercent()
{
	return m_SelfPro.curHP * 1.0f / m_SelfPro.maxHP;
}

void PhysicsUnit::setInvincible(int invincible )
{
	m_SelfPro.isInvincible = invincible;
	
	/*Node* node = this->getChildByTag(20150215);
	if (node)
	{
		node->removeFromParentAndCleanup(true);
	}
	if (invincible)
	{
		node = Sprite::create("wudi.png");
		float w = getContentSize().width;
		float h =getContentSize().height;

		float scale = w > h ? w : h;
		node->setScale(scale / node->getContentSize().width);
		node->setPosition(w*0.5, h*0.5);
		node->runAction(RotateBy::create(1000,3600000));
		addChild(node,0,20150215);
	}*/
	

}

void PhysicsUnit::ChangeProperty( const ContactPro& rs)
{
	PlayMusicWhenContact();

	//其他碰撞处理
	OtherDealWhenContact(rs);

	//碰撞后对方能够给与我方的属相影响
	ImpactOfSpecialEffects(rs.effectsid);
	ImpactOfBuff(rs);
	ImpactOfHp(rs);
}

bool PhysicsUnit::isLiving()
{
	//log("-- my curHP = %d --",m_SelfPro.curHP);
	if (m_SelfPro.curHP <= 0)
	{
		return false;
	}

	return true;
}

void PhysicsUnit::ImpactOfSpecialEffects(const int& idx )
{
	//log("effect num = %d ",idx);
	if (idx < 1)
	{
		return;
	}
	Vec2 pos = getPosition();
	pos.y = pos.y - 10;
	UIController::getInstance()->getEffectsLayer()->displayScopeAmmo(pos);
}

void PhysicsUnit::DeathImpactOfDrops()
{
	bool b = isLiving();
	if (b)
	{
		return;
	}

	GameData::getInstance()->addScore(m_SelfPro.score);
	GameData::getInstance()->addGoldNum(m_SelfPro.goldsNum);
}

void PhysicsUnit::DeathImpactOfDropReward()
{
	bool isSpeedup = GameData::getInstance()->getisSpeedUp();
	if (isSpeedup)
	{
		return;
	}
	//当前血量百分比
	float curblood = getHpPercent();
	//log("----- cur curHP = %f ", curblood);
	std::vector<DropReward> drVec = m_SelfPro.rewardVec;
	std::vector<DropReward>::iterator it = drVec.begin();

	//搜索在当前血量能够触发的奖励数量
	int rewardsNums = 0;
	for (int i = 0; i < drVec.size(); i++)
	{
		DropReward dr = drVec.at(i);
		if (dr.HpPercent >= curblood)
		{
			rewardsNums++;
		}
	}

	//向下取值N次,并且把已经取得数从容器删除
	for (int i = 0; i < rewardsNums; i++)
	{
		//掉落对应ID物品
		DropReward dr = (*it);
		DropGoodsType tp = dr.DropTp;
		int num = dr.nums;

		Point DropPos = convertToWorldSpaceAR(Point::ZERO);

		GameData::getInstance()->getDropsFactory()->AddDrops(DropPos,tp, num);

		it = drVec.erase(it);
	}
	m_SelfPro.rewardVec = drVec;
}

void PhysicsUnit::ImpactOfBuff( const ContactPro& rs )
{
	bool isSpeedUp = GameData::getInstance()->getisSpeedUp();
	if (isSpeedUp)
	{
		//冲刺阶段，不触发特效
		return;
	}

	if (rs.buffPro.buffId <= BuffId::Buff_default)
	{
		return;
	}
	bool isActive = isBuffActive(rs.buffPro.buffId);
	if (isActive)
	{
		resetBuff(rs.buffPro.buffId);
		return;
	}
	auto newBuff = BaseBuff::create(this);
	newBuff->ActiveBuff(this,rs.buffPro.buffId,rs.buffPro.durarion);
	addChild(newBuff);
}

void PhysicsUnit::CanRemoveAtLine()
{
	schedule(schedule_selector(PhysicsUnit::CheckIsOutScreen));
}

void PhysicsUnit::setInvincibleAtLine(bool b )
{
	m_isInvincibleAtLine = b;
}

void PhysicsUnit::InitEdgeLine()
{
	m_EdgeLineVec.push_back(-60);
	m_EdgeLineVec.push_back(m_WinSize.width + 60);
	m_EdgeLineVec.push_back(-100);
	m_EdgeLineVec.push_back(m_WinSize.height);
}

void PhysicsUnit::CheckDead( float dt )
{
	if (m_isRemoving)		//测试，加个删除保护
	{
		return;
	}
	//自然死亡优先
	if (m_isNaturalDead)
	{
		RemoveMyself();
		return;
	}
	
	//其次强制死亡
	if (m_isDirectDeath)
	{
		RemoveMyself();
		return;
	}

	//其次判断击中死亡
	if (!isLiving() && !m_SelfPro.isInvincible)
	{
		PlayMusicWhenDead();
		AnimationWhenDead();
		EnemyPlaneStatistics();
		RemoveMyself();
	}
}

void PhysicsUnit::setDirectDeath()
{
	setHpEmpty();
	m_isDirectDeath = true;
}

void PhysicsUnit::setHpEmpty()
{
	m_SelfPro.isInvincible = 0;
	m_SelfPro.curHP = -1000;
}

void PhysicsUnit::setHpEmptyAndInvincible()
{
	m_SelfPro.curHP = -1;
	m_SelfPro.isInvincible = 1;
}

void PhysicsUnit::setUnitId( const UnitId& idx )
{
	m_SelfPro.idx = idx;
	m_ContactPro.idx = idx;
}

void PhysicsUnit::setBelongTo( const UnitId& idx )
{
	m_SelfPro.belongTo = idx;
	m_ContactPro.belongTo = idx;
}

void PhysicsUnit::setCantContact()
{
	_physicsBody->setCollisionBitmask(0);
	_physicsBody->setContactTestBitmask(0);
}
//*****************设置色调*********************/

//bool PhysicsUnit::initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect &rect, bool rotated)
//{
//	bool ret = Sprite::initWithTexture(texture, rect, rotated);
//	if(ret)
//	{
//		setupDefaultSettings();
//		initShader();
//	}
//	return ret;
//}
//
//bool PhysicsUnit::initWithTexture(cocos2d::Texture2D *texture)
//{
//	CCASSERT(texture != nullptr, "Invalid texture for sprite");
//
//	cocos2d::Rect rect = cocos2d::Rect::ZERO;
//	rect.size = texture->getContentSize();
//
//	return initWithTexture(texture, rect);
//}
//
//bool PhysicsUnit::initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect)
//{
//	return initWithTexture(texture, rect, false);
//}
//
//bool PhysicsUnit::initWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame)
//{
//	CCASSERT(spriteFrame != nullptr, "");
//
//	bool bRet = initWithTexture(spriteFrame->getTexture(), spriteFrame->getRect());
//	setSpriteFrame(spriteFrame);
//
//	return bRet;
//}
//
//void PhysicsUnit::setupDefaultSettings()
//{
//	_hue = 0.0f;
//}
//
//void PhysicsUnit::initShader()
//{
//	auto glprogram = cocos2d::GLProgramCache::getInstance()->getGLProgram("hue_program");
//	if(!glprogram)
//	{
//		glprogram = cocos2d::GLProgram::createWithByteArrays(cocos2d::ccPositionTextureColor_noMVP_vert, shaderBody());
//		cocos2d::GLProgramCache::getInstance()->addGLProgram(glprogram, "hue_program");
//	}
//	auto glprogramstate = cocos2d::GLProgramState::create(glprogram);
//	setGLProgramState(glprogramstate);
//	updateColor();
//}
//
//const GLchar* PhysicsUnit::shaderBody()
//{
//	return colorRotationShaderBody();
//}
//
//void PhysicsUnit::updateColor()
//{
//	Sprite::updateColor();
//	updateColorMatrix();
//	updateAlpha();
//}
//
//void PhysicsUnit::hueUniformCallback(cocos2d::GLProgram *p, cocos2d::Uniform *u)
//{
//	glUniformMatrix3fv(u->location, 1, GL_FALSE, (GLfloat*)&_mat);
//}
//
//void PhysicsUnit::updateColorMatrix()
//{
//	hueMatrix(_mat, _hue);
//	premultiplyAlpha(_mat, getAlpha());
//
//	getGLProgramState()->setUniformCallback("u_hue", CC_CALLBACK_2(PhysicsUnit::hueUniformCallback, this));
//}
//
//void PhysicsUnit::updateAlpha()
//{
//	getGLProgramState()->setUniformFloat("u_alpha", getAlpha());
//}
//
//GLfloat PhysicsUnit::getAlpha()
//{
//	return _displayedOpacity/255.0f;
//}
//
//float PhysicsUnit::getHue()
//{
//	return _hue;
//}
//
//void PhysicsUnit::setHue(float hue)
//{
//	_hue = hue;
//	updateColorMatrix();
//}



const GLchar * colorRotationShaderBody()
{
	return
		"                                                               \n\
		#ifdef GL_ES                                                    \n\
		precision mediump float;                                        \n\
		#endif                                                          \n\
		\n\
		varying vec2 v_texCoord;                                        \n\
		uniform mat3 u_hue;                                             \n\
		uniform float u_alpha;                                          \n\
		\n\
		void main()                                                     \n\
		{                                                               \n\
		vec4 pixColor = texture2D(CC_Texture0, v_texCoord);             \n\
		vec3 rgbColor = u_hue * pixColor.rgb;                           \n\
		gl_FragColor = vec4(rgbColor, pixColor.a * u_alpha);            \n\
		}                                                               \n\
		";
}

void xRotateMat(float mat[3][3], float rs, float rc)
{
	mat[0][0] = 1.0;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;

	mat[1][0] = 0.0;
	mat[1][1] = rc;
	mat[1][2] = rs;

	mat[2][0] = 0.0;
	mat[2][1] = -rs;
	mat[2][2] = rc;
}

void yRotateMat(float mat[3][3], float rs, float rc)
{
	mat[0][0] = rc;
	mat[0][1] = 0.0;
	mat[0][2] = -rs;

	mat[1][0] = 0.0;
	mat[1][1] = 1.0;
	mat[1][2] = 0.0;

	mat[2][0] = rs;
	mat[2][1] = 0.0;
	mat[2][2] = rc;
}

void zRotateMat(float mat[3][3], float rs, float rc)
{
	mat[0][0] = rc;
	mat[0][1] = rs;
	mat[0][2] = 0.0;

	mat[1][0] = -rs;
	mat[1][1] = rc;
	mat[1][2] = 0.0;

	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 1.0;
}

void matrixMult(float a[3][3], float b[3][3], float c[3][3])
{
	int x, y;
	float temp[3][3];

	for(y=0; y<3; y++) {
		for(x=0; x<3; x++) {
			temp[y][x] = b[y][0] * a[0][x] + b[y][1] * a[1][x] + b[y][2] * a[2][x];
		}
	}
	for(y=0; y<3; y++) {
		for(x=0; x<3; x++) {
			c[y][x] = temp[y][x];
		}
	}
}

void hueMatrix(GLfloat mat[3][3], float angle)
{
#define SQRT_2      sqrt(2.0)
#define SQRT_3      sqrt(3.0)

	float mag, rot[3][3];
	float xrs, xrc;
	float yrs, yrc;
	float zrs, zrc;

	// Rotate the grey vector into positive Z
	mag = SQRT_2;
	xrs = 1.0/mag;
	xrc = 1.0/mag;
	xRotateMat(mat, xrs, xrc);
	mag = SQRT_3;
	yrs = -1.0/mag;
	yrc = SQRT_2/mag;
	yRotateMat(rot, yrs, yrc);
	matrixMult(rot, mat, mat);

	// Rotate the hue
	zrs = sin(angle);
	zrc = cos(angle);
	zRotateMat(rot, zrs, zrc);
	matrixMult(rot, mat, mat);

	// Rotate the grey vector back into place
	yRotateMat(rot, -yrs, yrc);
	matrixMult(rot,  mat, mat);
	xRotateMat(rot, -xrs, xrc);
	matrixMult(rot,  mat, mat);
}

void premultiplyAlpha(GLfloat mat[3][3], float alpha)
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			mat[i][j] *= alpha;
		}
	}
}





