#ifndef TREE_H
#define TREE_H

#include <memory>
#include <vector>
#include "board.h"

class Tree
{
public:
    //搜索的最大深度
    int maxDepth=2;

    //根节点的相关信息
    int rootcolor;

    //指向根节点
    std::shared_ptr<Board> boardRoot=nullptr;

public:
    Tree(Board board);
    //扩展当前棋局，也就是生成一棵树,传过来的参数有当前的棋盘，当前掷骰子得到的棋子
    double expandChildrenBoard(std::shared_ptr<Board> board,std::vector<int> pawns,int depth);
    //找到需要的棋子以及棋子的走法
    std::vector<int> findBest(std::vector<int> pawns);
};
#endif // TREE_H
