#ifndef __LUA_HELP_H__
#define __LUA_HELP_H__

#include "cocos2d.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif
enum FormulaType
{
	FormulaType_RoleAtk,			//������ʽ
	FormulaType_RoleWeaponNum,		//��������ʽ	
	FormulaType_RoleDefense,		//������ʽ
	FormulaType_RoleLuck,			//���˹�ʽ
	FormulaType_RoleExp,			//���鹫ʽ
	FormulaType_RoleHp,				//Ѫ����ʽ

};

class LuaHelp
{
public:

	static LuaHelp* getInstance()
	{
		static LuaHelp instance;
		return &instance;
	}

	int callLuaFormulaWithType( FormulaType type );

private:
	const char* getFileFullPath(const char *fileName);
};



#endif