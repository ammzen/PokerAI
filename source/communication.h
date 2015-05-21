#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H 1;

#include <string>
using namespace std;

void     ParseMsgSeat(string msg);
void    ParseMsgBlind(string msg);
void     ParseMsgHold(string msg);
void  ParseMsgInquire(string msg);
void   ParseMsgAction(string msg);
void     ParseMsgFlop(string msg);
void     ParseMsgTurn(string msg);
void    ParseMsgRiver(string msg);
void ParseMsgShowdown(string msg);
void  ParseMsgCommmon(string msg);
void   ParseMsgPotwin(string msg);
void   ParseMsgNotify(string msg);
void ParseMsgGameover(string msg);
int OnServerMessage(int length, const char* buffer);

#endif
