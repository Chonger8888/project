#include "LuaHelp.h"
#include "CCLuaEngine.h"


USING_NS_CC;

const char* LuaHelp::getFileFullPath( const char *fileName )
{
	std::string str =  StringUtils::format("%s",fileName);
	std::string path = FileUtils::getInstance()->fullPathForFilename(str);
	return path.c_str();
}

int LuaHelp::callLuaFormulaWithType( FormulaType type )
{
	lua_State* ls = LuaEngine::getInstance()->getLuaStack()->getLuaState();
	int isOpen = luaL_dofile(ls, "luaTest.lua");

	if (isOpen) {
		log("Open Lua Error:%i",isOpen);
		return 0;
         
	}

	switch ( type )
	{
	case FormulaType_RoleAtk:
		lua_getglobal(ls, "RoleAtkFormula");
		lua_pushnumber(ls, 1.5);
		lua_pushnumber(ls, 4);
		lua_pushnumber(ls, 3);
		lua_pushnumber(ls, 1);
		lua_pushnumber(ls, 2);
		lua_call(ls, 5, 1);//参数数量和返回值数量	
		break;

	case FormulaType_RoleWeaponNum:
		lua_getglobal(ls, "RoleWeaponNumFormula");
		lua_pushnumber(ls, 1.5);
		lua_pushnumber(ls, 4);
		lua_pushnumber(ls, 3);
		lua_call(ls, 3, 1);//参数数量和返回值数量	
		break;

	case FormulaType_RoleDefense:
		lua_getglobal(ls, "RoleDefenseFormula");
		lua_pushnumber(ls, 1.5);
		lua_pushnumber(ls, 4);
		lua_pushnumber(ls, 3);
		lua_call(ls, 3, 1);//参数数量和返回值数量	
		break;

	case FormulaType_RoleLuck:
		lua_getglobal(ls, "RoleLuckFormula");
		lua_pushnumber(ls, 1.5);
		lua_pushnumber(ls, 4);
		lua_call(ls, 2, 1);//参数数量和返回值数量	
		break;

	case FormulaType_RoleExp:
		lua_getglobal(ls, "RoleExpFormula");
		lua_pushnumber(ls, 1.5);
		lua_pushnumber(ls, 4);
		lua_pushnumber(ls, 3);
		lua_pushnumber(ls, 1);
		lua_pushnumber(ls, 2);
		lua_call(ls, 5, 1);//参数数量和返回值数量	
		break;

	case FormulaType_RoleHp:
		lua_getglobal(ls, "RoleHpFormula");
		lua_pushnumber(ls, 1.5);
		lua_pushnumber(ls, 4);
		lua_pushnumber(ls, 3);
		lua_pushnumber(ls, 1);
		lua_pushnumber(ls, 2);
		lua_call(ls, 5, 1);//参数数量和返回值数量	
		break;

	default:
		break;
	}
     

	float iResult = lua_tonumber(ls, -1);
	return iResult;
}
