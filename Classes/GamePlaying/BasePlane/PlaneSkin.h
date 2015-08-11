#ifndef PlaneSkin_h__
#define PlaneSkin_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "..\..\..\cocos2d\cocos\editor-support\cocostudio\CCArmature.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocostudio;

class EnemyPlane;

//�ɻ�Ƥ��
class PlaneSkin
{
public:
	//Ƥ����������ʽ
	enum class SkinType
	{
		Skin_Picture,		//ͼƬ
		Skin_Animation,		//����֡
		Skin_Armature,		//��������
	};

	Sprite* createSkinWithPicture(const std::string &name);
	//Animate* createSkinWithAnimate();
	Armature* createSkinWithArmatureById(int id);
};

//�ɻ���������
class PlaneArmature : public Armature
{
public:
	PlaneArmature()
	{
		m_Target = nullptr;
	}
	static PlaneArmature* create(const std::string& name, EnemyPlane* target);
	
	void playRestAnimation(int idx = 0);		//��Ϣ������Ĭ��Ϊ0
	void playShootAnimation(int idx = 1);		//���������Ĭ��Ϊ1
	void playHackAnimation(int idx = 2);		//��ɱ������Ĭ��Ϊ2
	
	//��������
	void StartShoot();

private:
	void setPlaneTarget(EnemyPlane* target);

	void onFrameEventOfShoot( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex );
	void onFrameEventOfHack( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex );

	EnemyPlane*					m_Target;	
};


#endif // PlaneSkin_h__