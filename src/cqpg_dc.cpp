#include <iostream>
#include "string.h"
#include <math.h>

#include "../inc/cqpg.h"
#include "../inc/cktso.h"

const string cktso_return [] = {
    "Successful",                   // 0
    "Invalid instance handle",      // -1
    "Argument error",               // -2
    "Invalid matrix data",          // -3
    "Out of memory",                // -4
    "Structurally singular",        // -5
    "Numerically singular"          // -6
};

bool cqpg_do_DC (
    cqpg  *&mycqpg
) {
    matrix_dc *&spmtx_dc = mycqpg->spmtx_dc;
    int32   n            = spmtx_dc->n;
    int32   *&G_rowptr    = spmtx_dc->G_rowptr;
    int32   *&G_colidx    = spmtx_dc->G_colidx;
    f64     *&G_value     = spmtx_dc->G_value;
    f64     *&ut          = spmtx_dc->ut;

    f64* xt = new f64[n];
    memset(xt, 0, sizeof(f64)*n);

    // 1. 求解 G*xt=ut
    ICktSo CKTSO_G = nullptr;
    int ret;
    const long long *oparm = new long long[15];
    int *iparm = new int[14];
    ret = CKTSO_CreateSolver(&CKTSO_G, &iparm, &oparm);
    if (ret < 0) { cout << "CKTSO->CreateSolver 错误! ret = " << cktso_return[0-ret] << endl; return false; }
    iparm[0] = 1; // 启动CKTSO内部定时器
    ret = CKTSO_G->Analyze(n, G_rowptr, G_colidx, G_value, false, 1);
    if (ret < 0) { cout << "CKTSO_G->Analyze 错误! ret = " << cktso_return[0-ret] << endl; return false; }
    ret = CKTSO_G->Factorize(G_value, false);
    if (ret < 0) { cout << "CKTSO_G->Factorize 错误! ret = " << cktso_return[0-ret] << endl; return false; }
    ret = CKTSO_G->Solve(ut, xt, false);
    if (ret < 0) { cout << "CKTSO_G->Solve 错误! ret = " << cktso_return[0-ret] << endl; return false; }

    cout << "n(" << n << "), nnz(" << G_rowptr[n] << "), L+U(" << oparm[5]+oparm[6] << ")" << endl;
    cout << "Analysis  time (" << oparm[0]/1e6 << ") s" << endl;
    cout << "Factorize time (" << oparm[1]/1e6 << ") s" << endl;
    cout << "RHS time (" << oparm[2]/1e6 << ") s" << endl;
    CKTSO_G->DestroySolver();

#ifdef DEBUG
{
    //Calculate residual (L2 norm).
    int *ap = G_rowptr;
    int *ai = G_colidx;
    double *ax = G_value;

    double err = 0.;
    for (int r = 0; r < n; ++r)
    {
        double s = 0.;
        const int start = ap[r];
        const int end = ap[r + 1];
        for (int c = start; c < end; ++c)
        {
            s += ax[c] * xt[ai[c]];
        }
        s -= ut[r];
        err += s * s;
    }
    printf("DC Residual = %g.\n", sqrt(err));
}
#endif

    // 2. 存储结果
    spmtx_dc->xt = xt;

    // 3. 删除 G 矩阵
    delete [] G_rowptr; G_rowptr = nullptr;
    delete [] G_colidx; G_colidx = nullptr;
    delete [] G_value;  G_value  = nullptr;
    delete [] ut;       ut       = nullptr;

    return true;
}
