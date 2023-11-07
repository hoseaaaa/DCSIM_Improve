#ifndef __CQPG__
#define __CQPG__

#include "cqpg_config.h"
#include <string>
#include <unordered_map>

#define R       (1)
#define C       (2)
#define L       (3)
#define V       (4)
#define I       (5)

// #define DEBUG

using namespace std;

struct node {
    string      name;

    uint8       link_nums;          // 根据IBM模型，每个节点连接的元件很有限，GND连接的其他节点不关注，所以8位足够
    node        **link_nodes;
    int8        *link_types;        // R C L V I
    f64         *link_values;       // 电流和电压有方向，电流流入为正，流出为负

    f64         solutions;          // 节点电压
    int32       node2mtx;           // 节点与矩阵的映射关系
};

struct matrix_dc {
    int32       n;                  // 维度

    int32       G_nnz;              // 电导矩阵
    int32       *G_rowptr;
    int32       *G_colidx;
    f64         *G_value;

    f64         *ut;                // 电流源负载

    f64         *xt;                // 待求的节点电压
};

struct matrix_tr {

};

struct cqpg
{
    bool                            sim_type;                   // 仿真类型，0：DC仿真，1：瞬态仿真
    unordered_map<string, node*>    *netlist;

    node        **nodes_addrs;              // 所有节点首地址
    int32       nodes_nums;

    matrix_dc   *spmtx_dc;
    matrix_tr   *spmtx_tr;
};


bool cqpg_deck_parse (
    string  &f_deck,
    cqpg  *&mycqpg
);

bool cqpg_do_DC (
    cqpg  *&mycqpg
);

bool cqpg_do_TR (
    cqpg  *&mycqpg
);

bool cqpg_print (
    cqpg  *&mycqpg,
    string &ofile,
    string &ofile_x
);

bool cqpg_verify (
    string &ofile,
    string &f_solution
);

bool y_para_output(cqpg  *&mycqpg, string Gfile,string Utfile); 


#endif
