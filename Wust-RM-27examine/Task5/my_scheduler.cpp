#include "my_scheduler.hpp"

Action MyScheduler::decide(const View& v, int e) {
    int my_floor = v.cars[e].floor;
    bool is_full = (v.cars[e].load >= v.cars[e].capacity);
    Action f_action = Action::Idle;//默认发呆

    if (v.cars[e].car_call[my_floor] != -1) {
        f_action =(current_dir[e]==-1)?Action::OpenDown:Action::OpenUp;
        return f_action;//内部需求
    }
    //高峰期响应
    int up_peak[5][2]={{29100,30000},{36300,37200},{49500,50400},{56700,57600},{66300,67200}};
    int down_peak[5][2]={{36000,36600},{43200,43800},{56400,57000},{61200,61800},{73200,73800}};
    bool is_up_peak=false;
    bool is_down_peak=false;
    for(int i=0;i<5;++i){
        if(v.now>up_peak[i][0] &&v.now<up_peak[i][1]){
            is_up_peak=true;
            break;
        }
    }
    for(int i=0;i<5;++i){
        if(v.now>down_peak[i][0] &&v.now<down_peak[i][1]){
            is_down_peak=true;
            break;
        }
    }
    if(is_up_peak){
        if(e==0 && !is_full && my_floor>1 && v.cars[e].load==0){
            current_dir[e]=-1;
            f_action=Action::Down;
        }else if(e==1 &&my_floor>1 &&!is_full &&v.cars[0].floor!=1){
            if(v.hall_down[my_floor]!=-1){
                f_action=Action::OpenDown;
                current_dir[e]=-1;
            }
        }
    }else if(is_down_peak){
        if(e==0 && !is_full && my_floor<6 && v.cars[e].load==0){
            current_dir[e]=1;
        }else if(e==1 &&my_floor<6 &&!is_full && v.cars[0].floor!=6){
            if(v.hall_down[my_floor]!=-1){
                current_dir[e]=1;
            }
        }
    }
    //外部响应
    if (!is_full) {
        if(is_up_peak){
            if(v.hall_up[my_floor]!=-1 && current_dir[e]!=-1){
                current_dir[e]=1;
                f_action=Action::OpenUp;
            }
        }else if(is_down_peak){
            if(v.hall_up[my_floor]!=-1 && current_dir[e]!=1){
                current_dir[e]=-1;
                f_action=Action::OpenDown;
            }
        }else{
            if (v.hall_down[my_floor] != -1 && current_dir[e] != 1) {
                current_dir[e] = -1;
                f_action=Action::OpenDown;
            }
            if (v.hall_up[my_floor] != -1 && current_dir[e] != -1) {
                current_dir[e] = 1;
                f_action=Action::OpenUp;
            }
        }
    }
    //更新需求状态
    bool target_up = false;
    bool target_down = false;
    if(f_action==Action::Idle){
        

        for (int f = my_floor + 1; f <= 6; ++f) {
            if (v.cars[e].car_call[f] != -1 || v.hall_up[f] != -1) {
                target_up = true; 
                break;
            }
        }
        for (int f = my_floor - 1; f >= 1; --f) {
            if (v.cars[e].car_call[f] != -1 || v.hall_down[f] != -1) {
                target_down = true; 
                break;
            }
        }
    }
        //运行逻辑
        if (current_dir[e] == 1) {
            if (target_up){
                f_action=Action::Up;
            }else if (target_down) {
                current_dir[e] = -1;
                f_action=Action::Down; 
            } else {
                current_dir[e] = 0;
            }
        } else if (current_dir[e] == -1) {
            if (target_down){
              f_action=Action::Down;  
            } else if (target_up) {
                current_dir[e] = 1;
                f_action=Action::Up;
            } else {
                current_dir[e] = 0;
            }
        } else {
            if (target_up) {
                current_dir[e] = 1;
                f_action=Action::Up;
            } else if (target_down) {
                current_dir[e] = -1;
                f_action=Action::Down;
            }
        }
    
    return f_action;
}