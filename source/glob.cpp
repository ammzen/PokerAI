#include "glob.h"

/*************************** 定义全局变量 ***************************/
int m_socket_id; /** 服务器socket地址 */
Player player[8];
Player *myself;
int myself_pid;
Table table_board;
/** 输出重定向输出到文件 */
FILE * logfile;


void InitGlob(void)
{
    m_socket_id = -1;
    table_board.rounds = 0; /** 第几回合/局 */
}
