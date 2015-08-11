//
//  EnemyPlayController.cpp
//  PlaneGame2
//
//  Created by 叶飞 on 15/7/23.
//
//

#include "Layer/UIController.h"
#include "Layer/PhysicsLayer.h"
#include "../GameCommon/GameData.h"
#include "BasePlane/PlaneFactory.h"

void MatrixItemConfigMap::removeMatrixInitInfoFromVector(Vector<MatrixInitInfo*>* vector)
{
    for (Vector<MatrixInitInfo *>::iterator i = vector->begin(); i < vector->end(); i++) {
        this->matrix_init_info.eraseObject(*i);
    }
}

bool MatrixItemConfig::initData(rapidjson::Value &value) {
    this->set_itemId(value["itemId"].GetInt());
    this->set_position(Vec2(value["position"]["x"].GetDouble(), value["position"]["y"].GetInt()));
    return true;
};

bool MatrixInitInfo::initData(rapidjson::Value &value, std::string key) {
    this->set_init_size(value["height"].GetDouble());
    CCLOG("key is %s",key.c_str());
    this->set_matrix_id(key);
    return true;
};

bool WeaponInfoObj::initData(rapidjson::Value &value) {
    this->set_fireAngle(value["fireAngle"].GetDouble());
    this->set_fireSpeed(value["fireSpeed"].GetDouble());
    this->set_bulletNumber(value["bulletNumber"].GetInt());
    this->set_barrageFireGap(value["barrageFireGap"].GetDouble());
    this->set_fireDuration(value["fireDuration"].GetDouble());
    this->set_bulletMinGap(value["bulletMinGap"].GetDouble());
    this->set_bulletGapAmplitude(value["bulletGapAmplitude"].GetDouble());
    this->set_bulletGap(value["bulletGap"].GetDouble());
    this->set_distanceToWeaponX(value["distanceToWeaponX"].GetDouble());
    this->set_distanceToWeaponY(value["distanceToWeaponY"].GetDouble());
    this->set_fireChangedAngle(value["fireChangedAngle"].GetDouble());
    this->set_fireMaxAngle(value["fireMaxAngle"].GetDouble());
    this->set_resIndex(value["resIndex"].GetInt());
    this->set_scaleX(value["scaleX"].GetDouble());
    this->set_scaleY(value["scaleY"].GetDouble());
    return true;
};


WeaponInfoObjList::~WeaponInfoObjList() {
    this->weapon_vector_.clear();
}

bool TrajectoryConfigData::initData(rapidjson::Value &value) {
    rapidjson::Value &pointList = value["pointList"];
    if (pointList.IsNull() || !pointList.IsArray()) {
        CCLOG("pointList err %s", "");
        return false;
    }

    rapidjson::Value &eventList = value["eventList"];
    if (pointList.IsNull() || !pointList.IsArray()) {
        CCLOG("eventList err %s", "");
        return false;
    }

    std::vector<Vec2> p_list;

    for (unsigned int i = 0; i < pointList.Size(); i++) {
        float x = pointList[i]["x"].GetDouble();
        float y = pointList[i]["y"].GetDouble();
        p_list.push_back(Vec2(x, y));
    }
    this->set_point_list(p_list);
    this->set_time(value["time"].GetDouble());
    this->set_stayTime(value["stayTime"].GetDouble());
    this->set_isLoop(value["isLoop"].GetBool());

    Vector<EventInfoObj *> e_list;
    for (unsigned int i = 0; i < eventList.Size(); i++) {
        EventInfoObj *event = EventInfoObj::create();
        rapidjson::Value &e = eventList[i];
        if (e["eventName"].IsNull() || e["eventTime"].IsNull()) {
            return false;
        }
        event->set_event_time(e["eventTime"].GetDouble());
        event->set_func_str(e["eventName"].GetString());
        e_list.pushBack(event);
    }
    this->set_event_list(e_list);
    return true;
};

TrajectoryConfigData::~TrajectoryConfigData() {
    this->point_list_.clear();
    this->event_list_.clear();
}

