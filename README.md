# PokerAI

## 项目说明


/makeproject 是 make 文件，用于编写编译命令，我没有仔细学习它，因此里面的编译脚本很初步，没有任何的编译依赖和编译优化。

/source 包含了游戏程序，现在通信和解析服务器端的都已经写完，但是注释可能比较少。里面留了一个空函数 ThinkThreeTime() 做为出牌前的分析及策略环节。

/target 包含咱们生成的 game 程序和其他 7 个葫芦娃程序，那 7 个程序的动作就和它的名字一样。

/libs 是第三方库的文件夹，暂时还没用到。

dist_check_and_run.sh

这个脚本是8个自己的 game 程序比赛。

new_dist_check_and_run.sh

这个脚本是修改官方的，自己的 game 程序和其他 7 个葫芦娃比赛。

这几个文件夹都是位于官方提供的 works 目录下。解压官方的[赛题信息](http://career-elite.huawei.com/CS/question/topic1_24.rar)里的 game.v1.24.tar.gz 就可以看到。

其他你需要用到资源有开发环境（一个官方比赛用的Ubuntu虚拟镜像）和辅助资源，到[这里](http://career-elite.huawei.com/CS/download.html)下载，根据官方的指南应该会很快就熟悉的，不明白的地方直接问我。
