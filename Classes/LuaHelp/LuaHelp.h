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
	FormulaType_RoleAtk,			//攻击公式
	FormulaType_RoleWeaponNum,		//武器数公式	
	FormulaType_RoleDefense,		//防御公式
	FormulaType_RoleLuck,			//幸运公式
	FormulaType_RoleExp,			//经验公式
	FormulaType_RoleHp,				//血量公式

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