bool LevelPlaneConfigData::initData(rapidjson::Value &value) {
    this->set_attack(value["attack"].GetDouble());
    this->set_barrageId(value["barrageId"].GetString());
    this->set_barrageInterval(value["barrageInterval"].GetDouble());
    this->set_dropArr(value["dropArr"].GetString());
    this->set_health(value["health"].GetInt());
    this->set_name(value["name"].GetInt());
    this->set_score(value["score"].GetInt());
    this->set_speed(value["speed"].GetDouble());
    this->set_startPoint(Vec2(value["startPoint"]["x"].GetInt(), value["startPoint"]["y"].GetInt()));
    this->set_trajectoryID(value["trajectoryID"].GetString());
    return true;
}

TrajectoryConfigDataList::~TrajectoryConfigDataList() {
    this->tre_vector_.clear();
}

bool LevelPlaneConfigDataList::initData(rapidjson::Value &value) {

    rapidjson::Value &plane_list = value["plane_list"];
    if (plane_list.IsNull() || !plane_list.IsArray()) {
        CCLOG("plane_list is err ==================> %s ", "");
        return false;
    }

//    this->directNextBatch = value["directNextBatch"].GetInt();
    for (unsigned int i = 0; i < plane_list.Size(); i++) {
        LevelPlaneConfigData *oneData = LevelPlaneConfigData::create();
        rapidjson::Value &planeValue = plane_list[i];
        if (!oneData->initData(planeValue)) {
            return false;
        }
        this->config_vector.pushBack(oneData);
    }
    return true;
}

LevelPlaneConfigDataList::~LevelPlaneConfigDataList() {
    this->config_vector.clear();
}


static EnemyPlaneController *controller_singleInstance = nullptr;

EnemyPlaneController *EnemyPlaneController::getInstance() {
    if (!controller_singleInstance) {
        controller_singleInstance = new(std::nothrow) EnemyPlaneController();
    }
    return controller_singleInstance;
}

void EnemyPlaneController::destroyInstance() {
    CC_SAFE_DELETE(controller_singleInstance);
}

EnemyPlaneController::EnemyPlaneController() {
    this->waitTime = -1;
    this->fly_size = 0;
    this->start_wait_time = 6;
    this->matrix_fly_speed = 0;
    this->game_state = this->GAME_STATE_INIT;
    this->parserTrajectoryFile();
    this->parserWeaponFile();
}

EnemyPlaneController::~EnemyPlaneController() {
    this->endLoop();
    this->endMatrixLoop();
    this->level_plane_config_vector.clear();
    this->send_plane_vector.clear();
    this->trajectory_map.clear();
    this->matrix_map.clear();
    this->remove_vector.clear();
    this->dropgold_recover_vector.clear();
    this->dropgold_vector.clear();
}

bool EnemyPlaneController::startLevelWithConfigFile(int levelNum, bool isFirst) {
    if (!this->parserMatrixItemDropFile(levelNum, isFirst)) {
        return false;
    }
    auto fileUtils = cocos2d::FileUtils::getInstance();
    std::string filePath = "level_config/1.json.enemy";
//    auto thePath =  filePath;
    if (!fileUtils->isFileExist(filePath)) {
        CCLOG("cant nont find level config file ==================> %s ", filePath.c_str());
        return false;
    }

    rapidjson::Document d;
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(filePath);

    d.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

    if (d.HasParseError()) {
        CCLOG("the level config file erro \nGetParseError %s", d.GetParseError());
        return false;
    }

    rapidjson::Value &list = d["list"];
    if (list.IsNull() || !list.IsArray()) {
        CCLOG("the level config data is erro %s", "");
        return false;
    }
    for (unsigned int i = 0; i < list.Size(); i++) {
        rapidjson::Value &onePosData = list[i];
        LevelPlaneConfigDataList *data = LevelPlaneConfigDataList::create();
        if (!data->initData(onePosData)) {
            this->level_plane_config_vector.clear();
            return false;
        }
        this->level_plane_config_vector.pushBack(data);
    }
    this->start_wait_time = 6;
    this->config_index = this->level_plane_config_vector.begin();
    this->_scheduler = Director::getInstance()->getScheduler();
    this->_scheduler->schedule(CC_SCHEDULE_SELECTOR(EnemyPlaneController::updateUserFlySize), this, 0.1, false);
    this->game_state = this->GAME_STATE_PLAYING;

    return true;
}

