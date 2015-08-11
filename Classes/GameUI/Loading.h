/********************************************************************
������:		2014/11/27  12:28	
�ļ���: 	E:\Cocos3.3_Project\MyGame\Classes\Loading.h 
��  ��:		alex(alex)
�޸��ڣ�	2014/11/27  12:28	

Ŀ  ��:		��Դ���ؽ���
*********************************************************************/
#ifndef _LOADING_H_
#define _LOADING_H_


#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

typedef struct ResStruct
{
	std::string plistFile;
	std::string pngFile;
	std::string configFile;
	ResStruct()
	{
		plistFile = "";
		pngFile = "";
		configFile = "";
	}
}ResStruct;

class LoadingLayer : public Layer
{
public:
	CREATE_FUNC(LoadingLayer);

	static Scene* createScene();

	virtual bool init();

	void initProgress();
	void refreshProgress();

	//��������֡��Դ
	void initFile();

	void LoadResources();

	void LoadResCallback(cocos2d::Texture2D* texture);

	void LoadArmatureCallback(float dt);

	//��������֡���������Դ
	void addFrameRes(const char* plist, const char* pngfile);

	//���ع�������
	void addArmatureRes(const std::string& imagePath, const std::string& plistPath, const std::string& configFilePath);

	//Ϊ�˽����Щһ���Դ�����Ⱦ���״Σ�����˲�俨�ٵ����������ڱ�����֮��������Ⱦһ��
	void HideDraw();

	//�ӳټ��س���
	void delayDrawScene();

private:
	//Label*					m_loadProgress;

	int						m_curPreloadNum;
	int						m_totalNums;		//��Դ������

	std::vector<ResStruct>	m_FrameResVec;		//��ͨ��Դ�б�
	std::vector<ResStruct>	m_ArmatureResVec;	//����������Դ�б�

	ProgressTimer*			m_BloodBar;

	Sprite*					m_CurProgress;
};


#endif