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
    float result;
    #pragma omp parallel for
    for(i = 0; i < M; ++i){
        for(j = 0; j < N; ++j){
            register float sum = 0;
            for(k = 0; k < K; ++k){
               result = ALPHA*A[i*lda+k]*B[j*ldb + k];
               sum += inject_fc_transition_fault(fault, i, j, k, result);
            }
            C[i*ldc+j] += sum;
        }
    }
}

void gemm_nt_faulty_stuck_at(stuck_at_fault fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc)
{
    int i,j,k;
    float result;
   
    #pragma omp parallel for
    for(i = 0; i < M; ++i){
        for(j = 0; j < N; ++j){
            register float sum = 0;
            for(k = 0; k < K; ++k){
               result = ALPHA*A[i*lda+k]*B[j*ldb + k];
               sum += compute_faulty_multiplication(result, fault.bit, fault.type);
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
    float faulty_value;
    #pragma omp parallel for
    for(i = 0; i < M; ++i){
        for(k = 0; k < K; ++k){
            register float A_PART = ALPHA*A[i*lda+k];
            for(j = 0; j < N; ++j){
                faulty_value = A_PART*B[k*ldb+j];
                faulty_value = inject_transition_fault(fault, i, k, j, faulty_value);
                C[i*ldc+j] += faulty_value;
            }
        }
    }
}

void gemm_nn_faulty2(transition_fault fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc) {

            // delete the correct value from the matrix
            fault_value f = delete_correct_value(B, fault.output_neuron, fault.weight, ldb);

            int i,j,k;
            #pragma omp parallel for
            for(i = 0; i < M; ++i){
                for(k = 0; k < K; ++k){
                    register float A_PART = ALPHA*A[i*lda+k];
                    for(j = 0; j < N; ++j){
                        C[i*ldc+j] += A_PART*B[k*ldb+j];
                    }
                }
            }

            for (i = 0; i < M; ++i) {
                if (i == fault.filter) {
                    float faulty_value = inject_mul_fault(f, A, lda, fault.weight, fault.filter, fault.bit); 
                    C[i*ldc+fault.output_neuron] += faulty_value;
                } else {
                    C[i*ldc+fault.output_neuron] += A[i*lda+fault.weight] * f.actual_value;
                }
            }

	   B[fault.filter*ldb +fault.weight] = f.actual_value;
    }

void gemm_nn_faulty_seu(seu_fault fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc)
{
    int i,j,k;

    weight_fault weight_f = {0};
    save_correct_value(A, lda, fault.filter, fault.weight, fault.bit, &weight_f);
    
    #pragma omp parallel for
    for(i = 0; i < M; ++i){
        for(k = 0; k < K; ++k){
            register float A_PART = ALPHA*A[i*lda+k];
            for(j = 0; j < N; ++j){    
                C[i*ldc+j] += A_PART*B[k*ldb+j];   
            }
        }
    }

    for (i = 0; i < N; ++i) {
        if (i < fault.output_neuron || i > fault.output_neuron_end) {
            C[fault.filter*ldc + i] += weight_f.correct_value * B[fault.weight * ldb + i]; 
        } else {
            C[fault.filter*ldc + i] += weight_f.faulty_value *  B[fault.weight * ldb + i];
        }
    }

    B[fault.weight * lda + fault.filter] = weight_f.correct_value;
}

void gemm_nn_faulty_stuck_at(stuck_at_fault fault, int M, int N, int K, float ALPHA,
			     float *A, int lda,
			     float *B, int ldb, 
			     float *C, int ldc) {
    int i,j,k;
    float tmp;
    #pragma omp parallel for
    for(i = 0; i < M; ++i){
        for(k = 0; k < K; ++k){
            register float A_PART = ALPHA*A[i*lda+k];
            for(j = 0; j < N; ++j){
	        tmp = A_PART*B[k*ldb+j];
		C[i*ldc+j] += compute_faulty_multiplication(tmp, fault.bit, fault.type);
                //C[i*ldc+j] += A_PART*B[k*ldb+j];   
            }
        }
    }
}