bool EnemyPlaneController::parserWeaponFile() {
    auto fileUtils = cocos2d::FileUtils::getInstance();
    std::string thePath = "level_config/1.json.barrage";
    if (!fileUtils->isFileExist(thePath)) {
        CCLOG("cant nont find weapon file ==================> %s ", thePath.c_str());
        return false;
    }

    rapidjson::Document d;
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(thePath);

    d.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

    if (d.HasParseError()) {
        CCLOG("the level weapon file erro \nGetParseError %s", d.GetParseError());
        return false;
    }

    for (auto iter = d.MemberonBegin(); iter != d.MemberonEnd(); ++iter) {
        auto key = (iter->name).GetString();

        WeaponInfoObjList *weaponList = WeaponInfoObjList::create();
        Vector<WeaponInfoObj *> temp;
        for (unsigned int i = 0; i < d[key].Size(); i++) {
            WeaponInfoObj *data = WeaponInfoObj::create();
            rapidjson::Value &value = d[key][i];
            if (!data->initData(value)) {
                return false;
            }
            temp.pushBack(data);
        }
        weaponList->set_weapon_vector(temp);
        this->weapon_map.insert(key, weaponList);
    }
    return true;
}

bool EnemyPlaneController::parserTrajectoryFile() {
    auto fileUtils = cocos2d::FileUtils::getInstance();
    std::string thePath = "level_config/1.json.track";
    if (!fileUtils->isFileExist(thePath)) {
        CCLOG("cant nont find Trahectory file ==================> %s ", thePath.c_str());
        return false;
    }

    rapidjson::Document d;
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(thePath);

    d.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

    if (d.HasParseError()) {
        CCLOG("the level Trahectory file erro \nGetParseError %s", d.GetParseError());
        return false;
    }

    for (auto iter = d.MemberonBegin(); iter != d.MemberonEnd(); ++iter) {
        auto key = (iter->name).GetString();
        TrajectoryConfigDataList *list = TrajectoryConfigDataList::create();
        Vector<TrajectoryConfigData *> temp;
        for (unsigned int i = 0; i < d[key].Size(); i++) {
            TrajectoryConfigData *data = TrajectoryConfigData::create();
            rapidjson::Value &value = d[key][i];
            if (!data->initData(value)) {
                return false;
            }
            temp.pushBack(data);
        }
        list->set_tre_vector(temp);
        this->trajectory_map.insert(key, list);
    }
    return true;
}

TrajectoryConfigDataList *EnemyPlaneController::getTrajectoryConfigDataFromId(std::string id) {
    TrajectoryConfigDataList *trajectoryList = nullptr;
    if (trajectory_map.find(id) != this->trajectory_map.end()) {
        trajectoryList = this->trajectory_map.at(id);
    }
    return trajectoryList;
}

WeaponInfoObjList *EnemyPlaneController::getWeaponConfigDataFromId(std::string id) {
    WeaponInfoObjList *weapon_list = nullptr;
    if (this->weapon_map.find(id) != this->weapon_map.end()) {
        weapon_list = this->weapon_map.at(id);
    }
    return weapon_list;
}

MatrixItemConfigMap *EnemyPlaneController::getMatrixConfigListFromId(std::string id) {
    MatrixItemConfigMap *matrix = nullptr;
    if (this->matrix_map.find(id) != this->matrix_map.end()) {
        matrix = this->matrix_map.at(id);
    }
    return matrix;
}

void EnemyPlaneController::exitGame() {
    this->endLoop();
}

void EnemyPlaneController::pauseLoop() {
    this->_scheduler->pauseTarget(this);
}

void EnemyPlaneController::resumeLoop() {
    this->_scheduler->resumeTarget(this);
}


void EnemyPlaneController::endLoop() {
    if (this->game_state != this->GAME_STATE_DESTROY) {
        this->game_state = this->GAME_STATE_DESTROY;
        this->_scheduler->unschedule(CC_SCHEDULE_SELECTOR(EnemyPlaneController::updateUserFlySize), this);
    }
}

void EnemyPlaneController::endMatrixLoop() {
    this->_scheduler->unschedule(CC_SCHEDULE_SELECTOR(EnemyPlaneController::updateSizeInMatrix), this);
}

void EnemyPlaneController::destroyCallBack(BasePlane *basePlane) {
    CCASSERT(basePlane, "basePlane be non-nullptr");
    this->send_plane_vector.eraseObject(basePlane);

    if (this->send_plane_vector.size() == 0) {
        this->resumeSend();
    }
}

