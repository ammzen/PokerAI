#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include "glob.h"
#include "communication.h"

using namespace std;

void SplitString(const string& s, vector<string>& v, const string& c)
{
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
         
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}

int     ParseMsgSeat(string msg)
{
    cout<<"ParseMsgSeat: "<<endl<<msg<<endl;
    vector<string> msgv;
    SplitString(msg, msgv, "\n");
    cout<<"*************************"<<endl;
    for (vector<string>::size_type i = 0; i != msgv.size(); ++i)
    {
        cout<<"msgv["<<i<<"] : "<<msgv[i]<<endl;
    }
    
    int msgv_prefix_len;
    msgv_prefix_len = msgv[0].find(": ");
    msgv[0].erase(0, msgv_prefix_len+2);

    msgv_prefix_len = msgv[1].find(": ");
    msgv[1].erase(0, msgv_prefix_len+2);

    if (msgv.size()>2){
        msgv_prefix_len = msgv[2].find(": ");
        msgv[2].erase(0, msgv_prefix_len+2);
    }

    for (int i = 0; i < msgv.size(); ++i)
    {
        vector<string> player_info;

        SplitString(msgv[i], player_info, " ");

        player[i].pid = atoi(player_info[0].c_str());
        player[i].jetton = atoi(player_info[1].c_str());
        player[i].money = atoi(player_info[2].c_str());
        player[i].bet = 0;
        player[i].total_bet = 0;
        player[i].seat_num = i;
        player[i].type = NORMAL;
        // player[i].cards[0].color = UNKNOW;
        // player[i].cards[0].point = UNKNOW;
        // player[i].cards[1].color = UNKNOW;
        // player[i].cards[1].point = UNKNOW;
        if (player[i].pid == myself_pid)
            myself = &player[i];       
    }

    table_board.rounds += 1; /** 第几回合/局 */
    table_board.player_number = msgv.size();  
    table_board.stage = PREFLOP; /** 当前牌桌局面 */
    table_board.total_pot = 0;

    cout<<"*************************"<<endl;
    cout<<"myself:"<<endl;
    cout<<"pid: "<<myself->pid<<" jetton: "<<myself->jetton<<" money:"<<myself->money<<" seat:"<<myself->seat_num<<endl;
    cout<<"mycards: "<<myself->cards[0].color<<myself->cards[0].point<<myself->cards[1].color<<myself->cards[1].point<<endl;

    for (int i = 0; i < msgv.size(); ++i)
    { 
        cout<<"player["<<i<<"]:"<<endl;
        cout<<"pid: "<<player[i].pid<<" jetton: "<<player[i].jetton<<" money:"<<player[i].money<<" seat:"<<player[i].seat_num<<endl;
        cout<<"cards: "<<player[i].cards[0].color<<player[i].cards[0].point<<player[i].cards[1].color<<player[i].cards[1].point<<endl;
    }

    return MSG_SEAT;
}

int    ParseMsgBlind(string msg)
{
    cout<<"ParseMsgBlind: "<<endl<<msg<<endl;
    
    vector<string> msgv;
    SplitString(msg, msgv, "\n");

    vector<string> blind_info;
    SplitString(msgv[0], blind_info, ":");
    player[1].bet = atoi(blind_info[1].c_str());
    table_board.blind_bet = player[1].bet; /** 当前回合的大盲注 */

    if (msgv.size() > 1)
    {
        vector<string> blind_info;
        SplitString(msgv[1], blind_info, ":");
        player[2].bet = atoi(blind_info[1].c_str());
        table_board.blind_bet = player[2].bet; /** 当前回合的大盲注 */
    }

    cout<<"player[1].bet: "<<player[1].bet<<endl;
    cout<<"player[2].bet: "<<player[2].bet<<endl;
    cout<<"table_board.blind_bet: "<<table_board.blind_bet<<endl;

    return BLIND;
}

int ParseCardColor(string msg)
{
    switch (msg[0]) {
        case 'S':
            return SPADES;
        case 'H':
            return HEARTS;
        case 'C':
            return CLUBS;
        case 'D':
            return DIAMONDS;
        default :
            return UNKNOW;
        }
}

int ParseCardPoint(string msg)
{
    switch (msg[0]) {
        case 'A':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'T':
            return 10;
        case 'J':
            return 11;
        case 'Q':
            return 12;
        case 'K':
            return 13;
        default :
            return UNKNOW;
        }
}

