#include <iostream>

#include "board.h"
#include "tree.h"

int main()
{
    std::cout<<"红方进行布局"<<std::endl;
    int redlay[6]={1,2,3,4,5,6};
    std::cout<<"蓝方进行布局"<<std::endl;
    int bluelay[6]={6,5,4,3,2,1};
    //初始化棋盘
    Board board(0,redlay,bluelay);
    board.drawTable();

    //人人对战
    /*while (1) {
        board.color=0;
        //红方掷骰子，输入要走的棋子，改变棋盘状态
        board.throwDice();
        board.pawnRun();
        board.boardChange();
        board.drawTable();
        if(board.isEnd()!=-1){
            break;
        }

        //改变棋盘颜色
        board.color=1;
        board.throwDice();
        board.pawnRun();
        board.boardChange();
        board.drawTable();
        if(board.isEnd()!=-1){
            break;
        }
    }*/

    //创建一棵树，对根节点进行初始化
//    Tree tree(0,redlay,bluelay);
      Tree tree(board);

    //人机对战
    while (1) {
        board.color=0;
        //红方掷骰子，得到要走的棋子chessValid

        std::vector<int> chessValid=board.throwDice();
        std::shared_ptr<Board> b=std::make_shared<Board> (board);
        tree.boardRoot=b;

        //扩展树的节点
        std::vector<int> t=tree.findBest(chessValid);
        chessValid.clear();
        std::cout<<"红方计算出的最好的棋子为"<<t[0]<<std::endl;
        std::cout<<"红方计算出最好的走法为"<<t[1]<<std::endl;

        //改变棋盘
        board.chess[0]=t[0];
        board.chess[1]=t[1];
        board.boardChange();
        board.drawTable();
        if(board.isEnd()!=-1){
            break;
        }

        //改变棋盘颜色
        board.color=1;
        board.throwDice();
        board.pawnRun();
        board.boardChange();
        board.drawTable();
        if(board.isEnd()!=-1){
            break;
        }
    }
}