void EnemyPlaneController::pauseSend() {
    this->game_state = this->GAME_STATE_PAUSE;
    this->pauseLoop();
}

void EnemyPlaneController::resumeSend() {
    this->game_state = this->GAME_STATE_PLAYING;
    this->resumeLoop();
}

void EnemyPlaneController::waitSend(float time) {
    CCASSERT(time, "wait time be non-nullptr");
    if (time >= 0) {
        this->waitTime = time;
    }
}

std::string EnemyPlaneController::getMatrixItemDropConfigFileName(int levelNum) {
    return "level_config/1.json.item";
}

void EnemyPlaneController::startMatrixItemDropLevel(std::string id) {
    this->matrix_item_data = this->getMatrixConfigListFromId(id);
    this->matrix_fly_speed = this->matrix_item_data->move_speed();
    this->_scheduler->schedule(CC_SCHEDULE_SELECTOR(EnemyPlaneController::updateSizeInMatrix), this, 0.2, false);
}

void EnemyPlaneController::updateSizeInMatrix(float dt) {

    if(this->dropgold_vector.size() == 0 && this->matrix_item_data->matrix_init_info.size() == 0) {
        this->endMatrixLoop();
        GameData::getInstance()->GameOver(true);
        return;
    }

    this->fly_size += this->matrix_item_data->move_speed() * 0.2;
//    CCLOG("size is ========================> %d",this->matrix_item_data->matrix_init_info().size());
    for (Vector<MatrixInitInfo *>::iterator it = this->matrix_item_data->matrix_init_info.begin(); it < this->matrix_item_data->matrix_init_info.end(); it++) {
        MatrixInitInfo *info = *it;
        if (this->fly_size >= info->init_size()) {
            this->remove_vector.pushBack(info);
            this->createMatrixItemGroup(info->matrix_id());
        }
    }


    if (this->remove_vector.size() > 0) {
        this->matrix_item_data->removeMatrixInitInfoFromVector(&this->remove_vector);
        this->remove_vector.clear();
    }
//    CCLOG("after size is =====================> %d",this->matrix_item_data->matrix_init_info().size());
}

void EnemyPlaneController::createMatrixItemGroup(std::string groupId) {
    MatrixItemConfigList *list = nullptr;
    if (this->matrix_item_data->group_map.find(groupId) != this->matrix_item_data->group_map.end()) {
        list = this->matrix_item_data->group_map.at(groupId);
    }
    if (list != nullptr) {
        CCLOG("list size is =====================> %d",list->matrix_config_list.size());
//        for (Vector<MatrixItemConfig*>::iterator i = list->matrix_config_list().begin(); i < list->matrix_config_list().end(); i++) {
        for(Vector<MatrixItemConfig*>::iterator i = list->matrix_config_list.begin();i<list->matrix_config_list.end();i++) {
            MatrixItemConfig *config = *i;
//            CCLOG("create the item %d",config->itemId());
            this->createItemByItemId(config->itemId(), config->position());
        }
    }
}

void EnemyPlaneController::dropDestroyCallBack(DropGold *item)
{
    if(this->dropgold_vector.size() > 0) {
        this->dropgold_vector.eraseObject(item);
    }
//    this->dropgold_recover_vector.pushBack(item);
}

void EnemyPlaneController::createItemByItemId(int itemId, Vec2 pos) {
    CCLOG("create item id %d", itemId);
    Vec2 position = Vec2(pos.x,pos.y);
    DropGold* testGrop = DropGold::create(position,
            ResTag::Drops_Golds1, 10, 100);
    this->dropgold_vector.pushBack(testGrop);
    UIController::getInstance()->getPhysicsLayer()->addChild(testGrop, -1000);
}

