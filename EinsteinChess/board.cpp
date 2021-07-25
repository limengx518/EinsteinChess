#include "board.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>

Board::Board(int c,int redLay[6],int blueLay[6])
{
    //父亲、孩子节点都为空
    parent=nullptr;
    children.clear();
    color=c;
    //初始化棋盘
    initTable();
    redLayout(redLay);
    blueLayout(blueLay);
    //初始化棋子的状态
    for(int i=0;i<12;i++){
        pawn[i]=1;
    }

}

Board::Board(std::shared_ptr<Board> par,int pos[2])
{
    //初始化父对象，导致当前棋局走的棋子和方向
    parent=par;
    pchess[0]=pos[0];
    pchess[1]=pos[1];

    chess[0]=pos[0];
    chess[1]=pos[1];

    //初始化棋子是否可以走
    for(int i=0;i<12;i++){
        pawn[i]=par->pawn[i];
    }

    //初始化成父对象的棋盘后，改变棋盘
    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
            board[i][j]=par->board[i][j];
        }
    }

    //首先将颜色变为父对象的颜色，将所走的棋子走完后，将颜色恢复
    color=par->color;
    boardChange();

    //改变棋盘颜色
    color=!(par->color);

    //可以进行走棋的棋子

    //如果是叶子节点，要计算分值
    value=0.0;
}

void Board::initTable()
{
    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
            board[i][j]=0;
        }
    }
}
void Board::blueLayout(int layout[])
{
    board[4][4]=layout[0]+6;
    board[4][3]=layout[1]+6;
    board[3][4]=layout[2]+6;
    board[4][2]=layout[3]+6;
    board[3][3]=layout[4]+6;
    board[2][4]=layout[5]+6;
}

//生成随机数
int Board::random(int min,int max)
{
    // use current time as seed for random generator
    std::srand(std::time(nullptr));

    int k;
    double d;

    d=(double)rand()/((double)RAND_MAX+1);
    k=(int)(d*(max-min+1));

    return min+k;
}

//寻找附近可以移动的棋子
std::vector<int> Board::findNearby(int color,int rand)
{
    std::vector<int> temp;

    if(color==1){
        rand+=6;
        if(pawn[rand-1]!=0){
            temp.push_back(rand);
            return temp;
        }
        for(int j=rand-1;j>6;j--){
            if(pawn[j-1]!=0){
                temp.push_back(j);
                break;
            }
        }
        for(int j=rand+1;j<13;j++){
            if(pawn[j-1]!=0){
                temp.push_back(j);
                break;
            }
        }

    }else if(color==0){
        if(pawn[rand-1]!=0){
            temp.push_back(rand);
            return temp;
        }
        for(int i=rand-1;i>0;i--){
            if(pawn[i-1]!=0){
                temp.push_back(i);
                break;
            }
        }
        for(int i=rand+1;i<7;i++){
            if(pawn[i-1]!=0){
                temp.push_back(i);
                break;
            }
        }
    }

    if(temp.size()==0){
        std::cout<<"为0"<<std::endl;
    }

    return temp;
}

std::vector<int> Board::throwDice()
{
    //生成1到6的随机数
    int r=random(1,6);
    std::cout<<"生成的随机数为"<<r<<", ";

    std::vector<int> temp;

    //红色是0,蓝色是1
    temp=findNearby(color,r);

    int n=temp.size();

    if(color==0){
        std::cout<<"红方可以走的棋子为：";
    }else{
        std::cout<<"蓝方可以走的棋子为：";
    }

    for(int i=0;i<n;i++){
        std::cout<<"temp["<<i<<"]"<<temp[i];
    }

    std::cout<<std::endl;

    if(temp.size()==0){
        std::cout<<"throwDice(),temp.size==0"<<std::endl;
    }
    return temp;

}

