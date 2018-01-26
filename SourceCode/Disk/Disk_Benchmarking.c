#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<memory.h>
#include<fcntl.h>
#include <unistd.h>

# define file_size  671088640			//640 MB

unsigned long long int block_size=0,operation_div1=0,pos=0,curr_pos=0;
int thread_count=0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;				//Used to avoid deadlock between threads
int f1,f2;
int file_pos = 0;

//Thread function for read+write
void *disk_read_write(void *input){
	
	pthread_mutex_lock( &mutex1 );
	
	char * buffer = (char *)malloc(block_size * sizeof(char));		//Creating a buffer of block size
    
    for (unsigned long int i = 0; i < ((file_size/block_size)/thread_count); i++) {
        memset(buffer, '0', block_size);	
        write(f1, buffer, block_size);
        
    }
	free(buffer);
	pos  = lseek(f1, pos, SEEK_SET);
	//printf("%llu\n",pos);
	unsigned long int i=0;
	for (i = 0; i < (file_size / block_size)/thread_count; i++) {
		
		
        read(f1, buffer, block_size);
    }
    curr_pos = pos + i*block_size;
    //printf("curr_pos %llu pos obtained ",curr_pos);
    pos=lseek(f1,curr_pos,SEEK_SET);
    //printf("%llu\n",pos);
    
	pthread_mutex_unlock( &mutex1 );

}
//Thread function for sequential read
void * disk_read_sequential(void * arg)
{
    pthread_mutex_lock( &mutex1 );
    char * buffer = (char *)malloc(block_size * sizeof(char));
  
    for (unsigned long int i = 0; i < (file_size/ block_size)/thread_count; i++) {
        read(f2, buffer, block_size);
    }
   
    free(buffer);
    pthread_mutex_unlock( &mutex1 );
    
    
}
//Thread function for random read
void * disk_read_random(void * arg)
{
    pthread_mutex_lock( &mutex1 );
    char * buffer = (char *)malloc(block_size * sizeof(char));
  
    for (unsigned long int i = 0; i < (file_size/ block_size)/thread_count; i++) {
    	file_pos = (rand() % ((file_size / block_size)/thread_count) * block_size); //randomly generating a position or offset
        lseek(f2, file_pos, SEEK_SET);												//moving cursor to position generated above
        read(f2, buffer, block_size);												//reading the content
    }
   
    free(buffer);
    pthread_mutex_unlock( &mutex1 );
    
    
}
//Function for read+write
int read_write_disk(int count_thread){

	pthread_t threads[count_thread];
	clock_t begin = clock();
	if ((f1 = open("test.bin", O_CREAT|O_TRUNC|O_WRONLY|O_APPEND, 0666)) < 0) {
        printf("File open failed.\n");
        exit(-1);
    }


	for(int j=0;j<count_thread;j++)
		pthread_create(&threads[j], NULL, &disk_read_write, (void *)&threads[j]); //Thread Creation
	
	for(int k=0;k<count_thread; k++)
		pthread_join(threads[k], NULL);				//Join all thread to the main function or wait for all threads to complete before proceding further

	close(f1);
	clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\nTime Spent=%lf",time_spent);
    printf("\nThroughput for read_write_disk = %lf MB/s \n",(file_size/time_spent)/(1024*1024));
    if(block_size==8){
    	printf("\nLatency for block size of 8 byte =%lf microseconds\n",((time_spent)/((file_size))*1000000));
    	
    }


}
//Function for creating a binary file
void create_file(){
	f1=0;
	if ((f1 = open("test.bin", O_CREAT|O_TRUNC|O_WRONLY|O_APPEND, 0666)) < 0) {
        printf("File open failed.\n");
        exit(-1);
    }
    char * buffer = (char *)malloc(block_size * sizeof(char));
    
    for (unsigned long int i = 0; i < ((file_size/block_size)); i++) {
        memset(buffer, '0', block_size);									//Copying 0 to buffer of specified block size
        write(f1, buffer, block_size);										//Writing buffer data to disk
        
    }
	free(buffer);
	close(f1);


}
//Function for sequential read
int read_disk_seq(int count_thread){

	
	printf("\nCreating a file for reading\n");
	create_file();
	printf("\nStarting Benchmarking\n");
	clock_t begin = clock();

	if ((f2 = open("test.bin", O_RDONLY, 0666)) < 0) {
        printf("File open failed.\n");
        exit(-1);
    }

    pthread_t threads[count_thread];
	for(int j=0;j<count_thread;j++)
		pthread_create(&threads[j], NULL, &disk_read_sequential, (void *)&threads[j]); //Thread Creation
	
	for(int k=0;k<count_thread; k++)
		pthread_join(threads[k], NULL);				//Join all thread to the main function or wait for all threads to complete before proceding further

	close(f2);
	clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\nTime Spent=%lf",time_spent);
    printf("\nThroughput for read_disk_sequential = %lf MB/s \n",(file_size/time_spent)/(1024*1024));
    if(block_size==8){
    	printf("\nLatency for block size of 8 byte =%lf microseconds\n",((time_spent)/((file_size))*1000000));
    	
    }


}
//Function for random read
int read_disk_ran(int count_thread){

	
	printf("\nCreating a file for reading\n");
	create_file();
	printf("\nStarting Benchmarking\n");
	clock_t begin = clock();

	if ((f2 = open("test.bin", O_RDONLY, 0666)) < 0) {
        printf("File open failed.\n");
        exit(-1);
    }

    pthread_t threads[count_thread];
	for(int j=0;j<count_thread;j++)
		pthread_create(&threads[j], NULL, &disk_read_random, (void *)&threads[j]); //Thread Creation
	
	for(int k=0;k<count_thread; k++)
		pthread_join(threads[k], NULL);				//Join all thread to the main function or wait for all threads to complete before proceding further

	close(f2);
	clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\nTime Spent=%lf",time_spent);
    printf("\nThroughput for read_disk_random = %lf MB/s \n",(file_size/time_spent)/(1024*1024));
    if(block_size==8){
    	printf("\nLatency for block size of 8 byte =%lf microseconds\n",((time_spent)/((file_size))*1000000));
    	

    }



}

//Main function of execution
int main(){

	int ch,op,size1;
	int share_op;
	printf("Perform Disk Benchmarking on \n1. 1 Thread\n2. 2 Thread\n3. 4 Thread\n4. 8 Thread\n");
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

	printf("\nChoose Operation to be performed \n1. Read and Write \n2. Sequential Read\n3. Random Read\n");
	
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
	
	printf("Disk Benchmark Execution based on operation choosed,number of threads and block size\n");
	

	if (op==1)
		read_write_disk(thread_count);
	else if(op==2)
		read_disk_seq(thread_count);
	else if(op==3)
		read_disk_ran(thread_count);

	if(block_size!=8)
    	printf("\nNOTE:Latency will be calculated only for block size of 8 byte\n");

	



}