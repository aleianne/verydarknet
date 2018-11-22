#include "gemm_faulty.h"

void gemm_nt_faulty(fc_transition_fault fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc)
{
    // if we are here for operation that happens into connected layers the parameters meaning is:
    // M is the number of batch
    // N is the number of outputs
    // K is the number of inputs
    int i,j,k;
    #pragma omp parallel for
    for(i = 0; i < M; ++i){
        for(j = 0; j < N; ++j){
            register float sum = 0;
            float sum2;
            for(k = 0; k < K; ++k){
                sum += ALPHA*A[i*lda+k]*B[j*ldb + k];
                sum2 = sum;
                inject_fc_transition_fault(fault, i, j, k, &sum2);
            }
            C[i*ldc+j] += sum;
        }
    }
}

void gemm_nn_faulty(transition_fault fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc)
{
    int i,j,k;
    #pragma omp parallel for
    for(i = 0; i < M; ++i){
        for(k = 0; k < K; ++k){
            register float A_PART = ALPHA*A[i*lda+k];
            for(j = 0; j < N; ++j){                          
                C[i*ldc+j] += A_PART*B[k*ldb+j];
                inject_transition_fault(fault, i, k, j, &C[i*ldc+j]);
            }
        }
    }
}

void gemm_nn_faulty_seu(seu_fault fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc)
{
    int i,j,k;
    #pragma omp parallel for
    for(i = 0; i < M; ++i){
        for(k = 0; k < K; ++k){
            register float A_PART = ALPHA*A[i*lda+k];
            register float faulty_weight = flip_bit(fault.bit, ALPHA*A[i*lda+k]);
            for(j = 0; j < N; ++j){    
                if (j >= fault.output_neuron && k == fault.weight && i == fault.filter) {
                    C[i*ldc+j] += faulty_weight*B[k*ldb+j];
                } else {
                    C[i*ldc+j] += A_PART*B[k*ldb+j];   
                }                      
            }
        }
    }
}