void Board::pawnRun()
{
    if(color==0){
        std::cout<<"红方所走的棋子为：";
        std::cin>>chess[0];
        if(chess[0]<=0||chess[0]>6){
            std::cerr<<"输入棋子错误，请重新输入";
            exit(0);
        }
    }else {
        std::cout<<"蓝方所走的棋子为：";
        std::cin>>chess[0];
        if(chess[0]<6||chess[0]>12){
            std::cerr<<"输入棋子错误，请重新输入";
            exit(0);
        }
    }
    std::cout<<"所走方向为：（0代表水平，1代表垂直，2代表对角）";
    std::cin>>chess[1];
}

int Board::boardChange()
{
    //找到所走棋子对应的棋盘位置
    std::vector<int> pos=findPawn(chess[0]);
    int row=pos[0];
    int col=pos[1];

    if(row<0||col<0||row>4||col>4){
        printf("位置错误\n");
    }

    //打印这颗棋子的位置
//    std::cout<<"row= "<<row<<std::endl;
//    std::cout<<"col= "<<col<<std::endl;

    int x,y;
    x=y=0;

    //红方行棋
    if(color==0){
        if(chess[1]==0){//水平
            y+=1;
        }else if(chess[1]==1){//垂直
            x+=1;
        }else if(chess[1]==2){//对角
            x+=1;
            y+=1;
        }else{
            return false;
        }
    }else{//蓝方行棋
        if(chess[1]==0){//水平
            y-=1;
        }else if(chess[1]==1){//垂直
            x-=1;
        }else if(chess[1]==2){//对角
            x-=1;
            y-=1;
        }else{
            return false;
        }
    }

    //如果没有在棋盘范围内，则移动无效
    if(!judgeRange(row+x,col+y)){
        std::cout<<"没有在棋盘范围内，移动无效"<<std::endl;
        return false;
    }

    //如果下一个位置有棋子，将下一个位置的棋子吃掉，并将吃掉的棋子设为不可以移动
    if(board[row+x][col+y]!=0){
        int temp=board[row+x][col+y];
        pawn[temp-1]=0;
        board[row+x][col+y]=board[row][col];
        board[row][col]=0;
    }else{
        board[row+x][col+y]=board[row][col];
        board[row][col]=0;
    }

    return true;

}
std::vector<int> Board::findPawn(int pawn)
{
    std::vector<int> temp;
    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
            if(board[i][j]==pawn){
                temp.push_back(i);
                temp.push_back(j);
                break;
            }
        }
    }
    return temp;
}

void Board::printPawn()
{
    for(int i=0;i<12;i++){
        int temp=i;
        temp+=1;
        std::cout<<temp;
        if(pawn[i]==0){
            printf("不可以走\n");
        }else{
            printf("可以走\n");
        }
    }
}

bool Board::judgeRange(int x,int y)
{
    if(x>=0&&x<5&&y>=0&&y<5){
        return true;
    }
    return false;
}

void Board::resetDir()
{

    std::cout<<"请重新输入方向,";
    std::cout<<"所走方向为：（0代表水平，1代表垂直，2代表对角）";
    std::cin>>chess[1];
}

int Board::isEnd()
{
    int num=0;
    //判断红方棋子是否被吃完
    for(int i=0;i<6;i++){
        if(pawn[i]==0){
            num++;
        }
    }
    if(num==6){
        printf("红方棋子被吃完，红方胜\n");
        return 0;
    }

    if(board[4][4]!=0&&board[4][4]<7){
        printf("红方棋子到达对方顶角，红方胜\n");
        return 0;
    }

    num=0;
    //判断蓝方棋子是否被吃完
    for(int j=6;j<12;j++){
        if(pawn[j]==0){
          num++;
        }
    }
    if(num==6){
        printf("蓝方棋子被吃完，蓝方胜\n");
        return 1;
    }
    if(board[0][0]>6){
        printf("蓝方棋子到达对方顶角，蓝方胜\n");
        return 1;
    }


    return -1;
}

