#include <iostream>
#include <fstream>
#include <vector>
#include "../inc/cqpg.h"
using namespace std ; 

// 文件解析输出

bool y_para_output(cqpg  *&mycqpg, string Gfile,string Utfile) {
    // 打开一个文件用于输出
	ofstream outputFile;
    outputFile.open(Gfile) ;
    
    // 检查文件是否成功打开
    if (!outputFile.is_open()) {
        std::cerr << "Error: Failed to open outputG file." << std::endl;
        return false;
    }
    //维度 nxn
    outputFile << "(" << mycqpg->spmtx_dc->n << ","  << mycqpg->spmtx_dc->n << ")   "; 
    outputFile << mycqpg->spmtx_dc->G_nnz << endl ; 
    
    // 输出变量的值到文件
    int32   *G_rowptr    = mycqpg->spmtx_dc->G_rowptr;
    int32   *G_colidx    = mycqpg->spmtx_dc->G_colidx;
    f64     *G_value     = mycqpg->spmtx_dc->G_value ;

    // outputFile << "G_rowptr: ";
    int size_of_G_row = mycqpg->spmtx_dc->n ;
    int size_of_G_colidx = mycqpg->spmtx_dc->G_nnz ;
    int size_of_G_value = mycqpg->spmtx_dc->G_nnz ;

    for (int i = 0;  i <= size_of_G_row ; ++i) {
        outputFile << G_rowptr[i] << " ";
    }
    outputFile << std::endl;

    // outputFile << "G_colidx: ";
    for (int i = 0; i < size_of_G_colidx; ++i) {
        outputFile << G_colidx[i] << " ";
    }
    outputFile << std::endl;

    // outputFile << "G_value: ";
    for (int i = 0; i < size_of_G_value; ++i) {
        outputFile << G_value[i] << " ";
    }
    outputFile << std::endl;
    outputFile.close();

    outputFile.open(Utfile) ;
    int size_of_ut = mycqpg->spmtx_dc->n ;
    f64     *ut    = mycqpg->spmtx_dc->ut ;
    outputFile << "ut: " << "(" << size_of_ut << ", 1)" ;
    outputFile << std::endl;
    
    for (int i = 0; i < size_of_ut; ++i) {
        outputFile << ut[i] << endl;
    }
    outputFile << std::endl;
    outputFile.close();

    return true;
}
