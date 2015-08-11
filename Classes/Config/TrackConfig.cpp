#include "TrackConfig.h"
#include "GameCommon\GameData.h"
#include "GamePlaying\AI.h"

bool TimeCompare( EventInfoObj* ep1, EventInfoObj* ep2 )
{
	if (ep1->event_time() > ep2->event_time())
	{
		return true;
	}
	return false;
}

TrackManager* TrackManager::create(BasePlane* plane, const std::string& trackId)
{
	TrackManager* tc = new (std::nothrow) TrackManager();
	if (tc)
	{
		tc->retain();
		tc->setTarget(plane);
		tc->getTrackProVec(trackId);
		tc->autorelease();
		return tc;
	}
	CC_SAFE_DELETE(tc);
	return nullptr;
}

TrackManager::TrackManager()
{
    this->haveLoop = false;
}

void TrackManager::purge()
{
	release();
}

void TrackManager::setTarget( BasePlane* plane )
{
	m_target = plane;
}

void TrackManager::getTrackProVec( const std::string& id )
{
	auto dataObj = EnemyPlaneController::getInstance()->getTrajectoryConfigDataFromId(id);
	if (dataObj)
	{
		m_TrackProVec = dataObj->tre_vector();
		m_TrackProVecIte = m_TrackProVec.begin();
	}
}

void TrackManager::startAction()
{
	OnceTrackAction();
}

ActionInterval* TrackManager::getBezierActions( std::vector<Vec2>& PosVec, Vector<EventInfoObj*> eventSet, float mvTime, float stayTime )
{
	ccBezierConfig config;
	config.controlPoint_1 = PosVec.at(0);
	config.controlPoint_2 = PosVec.at(1);
	config.endPosition = PosVec.at(2);
	
	BezierBy* bezier = BezierBy::create(mvTime, config);

	EventInfoObj* ep = nullptr;
	const char* str = "";
	float shootTime = 0.0f;

	ActionInterval* tempAction = nullptr;
	ActionInterval* spw = nullptr;

	//运动过程中发射子弹
	sort(eventSet.begin(), eventSet.end(), TimeCompare);	//按时间先后排序

	for (int i = 0; i < eventSet.size(); i++)
	{
		ep = eventSet.at(i);
		shootTime = ep->event_time();

		//int DanMuId = atoi(ep->func_str().c_str());
		int DanMuId = 0;
		void* data = (void*)DanMuId;
		auto wait = DelayTime::create(shootTime);
		auto func = __CCCallFuncND::create(this, callfuncND_selector(TrackManager::ActionFuncND), data);
		auto seq = Sequence::create(wait, func, nullptr);
		if (tempAction)
		{
			spw = Spawn::createWithTwoActions(tempAction, seq);
		}
		else
		{
			spw = seq;
		}
		tempAction = spw;
	}
	auto stay = DelayTime::create(stayTime);

	if (spw)
	{
		spw = Spawn::createWithTwoActions(spw, bezier);
		auto onceAction = Sequence::create(spw, stay, nullptr); 
		return onceAction;
	}
	else
	{
		auto onceAction = Sequence::create(bezier, stay, nullptr); 
		return onceAction;
	}
	
}

void TrackManager::OnceTrackAction()
{
	if (m_TrackProVec.empty())
	{
		return;
	}
	if (m_TrackProVecIte != m_TrackProVec.end())
	{
		TrajectoryConfigData* tp = (*m_TrackProVecIte);

        if(this->haveLoop) {
            this->m_LoopVector.pushBack(tp);
        }
        if(tp->isLoop()) {
            this->haveLoop = true;
        }
        this->runTrackAction(tp, CC_CALLBACK_0(TrackManager::OnceTrackAction, this));
		m_TrackProVecIte++;
	}
	else
	{
		if(this->m_LoopVector.size() > 0) {
            this->m_LoopVectorIt = this->m_LoopVector.begin();
            LoopTrackAction();
        }
	}
}

void TrackManager::LoopTrackAction()
{
    if(this->m_LoopVectorIt >= this->m_LoopVector.end()) {
        this->m_LoopVectorIt = this->m_LoopVector.begin();
    }
    this->runTrackAction(*this->m_LoopVectorIt, CC_CALLBACK_0(TrackManager::LoopTrackAction, this));
    this->m_LoopVectorIt++;
}

void TrackManager::runTrackAction(TrajectoryConfigData* data,const std::function<void()> &func)
{
    std::vector<Vec2> posVec = data->point_list();
    Vector<EventInfoObj*> eventVec = data->event_list();
    float stayTime = data->stayTime();
    auto mvShoot = getBezierActions(posVec, eventVec, data->time(), stayTime);
    auto next = CallFunc::create(func);
    auto seq = Sequence::create(mvShoot, next, nullptr);
    this->m_target->runAction(seq);
}

void TrackManager::ActionFuncND( Node* node, void* data )
{
	m_target->StartFire();
}




