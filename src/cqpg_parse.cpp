#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "string.h"
#include "malloc.h"
#include "assert.h"
#include <vector>
#include <algorithm>
#include <set>

#include "../inc/cqpg.h"

struct ai_map {
    int pos;
    int col;
};

inline bool cmp (ai_map A, ai_map B) {
    return A.col < B.col;
}

void sort_matrix( int n, const int *ap, int *&ai, double *&ax) {

    int nnz = ap[n];

    int *ai_ = new int[nnz];
    double *ax_ = new double[nnz];

    for (int i = 0; i < n; ++i) {
        int rs = ap[i];
        int re = ap[i+1];
        vector<ai_map> ai_map_array;
        for (int idx = rs; idx < re; ++idx) {
            ai_map ai_map_;
            ai_map_.col = ai[idx];
            ai_map_.pos = idx;
            ai_map_array.emplace_back(ai_map_);
        }
        sort(ai_map_array.begin(), ai_map_array.end() ,cmp);
        for (int idx = rs; idx < re; ++idx) {
            ai_[idx] = ai_map_array[idx-rs].col;
            ax_[idx] = ax[ai_map_array[idx-rs].pos];
        }
    }

    delete [] ai; ai = ai_;
    delete [] ax; ax = ax_;
}

bool test( int n, const int *colptr, const int *rowidx, const double *value) {

    int nnz = colptr[n];

    // CSR
    int         *rowptr = new int[n+1];
    int         *colidx = new int[nnz];
    double      *value_csr  = new double[nnz];

    // workspace
    int         *cnt    = new int[n];

    // CSC --> CSR
    memset(cnt,0,sizeof(int)*n);
    memset(rowptr,0,sizeof(int)*n);
    rowptr[n] = nnz;
    for (int j=0; j<n; ++j) {
        int js = colptr[j];
        int je = colptr[j+1];
        for (int idx=js; idx<je; ++idx) {
            int i = rowidx[idx];
            ++rowptr[i];
        }
    }
    for (int i=n-1; i>-1; --i) {
        rowptr[i] = rowptr[i+1] - rowptr[i];
    }
    if (rowptr[0] != 0) {
        std::cout << "error!" << std::endl;
        return false;
    }
    for (int j=0; j<n; ++j) {
        int js = colptr[j];
        int je = colptr[j+1];
        for (int idx=js; idx<je; ++idx) {
            int i = rowidx[idx];
            double val = value[idx];
            colidx[rowptr[i]+cnt[i]] = j;
            value_csr[rowptr[i]+cnt[i]] = val;
            ++ cnt[i];
        }
    }

    // 判断是否对称
    for (int i=0; i<n+1; ++i) {
        if (colptr[i] != rowptr[i]) {
           std::cout << "矩阵不对称" << std::endl; 
            delete [] rowptr; rowptr = nullptr;
            delete [] colidx; colidx = nullptr;
            delete [] value_csr; value_csr = nullptr;
            delete [] cnt; cnt = nullptr;
           return false;
        }
    }
    for (int i=0; i<nnz; ++i) {
        if (rowidx[i] != colidx[i] || value[i] != value_csr[i]) {
           std::cout << "矩阵不对称" << std::endl; 
            delete [] rowptr; rowptr = nullptr;
            delete [] colidx; colidx = nullptr;
            delete [] value_csr; value_csr = nullptr;
            delete [] cnt; cnt = nullptr;
           return false;
        }
    }

    std::cout << "矩阵对称!" << std::endl; 

    delete [] rowptr; rowptr = nullptr;
    delete [] colidx; colidx = nullptr;
    delete [] value_csr; value_csr = nullptr;
    delete [] cnt; cnt = nullptr;

    return true;

}

