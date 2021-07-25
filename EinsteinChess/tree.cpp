#include "tree.h"
#include <iostream>

Tree::Tree(Board board)
{
    //初始化根节点是哪方在走棋
    rootcolor=board.color;
    //初始化根节点
    boardRoot=std::make_shared<Board> (board);
}

double Tree::expandChildrenBoard(std::shared_ptr<Board> boardnow,std::vector<int> pawns,int depth)
{
    if(depth==maxDepth){
        return boardnow->getScore();
    }

    //检测棋局是否结束，如果结束，则算是一个特殊的叶子节点
    if(boardnow->isEnd()!=-1){
        return boardnow->getScore();
    }

//    std::cout<<"boardnow->color="<<boardnow->color<<std::endl;

    double t;

    if(boardnow->color==rootcolor){//极小值点，电脑下棋
        boardnow->value=-999;
    }else{
        boardnow->value=999;
    }

    std::shared_ptr<Board> bx;

    //对每一棵棋子进行循环
    for(std::size_t i=0;i<pawns.size();i++){
        //得到每一棵棋子的走法
        std::vector<int> ways=boardnow->computeWay(pawns[i]);

        //对每一种走法进行循环
        for(std::size_t w=0;w<ways.size();w++){
            boardnow->chess[0]=pawns[i];
            boardnow->chess[1]=ways[w];
            //创建一个临时指针，指向当前的棋盘扩展后的某一个棋盘
            std::shared_ptr<Board> board=std::make_shared<Board> (boardnow,boardnow->chess);
            bx=board;
            boardnow->children.push_back(board);

//            std::cout<<"当前棋盘为（红0蓝1）"<<board->color<<std::endl;
//            std::cout<<"当前棋盘的深度为："<<depth<<std::endl;
//            board->drawTable();

            int o=0;
            std::vector<int> chessable;
            //寻找敌方所有可以走的棋子
            if(board->color==0){//代表蓝方已经走子，该红方进行走子
                o=0;
            }else if(board->color==1){//代表红方已经走子，该蓝方进行走子
                o=6;
            }
            for(int p=0;p<6;p++){
                //如果棋子可以走，将其加入到可以走的棋子的数组中
                if(board->pawn[p+o]!=0){
                    chessable.push_back(p+o+1);
                }
            }

            //对敌方所有可以走的棋子进行扩展
            t=expandChildrenBoard(board,chessable,depth+1);

            if(board->color==rootcolor&&t>board->value){//如果是电脑下棋
                board->value=t;
            }
            if(board->color==(!rootcolor)&&t<board->value){
                board->value=t;
            }
        }
    }

    return bx->value;
}

std::vector<int> Tree::findBest(std::vector<int> pawns)
{
    std::vector<int> temp1;
    double temp=expandChildrenBoard(boardRoot,pawns,0);

    for(std::size_t i=0;i<boardRoot->children.size();i++){
        if(boardRoot->children[i]->value==temp){

            temp1.push_back(boardRoot->children[i]->pchess[0]);
            temp1.push_back(boardRoot->children[i]->pchess[1]);
            break;
        }
    }

    if(temp1.size()==0){
        std::cout<<"找不到值"<<std::endl;
    }

    return temp1;
}

