#include <iostream>
#include <iomanip>

#include "../inc/cqpg.h"
#include "../inc/cqpg_timer.h"
#include "../inc/cqpg_memory.h"

int main (int argc, char *argv[]) {

    CalMem("0). 启动");
    cout << endl;

#ifdef DEBUG
    if (argc != 4) {
        std::cout << "输入格式错误!" << std::endl;
        std::cout << "输入格式：./cqpgsim 网表文件 输出文件 标准解文件" << std::endl;
        return -1;
    }
    string f_deck       = argv[1];
    string ofile        = argv[2];
    string f_solution   = argv[3];
#else
    if (argc != 3) {
        std::cout << "输入格式错误!" << std::endl;
        std::cout << "输入格式：./cqpgsim 网表文件 输出文件" << std::endl;
        return -1;
    }
    string f_deck       = argv[1];
    string ofile        = argv[2];
#endif

    bool res;

    cqpg *mycqpg = new cqpg;

    // 1. 读取文件，解析网表，构建矩阵
    res = cqpg_deck_parse(f_deck, mycqpg);
    if (!res) {
        cout << "解析网表错误!" << endl;
        return -1;
    }
    else {
        CalMem("1). 解析网表");
        cout << endl;
    }

    // 2. DC仿真
    if (!mycqpg->sim_type) {
        res = cqpg_do_DC(mycqpg);
        if (!res) {
            cout << "DC仿真错误!" << endl;
            return -1;
        }
        else {
            CalMem("2). DC仿真");
            cout << endl;
        }
    }
    // 2. TR仿真
    else {
        res = cqpg_do_TR(mycqpg);
        if (!res) {
            cout << "TR仿真错误!" << endl;
            return -1;
        }
        else {
            CalMem("2). TR仿真");
            cout << endl;
        }
    }

    // 3. 输出结果
    res = cqpg_print(mycqpg, ofile);
    if (!res) {
        cout << "输出结果错误!" << endl;
        return -1;
    }
    else {
        CalMem("3). 输出结果");
        cout << endl;
    }

#ifdef DEBUG
    // 4. 验证结果
    res = cqpg_verify(ofile, f_solution);
    if (!res) {
        cout << "验证结果错误!" << endl;
        return -1;
    }
    else {
        CalMem("4). 验证结果");
        cout << endl;
    }
#endif

    return 0;
}
