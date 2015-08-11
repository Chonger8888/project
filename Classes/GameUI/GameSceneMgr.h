#ifndef __GAME_SCENE_MGR_H__
#define __GAME_SCENE_MGR_H__
#include "cocos2d.h"

USING_NS_CC;


/************************************************************************/
/* ������������                                                         */
/************************************************************************/

typedef enum GameSceneType
{
	Scene_Type_FristInterface,		// Game entry first interface
	Scene_Type_LevelInterface,		// Game level select interface
	Scene_Type_RoleInterface,		// Game show role or role select interface
	Scene_Type_PlayInterface,		// Game play entry...
	Scene_Type_PlayILoading,			// Game play loading
}GameSceneType_E;



class GameSceneMgr
{

public:

	typedef enum TransitionType
	{
		CCTransitionJumpZoom,				// ����һ�������Ĺ��ɶ���
		CCTransitionProgressRadialCCW,		// ����һ����������ʽ�Ĺ��ɶ����� ��ʱ�뷽��
		CCTransitionProgressRadialCW,		// ����һ����������ʽ�Ĺ��ɶ����� ˳ʱ�뷽��
		CCTransitionProgressHorizontal,		// ����һ��ˮƽ����ʽ�Ĺ��ɶ���
		CCTransitionProgressVertical,		// ����һ����ֱ����ʽ�Ĺ��ɶ���
		CCTransitionProgressInOut,			// ����һ������������չ�Ĺ��ɶ���
		CCTransitionProgressOutIn,			// ����һ������������չ�Ĺ��ɶ���
		CCTransitionCrossFade,				// ����һ����͸���Ĺ��ɶ���
		CCTransitionPageTurn,				// ����һ����ҳ�Ĺ��ɶ���
		CCTransitionFadeTR,					// ����һ���������ɶ����� �����µ�����
		CCTransitionFadeBL,					// ����һ���������ɶ����� �����ϵ�����
		CCTransitionFadeUp,					// ����һ�����µ��ϣ������۵��Ĺ��ɶ���
		CCTransitionFadeDown,				// ����һ�����ϵ��£������۵��Ĺ��ɶ���
		CCTransitionTurnOffTiles,			// ����һ�����������ʧ�Ĺ��ɶ���
		CCTransitionSplitRows,				// ����һ�����л����л��Ĺ��ɶ���
		CCTransitionSplitCols,				// ����һ�����л����л��Ĺ��ɶ���
		CCTransitionFade,					// ����һ���𽥹��ɵ�Ŀ����ɫ���л�����
		CCTransitionFlipX,					// ����һ��x�ᷴת���л�����
		CCTransitionFlipY,					// ���ɶ����ĳ���ʱ��
		CCTransitionFlipAngular,			// ����һ�����з�ת���л�����
		CCTransitionZoomFlipX,				// ����һ���������ŵ�x�ᷴת�л��Ķ���
		CCTransitionZoomFlipY,				// ����һ���������ŵ�Y�ᷴת�л��Ķ���
		CCTransitionZoomFlipAngular,		// ����һ���������� ����ת���л��Ķ���
		CCTransitionShrinkGrow,				// ����һ����������Ĺ��ɶ���
		CCTransitionRotoZoom,				// ����һ����ת��������Ĺ��ɶ���
		CCTransitionMoveInL,				// ����һ����������븲�ǵĹ��ɶ���
		CCTransitionMoveInR,				// ����һ�����ұ����븲�ǵĹ��ɶ���
		CCTransitionMoveInB,				// ����һ�����±����븲�ǵĹ��ɶ���
		CCTransitionMoveInT,				// ����һ�����ϱ����븲�ǵĹ��ɶ���
		CCTransitionSlideInL,				// ����һ����������벢�����ɳ����Ĺ��ɶ���
		CCTransitionSlideInR,				// ����һ�����Ҳ����벢�����ɳ����Ĺ��ɶ���
		CCTransitionSlideInT,				// ����һ���Ӷ������벢�����ɳ����Ĺ��ɶ���
		CCTransitionSlideInB,				// ����һ�����²����벢�����ɳ����Ĺ��ɶ���
											   
		TransitionType_Max
	}TransitionType;


	GameSceneMgr()
		:_isShowLoading(true)
	{

	}
	~GameSceneMgr()
	{

	}
	// ����������ʵ��
	static GameSceneMgr* getInstance(void);
	static void purgeInstance(void);

	// �л�����
	void changeScene( GameSceneType type , bool isfrist = false );

	Scene* getTransitionAction( float t, Scene* s , TransitionType type);
	Scene* getRandTransitionAction( float t, Scene* s );


private:
	static GameSceneMgr* _gameSceneMgr;

	bool _isShowLoading;


};





#endif	//!__GAME_SCENE_MGR_H__