int     ParseMsgHold(string msg)
{
    cout<<"ParseMsgHold： "<<endl<<msg<<endl;

    vector<string> msgv;
    SplitString(msg, msgv, "\n");

    vector<string> card0_info;
    SplitString(msgv[0], card0_info, " ");
    vector<string> card1_info;
    SplitString(msgv[1], card1_info, " ");

    myself->cards[0].color = ParseCardColor(card0_info[0]);
    myself->cards[0].point = ParseCardPoint(card0_info[1]);
    myself->cards[1].color = ParseCardColor(card1_info[0]);
    myself->cards[1].point = ParseCardPoint(card1_info[1]);

    cout<<myself->cards[0].color<<endl;
    cout<<myself->cards[0].point<<endl;
    cout<<myself->cards[1].color<<endl;
    cout<<myself->cards[1].point<<endl;

    return MSG_HOLD_;
}

int ParseAction(string action)
{
    switch (action[0]) {
        case 'a':
            return ALLIN;
        case 'b':
            return BLIND;
        case 'c':
            switch (action[1]) {
                case 'h':
                    return CHECK;
                case 'a':
                    return CALL;
            }
        case 'f':
            return FOLD;
        case 'r':
            return RAISE;
        default :
            return UNKNOW;
        }  
}
int  ParseMsgInquireOrNotify(string msg)
{
    vector<string> msgv;
    SplitString(msg, msgv, "\n");

    for (int i = 0; i < msgv.size() - 1; ++i)
    {
        vector<string> player_info;
        SplitString(msgv[i], player_info, " ");
        for (int j = 0; j < table_board.player_number; ++j)
        {
            if (player[j].pid == atoi(player_info[0].c_str())){
                player[j].jetton = atoi(player_info[1].c_str());
                player[j].money = atoi(player_info[2].c_str());
                player[j].total_bet = atoi(player_info[3].c_str());
                player[j].action = ParseAction(player_info[4]);
                break;
            }
        }    
    }

    vector<string> pot_info;
    SplitString(msgv[msgv.size()-1], pot_info, ":");
    table_board.total_pot = atoi(pot_info[1].c_str());

    for (int i = 0; i < table_board.player_number; ++i)
    {
        cout<<"player["<<i<<"].pid: "<<player[i].pid<<endl;
        cout<<"player["<<i<<"].jetton: "<<player[i].jetton<<endl;
        cout<<"player["<<i<<"].money: "<<player[i].money<<endl;
        cout<<"player["<<i<<"].total_bet: "<<player[i].total_bet<<endl;
        cout<<"player["<<i<<"].action: "<<player[i].action<<endl;
    }
    cout<<"table_board.total_pot: "<<table_board.total_pot<<endl;

    return PARSEOVER;
}

int  ParseMsgInquire(string msg)
{
    cout<<"ParseMsgInquire: "<<endl<<msg<<endl;
    ParseMsgInquireOrNotify(msg);
    return MSG_INQUIRE;
}

int   SendActionMsg(int action, int raise_num = 0)
{
    /** 向server发送的消息 */
    char action_msg[50] = {'\0'};

    switch (action){
        case FOLD:
            snprintf(action_msg, sizeof(action_msg) - 1, "fold \n"); 
            send(m_socket_id, action_msg, strlen(action_msg) + 1, 0);
            break;
        case CHECK:
            snprintf(action_msg, sizeof(action_msg) - 1, "check \n"); 
            send(m_socket_id, action_msg, strlen(action_msg) + 1, 0);
            break;
        case CALL:
            snprintf(action_msg, sizeof(action_msg) - 1, "call \n"); 
            send(m_socket_id, action_msg, strlen(action_msg) + 1, 0);
            break;
        case RAISE:
            snprintf(action_msg, sizeof(action_msg) - 1, "raise %d \n", raise_num); 
            send(m_socket_id, action_msg, strlen(action_msg) + 1, 0);
            break;
        case ALLIN:
            snprintf(action_msg, sizeof(action_msg) - 1, "all_in \n"); 
            send(m_socket_id, action_msg, strlen(action_msg) + 1, 0);
            break;
        default :
            break; 
    }
    return RANDOM;
}

