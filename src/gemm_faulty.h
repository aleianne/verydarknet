#ifndef GEMM_FAULTY_H 
#define GEMM_FAULTY_H 

#include "darknet.h"
#include "fault_injector.h"
#include "transition_fault_injector.h"
#include "transient_fault_injector.h"
#include "stuck_at_fault_injector.h"

void gemm_nn_faulty2(void *fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc);

void gemm_nt_faulty(void *fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc);

void gemm_nn_faulty(void *fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc);

void gemm_nn_faulty_seu(void *fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc);     

void gemm_nn_faulty_stuck_at(void *fault, int M, int N, int K, float ALPHA,
	float *A, int lda,
	float *B, int ldb, 
        float *C, int ldc); 

void gemm_nt_faulty_stuck_at(void *fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc);  

#endif
