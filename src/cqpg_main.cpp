#include <iostream>
#include <iomanip>
#include <chrono>
#include "../inc/cqpg.h"
#include "../inc/cqpg_timer.h"
#include "../inc/cqpg_memory.h"

int main (int argc, char *argv[]) {

    CalMem("0). 启动");
    cout << endl;

#ifdef DEBUG
    if (argc != 7) {
        std::cout << "输入格式错误!" << std::endl;
        std::cout << "输入格式：./cqpgsim  .spice网表文件 .cqpg输出文件 .solution标准解文件 构建G矩阵文件 构建ut矩阵文件" << std::endl;
        return -1;
    }
    string f_deck       = argv[1];  // spice 网表
    string ofile        = argv[2];  // cqpg  输出文件，各个节点计算结果
    string f_solution   = argv[3];  // .soltion  标准解文件
    string gfile        = argv[4];  // 输出构建的G矩阵文件
    string utfile       = argv[5];  // 输出构建的ut文件
    string ofile_x      = argv[6];  // 输出x矩阵参数
#else
    if (argc != 5) {
        std::cout << "输入格式错误!" << std::endl;
        std::cout << "输入格式：./cqpgsim  .spice网表文件 .cqpg输出文件 .solution标准解文件 构建G矩阵文件 构建ut矩阵文件" << std::endl;
        return -1;
    }
    string f_deck       = argv[1];  // spice 网表
    string ofile        = argv[2];  // cqpg  输出文件，各个节点计算结果
    string gfile        = argv[3];  // 输出构建的G矩阵文件
    string utfile       = argv[4];  // 输出构建的ut文件
#endif

    bool res;

    cqpg *mycqpg = new cqpg;
auto start = std::chrono::high_resolution_clock::now();
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
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
std::cout << "======解析网表所用的时间: ======" << duration/1000.0000 << " s " << std::endl;
   
    //1.5 输出构建矩阵值，后续传递给AMGCL计算
    #ifdef DEBUG        
    y_para_output(mycqpg ,gfile,utfile ) ;
    #endif

    return 0;
}
