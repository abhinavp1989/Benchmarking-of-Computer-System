#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>


# define file_size  671088640		//File Size 640MB //Choose small file size in the multiples of 80 MB to run quickly in local machine

int thread_count,block_size;
unsigned long long int operation_div1=0,operation_div2=0,file_pos1=0;
int sock1 = 0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;


void *send_data_UDP(void *input){
	
	char *buffer1 =(char *)malloc(8*sizeof(char));
	pthread_mutex_lock( &mutex1 );
	unsigned long long int x;
	for(x = file_pos1 ; x<operation_div1 ; x+=8){

		memset(buffer1,'0',8);
		send(sock1 , buffer1 , 8 , 0 );
    	//printf("%s\n",buffer1 );
    	
	}
	operation_div1 +=operation_div2;
	file_pos1 =x;
	pthread_mutex_unlock( &mutex1 );

}
void *send_data_TCP(void *input){
	
	char *buffer1 =(char *)malloc(8*sizeof(char));
	pthread_mutex_lock( &mutex1 );
	unsigned long long int x;
	for(x = file_pos1 ; x<operation_div1 ; x+=8){

		memset(buffer1,'0',8);
		send(sock1 , buffer1 , 8 , 0 );
    	//printf("%s\n",buffer1 );
    	
	}
	operation_div1 +=operation_div2;
	file_pos1 =x;
	pthread_mutex_unlock( &mutex1 );

}

int TCP_loopback_1(int th){
	
    struct sockaddr_in server_address;
    char buffer[1024] = {0};

    if ((sock1 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&server_address, '0', sizeof(server_address));
  
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock1, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    pthread_t threads[thread_count];
    operation_div1 = file_size/thread_count;
	operation_div2 = operation_div1;

	clock_t begin = clock();

	for(int j=0;j<thread_count;j++)
		pthread_create(&threads[j], NULL, &send_data_TCP, (void *)&threads[j]); //Thread Creation
	
	for(int k=0;k<thread_count; k++)
		pthread_join(threads[k], NULL);				//Join all thread to the main function or wait for all threads to complete before proceding further
	close(sock1);

	clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\nTime Spent=%lf",time_spent);
    printf("\nThroughput for Network = %lf MB/s\n",(file_size/time_spent)/(1024*1024));
    printf("\nLatency for block size of 8 byte =%lf microseconds\n",((time_spent)/(file_size)*1000000));

    
    return 0;
    
}
int UDP_loopback_1(int th){
	
    struct sockaddr_in server_address;
    if ((sock1 = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&server_address, '0', sizeof(server_address));
  
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5000);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock1, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }


    pthread_t threads[thread_count];
    operation_div1 = file_size/thread_count;
	operation_div2 = operation_div1;

	clock_t begin = clock();

	for(int j=0;j<thread_count;j++)
		pthread_create(&threads[j], NULL, &send_data_UDP, (void *)&threads[j]); //Thread Creation
	
	for(int k=0;k<thread_count; k++)
		pthread_join(threads[k], NULL);				//Join all thread to the main function or wait for all threads to complete before proceding further
	close(sock1);
	clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\nTime Spent=%lf",time_spent);
    printf("\nThroughput for Network = %lf MB/s\n",(file_size/time_spent)/(1024*1024));
    printf("\nLatency for block size of 8 byte =%lf microseconds\n",((time_spent)/(file_size)*1000000));

    
    return 0;
    
}


int main(){

	int ch,op,size1;
	int share_op;
	printf("Perform Network Benchmarking on \n1. 1 Thread\n2. 2 Thread\n3. 4 Thread\n4. 8 Thread\n");
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

	printf("Perform Network Benchmarking on \n1. TCP Stack\n2. UDP\n");
	printf("Note:If server and cient are on different system tr to keep the respective files in those systems\n");
	scanf("%d",&op);
	if(op<0 || op>2){
		printf("Invalid Choice");
		return 0;
	}

	printf("Disk Benchmark Execution based on operation choosed,number of threads\n");
	

	if (op==1)
		TCP_loopback_1(thread_count);
	else if(op==2)
		UDP_loopback_1(thread_count);

	if(block_size!=8*1024)
    	printf("\nNOTE:Latency will be calculated only for block size of 8 Kilobyte\n");

	

    return 0;

}