int     ParseMsgFlop(string msg)
{
    cout<<"ParseMsgFlop: "<<endl<<msg<<endl;
    
    vector<string> msgv;
    SplitString(msg, msgv, "\n");

    vector<string> board_card0_info;
    SplitString(msgv[0], board_card0_info, " ");
    vector<string> board_card1_info;
    SplitString(msgv[1], board_card1_info, " ");
    vector<string> board_card2_info;
    SplitString(msgv[2], board_card2_info, " ");

    table_board.board_cards[0].color = ParseCardColor(board_card0_info[0]);
    table_board.board_cards[0].point = ParseCardPoint(board_card0_info[1]);
    table_board.board_cards[1].color = ParseCardColor(board_card1_info[0]);
    table_board.board_cards[1].point = ParseCardPoint(board_card1_info[1]);
    table_board.board_cards[2].color = ParseCardColor(board_card2_info[0]);
    table_board.board_cards[2].point = ParseCardPoint(board_card2_info[1]);

    table_board.stage = FLOP;

    cout<<table_board.board_cards[0].color<<endl;
    cout<<table_board.board_cards[0].point<<endl;
    cout<<table_board.board_cards[1].color<<endl;
    cout<<table_board.board_cards[1].point<<endl;
    cout<<table_board.board_cards[2].color<<endl;
    cout<<table_board.board_cards[2].point<<endl;
    cout<<"table_board.stage: "<<table_board.stage<<endl;

    return MSG_FLOP;
}

int     ParseMsgTurn(string msg)
{
    cout<<"ParseMsgTurn: "<<endl<<msg<<endl;

    vector<string> board_card3_info;
    SplitString(msg, board_card3_info, " ");

    table_board.board_cards[3].color = ParseCardColor(board_card3_info[0]);
    table_board.board_cards[3].point = ParseCardPoint(board_card3_info[1]);

    table_board.stage = TURN;

    cout<<table_board.board_cards[3].color<<endl;
    cout<<table_board.board_cards[3].point<<endl;

    cout<<"table_board.stage: "<<table_board.stage<<endl;

    return MSG_TURN;
}

int    ParseMsgRiver(string msg)
{
    cout<<"ParseMsgRiver: "<<endl<<msg<<endl;

    vector<string> board_card4_info;
    SplitString(msg, board_card4_info, " ");

    table_board.board_cards[4].color = ParseCardColor(board_card4_info[0]);
    table_board.board_cards[4].point = ParseCardPoint(board_card4_info[1]);

    table_board.stage = RIVER;

    cout<<table_board.board_cards[4].color<<endl;
    cout<<table_board.board_cards[4].point<<endl;

    cout<<"table_board.stage: "<<table_board.stage<<endl;

    return MSG_RIVER;
}

int ParseMsgShowdown(string msg)
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

    cout<<"ParseMsgShowdown: "<<endl<<msg<<endl;

    vector<string> msgv;
    SplitString(msg, msgv, "\n");

    for (int i = 0; i < msgv.size(); ++i)
    {
        vector<string> showdown_info;
        msgv[i].erase(0, 3);
        cout<<msgv[i]<<endl;
        SplitString(msgv[i], showdown_info, " ");
        for (int j = 0; j < table_board.player_number; ++j)
        {
            if (player[j].pid == atoi(showdown_info[0].c_str())){
                player[j].cards[0].color = ParseCardColor(showdown_info[1]);
                player[j].cards[0].point = ParseCardPoint(showdown_info[2]);
                player[j].cards[1].color = ParseCardColor(showdown_info[3]);
                player[j].cards[1].point = ParseCardPoint(showdown_info[4]);
                break;
            }
        }    
    }

    return MSG_SHOWDOWN;
}

