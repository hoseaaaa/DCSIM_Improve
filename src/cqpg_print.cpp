#include <fstream>
#include <iomanip>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include "malloc.h"

#include "../inc/cqpg.h"

bool cqpg_print (
    cqpg  *&mycqpg,
    string &ofile,
    string &ofile_x
) {
    
    if (!mycqpg->sim_type) { // DC
        // 1. 输出结果
        f64     *&xt            = mycqpg->spmtx_dc->xt;
        // int32   n               = mycqpg->spmtx_dc->n;
        int32   nodes_nums      = mycqpg->nodes_nums;
        node    **&nodes_addrs  = mycqpg->nodes_addrs;
        ofstream out(ofile);
        for (int i = 0; i < nodes_nums; ++i) {
            node *node1 = nodes_addrs[i];
            out << node1->name << " ";
            out << scientific << setprecision(5);
            if (node1->node2mtx == -1) {
                out << node1->solutions << endl;
            }
            else {
                out << xt[node1->node2mtx] << endl;
            }
        }
        out.close();
        ofstream outx(ofile_x);
        for (int i = 0; i < mycqpg->spmtx_dc->n; ++i) {
            outx << scientific << setprecision(5);
                outx << xt[i] << endl;
        }
        outx.close();
        // 2. 删除内存
        delete [] xt; xt = nullptr;
        for (int i = 0; i < nodes_nums; ++i) {
            delete nodes_addrs[i]; nodes_addrs[i] = nullptr;
        }
        delete [] nodes_addrs; nodes_addrs = nullptr;
    }
    else { // TR

    }

    return true;
}

bool cqpg_verify (
    string &ofile,
    string &f_solution
) {

    f64 max_err = 0.;
    f64 avg_err = 0.;
    string max_name;

    unordered_map<string, f64> cqpg_solution;
    string line;
    ifstream in;

    in.open(ofile);
    while (getline(in, line)) {
        stringstream sstr(line);
        string name;
        f64 val;
        sstr >> name >> val;
        cqpg_solution.emplace(name, val);
    }
    in.close();

    in.open(f_solution);
    while (getline(in, line)) {
        stringstream sstr(line);
        string name;
        f64 val;
        sstr >> name >> val;
        if (cqpg_solution.find(name) == cqpg_solution.end()) {
            cout << "文件验证失败，元件不全！" << endl;
            return false;
        }
        val = cqpg_solution[name] - val;
        if (val < 0) val = 0 - val;
        if (val > max_err) { max_err = val; max_name = name; }
        avg_err += val;
    }
    in.close();
    avg_err /= cqpg_solution.size();

    cout.setf(ios::right);
    cout << setprecision(6);
    cout << "Max     Error = " << setw(10) << max_err*1000. << " mv (" << max_name << ")" << endl;
    cout << "Average Error = " << setw(10) << avg_err*1000. << " mv" << endl;

    unordered_map<string, f64>().swap(cqpg_solution); // 删除哈希表，释放内存
    malloc_trim(0);

    return true;
}
