#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <queue>
#include <algorithm>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;

typedef struct
{
    double 利;
    double 重;
} 宝;

typedef struct
{
    int 层;
    int 列;
    double 利;
    double 重;
    vector<int> 宝堆;
} 点;

// 广度优先算法
class 背包
{
public:              /* 数据 */
    vector<宝> 宝列; // 已按单价降序排好
    double 包载;
    int 宝数;
    double 最大收获;
    vector<int> 最优集;
    vector<int> 列号;
    int 总点数;
    int 出队点数;

public:
    背包(const vector<宝> &宝列, const double &包载);
    double 界(点 本点);
    void 装宝();
    void 印包();
    void 印点(const 点 &本点, const double &界);
    void 印队(const queue<点>& 队);
};

void 背包::印包()
{
    cout << "总点数:" << 总点数 << endl;
    cout << "出队点数:" << 出队点数 << endl;
    cout << "最大获利:" << 最大收获 << "[";
    int 最优宝数 = 最优集.size();
    for (int i = 0; i < 最优宝数; i++)
    {

        cout << 最优集[i] << "(" << 宝列[最优集[i]].利 << "," << 宝列[最优集[i]].重 << ")";

        cout << (i == 最优宝数 - 1 ? "]" : ",");
    }
    cout << endl;
}

void 背包::印点(const 点 &本点, const double &界)
{

    cout << left << "算[(" << 本点.层 << "," << 本点.列 << ")(利" << setw(3) << 本点.利 << ",重" << setw(3)
         << 本点.重 << ",界" << setw(4) << 界 << "丄利:" << 最大收获 << ")]" << endl;
}

void 背包::印队(const queue<点>& 队)
{
    queue<点> d = 队;
    cout<<"[";
    while (!d.empty())
    {
        cout <<"("<< d.front().层<<"," <<d.front().列 << ")";
        d.pop();
    }
    
    cout<<"]" << endl;
}

背包::背包(const vector<宝> &宝列, const double &包载)
{
    this->宝列 = 宝列;
    宝数 = 宝列.size() - 1; // 浪费0号
    this->包载 = 包载;
    最大收获 = 0;
    列号 = vector<int>(宝列.size(), 1);
    列号[0] = 0;
    总点数 = 0;
    出队点数 = 0;
}

double 背包::界(点 本点)
{
    int 下号, 破号;
    double 全重;

    double 利界;

    if (本点.重 >= 包载)
    {
        return 0;
    }
    else
    {
        下号 = 本点.层 + 1;
        利界 = 本点.利;
        全重 = 本点.重;

        // 包载之内装尽可能多的宝
        while (下号 <= 宝数 && 全重 + 宝列[下号].重 <= 包载)
        {
            全重 += 宝列[下号].重;
            利界 += 宝列[下号].利;
            下号++;
        }
        破号 = 下号;
        if (破号 <= 宝数)
        {
            利界 += (包载 - 全重) * 宝列[破号].利 / 宝列[破号].重;
        }

        return 利界;
    }
}

void 背包::装宝()
{
    queue<点> 队;
    点 子, 父;

    // 初始父为根
    父.利 = 0, 父.层 = 0, 父.重 = 0, 父.列 = 0;
    印点(父, 界(父));
    父.宝堆.clear();

    最大收获 = 0;
    队.push(父);
    总点数++;
    cout << "入("<<父.层<<","<<父.列 << ")" << endl;
    while (!队.empty())
    {
        印队(队);
        父 = 队.front(), 队.pop();
        cout << "出("<<父.层<<","<<父.列 << ")" << endl;
        出队点数++;

        子.层 = 父.层 + 1; 
        子.宝堆 = 父.宝堆;

        // 取翌宝
        子.列 = 列号[子.层]++;
        子.重 = 父.重 + 宝列[子.层].重;
        子.利 = 父.利 + 宝列[子.层].利;
        double 取界 = 界(子);

        子.宝堆.push_back(子.层);
        总点数++;

        if (子.重 <= 包载 && 子.利 > 最大收获)
        {
            最大收获 = 子.利;
            最优集 = 子.宝堆;
        }
        印点(子, 取界);

        if (取界 > 最大收获)
        {
            队.push(子); 
            cout << "入("<<子.层<<","<<子.列 << ")" << endl;
        }
        else
        {
            cout << "灭("<<子.层<<","<<子.列 << ")" << endl;
        }
        

        //舍翌宝
        子.列 = 列号[子.层]++;
        子.利 = 父.利;
        子.重 = 父.重;
        double 舍界 = 界(子);
        子.宝堆 = 父.宝堆;
        总点数++;
        印点(子, 舍界);
        if (舍界 > 最大收获)
        {
            队.push(子); 
            cout << "入("<<子.层<<","<<子.列 << ")" << endl;
        }
        else
        {
            cout << "灭("<<子.层<<","<<子.列 << ")" << endl;
        }
    }
}

int main()
{
    // 课本251页例6.1
    cout << "===== 课本251页例6.1 =====" << endl;
    vector<宝> 宝列1 = {{0, 0}, // 浪费零号
                        {40, 2},
                        {30, 5},
                        {50, 10},
                        {10, 5}};

    double 包载1 = 16;

    背包 包1(宝列1, 包载1);
    包1.装宝();
    包1.印包();

#if 0
    vector<宝> 宝列2 = {{0, 0}, // 浪费0号
                        {240, 120},
                        {192, 98},
                        {184, 94},
                        {229, 118},
                        {149, 77},
                        {135, 70},
                        {173, 90},
                        {221, 115},
                        {210, 110},
                        {139, 73},
                        {156, 82},
                        {201, 106},
                        {214, 113},
                        {150, 80},
                        {163, 87}};

    double 包载2 = 750;

    背包 包2(宝列2, 包载2);
    包2.装宝();
    包2.印包();
#endif

#if 0
    vector<宝> 宝列3 = {{0,0}, {6,6}, {4,4}, {5,5}, {1,1}}; // 浪费零号
    // double 包载 = 12; // [6 5 1]
    double 包载3 = 13; // 子集和算法无解，分枝设界有解
    背包 包3(宝列3, 包载3);
    包3.装宝();
    包3.印包();
#endif
    // 课本281页题6.1
    cout << "\n======= 课本281页题6.1 =======" << endl;
    vector<宝> 宝列4 = {{0, 0}, // 浪费零号
                        {20, 2},
                        {30, 5},
                        {35, 7},
                        {12, 3},
                        {3, 1}};

    double 包载4 = 13;

    背包 包4(宝列4, 包载4);
    包4.装宝();
    包4.印包();
}