std::vector<int> Board::computeWay(int pawn)
{
    //找到所走棋子对应的棋盘位置
    std::vector<int> pos=findPawn(pawn);
    int row=pos[0];
    int col=pos[1];

    int x,y;
    std::vector<int> ways;

    //蓝方
    if(color==1){
        //i代表可能的走法
        for(int i=0;i<3;i++){
            if(i==0){//水平
                y=-1;
                x=0;
            }else if(i==1){//垂直
                x=-1;
                y=0;
            }else if(i==2){//对角
                x=-1;
                y=-1;
            }
            if(judgeRange(row+x,col+y)){
                ways.push_back(i);
            }
        }
    }else if(color==0){
        //i代表可能的走法
        for(int i=0;i<3;i++){
            if(i==0){//水平
                y=1;
                x=0;
            }else if(i==1){//垂直
                x=1;
                y=0;
            }else if(i==2){
                x=1;
                y=1;
            }
            if(judgeRange(row+x,col+y)){
                ways.push_back(i);
            }
        }
    }else{
        std::cout<<"棋子错误"<<std::endl;
    }

    return ways;
}

void Board::redLayout(int layout[])
{
    board[0][0]=layout[0];
    board[1][0]=layout[1];
    board[0][1]=layout[2];
    board[2][0]=layout[3];
    board[1][1]=layout[4];
    board[0][2]=layout[5];
}

void Board::drawTable()
{
//   int temp;
  std::cout<<"   0   1   2   3   4"<<"\n";
   for(int i=0;i<5;i++){
        std::cout<<"   ——  ——  ——  ——  ——"<<"\n";
        std::cout<<i<<" ";
        for(int j=0;j<5;j++){
            if(board[i][j]==0){
                printf("|  |");
            }else{
                printf("|%2d|",board[i][j]);
            }
        }
        std::cout<<"\n";
   }

   std::cout<<"\n";
}

std::vector<int> Board::getLocValue()
{
    int blueValue[5][5]={
        {99,10,6,3,1},
        {10,8,4,2,1},
        {6,4,4,2,1},
        {3,2,2,2,1},
        {1,1,1,1,1}
    };

    int redValue[5][5]={
        {1,1,1,1,1},
        {1,2,2,2,3},
        {1,2,4,4,6},
        {1,2,4,8,10},
        {1,3,6,10,99}

    };

    int v[12]={0,0,0,0,0,0,0,0,0,0,0,0};

    std::vector<int> temp;
    for(int p=0;p<6;p++){
        if(pawn[p]!=0){
            temp=findPawn(p+1);
            v[p]=redValue[temp[0]][temp[1]];
        }
    }

    for(int p=6;p<12;p++){
        if(pawn[p]!=0){
            temp=findPawn(p+1);
            v[p]=blueValue[temp[0]][temp[1]];
        }
    }

    std::vector<int> ret;
    for(int i=0;i<12;i++){
        ret.push_back(v[i]);
    }

    return ret;
}

std::vector<double> Board::getPawnPro()
{
    std::vector<int> posValue=getLocValue();

    std::vector<double> pro;
    for(int p=0;p<12;p++){
        pro.push_back(1.0/6);
    }

    for(int p=0;p<12;p++){
        if(pawn[p]==0){
            std::vector<int> temp;
            if(p<6){
                temp=findNearby(0,p+1);
            }else if(p>=6){
                temp=findNearby(1,p+1);
            }

            if(temp.size()>1){
                int pr=temp[1]-1;
                int pl=temp[0]-1;

                if(posValue[pr]>posValue[pl]){
                    pro[pr]+=pro[p];
                }else if(posValue[pr]==posValue[pl]){
                    pro[pr]+=(pro[p]/2);
                    pro[pl]+=(pro[p]/2);
                }else{
                    pro[pl]+=pro[p];
                }
            }else if(temp.size()==1){
                pro[temp[0]-1]+=pro[p];
            }else if(temp.size()==0){
                std::cerr<<"所找到的附近的棋子个数为0"<<std::endl;
            }
        }
    }

    return pro;
}

std::vector<double> Board::getPawnValue()
{
    std::vector<double> v;

    std::vector<double> pro=getPawnPro();
    std::vector<int> posValue=getLocValue();

    for(int i=0;i<12;i++){
        v.push_back(pro[i]*posValue[i]);
    }

    return v;
}