// 处理并联电阻情况，重复的元素直接相加(电导并联)
void handling_parallel_resistors (int32 n, int32 *&ap, int32 *&ai, f64 *&ax , int32* y_nnz ) {

    int32 *ap_new = new int32[n+1];
    ap_new[0] = 0;
    for (int i = 0; i < n; ++i) {
        int32 re = ap[i+1];
        set<int32> col_set;
        for (int32 idx = ap[i]; idx < re; ++idx) {
            col_set.insert(ai[idx]);
        }
        ap_new[i+1] = ap_new[i] + col_set.size();
    }
    int32 nnz = ap_new[n];

    if (nnz == ap[n]) { // 不存在电阻并联的情况
        delete [] ap_new;
        return;
    }
    else { // 存在电阻并联的情况
        int32   *ai_new = new int32[nnz];
        f64     *ax_new = new f64[nnz];
        memset(ax_new, 0, sizeof(f64)*nnz);

        for (int i = 0; i < n; ++i) {
            int32 rs            = ap[i];
            int32 re            = ap[i+1];
            int32 rs_new        = ap_new[i];
            int32 re_new        = ap_new[i+1];
            int32 col_set_size  = re_new - rs_new;
            if (col_set_size == re - rs) { // 这个节点没有电阻并联
                for (int32 idx = rs; idx < re; ++idx) {
                    ai_new[rs_new] = ai[idx];
                    ax_new[rs_new] = ax[idx];
                    ++ rs_new;
                }
            }
            else { // 这个节点有电阻并联
                set<int32> col_set;
                for (int32 idx = rs; idx < re; ++idx) {
                    col_set.insert(ai[idx]);
                }
                for (auto it = col_set.begin(); it != col_set.end(); ++it) {
                    int32 col = *it;
                    // ai
                    ai_new[rs_new] = col;
                    // ax
                    for (int32 idx = rs; idx < re; ++idx) {
                        if (ai[idx] == col) {
                            ax_new[rs_new] += ax[idx];
                        }
                    }
                    //
                    ++ rs_new;
                }
            }
        }
        cout << "ai_new--->nnz size "<<  nnz << endl ;
        *y_nnz = nnz ;
        delete [] ap; ap = ap_new;
        delete [] ai; ai = ai_new;
        delete [] ax; ax = ax_new;
    }

}

// 给 A 节点通孔的所有节点赋值
void vias_node2mtx (string &name, node* A, const int32 n) { // 单向递归
    A->node2mtx = n;
    uint8 lk_nums     = A->link_nums;
    node  **lk_nodes  = A->link_nodes;
    int8  *lk_types   = A->link_types;
    string curr_name  = A->name;
    for (uint8 i = 0; i < lk_nums; ++i) {
        if (lk_types[i] == V && lk_nodes[i]->name != name) {
            vias_node2mtx(curr_name, lk_nodes[i], n);
        }
    }
}
void vias_node2mtx (node* A, const int32 n) { // 入口函数
    A->node2mtx = n;
    uint8 lk_nums     = A->link_nums;
    node  **lk_nodes  = A->link_nodes;
    int8  *lk_types   = A->link_types;
    string curr_name  = A->name;
    for (uint8 i = 0; i < lk_nums; ++i) {
        if (lk_types[i] == V) {
            vias_node2mtx(curr_name, lk_nodes[i], n);
        }
    }
}

