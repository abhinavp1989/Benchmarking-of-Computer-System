All the Benchmarks has been written in C
------------------
1.CPU Benchmarking
------------------
CPU_Benchmarking.c contains the source code


	Compilation and Execute instructions
	a)Go the directory where the file is present
	b)Compile the file using command : gcc -pthread -mavx2 -o0 -std=c99 -o CPU_Benchmarking CPU_Benchmarking.c
	c)Run the file using command - ./CPU_Benchmarking
	d)Follow the instructions displayed

	Example:
	
[root@pa1-vuzzini-pimpalgaonkar cc]# gcc -pthread -mavx2 -o0 -std=c99 -o CPU_Benchmarking CPU_Benchmarking.c
[root@pa1-vuzzini-pimpalgaonkar cc]# ./CPU_Benchmarking
Perform CPU Benchmarking on
1. 1 Thread
2. 2 Thread
3. 4 Thread
4. 8 Thread1
Number of Threads 1
Total Operations needs to be executed 100000000
CPU Benchmark Execution

Time Spent: 3.040000

Total number of FLOPS 1348684210.526316
Total number of IOPS 1640000000.000000

Total number of GFLOPS=13.486842
Total number of GIOPS=16.400000

---------------------
2.Memory Benchmarking
---------------------
Mem_Benchmarking.c contains the source code

	Compilation and Execute instructions
	a)Go the directory where the file is present
	b)Compile the file using command: gcc -pthread -o0 -std=c99 -o Mem_Benchmarking Mem_Benchmarking.c
	c)Run the file using command: ./Mem_Benchmarking
	d)Follow the instructions displayed
		1.Choose the number of threads
		2.Choose the operation needs to be performed
		3.Choose the block size(Note:Latency will be only calculated for block size of 8 bytes)

	Example:
	Perform Memory Benchmarking on 
	1. 1 Thread
	2. 2 Thread
	3. 4 Thread
	4. 8 Thread
	2
	Number of Threads 2
	Choose Operation to be performed 
	1. Read and Write 
	2. Sequential Write
	3. Random Write
	1
	Choose an option of  block size
	1. 8 Byte
	2. 8 Kilobyte
	3. 8 Megabyte
	4.80 Megabyte
	1
	Memory Benchmark Execution based on operation choosed,number of threads and block size

	Time Spent=0.741220
	Throughput for read_write_memory = 1079.301692 MB/s 
	Latency Calculation for 10 samples of seconds
	Time Spent for sample 1=0.713608
	Time Spent for sample 2=0.637760
	Time Spent for sample 3=0.642376
	Time Spent for sample 4=0.635929
	Time Spent for sample 5=0.637269
	Time Spent for sample 6=0.638517
	Time Spent for sample 7=0.633289
	Time Spent for sample 8=0.637905
	Time Spent for sample 9=0.639654
	Time Spent for sample 10=0.714109
	Average Time=0.653042
	Latency for block size of 8 byte =5.224333 microseconds

-------------------
3.Disk Benchmarking
-------------------
Disk_Benchmarking.c contains the source code

	Compilation and Execute instructions
	a)Go the directory where the file is present
	b)Compile the file using command: gcc -pthread -o0 -std=c99 -o Disk_Benchmarking Disk_Benchmarking.c
	c)Run the file using command: ./Disk_Benchmarking
	d)Follow the instructions displayed
		1.1.Choose the number of threads
		2.Choose the operation needs to be performed
		3.Choose the block size(Note:Latency will be only calculated for block size of 8 bytes)

	Note:Operations are performed on binary file therefore speed will be faster


	Example
	Perform Disk Benchmarking on 
	1. 1 Thread
	2. 2 Thread
	3. 4 Thread
	4. 8 Thread
	1
	Number of Threads 1
	Choose Operation to be performed 
	1. Read and Write 
	2. Sequential Read
	3. Random Read
	1
	Choose an option of  block size
	1. 8 Byte
	2. 8 Kilobyte
	3. 8 Megabyte
	4.80 Megabyte
	1
	Disk Benchmark Execution based on operation choosed,number of threads and block size

	Time Spent=160.136635
	Throughput for read_write_disk = 3.996587 MB/s 

	Latency for block size of 8 byte =0.238622 microseconds

----------------------
4.Network Benchmarking
----------------------
Network_Benchmarking.c contains source code for TCP and UDP clients
TCP_server.c contains source code for TCP server
udp_server.c conatins source code for UDP server

	Compilation and Execute instructions
	a)Go the directory where the file is present
	b)Compile the server(Choose either TCP or UDP) file using command-
	c)Run the server file using command: gcc -pthread -o0 -std=c99 -o TCP_server TCP_server.c
	d)Enter the number of threads in the server 
	e)Open a new terminal to run client(Network_Benchmarking.c)
	f)Compile the client using command: gcc -pthread -o0 -std=c99 -o Network_Benchmarking Network_Benchmarking.c
	g)Run the client using command- ./Network_Benchmarking
	f)Choose the number of thread
	g)Choose the client for which server has been running(TCP or UDP)
	

	Note:At a time only 8 bytes is transferred in the network

	Example 
	Running TCP_server.c on first terminal
	Perform Network Benchmarking on 
	1. 1 Thread
	2. 2 Thread
	3. 4 Thread
	4. 8 Thread
	1

	Running Network_Benchmarking.c on second terminal
	Perform Network Benchmarking on 
	1. 1 Thread
	2. 2 Thread
	3. 4 Thread
	4. 8 Thread
	1
	Number of Threads 1Perform Network Benchmarking on 
	1. TCP Stack
	2. UDP
	Note:If server and cient are on different system try to keep the respective files in those systems
	1

	Output of Server Terminal
	.....
	Bytes Transferred 8096
	Bytes Transferred 8104
	Bytes Transferred 8112
	Bytes Transferred 8120
	Bytes Transferred 8128
	Bytes Transferred 8136
	Bytes Transferred 8144
	Bytes Transferred 8152
	Bytes Transferred 8160
	Bytes Transferred 8168
	Bytes Transferred 8176
	Bytes Transferred 8184
	Bytes Transferred 8192
	....

	Output of Client Terminal
	Disk Benchmark Execution based on operation choosed,number of threads

	Time Spent=0.000189
	Throughput for Network = 41.335979 MB/s

	Latency for block size of 8 byte =0.023071 microseconds

















