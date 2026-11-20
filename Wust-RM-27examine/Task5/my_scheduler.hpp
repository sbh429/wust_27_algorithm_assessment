#pragma once
#include "simulator.hpp"
class MyScheduler{
public:
    Action decide(const View& v, int e);
private:
    int current_dir[2]={0,0};//记录电梯当前方向 0为静止 1为上行 -1为下行
};