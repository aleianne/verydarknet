#include "gemm_faulty.h"

void gemm_nt_faulty(void *fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc)
{
    fc_transition_fault f_parsed = *((fc_transition_fault *) fault);
    int i,j,k;
    float result;
    #pragma omp parallel for
    for(i = 0; i < M; ++i){
        for(j = 0; j < N; ++j){
            register float sum = 0;
            for(k = 0; k < K; ++k){
               result = ALPHA*A[i*lda+k]*B[j*ldb + k];
               sum += inject_fc_transition_fault(f_parsed, i, j, k, result);
            }
            C[i*ldc+j] += sum;
        }
    }
}

void gemm_nt_faulty_stuck_at(void *fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc)
{
    stuck_at_fault f_parsed = *((stuck_at_fault *) fault);
    int mul_counter = f_parsed.fault_location;
    /*int counter1 = 0; int counter2 = 0;
    printf("total number of multiplication %d\n", K * M * N);
    printf("the fault location is %d, the stuck-at type is %d, the bit position is %d\n", mul_counter, f_parsed.type, f_parsed.bit);*/
    int i,j,k;
    float result;
    #pragma omp parallel for
    for(i = 0; i < M; ++i){
        for(j = 0; j < N; ++j){
            register float sum = 0;
            for(k = 0; k < K; ++k){
                result = ALPHA*A[i*lda+k]*B[j*ldb + k];
                mul_counter--;
                //counter2++;
                if (mul_counter == 0) {
                    float tmp = compute_faulty_multiplication(result, f_parsed.bit, f_parsed.type);
                    sum += tmp;
                    /*counter1++;
                    printf("the correct result is %f the faulty one is %f\n", result, tmp);
                    printf("the counter for moltiplication is %d, the counter for the mac injection is %d\n", counter2, counter1);*/
                    mul_counter = MAC_UNIT_N;
                } else {
                    sum += result;
                }    
            }
            C[i*ldc+j] += sum;
        }
    }
}

void gemm_nn_faulty(void *fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc)
{
    int i,j,k;
    float faulty_value;
    transition_fault f_parsed = *((transition_fault *) fault);
    #pragma omp parallel for
    for(i = 0; i < M; ++i){
        for(k = 0; k < K; ++k){
            register float A_PART = ALPHA*A[i*lda+k];
            for(j = 0; j < N; ++j){
                faulty_value = A_PART*B[k*ldb+j];
                faulty_value = inject_transition_fault(f_parsed, i, k, j, faulty_value);
                C[i*ldc+j] += faulty_value;
            }
        }
    }
}

void gemm_nn_faulty2(void *fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc) 
{
    transition_fault f_parsed = *((transition_fault *) fault);
    // delete the correct value from the matrix
    fault_value f = delete_correct_value(B, f_parsed.output_neuron, f_parsed.weight, ldb);
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
            if (i == f_parsed.filter) {
                float faulty_value = inject_mul_fault(f, A, lda, f_parsed.weight, f_parsed.filter, f_parsed.bit); 
                C[i*ldc+f_parsed.output_neuron] += faulty_value;
            } else {
                C[i*ldc+f_parsed.output_neuron] += A[i*lda+f_parsed.weight] * f.actual_value;
            }
        }        
	   B[f_parsed.filter*ldb +f_parsed.weight] = f.actual_value;
    }

void gemm_nn_faulty_seu(void *fault, int M, int N, int K, float ALPHA, 
        float *A, int lda, 
        float *B, int ldb,
        float *C, int ldc)
{
    seu_fault f_parsed = *((seu_fault *) fault);
    int i,j,k;
    weight_fault weight_f = {0};
    save_correct_value(A, lda, f_parsed.filter, f_parsed.weight, f_parsed.bit, &weight_f);
    
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
        if (i < f_parsed.output_neuron || i > f_parsed.output_neuron_end) {
            C[f_parsed.filter*ldc + i] += weight_f.correct_value * B[f_parsed.weight * ldb + i]; 
        } else {
            C[f_parsed.filter*ldc + i] += weight_f.faulty_value *  B[f_parsed.weight * ldb + i];
        }
    }

    B[f_parsed.weight * lda + f_parsed.filter] = weight_f.correct_value;
}

void gemm_nn_faulty_stuck_at(void *fault, int M, int N, int K, float ALPHA,
			     float *A, int lda,
			     float *B, int ldb, 
			     float *C, int ldc) {

    //  printf("the bit position is %d\n", fault.bit);
    //  printf("the stuck-at type is %d\n", fault.type);
    
    stuck_at_fault f_parsed = *((stuck_at_fault *) fault);
    int mul_counter = f_parsed.fault_location;

    // only for debug
   // fprintf(stderr, "the fault injected is type %d, the position is %d, the bit is %d", f_parsed.type, f_parsed.fault_location, f_parsed.bit);

    //int counter1 = 0; int counter2 = 0;
    /*
    printf("total number of multiplication %d\n", K * M * N);
    printf("the fault location is %d, the bit position is %d, the stuck-at type is %d \n", mul_counter, f_parsed.bit, f_parsed.type);*/
    int i,j,k;
    float tmp;
    #pragma omp parallel for
    for(i = 0; i < M; ++i){
        for(k = 0; k < K; ++k){
            register float A_PART = ALPHA*A[i*lda+k];
            for(j = 0; j < N; ++j){
	            tmp = A_PART*B[k*ldb+j];
                if (mul_counter == 1) {
                    float tmp2 = compute_faulty_multiplication(tmp, f_parsed.bit, f_parsed.type);
                    C[i*ldc+j] += tmp2;
                    //counter2++;
                    //printf("the correct result is %f the faulty one is %f\n", tmp, tmp2);
                    //printf("the counter for moltiplication is %d, the counter for the mac injection is %d\n", counter1, counter2);
                    mul_counter = MAC_UNIT_N;
                } else {
                    C[i*ldc+j] += tmp;
                }
                //C[i*ldc+j] += A_PART*B[k*ldb+j];   
               // mul_counter--;
               // counter1++;
            }
        }
    }
}
