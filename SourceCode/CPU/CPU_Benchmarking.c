#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include<immintrin.h>

long OPERATION_COUNT=100000000;

//Structure of thread for floating point opeartion with AVX 
struct FloatOp{

	__m256d a; 
	__m256d b; 
	double share;
	__m256d sum;
	pthread_t thread;	//Datatype for defining a thread similar to int and char

};
//Structure of thread for integer point operation with AVX
struct IntegerOp{

	__m256i a; 
	__m256i b; 
	double share;
	__m256i sum;
	pthread_t thread;
};

//Floating numbers Addition function of Thread 
void *Float_Addition(void *input){

	struct FloatOp *t;
	t = (struct FloatOp *) input;
	
	while(t->share > 0){

		t->sum=_mm256_add_pd(_mm256_add_pd(t->a,t->b),t->sum);
		t->sum=_mm256_add_pd(_mm256_add_pd(t->a,t->b),t->sum);
		t->sum=_mm256_add_pd(_mm256_add_pd(t->a,t->b),t->sum);
		t->sum=_mm256_add_pd(_mm256_add_pd(t->a,t->b),t->sum);
		t->sum=_mm256_add_pd(_mm256_add_pd(t->a,t->b),t->sum);
		t->share--;


	}

	/* Display the elements of the result vector */
  //double* f = (double*)&t->sum;
  //printf("Sum on individual thread %.10f %.10f %.10f %.10f \n",
  //  f[0], f[1], f[2], f[3]);

}

//Integer numbers Addition function of Thread
void *Integer_Addition(void *input1){

	struct IntegerOp *th;
	th = (struct IntegerOp *) input1;
	
	while(th->share > 0){
		th->sum=_mm256_add_epi32(_mm256_add_epi32(th->a,th->b),th->sum);
		th->sum=_mm256_add_epi32(_mm256_add_epi32(th->a,th->b),th->sum);
		th->sum=_mm256_add_epi32(_mm256_add_epi32(th->a,th->b),th->sum);
		th->sum=_mm256_add_epi32(_mm256_add_epi32(th->a,th->b),th->sum);
		th->sum=_mm256_add_epi32(_mm256_add_epi32(th->a,th->b),th->sum);
		th->share--;
	}

	//printf("\nSum of Individual Thread of integer arithmetic is %LF",th->sum);

}


//Calculation of FLOPS and GFlops
double CPUFlops(int count_thread,double op_share){

	
	struct FloatOp F[count_thread];

	for(int i=0;i<count_thread;i++){					//Assigning Each thread member its values
		F[i].a = _mm256_setr_pd(1.00, 1.11, 1.11, 1.11);
		F[i].b = _mm256_setr_pd(1.00, 1.11, 1.11, 1.11);
		F[i].share=op_share;
		F[i].sum = _mm256_set1_pd(0.00);
	}

	clock_t begin = clock();

	for(int j=0;j<count_thread;j++)
		pthread_create(&F[j].thread, NULL, &Float_Addition, (void *)&F[j]); //Thread Creation
	
	for(int k=0;k<count_thread; k++)
		pthread_join(F[k].thread, NULL);				//Join all thread to the main function or wait for all threads to complete before proceding further

	clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    double flops_calc = (OPERATION_COUNT*41UL) / (time_spent);
    
    double GFlops_cal = flops_calc/ (100000000UL);
    printf("\nTime Spent: %lf\n",time_spent);
    printf("\nTotal number of FLOPS %f",flops_calc);
    return(GFlops_cal);


}


//Calculation of IOPS and GIOPS

double CPUIops(int c_thread,double op_sh){


	struct IntegerOp I[c_thread];
	
	for(int i=0;i<c_thread;i++){						//Assigning Each thread member its values
		
		I[i].a = _mm256_set1_epi32(1);
		I[i].b = _mm256_setr_epi32(1, 1, 1, 1, 1, 1, 1, 1);
		I[i].share=op_sh;
		I[i].sum = _mm256_set1_epi32(0);
	}
	
	clock_t begin1 = clock();

	for(int j=0;j<c_thread;j++)
		pthread_create(&I[j].thread, NULL, &Integer_Addition, (void *)&I[j]); //Thread Creation
	
	for(int k=0;k<c_thread; k++)
		pthread_join(I[k].thread, NULL);				//Join all thread to the main function or wait for all threads to complete before proceding further

	clock_t end1 = clock();
    double time_spent = (double)(end1 - begin1) / CLOCKS_PER_SEC;
    double iops_calc = (OPERATION_COUNT*41UL) / (time_spent);
    double GIops_calc = (OPERATION_COUNT*41UL) / (time_spent * 100000000UL); //UL represents unsigned calculation as 10^9 *4 multiplication will produce a value out of range
    
    printf("\nTotal number of IOPS %f",iops_calc);
    return(GIops_calc);

}

//Main function of Execution
int main(){

	int thread_count=0,ch;
	int share_op;
	printf("Perform CPU Benchmarking on \n1. 1 Thread\n2. 2 Thread\n3. 4 Thread\n4. 8 Thread");
	scanf("%d",&ch);
	if (ch==1 || ch==2){
		thread_count=ch;
		printf("Number of Threads %d",thread_count);
	}
	else if(ch==3){
		thread_count=ch+1;
		printf("Number of Threads %d",thread_count);
	}
	else if (ch==4){
		thread_count=ch+ch;
		printf("Number of Threads %d",thread_count);
	}
	else{
		printf("Invalid Choice\nProgram terminated");
		return 0;		
	}

	printf("\nTotal Operations needs to be executed %li\n",OPERATION_COUNT);
	
	printf("CPU Benchmark Execution\n");
	
	share_op = OPERATION_COUNT/thread_count; //Dividing operations with thread count for parallel execution
	
	double GFlops_calc = CPUFlops(thread_count,share_op);	//Calling CPUFlops function to calculate FLOPS/GFLOPS
	double GIops_c = CPUIops(thread_count,share_op);		//Calling CPUIops function to calculate IOPS/GIOPS

	printf("\n\nTotal number of GFLOPS=%f",GFlops_calc);
	printf("\nTotal number of GIOPS=%f",GIops_c);
	
	return 0;

}

