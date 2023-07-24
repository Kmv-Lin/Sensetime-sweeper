#include "com_mission.h"

#if 0
//QString mis_requestHeader =QString("http://10.8.96.6:8087");                //test
//QString mis_requestHeader =QString("http://192.168.8.209:8087");                     //real
QString mis_requestHeader =QString("http://192.168.8.100:8087");

QString mis_requestState = QString("/v1/token");

//QString mis_Headername = "https://robosweeper.senseauto.com:32461/api/missions/";
QString mis_Headername = "/api/missions/";

QString mis_Missionlist = QString("http://192.168.8.100:10058/api/missions");
//QString mis_Missionlist = QString("http://192.168.1.13:10058/api/missions");
QString mis_Issuetask = QString("/perform_now");
#else
QString mis_requestHeader =QString("http://192.168.1.13:9958");

QString mis_requestState = QString("/api");

//QString mis_Headername = "https://robosweeper.senseauto.com:32461/api/missions/";
QString mis_Headername = "/api/missions/";

QString mis_Missionlist = QString("http://192.168.1.13:10058/api/missions");

QString mis_Issuetask = QString("/perform_now");

#endif