int  ParseMsgCommmon(string msg)
{
    cout<<"ParseMsgCommmon: "<<endl<<msg<<endl;

    vector<string> msgv;
    SplitString(msg, msgv, "\n");

    vector<string> board_card0_info;
    SplitString(msgv[0], board_card0_info, " ");
    vector<string> board_card1_info;
    SplitString(msgv[1], board_card1_info, " ");
    vector<string> board_card2_info;
    SplitString(msgv[2], board_card2_info, " ");
    vector<string> board_card3_info;
    SplitString(msgv[3], board_card3_info, " ");
    vector<string> board_card4_info;
    SplitString(msgv[4], board_card4_info, " ");

    table_board.board_cards[0].color = ParseCardColor(board_card0_info[0]);
    table_board.board_cards[0].point = ParseCardPoint(board_card0_info[1]);
    table_board.board_cards[1].color = ParseCardColor(board_card1_info[0]);
    table_board.board_cards[1].point = ParseCardPoint(board_card1_info[1]);
    table_board.board_cards[2].color = ParseCardColor(board_card2_info[0]);
    table_board.board_cards[2].point = ParseCardPoint(board_card2_info[1]);
    table_board.board_cards[3].color = ParseCardColor(board_card3_info[0]);
    table_board.board_cards[3].point = ParseCardPoint(board_card3_info[1]);
    table_board.board_cards[4].color = ParseCardColor(board_card4_info[0]);
    table_board.board_cards[4].point = ParseCardPoint(board_card4_info[1]);
    
    return MSG_COMMON;
}

int   ParseMsgPotwin(string msg)
{
    cout<<"ParseMsgPotwin: "<<endl<<msg<<endl;

    vector<string> potwin_info;
    SplitString(msg, potwin_info, " ");

    int winner_pid = atoi(potwin_info[0].c_str());
    int winner_jet = atoi(potwin_info[1].c_str());

    for (int i = 0; i < table_board.player_number; ++i)
    {
        if (player[i].pid == winner_pid)
            player[i].jetton += winner_jet;
    }

    cout<<"Winner is "<<winner_pid<<endl;
    cout<<"Win jetton :"<<winner_jet<<endl;

    return MSG_POTWIN;
}

int   ParseMsgNotify(string msg)
{
    cout<<"ParseMsgNotify: "<<endl<<msg<<endl;
    ParseMsgInquireOrNotify(msg);
    return MSG_NOTIFY;
}


/**
 * 解析Server发来的某一个类型的消息，更新牌桌及牌手状态
 * @param  length [description]
 * @param  buffer [description]
 * @return        [description]
 */
int OnServerMessage(int length, const char* buffer)
{
    fprintf(logfile, "Recieve Data From Server\n"
        "**************************\n"
        "%s\n"
        "**************************\n", buffer);

    string msg(buffer);

    if (msg.find("game-over") != string::npos)
        return GAMEOVER;

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
            if (!ParseMsgSeat(msg.substr(0, msg_suffix_pos)))
                return MSG_SEAT;
        }
        else if (msgType == "blind"){
            if (!ParseMsgBlind(msg.substr(0, msg_suffix_pos)))
                return MSG_BLIND;
        }
        else if (msgType == "hold"){
            if (!ParseMsgHold(msg.substr(0, msg_suffix_pos)))
                return MSG_HOLD_;
        }
        else if (msgType == "inquire"){
            if (!ParseMsgInquire(msg.substr(0, msg_suffix_pos)))
                return MSG_INQUIRE;
            /** ToDo: 这里添加分析及策略 */
            ThinkThreeTimes();
            SendActionMsg(CALL);
        }
        else if (msgType == "flop"){
            if (!ParseMsgFlop(msg.substr(0, msg_suffix_pos)))
                return MSG_FLOP;
        }
        else if (msgType == "turn"){
            if (!ParseMsgTurn(msg.substr(0, msg_suffix_pos)))
                return MSG_TURN;
        }
        else if (msgType == "river"){
            if (!ParseMsgRiver(msg.substr(0, msg_suffix_pos)))
                return MSG_RIVER;
        }
        else if (msgType == "showdown"){
            if (!ParseMsgShowdown(msg.substr(0, msg_suffix_pos)))
                return MSG_SHOWDOWN;
        }
        else if (msgType == "common"){
            if (!ParseMsgCommmon(msg.substr(0, msg_suffix_pos)))
                return  MSG_COMMON;
        }
        else if (msgType == "pot-win"){
            if (!ParseMsgPotwin(msg.substr(0, msg_suffix_pos)))
                return  MSG_POTWIN;
        }
        else if (msgType == "notify"){
            if (!ParseMsgNotify(msg.substr(0, msg_suffix_pos)))
                return  MSG_NOTIFY;
        }
        else break;
        /** 清除消息中的第一条信息 */
        msg.erase(0, msg_suffix_pos);
    }
    return PARSEOVER;
}

/**
 * 概率计算及策略分析
 */
void ThinkThreeTimes()
{

}
