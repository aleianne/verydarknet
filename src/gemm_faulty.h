#ifndef GEMM_FAULTY_H 
#define GEMM_FAULTY_H 

#include "darknet.h"
#include "transition_fault_injector.h"

void gemm_nn_faulty2(transition_fault fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc);


void gemm_nt_faulty(fc_transition_fault fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc);

void gemm_nn_faulty(transition_fault fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc);

void gemm_nn_faulty_seu(seu_fault fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc);        

#endif