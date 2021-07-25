#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <fstream>
#include <memory>

#define MAX 9999

//棋盘类
class Board
{
public:
    Board(int c,int redLayout[6],int blueLayout[6]);
    Board(std::shared_ptr<Board> parent,int pos[2]);

    //父节点
    std::shared_ptr<Board> parent;

    //孩子节点
    std::vector<std::shared_ptr<Board>> children;

    //5×5的棋盘
    int board[5][5]={};
    //导致棋局要走的棋子和方向,棋子1-6代表红方，棋子7-12代表蓝方
    //可能的走法,0为水平走位，1垂直走位，2对角走位
    int chess[2]={0,-1};
    int pchess[2]={0,-1};
    //红色为0,蓝色为1
    int color;
    //可以走的棋子都有谁，0代表这个棋子已经被吃掉，1代表这个棋子可以走,初始时，都可以走
    int pawn[12];

    //当前棋局的分值
    double value=0.0;

    //画棋盘
    void drawTable();
    //初始化棋盘
    void initTable();

    //红方进行布局
    void redLayout(int layout[6]);
    //蓝方进行布局
    void blueLayout(int layout[6]);

    //生成1-6的随机数
    int random(int min,int max);
    //找到最近的棋子
    std::vector<int> findNearby(int color,int rand);
    //以摇骰子的方式，返回红蓝双方要移动的棋子
    std::vector<int> throwDice();

    //行棋
    void pawnRun();
    //修改棋盘状态
    int boardChange();

    //找到棋子位置
    std::vector<int> findPawn(int pawn);

    //打印下每棵棋子是否可以走
    void printPawn();

    //判定是否在棋盘范围内
    bool judgeRange(int x,int y);
    void resetDir();

    //判定是否游戏结束
    int isEnd();

    //计算每颗棋子可能的走法
    std::vector<int> computeWay(int pawn);

    //得到棋子所在位置的价值
    std::vector<int> getLocValue();
    //得到棋子被摇到的概率
    std::vector<double> getPawnPro();
    //得到棋子的价值
    std::vector<double> getPawnValue();
    //搜索附近红方棋子的价值
    double searchNearbyRedMaxValue(int pawn);
    //搜索附近蓝方棋子的价值
    double searchNearbyBlueMaxValue(int pwan);
    //得到红方对蓝方的威胁值，蓝方对红方的威胁值
    std::vector<double> getThread();
    //得到整个局势的价值
    double getScore(double k=2.2,int lam=5);

    //极大极小搜索算法,返回的是要走的棋子、走法
//    std::vector<int> redByMinimax(double k=2.2,int lam=5,int step=2);

};

#endif // BOARD_H