//
////-------------------------------
//void TrackConfig::analyseConfig()
//{
//	if (!m_isRead)
//	{
//		std::string file_path = FileUtils::getInstance()->fullPathForFilename("testJson.txt").c_str();
//		log("external file path = %s",file_path);
//
//		rapidjson::Document doc;
//		std::string contentStr = FileUtils::getInstance()->getStringFromFile(file_path);
//		doc.Parse<0>(contentStr.c_str());
//
//		//打印JSon文件的内容
//		log("%s\n",contentStr.c_str());
//
//		AnalyseTrackPro(doc);
//		m_isRead = true;
//	}
//}
//
//std::vector<Vec2> TrackConfig::PosSetOfPartTarck( const rapidjson::Value& val )
//{
//	//点集
//	std::vector<Vec2> PosVec;
//	const rapidjson::Value& valueArr = val["valueArr"];
//	if (valueArr.IsArray())
//	{
//		int num = valueArr.Size();
//		for (int i = 0; i < valueArr.Size(); i++)
//		{
//			const rapidjson::Value& posSet = valueArr[i];
//			const rapidjson::Value& startPoint = posSet["startPoint"];
//			const rapidjson::Value& controlPoint1 = posSet["controlPoint1"];
//			const rapidjson::Value& controlPoint2 = posSet["controlPoint2"];
//			const rapidjson::Value& endPoint2 = posSet["endPoint2"];
//			Vec2 startPos = Vec2(startPoint["x"].GetDouble(), startPoint["y"].GetDouble());
//			Vec2 pos1 = Vec2(controlPoint1["x"].GetDouble(), controlPoint1["y"].GetDouble());
//			Vec2 pos2 = Vec2(controlPoint2["x"].GetDouble(), controlPoint2["y"].GetDouble());
//			Vec2 endpos = Vec2(endPoint2["x"].GetDouble(), endPoint2["y"].GetDouble());
//
//			PosVec.push_back(startPos);
//			PosVec.push_back(pos1);
//			PosVec.push_back(pos2);
//			PosVec.push_back(endpos);
//		}
//	}
//	return PosVec;
//}
//
//std::vector<EventPro> TrackConfig::EventSetOfPartTarck( const rapidjson::Value& val )
//{
//	//事件集
//	std::vector<EventPro> EventVec;
//	const rapidjson::Value& eventInfo = val["eventInfo"];
//	if (eventInfo.IsArray())
//	{
//		for (int i = 0; i < eventInfo.Size(); i++)
//		{
//			const rapidjson::Value& info = eventInfo[i];
//
//			EventPro ep;
//			ep.time = info["eventTime"].GetDouble();
//			ep.eventName = info["eventName"].GetString();
//			EventVec.push_back(ep);
//		}
//
//	}
//	return EventVec;
//}
//
//void TrackConfig::AnalyseTrackPro( const rapidjson::Document& doc )
//{
//	//轨迹数量
//	int trackNum = doc["count"].GetInt();
//
//	//所有轨迹的数组
//	const rapidjson::Value& Trajectory = doc["Trajectory"];
//	if (Trajectory.IsArray())
//	{
//		log("");
//	}
//	for (int i = 0; i < Trajectory.Size(); i++)
//	{
//		const rapidjson::Value& TrajectoryPro = Trajectory[i];
//
//		//一条轨迹属性
//		oneTrack one_track;
//
//		const rapidjson::Value& linkNode = TrajectoryPro["linkNode"];
//		if (linkNode.IsArray())
//		{
//			for (int i = 0; i < linkNode.Size(); i++)
//			{
//				const rapidjson::Value& linkNodePro = linkNode[i];
//
//				TrackPro track_pro;
//				track_pro.bezierPosVec = PosSetOfPartTarck(linkNodePro);
//				track_pro.eventVec = EventSetOfPartTarck(linkNodePro);
//				track_pro.isLoop = linkNodePro["isLoop"].GetBool();
//				track_pro.stayTime = linkNodePro["stayTime"].GetDouble();
//				one_track.push_back(track_pro);			//存储一段轨迹
//			}
//		}
//		CompleteTrack completeTrack;
//		completeTrack.track = one_track;
//		completeTrack.id = TrajectoryPro["id"].GetInt();	//轨迹ID
//
//		m_AllTrack.push_back(completeTrack);				//存储一条完整的轨迹
//	}
//}
//
//TrackPro TrackConfig::getTrackPro( const std::vector<Vec2>& pointVec, std::vector<EventPro> eventVec, bool isLoop, float stayTime )
//{
//	TrackPro track_pro;
//	track_pro.bezierPosVec = pointVec;
//	track_pro.eventVec = eventVec;
//	track_pro.isLoop = isLoop;
//	track_pro.stayTime = stayTime;
//
//	return track_pro;
//}
//
//oneTrack TrackConfig::getTrackProVecById( int id )
//{
//	analyseConfig();
//	oneTrack one_track;
//	bool isFind = false;
//	//通过轨迹ID，知道对应的轨迹
//	for (int i = 0; i < m_AllTrack.size(); i++)
//	{
//		CompleteTrack ct = m_AllTrack.at(i);
//		if (ct.id == id)
//		{
//			isFind = true;
//			one_track = ct.track;
//			break;
//		}
//	}
//	if (!isFind)
//	{
//		CCASSERT(isFind, "track id [ %d ] is not found", id);
//	}
//	
//	return one_track;
//}
//
