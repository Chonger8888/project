//
//  EnemyPlayController.h
//  PlaneGame2
//
//  Created by 叶飞 on 15/7/23.
//
//

#ifndef __PlaneGame2__EnemyPlayController__
#define __PlaneGame2__EnemyPlayController__

#include "cocostudio/CocoStudio.h"
#include "BasePlane/BasePlane.h"
#include "DropGoods.h"

USING_NS_CC;

#define SG_M(TYPE, M)\
public:\
void set_##M( const TYPE& in_##M ) {\
M##_ = in_##M;\
}\
TYPE M() {\
return M##_;\
}\
private:\
TYPE M##_;

#define MY_CREATE_FUNC(__TYPE__) \
static __TYPE__* create() \
{ \
__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
if (pRet) \
{ \
pRet->autorelease(); \
return pRet; \
} \
else \
{ \
delete pRet; \
pRet = NULL; \
return NULL; \
} \
}

class MatrixItemConfig : public cocos2d::Ref {
public:
    MY_CREATE_FUNC(MatrixItemConfig);
    bool initData(rapidjson::Value& value);
    SG_M(int, itemId);
    SG_M(Vec2, position);
};

class MatrixInitInfo : public cocos2d::Ref
{
public:
    MY_CREATE_FUNC(MatrixInitInfo);
    bool initData(rapidjson::Value& value,std::string key);
    SG_M(float, init_size);
    SG_M(std::string, matrix_id);
};

class MatrixItemConfigList : public cocos2d::Ref
{
    ~MatrixItemConfigList()
    {
//        this->matrix_config_list_.clear();
        this->matrix_config_list.clear();
    }
public:
    MY_CREATE_FUNC(MatrixItemConfigList);
    Vector<MatrixItemConfig*> matrix_config_list;
//    SG_M(Vector<MatrixItemConfig*>,matrix_config_list);
};

class MatrixItemConfigMap : public cocos2d::Ref
{
    ~MatrixItemConfigMap()
    {
        this->group_map.clear();
        this->matrix_init_info.clear();
    }
public:
    MY_CREATE_FUNC(MatrixItemConfigMap);
    Map<std::string,MatrixItemConfigList*> group_map;
    void removeMatrixInitInfoFromVector(Vector<MatrixInitInfo*>* vector);
    Vector<MatrixInitInfo*> matrix_init_info;
//    SG_M(Vector<MatrixInitInfo*>,matrix_init_info);
    SG_M(float, move_speed);
};

class WeaponInfoObj : public cocos2d::Ref {
public:
    MY_CREATE_FUNC(WeaponInfoObj);
    bool initData(rapidjson::Value& value);
    //发射角度
    SG_M(float, fireAngle);
    //子弹飞行速度
    SG_M(float, fireSpeed);
    SG_M(int, bulletNumber);
    //开火间隔 
    SG_M(float, barrageFireGap);
    //开火持续时间 
    SG_M(float, fireDuration);
    //子弹最小间距 
    SG_M(float, bulletMinGap);
    //开火一次移动距离 
    SG_M(float, bulletGapAmplitude);
    //初始距离
    SG_M(float, bulletGap);
    //X距离
    SG_M(float, distanceToWeaponX);
    //Y距离
    SG_M(float, distanceToWeaponY);

    SG_M(float, fireChangedAngle);
    SG_M(float, fireMaxAngle);
    SG_M(int, resIndex);
    SG_M(float, scaleX);
    SG_M(float, scaleY);
};

class WeaponInfoObjList : public cocos2d::Ref
{
    ~WeaponInfoObjList();
public:
    MY_CREATE_FUNC(WeaponInfoObjList);
    SG_M(Vector<WeaponInfoObj*>,weapon_vector);
};

class EventInfoObj : public cocos2d::Ref
{
public:
    MY_CREATE_FUNC(EventInfoObj);
    SG_M(float, event_time);
    SG_M(std::string, func_str);
};