double Board::searchNearbyRedMaxValue(int pawn)
{
    //传过来的应该是蓝方的棋子，搜索其附近红方棋子的价值
    std::vector<int> pos=findPawn(pawn);
    int row=pos[0];
    int col=pos[1];

    //每个棋子的价值
    std::vector<double> value=getPawnValue();

    std::vector<int> nearby;

    if((row-1)>0){
        if((board[row-1][col]>0)&&(board[row-1][col]<=6)){
            nearby.push_back(value[board[row-1][col]-1]);
        }
    }
    if((col-1)>0){
        if((board[row][col-1]>0)&&(board[row][col-1]<=6)){
            nearby.push_back(value[board[row][col-1]-1]);
        }
    }
    if((col-1)>0&&(row-1)>0){
        if(board[row-1][col-1]>0&&board[row-1][col-1]<=6){
            nearby.push_back(value[board[row-1][col-1]-1]);
        }
    }


    double sum=0.0;
    int expValue=0;

    for(std::size_t i=0;i<nearby.size();i++){
        sum+=nearby[i];
    }

    if(nearby.size()==0||sum==0.0){
        return 0;
    }

    for(std::size_t i=0;i<nearby.size();i++){
        expValue+=nearby[i]/sum;
    }

    return expValue;
}

double Board::searchNearbyBlueMaxValue(int pawn)
{
    //传过来的应该是红方的棋子，搜索其附近蓝方棋子的价值
    std::vector<int> pos=findPawn(pawn);
    int row=pos[0];
    int col=pos[1];

    //每个棋子的价值
    std::vector<double> value=getPawnValue();

    std::vector<int> nearby;

    if((row+1)<5){
        if((board[row+1][col]>6)&&(board[row+1][col]<=12)){
            nearby.push_back(value[board[row+1][col]-1]);
        }
    }
    if((col+1)<5){
        if((board[row][col+1]>6)&&(board[row][col+1]<=12)){
            nearby.push_back(value[board[row][col+1]-1]);
        }
    }
    if((col+1)<5&&(row+1)<5){
        if(board[row+1][col+1]>6&&board[row+1][col+1]<=12){
            nearby.push_back(value[board[row+1][col+1]-1]);
        }
    }


    double sum=0.0;

    int expValue=0;
    if(nearby.size()==0||sum==0.0){
        return 0;
    }

    for(std::size_t i=0;i<nearby.size();i++){
        sum+=nearby[i];
    }

    if(sum<=0.0){
        return 0;
    }
    for(std::size_t i=0;i<nearby.size();i++){
        expValue+=nearby[i]/sum;
    }

    return expValue;
}

std::vector<double> Board::getThread()
{
    std::vector<double> pro=getPawnPro();
    double redToBlueOfThread=0;
    double blueToRedOfThread=0;
    for(int p=0;p<12;p++){
        if(pawn[p]!=0){
            if(p<6){
                double nearbyBlueMaxValue=searchNearbyBlueMaxValue(p+1);
                redToBlueOfThread+=pro[p]*nearbyBlueMaxValue;
            }else{
                double nearbyRedMaxValue=searchNearbyRedMaxValue(p+1);
                blueToRedOfThread+=pro[p]*nearbyRedMaxValue;
            }
        }
    }

    std::vector<double> temp;
    temp.push_back(redToBlueOfThread);
    temp.push_back(blueToRedOfThread);

    return temp;
}

double Board::getScore(double k, int lam)
{
    std::vector<double> thread=getThread();
    double redToBlueOfThread=thread[0];
    double blueToRedOfThread=thread[1];

    std::vector<double> value=getPawnValue();

    double expRed=0;
    double expBlue=0;

    for(int i=0;i<12;i++){
        if(i<6){
            expRed+=value[i];
        }else{
            expBlue+=value[i];
        }
    }

    double theValue=lam*(k*expRed-expBlue)-blueToRedOfThread+redToBlueOfThread;

    return theValue;
}
