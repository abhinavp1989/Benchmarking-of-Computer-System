#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<fcntl.h>
#include <unistd.h>
# define file_size  671088640 //800 MB   

int thread_count=0;
int server_file_des, new_socket, bytes_read;
char temp[file_size];
unsigned long long int block_size=0,operation_div3=0,operation_div4=0,file_pos=0,bytes=0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void *receive_data(void *input){
	//printf("%llu",operation_div);
	pthread_mutex_lock( &mutex1 );
	char *buffer =(char *)malloc(8*sizeof(char));
	
	unsigned long long int i;
	for(i = file_pos ; i<operation_div3 ; i+=8){

		bytes_read = read( server_file_des , buffer, 8);
    	bytes+=bytes_read;
    	printf("\nBytes Transferred %llu",bytes);

	}
	
	operation_div3 +=operation_div4;
	file_pos =i;
	pthread_mutex_unlock( &mutex1 );

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
    
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    //char *hello = "Hello from server";
      
    // Creating socket file descriptor
    if ((server_file_des = socket(AF_INET, SOCK_DGRAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the port 8080
    /*if (setsockopt(server_file_des, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }*/
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( 5000 );
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_file_des, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
   
    pthread_t threads[thread_count];
    operation_div3 = file_size/thread_count;
	operation_div4 = operation_div3;

	

	for(int j=0;j<thread_count;j++)
		pthread_create(&threads[j], NULL, &receive_data, (void *)&threads[j]); //Thread Creation
	
	for(int k=0;k<thread_count; k++)
		pthread_join(threads[k], NULL);				//Join all thread to the main function or wait for all threads to complete before proceding further

	//close(server_file_des);
    
    return 0;
}