bool EnemyPlaneController::parserMatrixItemDropFile(int curLevel, bool isFirst) {
    this->have_matrix_item_drop_level = isFirst;
    if (this->needMatrixItemLevel()) {
        auto fileUtils = cocos2d::FileUtils::getInstance();
        std::string filePath = this->getMatrixItemDropConfigFileName(curLevel);
        if (!fileUtils->isFileExist(filePath)) {
            CCLOG("cant nont find MatrixItemD file ==================> %s ", filePath.c_str());
            return false;
        }
        rapidjson::Document d;
        std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(filePath);
        d.Parse<rapidjson::kParseDefaultFlags>(str.c_str());
        if (d.HasParseError()) {
            CCLOG("the MatrixItemD file erro \nGetParseError %s", d.GetParseError());
            return false;
        }
        for (auto iter = d.MemberonBegin(); iter != d.MemberonEnd(); ++iter) {
            auto key = (iter->name).GetString();
            MatrixItemConfigMap *map = MatrixItemConfigMap::create();
            rapidjson::Value &matrix_data = d[key];
            map->set_move_speed(matrix_data["moveSpeed"].GetDouble());
            rapidjson::Value &size_data = matrix_data["move_height"];
            rapidjson::Value &item_data = matrix_data["point_obj"];
//            Vector<MatrixInitInfo *> info_temp_vector;
            for (auto i = size_data.MemberonBegin(); i != size_data.MemberonEnd(); i++) {
                auto size_data_key = (i->name).GetString();
                rapidjson::Value &one_size_data = size_data[size_data_key];
                MatrixInitInfo *info = MatrixInitInfo::create();
                if (!info->initData(one_size_data, size_data_key)) {
                    return false;
                }
                map->matrix_init_info.pushBack(info);
            }

//            map->set_matrix_init_info(info_temp_vector);
//            list->matrix_config_list(info_temp_vector);
            for (auto j = item_data.MemberonBegin(); j != item_data.MemberonEnd(); j++) {
                MatrixItemConfigList *temp_list = MatrixItemConfigList::create();
                auto item_data_key = (j->name).GetString();
                rapidjson::Value &one_item_data = item_data[item_data_key];
//                Vector<MatrixItemConfig*> temp;
                for (unsigned int k = 0; k < one_item_data.Size(); k++) {
                    rapidjson::Value &oneValue = one_item_data[k];
                    MatrixItemConfig* config = MatrixItemConfig::create();
                    if (!config->initData(oneValue)) {
                        return false;
                    }
                    temp_list->matrix_config_list.pushBack(config);
//                    temp.pushBack(config);
                }
//                temp_list->set_matrix_config_list(temp);


                map->group_map.insert(item_data_key, temp_list);
            }
            this->matrix_map.insert(key, map);
        }
    }
    return true;
}

bool EnemyPlaneController::needMatrixItemLevel() {
    return this->have_matrix_item_drop_level;
}

void EnemyPlaneController::sendPlane() {
    if (this->config_index >= this->level_plane_config_vector.end()) {
//        CCLOG("leve config is end ===========================> %s", "");
        this->endLoop();

        if (!this->needMatrixItemLevel()) {
            GameData::getInstance()->GameOver(true);
        } else {
            this->startMatrixItemDropLevel("101");
        }
        return;
    }
    LevelPlaneConfigDataList *data = *this->config_index;
    Vector<LevelPlaneConfigData *> temp = data->config_vector;
	//for (Vector<LevelPlaneConfigData *>::iterator it = temp.begin(); it < temp.end(); it++) 
	{
		Vector<LevelPlaneConfigData *>::iterator it = temp.begin();
        LevelPlaneConfigData *config = *it;
        GameData* gameData = GameData::getInstance();
        auto basePlane = gameData->getPlaneFactory()->createEnemyPlane(config);
        basePlane->setPosition(config->startPoint());

//
//        EnemyPlane* normalPlane = dynamic_cast<EnemyPlane*>(basePlane);
//        if (normalPlane)
//        {
//            norPl->setWeaponPro(WeaponPro( weaponId, nullptr, userdata ));
//        }
//
//        gd->getPlaneAction()->AddEnemyActionByIdx(actionId,pl);
//        gd->getPlaneAiFactory()->AddEnemyAiByIdx(pl,AIid);
        UIController::getInstance()->getPhysicsLayer()->addChild(basePlane, -1000);
//        this->send_plane_vector.pushBack(testEnePl);
        this->send_plane_vector.pushBack(basePlane);
    }
    this->game_state = this->GAME_STATE_FIGHTING;
    this->pauseSend();
    this->config_index++;
}

void EnemyPlaneController::updateUserFlySize(float dt) {
    if (this->game_state != this->GAME_STATE_PLAYING) {
        return;
    }

    if (this->start_wait_time >= 0) {
        this->start_wait_time -= dt;
        return;
    }

    if (this->waitTime >= 0) {
        this->waitTime -= dt;
        return;
    }

    this->sendPlane();
}