bool cqpg_deck_parse_dc (
    string  &f_deck,
    cqpg  *&mycqpg
) {

    unordered_map<string, node*> &netlist = *(mycqpg->netlist);

    // 1. 第二次打开 deck 文件，建立网表
    ifstream in(f_deck);
    string line;
    while (getline(in,line)) {
        stringstream sstr(line);
        string dev;
        sstr >> dev;
        if (dev[0] == '*') {
            continue;
        }
        if (dev[0] == '.') {
            break;
        }
        dev[0] = toupper(dev[0]);
        string node1_name, node2_name;
        f64 val;
        // 过滤小电阻
        sstr >> node1_name >> node2_name >> val;
        // if ( val <= 1e-6 && val!=0) {
        //     continue;
        // }
        node *node1 = netlist[node1_name];
        node *node2 = netlist[node2_name];
        uint8 link_ptr1 = node1->link_nums++;
        uint8 link_ptr2 = node2->link_nums++;
        switch (dev[0]) {
            case 'R': { node1->link_nodes[link_ptr1]  = node2;
                        node1->link_types[link_ptr1]  = R;
                        node1->link_values[link_ptr1] = 1./val; // 存储电导值
                        node2->link_nodes[link_ptr2]  = node1;
                        node2->link_types[link_ptr2]  = R;
                        node2->link_values[link_ptr2] = 1./val; // 存储电导值
                        break; 
                        }
            case 'V': { node1->link_nodes[link_ptr1]  = node2;  // 两种情况：1.电压源供电 & 2.通孔连接
                        node1->link_types[link_ptr1]  = V;
                        node1->link_values[link_ptr1] = val;
                        node2->link_nodes[link_ptr2]  = node1;
                        node2->link_types[link_ptr2]  = V;
                        node2->link_values[link_ptr2] = 0.;
#ifdef DEBUG
                        if (val != 0 && node2->name != "G") {
                            cout << "电压源位置错误: " << node2->name << endl;
                            return false;
                        }
#endif
                        break; 
                        }
            case 'I': { node1->link_nodes[link_ptr1]  = node2;
                        node1->link_types[link_ptr1]  = I;
                        node1->link_values[link_ptr1] = -val;
                        node2->link_nodes[link_ptr2]  = node1;
                        node2->link_types[link_ptr2]  = I;
                        node2->link_values[link_ptr2] = val;
                        break; 
                        }
            default: { cout << "不存在的元件！----> ' " << dev << " '" << endl; return false; }
        }
    }
    in.close();

    // 2. 处理G节点
    node *&node_G = netlist[(string)"0"];
    node_G->solutions = 0.;
    node_G->link_nums = 0;
    delete [] node_G->link_values; node_G->link_values = nullptr;
    delete [] node_G->link_types;  node_G->link_types  = nullptr;
    delete [] node_G->link_nodes;  node_G->link_nodes  = nullptr;

    // 3. 删除哈希网表，构建节点地址数组 (哈希表遍历效率低)
    int32 node_nums = netlist.size();
    node **node_addrs = new node*[node_nums];
{
    int32 cnt = 0;
    for (auto iter = netlist.begin(); iter != netlist.end(); ++iter) {
        node_addrs[cnt++] = iter->second;
    }
    unordered_map<string, node*>().swap(netlist); // 删除哈希表，释放内存
    malloc_trim(0);
}

    // 3.5 处理电阻并联情况，IBM 的 benchmark 中存在电阻并联的情况
    // 通孔和普通节点之间都可能存在电阻并联
    // 若通孔建模为 0 电压源, 那么等效节点中可能存在更多并联电阻
    // 此处不从网表处理，直接从矩阵处理
    /*
    for (int i = 0; i < node_nums; ++i) {
        node        *&nd            = node_addrs[i];
        uint8       nd_lk_nums      = nd->link_nums;
        node        **&nd_lk_nodes  = nd->link_nodes;
        int8        *&nd_lk_types   = nd->link_types;
        set<string> lk_R_set;
        uint8       lk_R_all_nums   = 0;
        for (uint8 j = 0; j < nd_lk_nums; ++j) {
            if (nd_lk_types[j] == R) {
                lk_R_set.insert(nd_lk_nodes[j]->name);
                ++ lk_R_all_nums;
            }
        }
        uint8 lk_R_set_nums = lk_R_set.size();
        if (lk_R_set_nums < lk_R_all_nums) { // 存在并联电阻
            f64 *&nd_lk_values  = nd->link_values;
            // cout << nd->name << endl;
            uint8 nd_lk_nums_new    = nd_lk_nums - lk_R_all_nums + lk_R_set_nums;
            node  **nd_lk_nodes_new = new node*[nd_lk_nums_new];
            int8  *nd_lk_types_new  = new int8[nd_lk_nums_new];
            f64   *nd_lk_values_new = new f64[nd_lk_nums_new];
            
            // 0 --> lk_R_set_nums 并联电阻计算并填入
            nd_lk_nums_new = 0;
            for (auto it = lk_R_set.begin(); it != lk_R_set.end(); ++it) { 
                string nd_name = *it;
                // nodes
                for (uint8 j = 0; j < nd_lk_nums; ++j) {
                    if (nd_lk_nodes[j]->name == nd_name && nd_lk_types[j] == R) {
                        nd_lk_nodes_new[nd_lk_nums_new] = nd_lk_nodes[j];
                        break;
                    }
                }
                // values
                f64 val = 0;
                for (uint8 j = 0; j < nd_lk_nums; ++j) {
                    if (nd_lk_nodes[j]->name == nd_name && nd_lk_types[j] == R) {
                        val += nd_lk_values[j]; // 电导并联计算公式
                    }
                }
                nd_lk_values_new[nd_lk_nums_new] = val;
                // types
                nd_lk_types_new[nd_lk_nums_new]  = R;
                // 
                ++ nd_lk_nums_new;
            }

            // lk_R_set_nums --> nd_lk_nums_new 电压源电流源填入
            for (uint8 j = 0; j < nd_lk_nums; ++j) {
                if (nd_lk_types[j] != R) {
                    nd_lk_nodes_new[nd_lk_nums_new]  = nd_lk_nodes[j];
                    nd_lk_types_new[nd_lk_nums_new]  = nd_lk_types[j];
                    nd_lk_values_new[nd_lk_nums_new] = nd_lk_values[j];
                    ++ nd_lk_nums_new;
                }
            }
            
#ifdef DEBUG
            assert(nd_lk_nums_new == (nd_lk_nums - lk_R_all_nums + lk_R_set_nums));
#endif
            delete [] nd_lk_nodes;  nd_lk_nodes  = nd_lk_nodes_new;
            delete [] nd_lk_types;  nd_lk_types  = nd_lk_types_new;
            delete [] nd_lk_values; nd_lk_values = nd_lk_values_new;
            nd->link_nums = nd_lk_nums_new;
        }
    }
    */

    // 4. MNA填充矩阵，特殊情况: 电压源节点
    int32 n = 0;
    int32 G_nnz = 0;
    int32 *G_rowptr_ = new int32[node_nums];
    memset(G_rowptr_, 0, sizeof(int32)*node_nums);
    for (int i = 0; i < node_nums; ++i) {
        node *&node1 = node_addrs[i];
        string node1_name = node1->name;
        if (node1_name[0] == '_') { // package 节点, 结果已知
            uint8 node1_link_nums = node1->link_nums;
            node **node1_link_nodes = node1->link_nodes;
#ifdef DEBUG
            bool flag = false;
#endif
            for (uint8 j = 0; j < node1_link_nums; ++j) {
                if (node1_link_nodes[j]->name == "G") {
                    node1->solutions = node1->link_values[j];
#ifdef DEBUG
                    flag = true;
#endif
                    break;
                }
            }
#ifdef DEBUG
            if (!flag) {
                cout << "package 节点位置错误: " << node1_name << endl;
                return false;
            }
#endif
            delete [] node1->link_nodes;  node1->link_nodes  = nullptr;
            delete [] node1->link_types;  node1->link_types  = nullptr;
            delete [] node1->link_values; node1->link_values = nullptr;
        }
        else if (node1_name[0] == 'n') { // 其他节点
            // 先判断该节点是否被打了通孔
            uint8 node1_link_nums = node1->link_nums;
            node **node1_link_nodes = node1->link_nodes;
            int8 *link_types = node1->link_types;
            if (node1->node2mtx == -1) {
                vias_node2mtx(node1, n); // 给该节点以及通孔节点赋值
                ++ G_rowptr_[n]; // 对角线元素
                ++ G_nnz;
                ++ n;
            }
            int32 node1_node2mtx = node1->node2mtx;
            for (uint8 j = 0; j < node1_link_nums; ++j) {
                if (link_types[j] == R && node1_link_nodes[j]->name[0] != '_') {
                    ++ G_rowptr_[node1_node2mtx];
                    ++ G_nnz;
                }
            }
        }
    }

    int32 *G_rowptr = new int32[n+1];
    G_rowptr[n] = G_nnz;
    for (int i = n-1; i >= 0; --i) {
        G_rowptr[i] = G_rowptr[i+1] - G_rowptr_[i];
    }
    delete [] G_rowptr_; G_rowptr_ = nullptr;
    f64 *ut = new f64[n];
    memset(ut, 0, sizeof(f64)*n);
    int32 *G_colidx = new int32[G_nnz];
    f64   *G_value = new f64[G_nnz];
    memset(G_value, 0, sizeof(f64)*G_nnz);
    int32 *G_cnt = new int32[n];
    memset(G_cnt, 0, sizeof(int32)*n);
    f64 *G_diag = new f64[n];
    memset(G_diag, 0, sizeof(f64)*n);
    for (int i = 0; i < node_nums; ++i) {
        node *&node1 = node_addrs[i];
        if (node1->name[0] == 'n') {
            uint8   node1_link_nums     = node1->link_nums;
            node    **node1_link_nodes  = node1->link_nodes;
            int8    *link_types         = node1->link_types;
            f64     *link_values        = node1->link_values;
            int32   row                 = node1->node2mtx;
            int32   rs                  = G_rowptr[row];
            // 填入非对角线元素
            for (uint8 j = 0; j < node1_link_nums; ++j) {
                switch (link_types[j]) {
                    case R: {
                        f64 val = link_values[j];
                        G_diag[row] += val;
                        int32 col = node1_link_nodes[j]->node2mtx;
                        if (col != -1) { // 排除 package 节点
                            int32 G_cnt_ = rs + G_cnt[row]++;
                            G_colidx[G_cnt_] = col;
                            G_value[G_cnt_] = 0 - val;  // 电导负值  
                        }
                        else {
                            ut[row] += node1_link_nodes[j]->solutions * val;
                        }
                        break;
                    }
                    case I: {
                        ut[row] += link_values[j];
                        break;
                    }
                    default: break;
                }
            }
        }
        delete [] node1->link_nodes;  node1->link_nodes  = nullptr;
        delete [] node1->link_types;  node1->link_types  = nullptr;
        delete [] node1->link_values; node1->link_values = nullptr;
    }
    // 填入对角线元素
    for (int i = 0; i < n; ++i) {
        int32 G_cnt_ = G_rowptr[i] + G_cnt[i]++;
        G_colidx[G_cnt_] = i;
        G_value[G_cnt_] = G_diag[i];
    }
    delete [] G_cnt; G_cnt = nullptr;
    delete [] G_diag; G_diag = nullptr;

    // 5 从矩阵角度处理电阻并联情况
    int y_nnz ; 
    handling_parallel_resistors(n, G_rowptr, G_colidx, G_value, &y_nnz );

#ifdef DEBUG

    // ofstream in_spmatrix;
    // in_spmatrix.open("node2mtx.txt");
    // in_spmatrix << n << " " << n << " " << G_rowptr[n] << endl;
    // for (int i = 0; i < node_nums; ++i) {
    //     node *nd = node_addrs[i];
    //     if (nd->node2mtx != -1) {
    //         in_spmatrix << nd->node2mtx << "    " << nd->name << endl;
    //     }
    // }
    // in_spmatrix.close();

    // in_spmatrix.open("ut.mtx");
    // // in_spmatrix << n << " 1" << endl;
    // for (int i = 0; i < n; ++i) {
    //     in_spmatrix << i << " " << ut[i] << endl;
    // }
    // in_spmatrix.close();

    // sort_matrix(n, G_rowptr, G_colidx, G_value);

    // for (int i = 0; i < n; ++i) {
    //     int rs = G_rowptr[i];
    //     int re = G_rowptr[i+1];
    //     int col = G_colidx[rs];
    //     for (int idx = rs+1; idx < re; ++idx) {
    //         int j = G_colidx[idx];
    //         if (j == col) {
    //             cout << i << endl;
    //         }
    //         col = j;
    //     }
    // }

    // test(n, G_rowptr, G_colidx, G_value);

    // in_spmatrix.open("csr_G_2.mtx");
    // in_spmatrix << n << " " << n << " " << G_rowptr[n] << endl;
    // for (int i = 0; i < n; ++i) {
    //     int re = G_rowptr[i+1];
    //     for (int idx = G_rowptr[i]; idx < re; ++idx) {
    //         int j = G_colidx[idx];
    //         f64 val = G_value[idx];
    //         in_spmatrix << i << " " << j << " " << val << endl;
    //     }
    // }
    // in_spmatrix.close();

/*
    for (int i = 0; i < node_nums; ++i) {
        node *node1 = node_addrs[i];
        string node1_name = node1->name;
        if (node1_name == "G") continue;
        int32 rs = G_rowptr[node1->node2mtx];
        int32 re = G_rowptr[node1->node2mtx+1];
        if (re - rs == 2 && G_value[rs] + G_value[rs+1] == 0) {
            cout << node1_name << endl;
        }
    }
    for (int i = 0; i < node_nums; ++i) {
        node *node1 = node_addrs[i];
        if (node1->node2mtx == 4984) {
            cout << node1->name << endl;
        }
    }

    ofstream in_spmatrix("csr_G.mtx");
    in_spmatrix << n << " " << n << " " << G_rowptr[n] << endl;
    for (int i = 0; i < n; ++i) {
        int re = G_rowptr[i+1];
        for (int idx = G_rowptr[i]; idx < re; ++idx) {
            int j = G_colidx[idx];
            f64 val = G_value[idx];
            in_spmatrix << i << " " << j << " " << val << endl;
        }
    }
    in_spmatrix.close();

    sort_matrix(n, G_rowptr, G_colidx, G_value);

    in_spmatrix.open("csr_G_2.mtx");
    in_spmatrix << n << " " << n << " " << G_rowptr[n] << endl;
    for (int i = 0; i < n; ++i) {
        int re = G_rowptr[i+1];
        for (int idx = G_rowptr[i]; idx < re; ++idx) {
            int j = G_colidx[idx];
            f64 val = G_value[idx];
            in_spmatrix << i << " " << j << " " << val << endl;
        }
    }
    in_spmatrix.close();

    test(n, G_rowptr, G_colidx, G_value);

    for (int i = 0; i < n; ++i) {
        int re = G_rowptr[i+1];
        // if (re - G_rowptr[i] == 0) {
        //     cout << "err0\n";
        // }
        f64 max_entry;
        for (int idx = G_rowptr[i]; idx < re; ++idx) {
            if (G_colidx[idx] == i) {
                max_entry = G_value[idx];
                break;
            }
        }
        for (int idx = G_rowptr[i]; idx < re; ++idx) {
            f64 val = G_value[idx];
            if (val < 0) val = 0 - val;
            if (val > max_entry) {
                cout << "err1\n";
            }
        }
        if (max_entry == 0) {
            cout << "err2\n";
        }
    }
*/
#endif

    // 存储 G 矩阵 和 所有节点地址
    matrix_dc *spmtx_dc = new matrix_dc;
    spmtx_dc->n         = n;
    // spmtx_dc->G_nnz     = G_nnz;
    // spmtx_dc->G_nnz     = y_nnz;
    spmtx_dc->G_nnz     = G_rowptr[n];
    spmtx_dc->G_rowptr  = G_rowptr;
    spmtx_dc->G_colidx  = G_colidx;
    spmtx_dc->G_value   = G_value;
    spmtx_dc->ut        = ut;

    mycqpg->nodes_nums  = node_nums;
    mycqpg->nodes_addrs = node_addrs;
    mycqpg->spmtx_dc    = spmtx_dc;

    return true;
}

