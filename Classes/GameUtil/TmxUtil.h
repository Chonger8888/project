/********************************************************************
������:		2015/02/05  11:55	
�ļ���: 	E:\Cocos3.3_Project\PlaneGame\Classes\Util\TmxUtil.h 
��  ��:		alex(alex)
�޸��ڣ�	2015/02/05  11:55	

Ŀ  ��:		Tilemap ��ȡ�л���Ϲ���
*********************************************************************/
#ifndef __TmxUtil_H__
#define __TmxUtil_H__


#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class TmxUtil
{
public:
	static std::vector<std::string> SplitToStr(const std::string &s, char delim);
	static std::vector<int> SplitToInt(const std::string &s, char delim);
	static std::vector<float> SplitToFloat(const std::string &s, char delim);

	static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

	//����TMX�ġ����ơ��У����������ݣ�int��float
	static int getIntDataByRate(const std::string& str1, const std::string& str2);
	static float getFloatDataByRate(const std::string& str1, const std::string& str2);

	//�õ�ÿ�����Ͷ�Ӧ�ĸ���
	static int getRateIndex(std::vector<float> vec);

	//��#�ָ���ַ�������
	static std::vector<std::string> getStrVec( const std::string& str);


};

#endif // __TmxUtil_H__