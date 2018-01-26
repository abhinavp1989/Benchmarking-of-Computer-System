#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<memory.h>

# define file_size  838860800		//File Size 800MB

unsigned long long int file_pos=0;
char fileArray[file_size],temp[file_size],A[file_size];
unsigned long long int block_size=0,operation_div=0,operation_div1=0;
//lc, buffer_size;
//struct timeval t;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;		//Used to Avoid deadlocks by threads


//Thread function for sequential read and write
void *memory_read_write(void *input){
	//printf("%llu",operation_div);
	pthread_mutex_lock( &mutex1 );
	unsigned long long int i;
	for(i = file_pos ; i<operation_div ; i+=block_size)

		memcpy(&temp[i],&fileArray[i],block_size);
	//printf("Abhi");

	//for (unsigned long long int j=419430395 ; j<419430405 ; j++)
	//	printf("%c",temp[j]);
	
	//printf("/n");
	operation_div +=operation_div1;

	file_pos =i;
	pthread_mutex_unlock( &mutex1 );

}

//Thread Function for sequential write
void *write_sequential(void *input){
	//printf("%llu",operation_div);
	pthread_mutex_lock( &mutex1 );
	unsigned long long int i;
	for(i = file_pos ; i<operation_div ; i+=block_size)

		memset(temp+i,'A',block_size);
	//printf("Abhi");

	//for (unsigned long long int j=419430395 ; j<419430405 ; j++)
	//	printf("%c",temp[j]);
	
	//printf("/n");
	operation_div +=operation_div1;

	file_pos =i;
	pthread_mutex_unlock( &mutex1 );

}
//Thread function for random write
void *write_random(void *input){
	//printf("%llu",operation_div);
	pthread_mutex_lock( &mutex1 );									//lock indicates no other thread can acess the following things
	unsigned long long int i;
	for(i = file_pos ; i<operation_div ; i+=block_size){

		unsigned long int x =file_pos + rand()% 500;				//rand() function to generate random number
		memset(temp+x,'A',block_size);
	//printf("Abhi");
	}
	//for (unsigned long long int j=419430395 ; j<419430405 ; j++)
	//	printf("%c",temp[j]);
	
	//printf("/n");
	operation_div +=operation_div1;

	file_pos =i;
	pthread_mutex_unlock( &mutex1 );								//unlock the functions to be used by other threads

}
//Function for thread declaration of read and write acess
int read_write_memory(int count_thread){

	operation_div = file_size/count_thread;
	operation_div1 = operation_div;

	pthread_t threads[count_thread];
	clock_t begin = clock();

	for(int j=0;j<count_thread;j++)
		pthread_create(&threads[j], NULL, &memory_read_write, (void *)&threads[j]); //Thread Creation
	
	for(int k=0;k<count_thread; k++)
		pthread_join(threads[k], NULL);				//Join all thread to the main function or wait for all threads to complete before proceding further

	clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\nTime Spent=%lf",time_spent);
    printf("\nThroughput for read_write_memory = %lf MB/s \n",(file_size/time_spent)/(1024*1024));


}
//Function for thread declaration of sequential write acess
int sequential_write(int count_thread){

	operation_div = file_size/count_thread;
	operation_div1 = operation_div;

	pthread_t threads[count_thread];

	clock_t begin = clock();

	for(int j=0;j<count_thread;j++)
		pthread_create(&threads[j], NULL, &write_sequential, (void *)&threads[j]); //Thread Creation
	
	for(int k=0;k<count_thread; k++)
		pthread_join(threads[k], NULL);				//Join all thread to the main function or wait for all threads to complete before proceding further

	clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\nTime Spent=%lf",time_spent);
    printf("\nThroughput for Sequential Write on Memory = %lf MB/s\n",(file_size/time_spent)/(1024*1024));


}
//Function for thread declaration of random write access
int random_write(int count_thread){

	operation_div = file_size/count_thread;
	operation_div1 = operation_div;

	pthread_t threads[count_thread];

	clock_t begin = clock();

	for(int j=0;j<count_thread;j++)
		pthread_create(&threads[j], NULL, &write_random, (void *)&threads[j]); //Thread Creation
	
	for(int k=0;k<count_thread; k++)
		pthread_join(threads[k], NULL);				//Join all thread to the main function or wait for all threads to complete before proceding further

	clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\nTime Spent=%lf",time_spent);
    printf("\nThroughput for Random Write on Memory = %lf MB/s\n",(file_size/time_spent)/(1024*1024));


}


//Function to create a buffer file of 800MB
void create_file(){
	for (long long int i=0;i<file_size;i++){
		if(i<file_size/2)
			fileArray[i]='A';
		else
			fileArray[i]='S';
	}

}
//Function to calculate the latency in microseconds by taking 10 samples of time
void latency_calc(){

	//char A[1000000];
	float samples[10];
	float avg=0;
	for(int j=0;j<10;j++){

		clock_t begin1 = clock();
		//printf("Abhi");
		for (unsigned long int i=0;i<file_size;i+=block_size){

			memcpy(&A[i],&fileArray[i],block_size);
		}
		clock_t end1 = clock();

    	double time_spent = (double)(end1 - begin1) / CLOCKS_PER_SEC;
    	printf("\nTime Spent for sample %d=%lf",j+1,time_spent);
    	samples[j]=time_spent;
	}
    for(int j=0;j<10;j++){
    	avg+=samples[j];
    }
    printf("\nAverage Time=%lf",avg/10);

    printf("\nLatency for block size of 8 byte =%lf microseconds\n",((avg/10)/(file_size)*1000000));

}


//Main function of execution
int main(){

	int thread_count=0,ch,op,size1;
	int share_op;
	printf("Perform Memory Benchmarking on \n1. 1 Thread\n2. 2 Thread\n3. 4 Thread\n4. 8 Thread\n");
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
		printf("Invalid Choice\nProgram terminated\n");
		return 0;		
	}

	printf("\nChoose Operation to be performed \n1. Read and Write \n2. Sequential Write\n3. Random Write\n");
	
	scanf("%d",&op);
	if(op<1 && op>3)
	{
		printf("Invalid Choice\n Program Terminated");
		return 0;
	}
	printf("Choose an option of  block size\n1. 8 Byte\n2. 8 Kilobyte\n3. 8 Megabyte\n4.80 Megabyte\n");
	scanf("%d",&size1);
	if(op<1 && op>4)
	{
		printf("Invalid Choice\n Program Terminated\n");
		return 0;
	}
	else if(size1==1)
		block_size=8;
	else if (size1==2)
		block_size=8*1024;
	else if(size1==3)
		block_size=8*1024*1024;

	else if(size1==4)
		block_size=80*1024*1024;
	//total =size;
	//printf("total%llu",total);
	//printf("%llu",size);
	create_file();
	printf("Memory Benchmark Execution based on operation choosed,number of threads and block size\n");
	

	if (op==1)
		read_write_memory(thread_count);
	else if(op==2)
		sequential_write(thread_count);
	else if(op==3)
		random_write(thread_count);

	if(block_size!=8)
		printf("Note:Latency is only calculated for block size of 8 bytes\nRun a program again with block size 8 bytes\n");
	else{
		printf("Latency Calculation for 10 samples of seconds");
		latency_calc();
	}



}