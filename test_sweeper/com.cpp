#include"com.h"
unsigned int  long long  sentcont=0;
unsigned int  long long  receivecont=0  ;

unsigned int long long  cansentcont=0;
unsigned int long long canreceivecont=0  ;
#if 0           //real
QString requestHeader =QString("http://192.168.8.100:10058/api");

QString requestState = QString("/vehicle_state");

#else           //test
QString requestHeader =QString("http://192.168.1.13:10058");

QString requestState = QString("/api/vehicle_state");

#endif
