#ifndef _GLOBAL_H
#define _GLOBAL_H 1

/** 表示自己所在的位置。0为庄家，1为小盲注，2为大盲注，依次递增 */
int g_seat_num;


enum
{
    SEAT,
    BLIND,
    HOLD,
    INQUIRE,
    ACTION,
    FLOP,
    TURN,
    RIVER,
    SHOWDOWN,
    COMMON,
    POTWIN,
    NOTIFY,
    GAMEOVER
}MessageType;

#endif
