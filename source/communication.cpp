#include <iostream>
#include <string>
#include "glob.h"

using namespace std;


void     ParseMsgSeat(string msg)
{
    cout<<"ParseMsgSeat"<<endl<<msg<<endl;
}

void    ParseMsgBlind(string msg)
{
    cout<<"ParseMsgBlind"<<endl<<msg<<endl;
}

void     ParseMsgHold(string msg)
{
    cout<<"ParseMsgHold"<<endl<<msg<<endl;
}

void  ParseMsgInquire(string msg)
{
    cout<<"ParseMsgInquire"<<endl<<msg<<endl;
}

void   ParseMsgAction(string msg)
{
    cout<<"ParseMsgAction"<<endl<<msg<<endl;
}

void     ParseMsgFlop(string msg)
{
    cout<<"ParseMsgFlop"<<endl<<msg<<endl;
}

void     ParseMsgTurn(string msg)
{
    cout<<"ParseMsgTurn"<<endl<<msg<<endl;
}

void    ParseMsgRiver(string msg)
{
    cout<<"ParseMsgRiver"<<endl<<msg<<endl;
}

void ParseMsgShowdown(string msg)
{
    int msg_prefix_len = msg.find("/ \n");
    string msgType = msg.substr(0, msg_prefix_len);
    string msg_prefix = msgType + "/" + " \n";
    string msg_suffix = "/" + msgType + " \n";
    
    /** 清除消息中的第一条信息的前缀 */
    msg.erase(0, msg_prefix.length());
    /** 计算消息中的第一条信息的长度 */
    int msg_suffix_pos = msg.find(msg_suffix);
    /** 清除消息中的第一条信息的后缀 */
    msg.erase(msg_suffix_pos, msg_suffix.length());

    ParseMsgCommmon(msg.substr(0, msg_suffix_pos));
    /** 清除消息中的第一条信息 */
    msg.erase(0, msg_suffix_pos);

    cout<<"ParseMsgShowdown"<<endl<<msg<<endl;
}

void  ParseMsgCommmon(string msg)
{
    cout<<"ParseMsgCommmon"<<endl<<msg<<endl;
}

void   ParseMsgPotwin(string msg)
{
    cout<<"ParseMsgPotwin"<<endl<<msg<<endl;
}

void   ParseMsgNotify(string msg)
{
    cout<<"ParseMsgNotify"<<endl<<msg<<endl;
}

void ParseMsgGameover(string msg)
{
    cout<<"ParseMsgGameover"<<endl<<msg<<endl;
}


/**
 * 解析Server发来的某一个类型的消息，更新牌桌及牌手状态
 * @param  length [description]
 * @param  buffer [description]
 * @return        [description]
 */
int OnServerMessage(int length, const char* buffer)
{
    printf("Recieve Data From Server\n"
        "**************************\n"
        "%s\n"
        "**************************\n", buffer);

    string msg(buffer);

     while(!msg.empty())
    {
        int msg_prefix_len = msg.find("/ \n");
        string msgType = msg.substr(0, msg_prefix_len);
        string msg_prefix = msgType + "/" + " \n";
        string msg_suffix = "/" + msgType + " \n";
        
        /** 清除消息中的第一条信息的前缀 */
        msg.erase(0, msg_prefix.length());
        /** 计算消息中的第一条信息的长度 */
        int msg_suffix_pos = msg.find(msg_suffix);
        /** 清除消息中的第一条信息的后缀 */
        msg.erase(msg_suffix_pos, msg_suffix.length());

        if (msgType == "seat"){
            ParseMsgSeat(msg.substr(0, msg_suffix_pos));
        }
        else if (msgType == "blind"){
            ParseMsgBlind(msg.substr(0, msg_suffix_pos));
        }
        else if (msgType == "hold"){
            ParseMsgHold(msg.substr(0, msg_suffix_pos));
        }
        else if (msgType == "inquire"){
            ParseMsgInquire(msg.substr(0, msg_suffix_pos));
        }
        else if (msgType == "action"){
            ParseMsgAction(msg.substr(0, msg_suffix_pos));
        }
        else if (msgType == "flop"){
            ParseMsgFlop(msg.substr(0, msg_suffix_pos));
        }
        else if (msgType == "turn"){
            ParseMsgTurn(msg.substr(0, msg_suffix_pos));
        }
        else if (msgType == "river"){
            ParseMsgRiver(msg.substr(0, msg_suffix_pos));
        }
        else if (msgType == "showdown"){
            ParseMsgShowdown(msg.substr(0, msg_suffix_pos));
        }
        else if (msgType == "common"){
           ParseMsgCommmon(msg.substr(0, msg_suffix_pos));
        }
        else if (msgType == "pot-win"){
            ParseMsgPotwin(msg.substr(0, msg_suffix_pos));
        }
        else if (msgType == "notify"){
            ParseMsgNotify(msg.substr(0, msg_suffix_pos));
        }
        else if (msgType == "gameover"){
            ParseMsgGameover(msg.substr(0, msg_suffix_pos));
        }
        else break;
        /** 清除消息中的第一条信息 */
        msg.erase(0, msg_suffix_pos);

    }

    return 0;
}