bool cqpg_deck_parse_tran (
    string  &f_deck,
    cqpg  *&mycqpg
) {
    

    return true;
}

bool cqpg_deck_parse (
    string  &f_deck,
    cqpg  *&mycqpg
) {

    // 1. 第一次打开 deck 文件，确定节点数
    ifstream in(f_deck);
    if (!in) {
        cout << "网表文件打开失败！" << endl;
        return false;
    }
    unordered_map<string, node*> netlist;
    bool sim_type; // 仿真类型，0：DC仿真，1：瞬态仿真
    string line;
    while (getline(in,line)) {
        stringstream sstr(line);
        string dev;
        sstr >> dev;
        if (dev[0] == '*') {
            continue;
        }
        if (dev[0] == '.') {
            if (dev.substr(0,3) == ".op") {
                sim_type = false;
                break;
            }
            else if (dev.substr(0,5) == ".tran") {
                sim_type = true;
                break;
            }
            else {
                continue;
            }
        }
        string node1_name, node2_name;
        f64 val ;
        sstr >> node1_name >> node2_name >> val ;
        // 过滤小电阻
        // if ( val < 1e-6 && val!=0) {
        //     cout << node1_name <<endl ;
        //     cout << node2_name <<endl ;
        //     cout << val << endl ;
        //     continue;
        // }
        if (netlist.find(node1_name) == netlist.end()) {
            node *node1 = new node();
            node1->name = node1_name;
            node1->link_nums = 1;
            netlist.emplace(node1_name, node1);
        }
        else {
            ++ netlist[node1_name]->link_nums;
        }
        if (netlist.find(node2_name) == netlist.end()) {
            node *node2 = new node;
            node2->name = node2_name;
            node2->link_nums = 1;
            netlist.emplace(node2_name, node2);
        }
        else {
            ++ netlist[node2_name]->link_nums;
        }
    }
    in.close();

    // 2. netlist 分配空间
    for (auto iter = netlist.begin(); iter != netlist.end(); ++iter) {
        node *node1 = iter->second;
        if (iter->first == "0") {
            node1->name = "G";
            node1->link_nodes = new node*[256]; // 防止 8位的 link_ptr 溢出会导致内存访问越界，从而内存释放会出问题
            node1->link_types = new int8[256];
            node1->link_values = new f64[256];
        }
        else {
            uint8 link_nums = node1->link_nums;
            node1->link_nodes = new node*[link_nums];
            node1->link_types = new int8[link_nums];
            node1->link_values = new f64[link_nums];
        }
        node1->node2mtx = -1;
        node1->link_nums = 0;
    }

    // 3. 存储结果 & 根据仿真类型解析网表
    mycqpg->netlist = &netlist;
    mycqpg->sim_type = sim_type;
    if (!sim_type) {    // 直流仿真
        if (!cqpg_deck_parse_dc(f_deck, mycqpg)) {
            return false;
        }
    }
    else {  // 瞬态仿真
        if (!cqpg_deck_parse_tran(f_deck, mycqpg)) {
            return false;
        }
    }

    return true;
}

