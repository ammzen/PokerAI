#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H 1;

#include <string>
#include <vector>
using namespace std;

void SplitString(const string& s, vector<string>& v, const string& c);
int     ParseMsgSeat(string msg);
int    ParseMsgBlind(string msg);
int ParseCardColor(string msg);
int ParseCardPoint(string msg);
int     ParseMsgHold(string msg);
int ParseAction(string action);
int  ParseMsgInquireOrNotify(string msg);
int  ParseMsgInquire(string msg);
int   SendActionMsg(int action, int raise_num);
int     ParseMsgFlop(string msg);
int     ParseMsgTurn(string msg);
int    ParseMsgRiver(string msg);
int ParseMsgShowdown(string msg);
int  ParseMsgCommmon(string msg);
int   ParseMsgPotwin(string msg);
int   ParseMsgNotify(string msg);
int OnServerMessage(int length, const char* buffer);
void ThinkThreeTimes(void);

#endif