class TrajectoryConfigData : public cocos2d::Ref
{
    ~TrajectoryConfigData();
public:
    MY_CREATE_FUNC(TrajectoryConfigData);
    bool initData(rapidjson::Value& value);
    SG_M(std::vector<Vec2>, point_list);
    SG_M(float,time);
    SG_M(bool, isLoop);
    SG_M(float,stayTime);
    SG_M(Vector<EventInfoObj*>, event_list);
};

class TrajectoryConfigDataList : public cocos2d::Ref
{
    ~TrajectoryConfigDataList();
public:
    MY_CREATE_FUNC(TrajectoryConfigDataList);
    SG_M(Vector<TrajectoryConfigData*>,tre_vector);
};

class LevelPlaneConfigData : public cocos2d::Ref
{
public:
    MY_CREATE_FUNC(LevelPlaneConfigData);
    bool initData(rapidjson::Value& value);
    SG_M (float, attack);
    SG_M (std::string, barrageId);
    SG_M (float, barrageInterval);
    SG_M (std::string, dropArr);
    SG_M (int, health);
    SG_M (int, name);
    SG_M (int, score);
    SG_M (float, speed);
    SG_M (Vec2, startPoint);
    SG_M (std::string, trajectoryID);
};

class LevelPlaneConfigDataList : public cocos2d::Ref
{
    ~LevelPlaneConfigDataList();
public:
    MY_CREATE_FUNC(LevelPlaneConfigDataList);
    Vector<LevelPlaneConfigData*> config_vector;
    bool initData(rapidjson::Value& value);

protected:
//    int directNextBatch;
};

class EnemyPlaneController : public cocos2d::Ref
{
protected:
    static const unsigned int GAME_STATE_PLAYING = 1;
    static const unsigned int GAME_STATE_INIT = 2;
    static const unsigned int GAME_STATE_PAUSE = 3;
    static const unsigned int GAME_STATE_DESTROY = 4;
    static const unsigned int GAME_STATE_FIGHTING = 5;

    float start_wait_time;
    float waitTime;
    bool have_matrix_item_drop_level;
    float matrix_fly_speed;
    float fly_size;
    MatrixItemConfigMap* matrix_item_data;
//    float send_interval;
    Vector<LevelPlaneConfigDataList*> level_plane_config_vector;
    Vector<BasePlane*> send_plane_vector;
    Map<std::string,TrajectoryConfigDataList*> trajectory_map;
    Map<std::string,WeaponInfoObjList*> weapon_map;
    Map<std::string,MatrixItemConfigMap*> matrix_map;
    Vector<MatrixInitInfo*> remove_vector;
    Vector<DropGold*> dropgold_recover_vector;
    Vector<DropGold*> dropgold_vector;

    Scheduler* _scheduler;
    void pauseLoop();
    void resumeLoop();
    void endLoop();
    void endMatrixLoop();
    unsigned int game_state;
    bool parserTrajectoryFile();
    bool parserWeaponFile();
    std::string getMatrixItemDropConfigFileName(int levelNum);
    bool needMatrixItemLevel();
    Vector<LevelPlaneConfigDataList*>::iterator config_index;
    void startMatrixItemDropLevel(std::string id);
    void createMatrixItemGroup(std::string groupId);
    void createItemByItemId(int itemId,Vec2 position);
    void sendPlane();
    void updateUserFlySize(float dt);
    void updateSizeInMatrix(float dt);


public:
    EnemyPlaneController();
    ~EnemyPlaneController();
    static void destroyInstance();
    TrajectoryConfigDataList* getTrajectoryConfigDataFromId(std::string id);
    WeaponInfoObjList* getWeaponConfigDataFromId(std::string id);
    MatrixItemConfigMap* getMatrixConfigListFromId(std::string id);
    void pauseSend();
    void resumeSend();
    void exitGame();
    void waitSend(float time);
    static EnemyPlaneController* getInstance();
    bool startLevelWithConfigFile(int levelNum,bool isFirst = true);
    bool parserMatrixItemDropFile(int curLevel,bool isFirst = true);
    void destroyCallBack(BasePlane* basePlane);
    void dropDestroyCallBack(DropGold* item);
};

#endif /* defined(__PlaneGame2__EnemyPlayController__) */
