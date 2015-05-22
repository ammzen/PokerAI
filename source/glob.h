#ifndef _GLOBAL_H
#define _GLOBAL_H 1
#include <stdio.h>

/*************************** 类型申明 ***************************/
enum {UNKNOW, SPADES, HEARTS, CLUBS, DIAMONDS };
enum {PREFLOP, FLOP, TURN, RIVER }; 
enum {NORMAL, UNAGGRESSIVE, AGGRESSIVE };
enum {FOLD, CHECK, CALL, BLIND, RAISE, ALLIN, RANDOM };
enum {FAIL, GAMEOVER, MSG_SEAT, MSG_BLIND, MSG_HOLD_, MSG_INQUIRE, MSG_FLOP, MSG_TURN, MSG_RIVER, MSG_SHOWDOWN, MSG_COMMON, MSG_POTWIN, MSG_NOTIFY, PARSEOVER};

typedef struct 
{
    int color; /** 花色 SPADES = 1, HEARTS = 2, CLUBS = 3, DIAMONDS = 4, UNKNOW = 0*/
    int point; /** A-2-9-T-J-Q-K : 1-13, UNKNOW = 0 */
}Card;

typedef struct
{
    int pid; /** 玩家id */
    int jetton; /** 玩家手上筹码 */
    int money; /** 玩家金币 */
    int bet; /** 玩家下注额 */
    int total_bet; /** 玩家下注总额 */
    int seat_num; /** 表示牌手所在位置。0为庄家，1为小盲注，2为大盲注，依次递增 */
    int type; /** 表示牌手的牌风，UNAGGRESSIVE,NORMAL,AGGRESSIVE */
    int action; /** 表示玩家最近的动作 */
    Card cards[2]; /** 玩家手牌 */
}Player;

typedef struct
{
    int rounds; /** 第几回合/局 */
    int player_number; /** 牌桌人数 */
    int stage; /** 当前牌桌局面 */
    int blind_bet; /** 当前回合的大盲注 */
    int total_pot; /** 底池总奖金 */
    Card board_cards[5]; /** 5张公共牌 */
    Card nut_hand[5]; /** 本回合最佳手牌 */
}Table;

/*************************** 声明全局变量 ***************************/
extern int m_socket_id; /** 服务器socket地址 */
extern Player player[8];
extern Player *myself;
extern int myself_pid;
extern Table table_board;
extern FILE * logfile;


/*************************** 定义函数 ***************************/
void InitGlob(void);

#endif
