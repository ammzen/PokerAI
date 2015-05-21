#ifndef _GLOBAL_H
#define _GLOBAL_H 1

enum {UNKNOW, SPADES, HEARTS, CLUBS, DIAMONDS };
enum {PREFLOP, FLOP, TURN, RIVER }; 
enum {NORMAL, UNAGGRESSIVE, AGGRESSIVE };

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
    Card cards[2]; /** 玩家手牌 */
    int type; /** 表示牌手的牌风，UNAGGRESSIVE,NORMAL,AGGRESSIVE */
}Player;

typedef struct
{
    int rounds; /** 第几回合/局 */
    int stage; /** 当前牌桌局面 */
    int blind_bet; /** 当前回合的大盲注 */
    int total_pot; /** 底池总奖金 */
    Card board_cards[5]; /** 5张公共牌 */
    Card nut_hand[5]; /** 本回合最佳手牌 */
}Table;

#endif
